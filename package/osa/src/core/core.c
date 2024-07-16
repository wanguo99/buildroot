#include <linux/kernel.h>
#include <linux/module.h>

#include "osa.h"


OSA_INT32 OSA_init(OSA_VOID);
OSA_VOID OSA_exit(OSA_VOID);

OSA_INT32 OSA_init(OSA_VOID)
{
	OSA_INFO("OSA module init.\n");

	OSA_INFO("Hello World!\n");

	return OSA_SOK;
}

OSA_VOID OSA_exit(OSA_VOID)
{
	OSA_FATAL("OSA module exit.\n");

	return;
}

module_init(OSA_init);
module_exit(OSA_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Guo Hao");
MODULE_DESCRIPTION("OSA Module");
