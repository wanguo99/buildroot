#include "osa/osa.h"
#include "hal.h"
#include "hal_gpio_priv.h"


INT32 HAL_ledInit(VOID)
{
    INT32 iRet = OSA_EFAIL;

    OSA_INFO("");

    iRet = OSA_cdevCreate();
    if (OSA_isNotSuccess(iRet))
    {
        OSA_ERROR("OSA_cdevCreate failed.");
        return OSA_EFAIL;
    }

    return OSA_SOK;
}


INT32 HAL_ledExit(VOID)
{
    INT32 iRet = OSA_EFAIL;

    OSA_INFO("");

    iRet = OSA_cdevDelete();
    if (OSA_isNotSuccess(iRet))
    {
        OSA_ERROR("OSA_cdevDelete failed.");
    }

    return OSA_SOK;
}


