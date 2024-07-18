# Makefile for building wgtool application

# 目标文件名
TARGET = wgtool

# 源文件
SRCS = main.c

# 编译器和编译选项
ARCH ?= arm

CC = $(CROSS_COMPILE)gcc

CFLAGS = -Wall -O2
CFLAGS += -I./include

LDFLAGS =

# 对象文件
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

# 默认目标
all: $(TARGET)

# 链接
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

# 编译并生成依赖文件
%.o: %.c
	@echo "CROSS_COMPILE: $(CROSS_COMPILE)"
	$(CC) $(CFLAGS) -MMD -c $< -o $@

-include $(DEPS)

# 清理目标文件和可执行文件
clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)

.PHONY: all clean
