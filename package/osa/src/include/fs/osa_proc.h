#ifndef _OSA_PROC_H_
#define _OSA_PROC_H_

#include <linux/proc_fs.h>

typedef struct tagOSA_proc
{
    struct proc_ops *fops;
    struct proc_dir_entry *entry;
} OSA_proc_t;


extern INT32 OSA_proc_create(OSA_proc_t *pstProc, const INT8 *pcName);
extern VOID OSA_proc_delete(OSA_proc_t *pstProc);

#endif // _OSA_PROC_H_
