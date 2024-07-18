#ifndef _OSA_CDEV_H_
#define _OSA_CDEV_H_

#include <linux/device.h>
#include <linux/device/class.h>


extern INT32 OSA_cdev_create(VOID);

extern INT32 OSA_cdev_delete(VOID);

#endif // _OSA_CDEV_H_
