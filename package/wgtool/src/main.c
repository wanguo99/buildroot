#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "wg_test.h"

#define PDL_LED_CDEV_FILE   "/dev/pdl_led"

int main()
{
    int fd = -1;

    if (0 != access(PDL_LED_CDEV_FILE, F_OK))
    {
        printf("No such file or directory: %s. \n", PDL_LED_CDEV_FILE);
    }

    fd = open(PDL_LED_CDEV_FILE, O_RDWR);
    if (fd < 0)
	{
        perror("open");
        return -1;
    }
    ioctl(fd, LED_IOC_ON);
    sleep(1);
    ioctl(fd, LED_IOC_OFF);
    sleep(1);
    ioctl(fd, LED_IOC_BLINK);
    close(fd);
    return 0;
}
