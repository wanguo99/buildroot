#include <linux/kernel.h>
#include <linux/module.h>

#include "osa/osa.h"
#include "pdl.h"

static INT32 PDL_init(VOID);
static VOID PDL_exit(VOID);

static INT32 PDL_init(VOID)
{
    OSA_INFO("PDL module init.");

    PDL_ledInit();

    return 0;
}

static VOID PDL_exit(VOID)
{
    OSA_INFO("PDL module exit.");

    PDL_ledExit();

    return;
}

module_init(PDL_init);
module_exit(PDL_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Guo Hao");
MODULE_DESCRIPTION("Peripheral Driver Library Module");
