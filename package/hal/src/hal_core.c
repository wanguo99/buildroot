#include <linux/kernel.h>
#include <linux/module.h>

#include "osa/osa.h"
#include "hal.h"

INT32 HAL_init(VOID);
void HAL_exit(VOID);

INT32 HAL_init(VOID)
{
    OSA_INFO("HAL module init.");

    HAL_ledInit();

    return 0;
}

VOID HAL_exit(VOID)
{
    OSA_INFO("HAL module exit.");

    HAL_ledExit();

    return;
}

module_init(HAL_init);
module_exit(HAL_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Guo Hao");
MODULE_DESCRIPTION("Board Support Package Module");
