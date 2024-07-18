#include "osa.h"

#include "osa_proc_priv.h"


INT32 OSA_proc_create(OSA_proc_t *pstProcFile, const INT8 *pcName)
{
    if (OSA_isNull(pstProcFile) \
        || OSA_isNull(pstProcFile->fops) \
        || OSA_isNull(pcName))
    {
        return OSA_EINVAL;
    }

    OSA_INFO("");

    pstProcFile->entry = proc_create(pcName, 0666, NULL, pstProcFile->fops);
    if (OSA_isNull(pstProcFile->entry))
    {
        OSA_ERROR("Error: Could not initialize /proc/%s", pcName);
    }
    else
    {
        OSA_INFO("/proc/%s created", pcName);
    }

    return OSA_SOK;
}
EXPORT_SYMBOL_GPL(OSA_proc_create);

VOID OSA_proc_delete(OSA_proc_t *pstProcFile)
{
    if (OSA_isNull(pstProcFile) || OSA_isNull(pstProcFile->entry))
    {
        return;
    }

    OSA_INFO("");

    proc_remove(pstProcFile->entry);

    return;
}
EXPORT_SYMBOL_GPL(OSA_proc_delete);


