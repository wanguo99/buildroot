#include "osa.h"
#include "osa_cdev_priv.h"


INT32 OSA_cdev_create(OSA_cdev_t *pstCdev, INT8 *pcName)
{
    if (OSA_isNull(pstCdev) \
        || OSA_isNull(pstCdev->class) \
        || OSA_isNull(pstCdev->pstFileOps)
        || OSA_isNull(pcName))
    {
        return OSA_EINVAL;
    }

    OSA_INFO("");

    if (pstCdev->major)
    {
        pstCdev->dev_id = MKDEV(pstCdev->major, 0);  /* 次设备号为0 */
        register_chrdev_region(pstCdev->dev_id, pstCdev->count, pcName);
    }
    else
    {
        alloc_chrdev_region(&pstCdev->dev_id, 0, pstCdev->count, pcName);
        pstCdev->major = MAJOR(pstCdev->dev_id);
        pstCdev->minor = MINOR(pstCdev->dev_id);
    }

    cdev_init(&pstCdev->cdev, pstCdev->pstFileOps);
    pstCdev->cdev.owner = THIS_MODULE;

    cdev_add(&pstCdev->cdev, pstCdev->dev_id, pstCdev->count);

    pstCdev->device = device_create(pstCdev->class, NULL, \
                                    pstCdev->dev_id, NULL, pcName);
    if (IS_ERR(pstCdev->device))
    {
        return OSA_EFAIL;
    }

    return OSA_SOK;
}
EXPORT_SYMBOL_GPL(OSA_cdev_create);


VOID OSA_cdev_delete(OSA_cdev_t *pstCdev)
{
    if (OSA_isNull(pstCdev) || OSA_isNull(pstCdev->device))
    {
        return;
    }

    OSA_INFO("");

    cdev_del(&pstCdev->cdev);

    unregister_chrdev_region(pstCdev->dev_id, pstCdev->count);

    device_destroy(pstCdev->class, pstCdev->dev_id);

    return;
}
EXPORT_SYMBOL_GPL(OSA_cdev_delete);

