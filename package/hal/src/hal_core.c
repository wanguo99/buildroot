#include <linux/kernel.h>
#include <linux/module.h>

#include "osa/osa.h"
#include "hal.h"

STATIC INT32 HAL_init(VOID);
STATIC VOID HAL_exit(VOID);

STATIC INT32 HAL_init(VOID)
{
    OSA_INFO("HAL module init.");

    return 0;
}

STATIC VOID HAL_exit(VOID)
{
    OSA_INFO("HAL module exit.");

    return;
}

module_init(HAL_init);
module_exit(HAL_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Guo Hao");
MODULE_DESCRIPTION("Hardware Abstraction Layer Module");
