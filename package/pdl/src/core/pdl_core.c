#include <linux/kernel.h>
#include <linux/module.h>

#include "osa/osa.h"
#include "pdl.h"
#include "pdl_priv.h"

static INT32 PDL_init(VOID);
static VOID PDL_exit(VOID);

OSA_class_t *g_pdlClassHdl = NULL;

static INT32 PDL_class_create(VOID)
{
    INT32 iRet = OSA_EFAIL;

    iRet = OSA_class_create(&g_pdlClassHdl, PDL_CLASS_NAME);
    if (OSA_isFail(iRet))
    {
        OSA_ERROR("Failed to create device class: %s", PDL_CLASS_NAME);
        return iRet;
    }

    OSA_INFO("Create device class %s ok.", PDL_CLASS_NAME);

    return OSA_SOK;
}

static VOID PDL_class_delete(VOID)
{
    OSA_class_delete(&g_pdlClassHdl);

    OSA_INFO("Delete device class %s ok.", PDL_CLASS_NAME);

    return;
}

static INT32 PDL_init(VOID)
{
    INT32 iRet = OSA_EFAIL;

    OSA_INFO("PDL module init.");

    iRet = PDL_class_create();
    if (OSA_isFail(iRet))
    {
        OSA_ERROR("PDL_class_create faled.");
        return OSA_EFAIL;
    }

    PDL_ledInit();

    return 0;
}

static VOID PDL_exit(VOID)
{
    OSA_INFO("PDL module exit.");

    PDL_ledExit();

    PDL_class_delete();

    return;
}

module_init(PDL_init);
module_exit(PDL_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Guo Hao");
MODULE_DESCRIPTION("Peripheral Driver Library Module");
