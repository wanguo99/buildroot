#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/stdarg.h>
#include <linux/slab.h>

#include "osa.h"

typedef struct {
    log_log_fn  fn;
    OSA_VOID        *udata;
    OSA_INT32       level;
} Callback;

typedef struct
{
    OSA_VOID        *udata;                     // 用户数据指针
    log_lock_fn  lock_fn;                    // 锁定函数指针
    OSA_INT32       level;                      // 日志级别
    OSA_BOOL        quiet;                      // 静默模式标志
    Callback    callbacks[MAX_CALLBACKS];   // 回调函数数组
    spinlock_t  lock;                       // 自旋锁
} log_state_t;

OSA_STATIC log_state_t log_state =
{
    .lock = __SPIN_LOCK_UNLOCKED(log_state.lock)
};

OSA_STATIC OSA_CONST OSA_INT8 *level_strings[] =
{
    "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};

#ifdef LOG_USE_COLOR
OSA_STATIC OSA_CONST OSA_INT8 *level_colors[] =
{
    "\x1b[94m", // TRACE -> Bright Blue
    "\x1b[36m", // DEBUG -> Cyan
    "\x1b[32m", // INFO -> Green
    "\x1b[33m", // WARN -> Yellow
    "\x1b[31m", // ERROR -> Red
    "\x1b[35m"  // FATAL -> Magenta
};
#endif

OSA_STATIC OSA_VOID stdout_callback(log_Event *ev)
{
    OSA_INT8 buf[64];

    snprintf(buf, sizeof(buf), "%5llu", ktime_get_seconds());
#ifdef LOG_USE_COLOR
    printk(KERN_INFO "%s %s%-5s\x1b[0m %s:%d: ",
           buf, level_colors[ev->level], level_strings[ev->level], ev->file, ev->line);
    vprintk(ev->fmt, ev->ap);
    printk(KERN_INFO "\n");
#else
    printk(KERN_INFO "%s %-5s %s:%d: ",
           buf, level_strings[ev->level], ev->file, ev->line);
    vprintk(ev->fmt, ev->ap);
    printk(KERN_INFO "\n");
#endif
}

OSA_STATIC OSA_VOID lock(OSA_VOID)
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

OSA_STATIC OSA_VOID unlock(OSA_VOID)
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

OSA_CONST OSA_INT8* log_level_string(OSA_INT32 level)
{
    return level_strings[level];
}

OSA_VOID log_set_lock(log_lock_fn fn, OSA_VOID *udata)
{
    lock();
    log_state.lock_fn = fn;
    log_state.udata = udata;
    unlock();
}

OSA_VOID log_set_level(OSA_INT32 level)
{
    lock();
    log_state.level = level;
    unlock();
}

OSA_VOID log_set_quiet(OSA_BOOL enable)
{
    lock();
    log_state.quiet = enable;
    unlock();
}

OSA_INT32 log_add_callback(log_log_fn fn, OSA_VOID *udata, OSA_INT32 level)
{
    OSA_INT32 index;
    OSA_INT32 result = -1;

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

OSA_STATIC OSA_VOID init_event(log_Event *ev, OSA_VOID *udata)
{
    ev->udata = udata;
}

OSA_VOID OSA_log(OSA_INT32 level, OSA_CONST OSA_INT8 *file, OSA_INT32 line, OSA_CONST OSA_INT8 *fmt, ...)
{
    OSA_INT32 index;

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
