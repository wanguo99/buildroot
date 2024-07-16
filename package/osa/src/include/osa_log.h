#ifndef _OSA_LOG_KERN_H
#define _OSA_LOG_KERN_H

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/ktime.h>

#define LOG_VERSION "0.1.0"
#define MAX_CALLBACKS 32

typedef struct {
    va_list ap;
    OSA_CONST OSA_INT8 *fmt;
    OSA_CONST OSA_INT8 *file;
    struct tm *time; // Not frequently used in kernel
    OSA_VOID *udata;
    OSA_INT32 line;
    OSA_INT32 level;
} log_Event;

typedef OSA_VOID (*log_log_fn)(log_Event *ev);
typedef OSA_VOID (*log_lock_fn)(OSA_BOOL lock, OSA_VOID *udata);

enum { LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL };

#define OSA_TRACE(...) OSA_log(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define OSA_DEBUG(...) OSA_log(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define OSA_INFO(...)  OSA_log(LOG_INFO,  __FILE__, __LINE__, __VA_ARGS__)
#define OSA_WARN(...)  OSA_log(LOG_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#define OSA_ERROR(...) OSA_log(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define OSA_FATAL(...) OSA_log(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

OSA_CONST OSA_INT8* log_level_string(OSA_INT32 level);
OSA_VOID log_set_lock(log_lock_fn fn, OSA_VOID *udata);
OSA_VOID log_set_level(OSA_INT32 level);
OSA_VOID log_set_quiet(OSA_BOOL enable);
OSA_INT32 log_add_callback(log_log_fn fn, OSA_VOID *udata, OSA_INT32 level);

OSA_VOID OSA_log(OSA_INT32 level, OSA_CONST OSA_INT8 *file, OSA_INT32 line, OSA_CONST OSA_INT8 *fmt, ...);

#endif // _OSA_LOG_KERN_H
