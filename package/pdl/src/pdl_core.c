#include <linux/kernel.h>
#include <linux/module.h>

#include "osa/osa.h"
#include "pdl.h"

STATIC INT32 PDL_init(VOID);
STATIC VOID PDL_exit(VOID);

STATIC INT32 PDL_init(VOID)
{
    OSA_INFO("PDL module init.");

    PDL_ledInit();

    return 0;
}

STATIC VOID PDL_exit(VOID)
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
