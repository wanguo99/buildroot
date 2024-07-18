#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>

#include "osa/osa.h"
#include "pdl.h"
#include "pdl_priv.h"

static INT32 PDL_init(VOID);
static VOID PDL_exit(VOID);


static PDL_PRIVATE_DATA_t *g_pstPdlPrivData = NULL;

OSA_class_t *PDL_get_class_handler(VOID)
{
    return g_pstPdlPrivData->pstClass;
}
EXPORT_SYMBOL_GPL(PDL_get_class_handler);

static INT32 PDL_class_create(VOID)
{
    INT32 iRet = OSA_EFAIL;

    iRet = OSA_class_create(&g_pstPdlPrivData->pstClass, PDL_CLASS_NAME);
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
    OSA_class_delete(&g_pstPdlPrivData->pstClass);

    OSA_INFO("Delete device class %s ok.", PDL_CLASS_NAME);

    return;
}

static INT32 PDL_submodule_init(VOID)
{
    INT32 iRet = OSA_EFAIL;

    iRet = PDL_LED_init();
    if (OSA_isFail(iRet))
    {
        OSA_ERROR("PDL_LED_init failed.");
    }
    iRet = PDL_KEY_init();
    if (OSA_isFail(iRet))
    {
        OSA_ERROR("PDL_KEY_init failed.");
    }

    return OSA_SOK;
}

static VOID PDL_submodule_exit(VOID)
{
    PDL_KEY_exit();
    PDL_LED_exit();

    return;
}

static INT32 PDL_init(VOID)
{
    INT32 iRet = OSA_EFAIL;

    OSA_INFO("");

    g_pstPdlPrivData = kzalloc(sizeof(PDL_PRIVATE_DATA_t), GFP_KERNEL);
    if (OSA_isNull(g_pstPdlPrivData))
    {
        OSA_ERROR("Init PDL Priv Data failed.");
        return OSA_EFAIL;
    }

    iRet = PDL_class_create();
    if (OSA_isFail(iRet))
    {
        OSA_ERROR("PDL_class_create faled.");
        goto class_error;
    }

    iRet = PDL_submodule_init();
    if (OSA_isFail(iRet))
    {
        OSA_ERROR("PDL_submodule_init faled.");
        goto submodule_error;
    }

    return OSA_SOK;

submodule_error:
    PDL_class_delete();

class_error:
    kfree(g_pstPdlPrivData);

    return OSA_EFAIL;
}

static VOID PDL_exit(VOID)
{
    OSA_INFO("");

    PDL_submodule_exit();

    PDL_class_delete();

    kfree(g_pstPdlPrivData);

    return;
}

module_init(PDL_init);
module_exit(PDL_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Guo Hao");
MODULE_DESCRIPTION("Peripheral Driver Library Module");
