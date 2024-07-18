
#include "osa.h"
#include "osa_class_priv.h"


INT32 OSA_class_create(OSA_class_t **ppstClass, const INT8 *pcName)
{
    if (OSA_isNull(ppstClass) || OSA_isNull(pcName))
    {
        return OSA_EINVAL;
    }

    *ppstClass = class_create(pcName);
    if (IS_ERR(*ppstClass))
    {
        return OSA_EFAIL;
    }

    return OSA_SOK;
}
EXPORT_SYMBOL_GPL(OSA_class_create);


VOID OSA_class_delete(OSA_class_t **ppstClass)
{
    if (OSA_isNull(ppstClass))
    {
        return;
    }

    class_destroy(*ppstClass);

    return;
}
EXPORT_SYMBOL_GPL(OSA_class_delete);
