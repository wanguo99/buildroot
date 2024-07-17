TARGET := pdl
obj-m += $(TARGET).o

ARCH ?= arm
CROSS_COMPILE ?= arm-linux-gnueabihf-

PWD :=$(shell pwd)

SRCS = pdl_core.c \
	   led/pdl_led.c 

$(TARGET)-objs = $(SRCS:.c=.o)

EXTRA_CFLAGS += -I$(PWD)/include \
				-I$(STAGING_DIR)/include \
				-I$(STAGING_DIR)/usr/include

EXTRA_CFLAGS += -fmacro-prefix-map=$(PWD)/=

# 额外的符号表（其他模块导出符号）
KBUILD_EXTRA_SYMBOLS_PATH = $(STAGING_DIR)/usr/share/Module.symvers
KBUILD_EXTRA_SYMBOLS += $(KBUILD_EXTRA_SYMBOLS_PATH)/osa/Module.symvers \
						$(KBUILD_EXTRA_SYMBOLS_PATH)/pconfig/Module.symvers \
						$(KBUILD_EXTRA_SYMBOLS_PATH)/hal/Module.symvers

.PHONY: all clean

all:
	$(MAKE) -C $(LINUX_DIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) M=$(PWD) modules
clean:
	$(MAKE) -C $(LINUX_DIR) M=$(PWD) clean
