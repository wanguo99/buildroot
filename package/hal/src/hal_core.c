#include <linux/kernel.h>
#include <linux/module.h>

#include "osa/osa.h"
#include "hal.h"

static INT32 HAL_init(VOID);
static VOID HAL_exit(VOID);

static INT32 HAL_init(VOID)
{
    OSA_INFO("HAL module init.");

    return 0;
}

static VOID HAL_exit(VOID)
{
    OSA_INFO("HAL module exit.");

    return;
}

module_init(HAL_init);
module_exit(HAL_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Guo Hao");
MODULE_DESCRIPTION("Hardware Abstraction Layer Module");
