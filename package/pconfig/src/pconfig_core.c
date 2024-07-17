#include <linux/kernel.h>
#include <linux/module.h>
#include "osa/osa.h"


STATIC INT32 PCONFIG_init(VOID);
STATIC VOID PCONFIG_exit(VOID);

INT32 PCONFIG_init(VOID)
{
	OSA_INFO("PCONFIG module init.");

	OSA_INFO("Hello World!");

    OSA_logTest();


	return OSA_SOK;
}

VOID PCONFIG_exit(VOID)
{
	OSA_INFO("PCONFIG module exit.");

	return;
}

module_init(PCONFIG_init);
module_exit(PCONFIG_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Guo Hao");
MODULE_DESCRIPTION("Product Config Module");
