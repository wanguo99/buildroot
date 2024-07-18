#ifndef _OSA_ERRNO_H_
#define _OSA_ERRNO_H_

#include <linux/errno.h>

#define OSA_SOK             (0)                     /* 成功返回值 */
#define OSA_EFAIL           (-1)                    /* 标准失败返回值 */

#define OSA_EINVAL          (-21)                   /* invalid argument */

#define OSA_isSuccess(x)    ((x) == OSA_SOK)        /* 判断返回值是否为success */
#define OSA_isNotSuccess(x) ((x) != OSA_SOK)        /* 判断返回值是否不为success */
#define OSA_isFail(x)       ((x) == OSA_EFAIL)      /* 判断返回值是否为failed */

#define OSA_isNull(ptr)     ((ptr) == NULL)         /* 判断指针是否为空 */
#define OSA_isNotNull(ptr)  ((ptr) != NULL)         /* 判断指针是否不为空 */

#endif  /* _OSA_ERRNO_H_ */
