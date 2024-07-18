#include <linux/kernel.h>
#include <linux/module.h>

#include "osa.h"


static INT32 OSA_init(VOID);
static VOID OSA_exit(VOID);


static VOID OSA_logTest(VOID)
{
    OSA_INFO("OSA_log module test start.");

    OSA_EMERG("");
    OSA_ALERT("");
    OSA_CRIT("");
    OSA_ERROR("");
    OSA_WARN("");
    OSA_NOTICE("");
    OSA_INFO("");
    OSA_DEBUG("");
    OSA_log("");

    OSA_INFO("OSA_log module test ok.");

    return;
}


INT32 OSA_init(VOID)
{
	OSA_INFO("OSA module init.");

    OSA_logTest();

	return OSA_SOK;
}

VOID OSA_exit(VOID)
{
	OSA_INFO("OSA module exit.");

	return;
}

module_init(OSA_init);
module_exit(OSA_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Guo Hao");
MODULE_DESCRIPTION("OSA Module");
