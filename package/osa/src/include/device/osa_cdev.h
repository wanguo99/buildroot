#ifndef _OSA_CDEV_H_
#define _OSA_CDEV_H_

/*************************************************************************/
/*                             头文件                                       */
/*************************************************************************/
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/device/class.h>


/*************************************************************************/
/*                             宏定义                                       */
/*************************************************************************/

#define OSA_CDEV_NAME_MAX_LEN       (16)



/*************************************************************************/
/*                        结构体、数据类型                                       */
/*************************************************************************/


typedef struct tagOSA_cdev_t
{
    struct file_operations * pstFileOps;    /* 文件操作回调 */
    UINT32 count;                           /* 设备数量 */
    INT32 major;                            /* 主设备号 */
    INT32 minor;                            /* 次设备号 */
    dev_t dev_id;                            /* 设备ID */
    struct cdev cdev;                       /* cdev */
    struct class *class;                    /* class */
    struct device *device;                  /* 设备句柄 */
} OSA_cdev_t;




/*************************************************************************/
/*                             函数声明                                      */
/*************************************************************************/


extern INT32 OSA_cdev_create(OSA_cdev_t *pstCdev, INT8 *pcName);
extern VOID OSA_cdev_delete(OSA_cdev_t *ppstCdev);

#endif // _OSA_CDEV_H_
