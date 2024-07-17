#if 0
#include <linux/kernel.h>
#include <linux/stdarg.h>
#include <linux/slab.h>

#include "osa.h"

typedef struct {
    log_log_fn  fn;
    VOID        *udata;
    INT32       level;
} Callback;

typedef struct
{
    VOID        *udata;                     // 用户数据指针
    log_lock_fn lock_fn;                    // 锁定函数指针
    INT32       level;                      // 日志级别
    BOOL        quiet;                      // 静默模式标志
    Callback    callbacks[MAX_CALLBACKS];   // 回调函数数组
    spinlock_t  lock;                       // 自旋锁
} log_state_t;

STATIC log_state_t log_state =
{
    .lock = __SPIN_LOCK_UNLOCKED(log_state.lock)
};

STATIC CONST INT8 *level_strings[] =
{
    "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};

#ifdef LOG_USE_COLOR
STATIC CONST INT8 *level_colors[] =
{
    "\x1b[94m", // TRACE -> Bright Blue
    "\x1b[36m", // DEBUG -> Cyan
    "\x1b[32m", // INFO -> Green
    "\x1b[33m", // WARN -> Yellow
    "\x1b[31m", // ERROR -> Red
    "\x1b[35m"  // FATAL -> Magenta
};
#endif

STATIC VOID stdout_callback(log_Event *ev)
{

#ifdef LOG_USE_COLOR
    printk(KERN_INFO "%s%-5s\x1b[0m %s:%d: ", level_colors[ev->level], level_strings[ev->level], ev->file, ev->line);
    vprintk(ev->fmt, ev->ap);
#else
    printk(KERN_INFO "%s %-5s %s:%d: ", level_strings[ev->level], ev->file, ev->line);
    vprintk(ev->fmt, ev->ap);
#endif
}

STATIC VOID lock(VOID)
{
    if (log_state.lock_fn)
    {
        log_state.lock_fn(true, log_state.udata);
    }
    else
    {
        spin_lock(&log_state.lock);
    }
}

STATIC VOID unlock(VOID)
{
    if (log_state.lock_fn)
    {
        log_state.lock_fn(false, log_state.udata);
    }
    else
    {
        spin_unlock(&log_state.lock);
    }
}

CONST INT8* log_level_string(INT32 level)
{
    return level_strings[level];
}

VOID log_set_lock(log_lock_fn fn, VOID *udata)
{
    lock();
    log_state.lock_fn = fn;
    log_state.udata = udata;
    unlock();
}

VOID log_set_level(INT32 level)
{
    lock();
    log_state.level = level;
    unlock();
}

VOID log_set_quiet(BOOL enable)
{
    lock();
    log_state.quiet = enable;
    unlock();
}

INT32 log_add_callback(log_log_fn fn, VOID *udata, INT32 level)
{
    INT32 index;
    INT32 result = -1;

    lock();
    for (index = 0; index < MAX_CALLBACKS; index++)
    {
        if (!log_state.callbacks[index].fn)
        {
            log_state.callbacks[index] = (Callback){ fn, udata, level };
            result = 0;
            break;
        }
    }
    unlock();

    return result;
}

STATIC VOID init_event(log_Event *ev, VOID *udata)
{
    ev->udata = udata;
}

VOID OSA_log(INT32 level, CONST INT8 *file, INT32 line, CONST INT8 *fmt, ...)
{
    INT32 index;

    if (level < log_state.level)
    {
        return;
    }

    log_Event ev =
    {
        .fmt = fmt,
        .file = file,
        .line = line,
        .level = level,
    };
    pr_err("fmt: %s\n", fmt);
    pr_err("file: %s\n", file);
    pr_err("line: %d\n", line);
    pr_err("level: %d\n", level);

    va_start(ev.ap, fmt);
    lock();
    if (!log_state.quiet && level >= log_state.level)
    {
        init_event(&ev, NULL);
        stdout_callback(&ev);
    }

    for (index = 0; index < MAX_CALLBACKS && log_state.callbacks[index].fn; index++)
    {
        Callback *cb = &log_state.callbacks[index];
        if (level >= cb->level)
        {
            cb->fn(&ev);
        }
    }
    unlock();
    va_end(ev.ap);
}
#endif
