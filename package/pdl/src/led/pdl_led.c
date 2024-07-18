#include <linux/slab.h>

#include "osa/osa.h"
#include "pdl.h"
#include "pdl_led_priv.h"

OSA_cdev_t g_stLedCdev;
OSA_proc_t g_stLedProcFile;

struct file_operations  g_stLedCdevFops;


#define PROC_RW_TEST_BUFFER_SIZE 128

static char proc_rw_test_buffer[PROC_RW_TEST_BUFFER_SIZE];
static unsigned long buffer_used = 0;


static ssize_t LED_proc_read(struct file *file, char __user *user_buffer, size_t len, loff_t *offset)
{
    ssize_t ret = 0;
    if (*offset >= buffer_used)
    {
        ret = 0;
    }
    else if (*offset + len > buffer_used)
    {
        ret = copy_to_user(user_buffer, proc_rw_test_buffer + *offset, buffer_used - *offset);
        if (ret == 0)
        {
            ret = buffer_used - *offset;
            *offset = buffer_used;
        }
    }
    else
    {
        ret = copy_to_user(user_buffer, proc_rw_test_buffer + *offset, len);
        if (ret == 0)
        {
            ret = len;
            *offset += len;
        }
    }
    return ret;
}


static ssize_t LED_proc_write(struct file *file, const char __user *user_buffer, size_t len, loff_t *offset)
{
    if (len > PROC_RW_TEST_BUFFER_SIZE)
    {
        return -EFAULT;
    }

    if (copy_from_user(proc_rw_test_buffer, user_buffer, len))
    {
        return -EFAULT;
    }

    buffer_used = len;
    return len;
}


struct proc_ops g_stLedProcFops=
{
    .proc_read = LED_proc_read,
    .proc_write = LED_proc_write,
};


INT32 PDL_LED_init(VOID)
{
    INT32 iRet = OSA_EFAIL;

    OSA_INFO("");

    /* 创建字符设备 */
    memset(&g_stLedCdev, 0, sizeof(OSA_cdev_t));
    g_stLedCdev.class = PDL_get_class_handler();
    g_stLedCdev.pstFileOps = &g_stLedCdevFops;
    iRet = OSA_cdev_create(&g_stLedCdev, PDL_LED_CDEV_NAME);
    if (OSA_isNotSuccess(iRet))
    {
        OSA_ERROR("OSA_cdev_create failed.");
        return OSA_EFAIL;
    }

    /* 创建proc文件 */
    memset(&g_stLedProcFile, 0, sizeof(OSA_proc_t));
    g_stLedProcFile.fops = &g_stLedProcFops;
    iRet = OSA_proc_create(&g_stLedProcFile, PDL_LED_PROC_NAME);
    if (OSA_isNotSuccess(iRet))
    {
        OSA_ERROR("OSA_proc_create failed.");
        return OSA_EFAIL;
    }

    OSA_INFO("PDL_led_init success.");

    return OSA_SOK;
}


VOID PDL_LED_exit(VOID)
{
    OSA_INFO("");

    OSA_proc_delete(&g_stLedProcFile);
    OSA_cdev_delete(&g_stLedCdev);

    return;
}


