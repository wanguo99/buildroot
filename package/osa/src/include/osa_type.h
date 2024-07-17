#ifndef _OSA_TYPE_H_
#define _OSA_TYPE_H_

#include <linux/types.h>

#define OSA_SOK             (0)                     /* 成功返回值 */
#define OSA_EFAIL           (-1)                    /* 失败返回值 */

#define OSA_isSuccess(x)    ((x) == OSA_SOK)        /* 判断返回值是否为success */
#define OSA_isNotSuccess(x) ((x) != OSA_SOK)        /* 判断返回值是否不为success */
#define OSA_isFail(x)       ((x) == OSA_EFAIL)      /* 判断返回值是否为failed */



#define CONST               const                   /* const类型 */
#define STATIC              static                  /* static类型 */
#define EXTERN              extern                  /* extern类型 */


typedef void                VOID;                   /* void数据类型 */
typedef bool                BOOL;                   /* bool数据类型 */

typedef char                INT8;                   /* 8字节有符号数据类型 */
typedef short               INT16;                  /* 16字节有符号数据类型 */
typedef int                 INT32;                  /* 32字节有符号数据类型 */
typedef long long           INT64;                  /* 64字节有符号数据类型 */


typedef unsigned char       UINT8;                  /* 8字节无符号数据类型 */
typedef unsigned short      UINT16;                 /* 16字节无符号数据类型 */
typedef unsigned int        UINT32;                 /* 32字节无符号数据类型 */
typedef unsigned long long  UINT64;                 /* 64字节无符号数据类型 */

#endif  /* _OSA_TYPE_H_ */
