#include <linux/kernel.h>
#include <linux/module.h>

int BSP_init(void);
void BSP_exit(void);

int BSP_init(void)
{
	pr_info("Hello World!\n");
	pr_info("BSP module init.\n");

	return 0;
}

void BSP_exit(void)
{
	pr_info("BSP module exit.\n");

	return;
}

module_init(BSP_init);
module_exit(BSP_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Guo Hao");
MODULE_DESCRIPTION("BSP Module");
