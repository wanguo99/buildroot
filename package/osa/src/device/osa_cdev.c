#include "osa.h"
#include "osa_cdev_priv.h"

INT32 OSA_cdev_create(OSA_cdev_t *pstCdev, INT8 *pcName)
{
    if (OSA_isNull(pstCdev) \
        || OSA_isNull(pstCdev->class) \
        || OSA_isNull(pstCdev->pstFileOps)
        || OSA_isNull(pcName)
        || OSA_isZero(pstCdev->count))
    {
        return OSA_EINVAL;
    }

    if (OSA_isZero(pstCdev->major))
    {
        if (alloc_chrdev_region(&pstCdev->dev_id, 0, pstCdev->count, pcName) < 0)
        {
            return OSA_EFAIL;
        }
        pstCdev->major = MAJOR(pstCdev->dev_id);
        pstCdev->minor = MINOR(pstCdev->dev_id);
    }
    else
    {
        pstCdev->dev_id = MKDEV(pstCdev->major, 0);
        if (register_chrdev_region(pstCdev->dev_id, pstCdev->count, pcName) < 0)
        {
            return OSA_EFAIL;
        }
    }

    cdev_init(&pstCdev->cdev, pstCdev->pstFileOps);
    pstCdev->cdev.owner = THIS_MODULE;

    if (cdev_add(&pstCdev->cdev, pstCdev->dev_id, pstCdev->count) < 0)
    {
        unregister_chrdev_region(pstCdev->dev_id, pstCdev->count);
        return OSA_EFAIL;
    }

    pstCdev->device = device_create(pstCdev->class, NULL, pstCdev->dev_id, NULL, pcName);
    if (IS_ERR_OR_NULL(pstCdev->device))
    {
        cdev_del(&pstCdev->cdev);
        unregister_chrdev_region(pstCdev->dev_id, pstCdev->count);
        return OSA_EFAIL;
    }

    OSA_INFO("Character device registered: major=%d, minor=%d", pstCdev->major, pstCdev->minor);

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

    device_destroy(pstCdev->class, pstCdev->dev_id);
    cdev_del(&pstCdev->cdev);
    unregister_chrdev_region(pstCdev->dev_id, pstCdev->count);

    return;
}
EXPORT_SYMBOL_GPL(OSA_cdev_delete);
