#ifndef _OSA_LOG_KERN_H
#define _OSA_LOG_KERN_H

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/spinlock.h>

#define LOG_WITH_COLOR 1

/* ANSI颜色转义码定义，仅在LOG_WITH_COLOR启用时使用 */
#ifdef LOG_WITH_COLOR
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#endif // LOG_WITH_COLOR


// 校色版和无颜色版的宏定义，以便于控制是否启用颜色
#ifdef LOG_WITH_COLOR
#define OSA_EMERG(fmt, ...)  \
    do { \
        printk(KERN_EMERG   ANSI_COLOR_RED     "[EMERG] %s:%d " fmt ANSI_COLOR_RESET "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    } while (0)
#define OSA_ALERT(fmt, ...)  \
    do { \
        printk(KERN_ALERT   ANSI_COLOR_RED     "[ALERT] %s:%d " fmt ANSI_COLOR_RESET "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    } while (0)
#define OSA_CRIT(fmt, ...)   \
    do { \
        printk(KERN_CRIT    ANSI_COLOR_MAGENTA "[CRIT] %s:%d " fmt ANSI_COLOR_RESET "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    } while (0)
#define OSA_ERROR(fmt, ...)  \
    do { \
        printk(KERN_ERR     ANSI_COLOR_RED     "[ERROR] %s:%d " fmt ANSI_COLOR_RESET "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    } while (0)
#define OSA_WARN(fmt, ...)   \
    do { \
        printk(KERN_WARNING ANSI_COLOR_YELLOW  "[WARN] %s:%d " fmt ANSI_COLOR_RESET "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    } while (0)
#define OSA_NOTICE(fmt, ...) \
    do { \
        printk(KERN_NOTICE  ANSI_COLOR_GREEN   "[NOTICE] %s:%d " fmt ANSI_COLOR_RESET "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    } while (0)
#define OSA_INFO(fmt, ...)   \
    do { \
        printk(KERN_INFO    ANSI_COLOR_BLUE    "[INFO] %s:%d " fmt ANSI_COLOR_RESET "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    } while (0)
#define OSA_DEBUG(fmt, ...)  \
    do { \
        printk(KERN_DEBUG   ANSI_COLOR_CYAN    "[DEBUG] %s:%d " fmt ANSI_COLOR_RESET "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    } while (0)
#define OSA_log(fmt, ...)    \
    do { \
        printk(KERN_DEFAULT          "%s:%d " fmt ANSI_COLOR_RESET "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    } while (0)
#else // LOG_WITH_COLOR
#define OSA_EMERG(fmt, ...)  \
    do { \
        printk(KERN_EMERG   "[EMERG] %s:%d " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    } while (0)
#define OSA_ALERT(fmt, ...)  \
    do { \
        printk(KERN_ALERT   "[ALERT] %s:%d " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    } while (0)
#define OSA_CRIT(fmt, ...)   \
    do { \
        printk(KERN_CRIT    "[CRIT] %s:%d " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    } while (0)
#define OSA_ERROR(fmt, ...)  \
    do { \
        printk(KERN_ERR     "[ERROR] %s:%d " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    } while (0)
#define OSA_WARN(fmt, ...)   \
    do { \
        printk(KERN_WARNING "[WARN] %s:%d " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    } while (0)
#define OSA_NOTICE(fmt, ...) \
    do { \
        printk(KERN_NOTICE  "[NOTICE] %s:%d " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    } while (0)
#define OSA_INFO(fmt, ...)   \
    do { \
        printk(KERN_INFO    "[INFO] %s:%d " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    } while (0)
#define OSA_DEBUG(fmt, ...)  \
    do { \
        printk(KERN_DEBUG   "[DEBUG] %s:%d " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    } while (0)
#define OSA_log(fmt, ...)    \
    do { \
        printk(KERN_DEFAULT "%s:%d " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    } while (0)
#endif // LOG_WITH_COLOR

#endif // _OSA_LOG_KERN_H
