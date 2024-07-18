#include "osa/osa.h"
#include "pdl.h"
#include "pdl_key_priv.h"


/*************************************************************************/
/*                               全局变量                                    */
/*************************************************************************/
OSA_cdev_t g_stKeyCdev;
OSA_proc_t g_stKeyProcFile;


/*************************************************************************/
/*                              字符设备文件                                   */
/*************************************************************************/
static int KEY_cdev_open(struct inode *inode, struct file *file)
{
    OSA_INFO("");
    return 0;
}

static int KEY_cdev_release(struct inode *inode, struct file *file)
{
    OSA_INFO("");
    return 0;
}

static ssize_t KEY_cdev_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    OSA_INFO("");
    return 1;
}

static ssize_t KEY_cdev_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    char cmd;

    OSA_INFO("");

    if (copy_from_user(&cmd, buf, 1))
        return -EFAULT;

    switch(cmd)
    {
        default:
            pr_info("Invalid command\n");
            return -EINVAL;
    }
    return count;
}

static long KEY_cdev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    OSA_INFO("");
    return 0;
}

struct file_operations g_stKeyCdevFops = {
    .owner = THIS_MODULE,
    .open = KEY_cdev_open,
    .release = KEY_cdev_release,
    .read = KEY_cdev_read,
    .write = KEY_cdev_write,
    .unlocked_ioctl = KEY_cdev_ioctl,
};


/*************************************************************************/
/*                              proc文件                                   */
/*************************************************************************/
static VOID KEY_proc_show_help(VOID)
{
    OSA_print("Key proc usage:");
    OSA_print("  h: Show this message.");

    return;
}

static int KEY_proc_open(struct inode *inode, struct file *file)
{
    OSA_INFO("");
    return OSA_SOK;
}

static ssize_t KEY_proc_read(struct file *file, char __user *user_buffer, size_t len, loff_t *offset)
{
    OSA_INFO("");
    KEY_proc_show_help();
    return OSA_SOK;
}

static ssize_t KEY_proc_write(struct file *file, const char __user *user_buffer, size_t len, loff_t *offset)
{
    INT8 cmd = '\0';

    OSA_INFO("");

    if (len != (PDL_KEY_PROC_CMD_LEN + 1))
    {
        OSA_ERROR("Invalid argument.");
        KEY_proc_show_help();
        return len;
    }

    if (copy_from_user(&cmd, user_buffer, PDL_KEY_PROC_CMD_LEN))
    {
        return -EFAULT;
    }

    switch (cmd)
    {
        case 'h':
        case 'H':
        {
            KEY_proc_show_help();
            break;
        }
        default:
        {
            OSA_ERROR("Invalid argument.");
            KEY_proc_show_help();
            break;
        }
    }

    return len;
}

static int KEY_proc_release(struct inode *inode, struct file *file)
{
    OSA_INFO("");
    return OSA_SOK;
}

struct proc_ops g_stKeyProcFops=
{
    .proc_open = KEY_proc_open,
    .proc_read = KEY_proc_read,
    .proc_write = KEY_proc_write,
    .proc_release = KEY_proc_release,
};


INT32 PDL_KEY_init(VOID)
{
    INT32 iRet = OSA_EFAIL;

    OSA_DEBUG("");

    // 创建字符设备
    memset(&g_stKeyCdev, 0, sizeof(OSA_cdev_t));
    g_stKeyCdev.class = PDL_get_class_handler();
    g_stKeyCdev.count = PDL_KEY_CDEV_COUNT;
    g_stKeyCdev.pstFileOps = &g_stKeyCdevFops;
    iRet = OSA_cdev_create(&g_stKeyCdev, PDL_KEY_CDEV_NAME);
    if (OSA_isNotSuccess(iRet))
    {
        OSA_ERROR("OSA_cdev_create faikey.");
        return OSA_EFAIL;
    }

    /* 创建proc文件 */
    memset(&g_stKeyProcFile, 0, sizeof(OSA_proc_t));
    g_stKeyProcFile.fops = &g_stKeyProcFops;
    iRet = OSA_proc_create(&g_stKeyProcFile, PDL_KEY_PROC_NAME);
    if (OSA_isNotSuccess(iRet))
    {
        OSA_ERROR("OSA_proc_create faikey.");
        return OSA_EFAIL;
    }

    OSA_INFO("PDL_KEY_init success.");

    return OSA_SOK;
}


VOID PDL_KEY_exit(VOID)
{
    OSA_DEBUG("");

    OSA_proc_delete(&g_stKeyProcFile);
    OSA_cdev_delete(&g_stKeyCdev);

    return;
}


