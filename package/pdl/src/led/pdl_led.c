#include "osa/osa.h"
#include "pdl.h"
#include "pdl_led_priv.h"


INT32 PDL_ledInit(VOID)
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


INT32 PDL_ledExit(VOID)
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


