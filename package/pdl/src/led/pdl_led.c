#include <linux/slab.h>

#include "osa/osa.h"
#include "pdl.h"
#include "pdl_led_priv.h"

OSA_cdev_t g_stLedCdev;
OSA_proc_t g_stLedProcFile;

struct file_operations  g_stLedCdevFops;
struct proc_ops         g_stLedProcFops;

INT32 PDL_ledInit()
{
    INT32 iRet = OSA_EFAIL;

    OSA_INFO("");

    memset(&g_stLedCdev, 0, sizeof(OSA_cdev_t));
    g_stLedCdev.class = PDL_get_class_handler();
    g_stLedCdev.pstFileOps = &g_stLedCdevFops;
    iRet = OSA_cdev_create(&g_stLedCdev, PDL_LED_CDEV_NAME);
    if (OSA_isNotSuccess(iRet))
    {
        OSA_ERROR("OSA_cdev_create failed.");
        return OSA_EFAIL;
    }

    memset(&g_stLedProcFile, 0, sizeof(OSA_proc_t));
    g_stLedProcFile.fops = &g_stLedProcFops;
    iRet = OSA_proc_create(&g_stLedProcFile, PDL_LED_PROC_NAME);
    if (OSA_isNotSuccess(iRet))
    {
        OSA_ERROR("OSA_proc_create failed.");
        return OSA_EFAIL;
    }

    return OSA_SOK;
}


INT32 PDL_ledExit(VOID)
{
    OSA_INFO("");
    OSA_proc_delete(&g_stLedProcFile);
    OSA_cdev_delete(&g_stLedCdev);

    return OSA_SOK;
}


