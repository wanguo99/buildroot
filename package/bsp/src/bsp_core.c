#include <linux/kernel.h>
#include <linux/module.h>

#include "osa/osa.h"
#include "bsp.h"

INT32 BSP_init(VOID);
void BSP_exit(VOID);

INT32 BSP_init(VOID)
{
	OSA_INFO("Hello World!\n");
	OSA_INFO("BSP module init.\n");

	return 0;
}

VOID BSP_exit(VOID)
{
	OSA_INFO("BSP module exit.\n");

	return;
}

module_init(BSP_init);
module_exit(BSP_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Guo Hao");
MODULE_DESCRIPTION("BSP Module");
