#ifndef _OSA_TYPE_H_
#define _OSA_TYPE_H_

#define OSA_SOK             (0)                         /* 成功返回值 */
#define OSA_EFAIL           (-1)                        /* 失败返回值 */

#define OSA_CONST           const                       /* const类型 */
#define OSA_STATIC          static                      /* static类型 */

typedef void                OSA_VOID;                   /* void数据类型 */
typedef bool                OSA_BOOL;                   /* bool数据类型 */

typedef char                OSA_INT8;                   /* 8字节有符号数据类型 */
typedef short               OSA_INT16;                  /* 16字节有符号数据类型 */
typedef int                 OSA_INT32;                  /* 32字节有符号数据类型 */
typedef long long           OSA_INT64;                  /* 64字节有符号数据类型 */


typedef unsigned char       OSA_UINT8;                  /* 8字节无符号数据类型 */
typedef unsigned short      OSA_UINT16;                 /* 16字节无符号数据类型 */
typedef unsigned int        OSA_UINT32;                 /* 32字节无符号数据类型 */
typedef unsigned long long  OSA_UINT64;                 /* 64字节无符号数据类型 */

#endif  /* _OSA_TYPE_H_ */
