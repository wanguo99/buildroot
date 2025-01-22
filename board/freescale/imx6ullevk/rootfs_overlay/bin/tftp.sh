#!/bin/bash

# 检查是否提供了足够的参数
if [ "$#" -lt 2 ]; then
    echo "Usage: $0 <target_directory> <file1> [<file2> ...]"
    exit 1
fi

# 获取目标目录
TARGET_DIR="$1"
shift # 移除第一个参数，剩下的参数为文件列表

# 检查并创建目标目录（如果不存在）
if [ ! -d "$TARGET_DIR" ]; then
    mkdir -p "$TARGET_DIR"
    if [ $? -ne 0 ]; then
        echo "Failed to create target directory: $TARGET_DIR"
        exit 1
    fi
fi

# TFTP 服务器地址
TFTP_SERVER="10.10.0.201"

# 循环下载文件并将它们复制到目标目录
for FILE in "$@"; do
    # 提取文件名（假设参数直接提供文件名）
    FILENAME=$(basename "$FILE")

    # 使用 TFTP 下载文件到当前工作目录
    tftp -gr "$FILENAME" "$TFTP_SERVER"
    if [ $? -ne 0 ]; then
        echo "Failed to download file: $FILENAME from $TFTP_SERVER"
        continue
    fi

    # 检查文件是否成功下载
    if [ ! -f "$FILENAME" ]; then
        echo "Downloaded file not found: $FILENAME"
        continue
    fi

    # 将文件复制到目标目录
    cp "$FILENAME" "$TARGET_DIR/"
    if [ $? -ne 0 ]; then
        echo "Failed to copy file to target directory: $FILENAME"
    else
        echo "Successfully copied $FILENAME to $TARGET_DIR"
    fi

    # 可选：删除临时下载文件（如果不需要保留）
    rm -f "$FILENAME"
done
