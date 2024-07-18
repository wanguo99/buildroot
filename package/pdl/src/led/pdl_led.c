#include "osa/osa.h"
#include "pdl.h"
#include "pdl_led_priv.h"


INT32 PDL_ledInit(VOID)
{
    INT32 iRet = OSA_EFAIL;

    OSA_INFO("");

    iRet = OSA_cdev_create();
    if (OSA_isNotSuccess(iRet))
    {
        OSA_ERROR("OSA_cdev_create failed.");
        return OSA_EFAIL;
    }

    return OSA_SOK;
}


INT32 PDL_ledExit(VOID)
{
    INT32 iRet = OSA_EFAIL;

    OSA_INFO("");

    iRet = OSA_cdev_delete();
    if (OSA_isNotSuccess(iRet))
    {
        OSA_ERROR("OSA_cdev_delete failed.");
    }

    return OSA_SOK;
}


