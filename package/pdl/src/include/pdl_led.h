#ifndef _PDL_LED_H_
#define _PDL_LED_H_

#include "osa/osa.h"

#define PDL_LED_CDEV_NAME       "pdl_led"
#define PDL_LED_PROC_NAME       "pdl_led"

extern INT32 PDL_ledInit(VOID);
extern INT32 PDL_ledExit(VOID);

#endif // _PDL_LED_H_
