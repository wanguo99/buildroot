#include <linux/slab.h>

#include "osa/osa.h"
#include "pdl.h"
#include "pdl_led_priv.h"

OSA_cdev_t *g_pstLedCdev = NULL;
struct file_operations g_pstFileOps;

INT32 PDL_ledInit()
{
    INT32 iRet = OSA_EFAIL;

    OSA_INFO("");

    g_pstLedCdev = kzalloc(sizeof(OSA_cdev_t), GFP_KERNEL);
    if (OSA_isNull(g_pstLedCdev))
    {
        OSA_ERROR("Init LED Cdev Data failed.");
        return OSA_EFAIL;
    }

    g_pstLedCdev->class = PDL_get_class_handler();
    g_pstLedCdev->pstFileOps = &g_pstFileOps;

    iRet = OSA_cdev_create(g_pstLedCdev, PDL_LED_CDEV_NAME);
    if (OSA_isNotSuccess(iRet))
    {
        OSA_ERROR("OSA_cdev_create failed.");
        return OSA_EFAIL;
    }

    return OSA_SOK;
}


INT32 PDL_ledExit(VOID)
{
    OSA_INFO("");
    OSA_cdev_delete(g_pstLedCdev);

    kfree(g_pstLedCdev);
    return OSA_SOK;
}


