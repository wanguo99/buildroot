#include <linux/kernel.h>
#include <linux/module.h>

int OSA_init(void);
void OSA_exit(void);

int OSA_init(void)
{
	pr_info("Hello World!\n");
	pr_info("OSA module init.\n");

	return 0;
}

void OSA_exit(void)
{
	pr_info("OSA module exit.\n");

	return;
}

module_init(OSA_init);
module_exit(OSA_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Guo Hao");
MODULE_DESCRIPTION("OSA Module");
