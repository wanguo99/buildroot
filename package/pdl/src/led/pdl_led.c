#include <linux/io.h>
#include <linux/delay.h>

#include "osa/osa.h"
#include "pdl.h"
#include "pdl_led_priv.h"


/*************************************************************************/
/*                               全局变量                                    */
/*************************************************************************/
OSA_cdev_t g_stLedCdev;
OSA_proc_t g_stLedProcFile;

// 全局互斥锁
static DEFINE_MUTEX(g_stLedMutex);

static bool led_is_on = false;
static bool led_is_blinking = false;


#define LEDOFF 	0				/* 关灯 */
#define LEDON 	1				/* 开灯 */

/* 寄存器物理地址 */
#define CCM_CCGR1_BASE				(0X020C406C)
#define SW_MUX_GPIO1_IO03_BASE		(0X020E0068)
#define SW_PAD_GPIO1_IO03_BASE		(0X020E02F4)
#define GPIO1_DR_BASE				(0X0209C000)
#define GPIO1_GDIR_BASE				(0X0209C004)

/* 映射后的寄存器虚拟地址指针 */
static void __iomem *IMX6U_CCM_CCGR1;
static void __iomem *SW_MUX_GPIO1_IO03;
static void __iomem *SW_PAD_GPIO1_IO03;
static void __iomem *GPIO1_DR;
static void __iomem *GPIO1_GDIR;

/*************************************************************************/
/*                              LED驱动接口                                  */
/*************************************************************************/
static INT32 LED_drv_on(VOID)
{
	UINT32 val = 0;

    OSA_INFO("");

    val = readl(GPIO1_DR);
    val &= ~(1 << 3);
    writel(val, GPIO1_DR);

    return OSA_SOK;
}

static INT32 LED_drv_off(VOID)
{
	UINT32 val = 0;

    OSA_INFO("");
    val = readl(GPIO1_DR);
    val|= (1 << 3);
    writel(val, GPIO1_DR);

    return OSA_SOK;
}

static INT32 LED_drv_blink(VOID)
{
    OSA_INFO("");

    (VOID)LED_drv_on();

    msleep(500);

    (VOID)LED_drv_off();

    msleep(500);

    (VOID)LED_drv_on();

    msleep(500);

    (VOID)LED_drv_off();

    msleep(500);

    (VOID)LED_drv_on();

    msleep(500);

    (VOID)LED_drv_off();

    return OSA_SOK;
}


/* 初始化硬件管脚 */
static INT32 LED_drv_setup(VOID)
{
	UINT32 val = 0;

	/* 初始化LED */
	/* 1、寄存器地址映射 */
  	IMX6U_CCM_CCGR1 = ioremap(CCM_CCGR1_BASE, 4);
	SW_MUX_GPIO1_IO03 = ioremap(SW_MUX_GPIO1_IO03_BASE, 4);
  	SW_PAD_GPIO1_IO03 = ioremap(SW_PAD_GPIO1_IO03_BASE, 4);
	GPIO1_DR = ioremap(GPIO1_DR_BASE, 4);
	GPIO1_GDIR = ioremap(GPIO1_GDIR_BASE, 4);

	/* 2、使能GPIO1时钟 */
	val = readl(IMX6U_CCM_CCGR1);
	val &= ~(3 << 26);	/* 清楚以前的设置 */
	val |= (3 << 26);	/* 设置新值 */
	writel(val, IMX6U_CCM_CCGR1);

	/* 3、设置GPIO1_IO03的复用功能，将其复用为
	 *    GPIO1_IO03，最后设置IO属性。
	 */
	writel(5, SW_MUX_GPIO1_IO03);

	/*寄存器SW_PAD_GPIO1_IO03设置IO属性
	 *bit 16:0 HYS关闭
	 *bit [15:14]: 00 默认下拉
     *bit [13]: 0 kepper功能
     *bit [12]: 1 pull/keeper使能
     *bit [11]: 0 关闭开路输出
     *bit [7:6]: 10 速度100Mhz
     *bit [5:3]: 110 R0/6驱动能力
     *bit [0]: 0 低转换率
	 */
	writel(0x10B0, SW_PAD_GPIO1_IO03);

	/* 4、设置GPIO1_IO03为输出功能 */
	val = readl(GPIO1_GDIR);
	val &= ~(1 << 3);	/* 清除以前的设置 */
	val |= (1 << 3);	/* 设置为输出 */
	writel(val, GPIO1_GDIR);

	/* 5、默认关闭LED */
	val = readl(GPIO1_DR);
	val |= (1 << 3);
	writel(val, GPIO1_DR);

    return OSA_SOK;
}

/*************************************************************************/
/*                              字符设备文件                                   */
/*************************************************************************/


// LED操作函数
static void LED_cdev_on(void)
{
    led_is_on = true;
    pr_info("LED is turned ON\n");
}

static void LED_cdev_off(void)
{
    led_is_on = false;
    pr_info("LED is turned OFF\n");
}

static void LED_cdev_blink(void)
{
    led_is_blinking = true;
    pr_info("LED is set to BLINK\n");
}

// 文件操作函数
static int LED_cdev_open(struct inode *inode, struct file *file)
{
    return 0;
}

static int LED_cdev_release(struct inode *inode, struct file *file)
{
    return 0;
}

static ssize_t LED_cdev_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    // 读取操作，这里简单返回LED状态
    char status = led_is_on ? '1' : '0';
    if (copy_to_user(buf, &status, 1))
        return -EFAULT;
    return 1;
}

static ssize_t LED_cdev_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    // 写操作，根据写入内容控制LED
    char cmd;
    if (copy_from_user(&cmd, buf, 1))
        return -EFAULT;

    switch(cmd)
    {
        case '1':
            LED_cdev_on();
            break;
        case '0':
            LED_cdev_off();
            break;
        default:
            pr_info("Invalid command\n");
            return -EINVAL;
    }
    return count;
}

static long LED_cdev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    if (_IOC_TYPE(cmd) != LED_IOC_MAGIC)
        return -ENOTTY;
    if (_IOC_NR(cmd) > LED_IOC_MAXNR)
        return -ENOTTY;

    mutex_lock(&g_stLedMutex);

    switch (cmd) {
    case LED_IOC_ON:
        LED_cdev_on();
        break;
    case LED_IOC_OFF:
        LED_cdev_off();
        break;
    case LED_IOC_BLINK:
        LED_cdev_blink();
        break;
    default:
        mutex_unlock(&g_stLedMutex);
        return -ENOTTY;
    }

    mutex_unlock(&g_stLedMutex);

    return 0;
}

struct file_operations g_stLedCdevFops = {
    .owner = THIS_MODULE,
    .open = LED_cdev_open,
    .release = LED_cdev_release,
    .read = LED_cdev_read,
    .write = LED_cdev_write,
    .unlocked_ioctl = LED_cdev_ioctl,
};


/*************************************************************************/
/*                              proc文件                                   */
/*************************************************************************/
static VOID LED_proc_on(VOID)
{
    OSA_print("o: Open the led.");

    (VOID)LED_drv_on();

    return;
}

static VOID LED_proc_off(VOID)
{
    OSA_print("c: Close the led.");

    (VOID)LED_drv_off();

    return;
}

static VOID LED_proc_blink(VOID)
{
    OSA_print("b: Set the led blink.");

    (VOID)LED_drv_blink();

    return;
}

static VOID LED_proc_show_help(VOID)
{
    OSA_print("Led proc usage:");
    OSA_print("  o: Open the led.");
    OSA_print("  c: Close the led.");
    OSA_print("  b: Set the led blink.");
    OSA_print("  h: Show this message.");

    return;
}

static int LED_proc_open(struct inode *inode, struct file *file)
{
    return OSA_SOK;
}

static ssize_t LED_proc_read(struct file *file, char __user *user_buffer, size_t len, loff_t *offset)
{
    LED_proc_show_help();
    return OSA_SOK;
}

static ssize_t LED_proc_write(struct file *file, const char __user *user_buffer, size_t len, loff_t *offset)
{
    INT8 cmd = '\0';

    if (len != (PDL_LED_PROC_CMD_LEN + 1))
    {
        OSA_ERROR("Invalid argument.");
        LED_proc_show_help();
        return len;
    }

    if (copy_from_user(&cmd, user_buffer, PDL_LED_PROC_CMD_LEN))
    {
        return -EFAULT;
    }

    switch (cmd)
    {
        case 'o':
        case 'O':
        {
            LED_proc_on();
            break;
        }
        case 'c':
        case 'C':
        {
            LED_proc_off();
            break;
        }
        case 'b':
        case 'B':
        {
            LED_proc_blink();
            break;
        }
        case 'h':
        case 'H':
        {
            LED_proc_show_help();
            break;
        }
        default:
        {
            OSA_ERROR("Invalid argument.");
            LED_proc_show_help();
            break;
        }
    }

    return len;
}

static int LED_proc_release(struct inode *inode, struct file *file)
{
    return OSA_SOK;
}

struct proc_ops g_stLedProcFops=
{
    .proc_open = LED_proc_open,
    .proc_read = LED_proc_read,
    .proc_write = LED_proc_write,
    .proc_release = LED_proc_release,
};


INT32 PDL_LED_init(VOID)
{
    INT32 iRet = OSA_EFAIL;

    OSA_DEBUG("");

    // 创建字符设备
    memset(&g_stLedCdev, 0, sizeof(OSA_cdev_t));
    g_stLedCdev.class = PDL_get_class_handler();
    g_stLedCdev.count = PDL_LED_CDEV_COUNT;
    g_stLedCdev.pstFileOps = &g_stLedCdevFops;
    iRet = OSA_cdev_create(&g_stLedCdev, PDL_LED_CDEV_NAME);
    if (OSA_isNotSuccess(iRet))
    {
        OSA_ERROR("OSA_cdev_create failed.");
        return OSA_EFAIL;
    }

    /* 创建proc文件 */
    memset(&g_stLedProcFile, 0, sizeof(OSA_proc_t));
    g_stLedProcFile.fops = &g_stLedProcFops;
    iRet = OSA_proc_create(&g_stLedProcFile, PDL_LED_PROC_NAME);
    if (OSA_isNotSuccess(iRet))
    {
        OSA_ERROR("OSA_proc_create failed.");
        return OSA_EFAIL;
    }

    iRet = LED_drv_setup();
    if (OSA_isNotSuccess(iRet))
    {
        OSA_ERROR("LED_drv_setup failed.");
        return OSA_EFAIL;
    }

    OSA_INFO("PDL_LED_init success.");

    return OSA_SOK;
}


VOID PDL_LED_exit(VOID)
{
    OSA_DEBUG("");

    OSA_proc_delete(&g_stLedProcFile);
    OSA_cdev_delete(&g_stLedCdev);

    return;
}


