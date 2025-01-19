#!/bin/sh

# 获取全局范围的 IPv4 地址
IP=$(ip -4 addr show scope global | grep -o 'inet [^ ]*' | awk '{print $2}' | cut -d/ -f1 | head -n 1)

# 检查是否获取到 IP 地址
if [ -z "$IP" ]; then
    # 如果没有找到 IP 地址，返回错误信息
    echo "Content-Type: application/json"
    echo
    echo '{"error": "No IP address found"}'
    exit 1
fi

# 构造 WebSocket URI（假设使用 wss:// 协议）
WS_URI="ws://${IP}:8080"

# 输出 JSON 格式的 WebSocket URI
echo "{\"wsUri\": \"$WS_URI\"}"

