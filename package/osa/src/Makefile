
TARGET := osa
obj-m += $(TARGET).o

ARCH ?= arm
CROSS_COMPILE ?= arm-wanguo-linux-gnueabihf-

PWD :=$(shell pwd)

$(TARGET)-objs += ./core/core.o \
				./log/osa_log.o
EXTRA_CFLAGS += -I$(PWD)/include


.PHONY: all clean

all:
	$(MAKE) -C '$(LINUX_DIR)' ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) M='$(PWD)' modules
clean:
	$(MAKE) -C '$(LINUX_DIR)' M='$(PWD)' clean
