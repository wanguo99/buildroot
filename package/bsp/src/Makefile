TARGET := bsp
obj-m += $(TARGET).o

ARCH ?= arm
CROSS_COMPILE ?= arm-linux-gnueabihf-

PWD :=$(shell pwd)

$(TARGET)-objs += core/core.o
EXTRA_CFLAGS += -I$(PWD)/include \
				-I$(STAGING_DIR)/include \
				-I$(STAGING_DIR)/usr/include

EXTRA_CFLAGS += -fmacro-prefix-map=$(PWD)/=


.PHONY: all clean

all:
	$(MAKE) -C '$(LINUX_DIR)' ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) M='$(PWD)' modules
clean:
	$(MAKE) -C '$(LINUX_DIR)' M='$(PWD)' clean
