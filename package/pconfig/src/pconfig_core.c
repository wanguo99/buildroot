#include <linux/kernel.h>
#include <linux/module.h>
#include "osa/osa.h"


static INT32 PCONFIG_init(VOID);
static VOID PCONFIG_exit(VOID);

static INT32 PCONFIG_init(VOID)
{
	OSA_INFO("PCONFIG module init.");

	return OSA_SOK;
}

static VOID PCONFIG_exit(VOID)
{
	OSA_INFO("PCONFIG module exit.");

	return;
}

module_init(PCONFIG_init);
module_exit(PCONFIG_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Guo Hao");
MODULE_DESCRIPTION("Product Config Module");
