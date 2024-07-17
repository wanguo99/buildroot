#include <linux/kernel.h>
#include <linux/module.h>
#include "osa/osa.h"


static INT32 CFG_init(VOID);
static VOID CFG_exit(VOID);

static INT32 CFG_init(VOID)
{
	OSA_INFO("CFG module init.");

	return OSA_SOK;
}

static VOID CFG_exit(VOID)
{
	OSA_INFO("CFG module exit.");

	return;
}

module_init(CFG_init);
module_exit(CFG_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Guo Hao");
MODULE_DESCRIPTION("Product Config Module");
