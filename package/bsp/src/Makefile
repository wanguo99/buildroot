
TARGET := bsp
obj-m += $(TARGET).o

ARCH ?= arm
CROSS_COMPILE ?= arm-wanguo-linux-gnueabihf-

CC := $(CROSS_COMPILE)gcc
LD := $(CROSS_COMPILE)ld
AR := $(CROSS_COMPILE)ar
AS := $(CROSS_COMPILE)as
NM := $(CROSS_COMPILE)nm
OBJCOPY := $(CROSS_COMPILE)objcopy
OBJDUMP := $(CROSS_COMPILE)objdump
export CC LD AD AS NM OBJCOPY OBJDUMP

PWD :=$(shell pwd)
$(TARGET)-objs += ./core/core.o

.PHONY: all clean
all:
	echo "$(SRC_DIR)/core.o"
	$(MAKE) -C '$(LINUX_DIR)' ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) M='$(PWD)' modules
clean:
	$(MAKE) -C '$(LINUX_DIR)' M='$(PWD)' clean
