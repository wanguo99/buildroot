#ifndef _WG_TEST_LED_H_
#define _WG_TEST_LED_H_

#define LED_IOC_MAGIC 'L'
#define LED_IOC_ON    _IO(LED_IOC_MAGIC, 0)
#define LED_IOC_OFF   _IO(LED_IOC_MAGIC, 1)
#define LED_IOC_BLINK _IO(LED_IOC_MAGIC, 2)


#endif // _WG_TEST_LED_H_

