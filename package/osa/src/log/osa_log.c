#include <linux/timekeeping.h>
#include <linux/stdarg.h>
#include <linux/slab.h>

#include "osa_log.h"

typedef struct {
    log_LogFn fn;
    void *udata;
    int level;
} Callback;

typedef struct
{
    void *udata;                        // 用户数据指针
    log_LockFn lock_fn;                 // 锁定函数指针
    int level;                          // 日志级别
    bool quiet;                         // 静默模式标志
    Callback callbacks[MAX_CALLBACKS];  // 回调函数数组
    spinlock_t lock;                    // 自旋锁
} log_state_t;

static log_state_t log_state =
{
    .lock = __SPIN_LOCK_UNLOCKED(log_state.lock)
};

static const char *level_strings[] =
{
    "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};

#ifdef LOG_USE_COLOR
static const char *level_colors[] =
{
    "\x1b[94m", // TRACE -> Bright Blue
    "\x1b[36m", // DEBUG -> Cyan
    "\x1b[32m", // INFO -> Green
    "\x1b[33m", // WARN -> Yellow
    "\x1b[31m", // ERROR -> Red
    "\x1b[35m"  // FATAL -> Magenta
};
#endif

static void stdout_callback(log_Event *ev)
{
    char buf[64];

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

static void lock(void)
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

static void unlock(void)
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

const char* log_level_string(int level)
{
    return level_strings[level];
}

void log_set_lock(log_LockFn fn, void *udata)
{
    lock();
    log_state.lock_fn = fn;
    log_state.udata = udata;
    unlock();
}

void log_set_level(int level)
{
    lock();
    log_state.level = level;
    unlock();
}

void log_set_quiet(bool enable)
{
    lock();
    log_state.quiet = enable;
    unlock();
}

int log_add_callback(log_LogFn fn, void *udata, int level)
{
    int i;
    int result = -1;

    lock();
    for (i = 0; i < MAX_CALLBACKS; i++)
    {
        if (!log_state.callbacks[i].fn)
        {
            log_state.callbacks[i] = (Callback){ fn, udata, level };
            result = 0;
            break;
        }
    }
    unlock();

    return result;
}

static void init_event(log_Event *ev, void *udata)
{
    ev->udata = udata;
}

void log_log(int level, const char *file, int line, const char *fmt, ...)
{
    if (level < log_state.level)
        return;

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

    for (int i = 0; i < MAX_CALLBACKS && log_state.callbacks[i].fn; i++)
    {
        Callback *cb = &log_state.callbacks[i];
        if (level >= cb->level)
        {
            cb->fn(&ev);
        }
    }

    unlock();
    va_end(ev.ap);
}
