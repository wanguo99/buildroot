#include <linux/kernel.h>
#include <linux/module.h>

int bsp_init(void);
void bsp_exit(void);

int bsp_init(void)
{
	pr_info("Hello World!\n");
	pr_info("bsp module init.\n");

	return 0;
}

void bsp_exit(void)
{
	pr_info("bsp module exit.\n");

	return;
}

module_init(bsp_init);
module_exit(bsp_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Guo Hao");
MODULE_DESCRIPTION("A Simple BSP Module");
