#!/bin/bash

# 查找以 _server 结尾的进程PID（排除grep自身）
pids=$(ps aux | grep -E "[^[:space:]]+_server " | grep -v grep | awk '{print $2}')

if [ -z "$pids" ]; then
    echo "没有找到以 _server 结尾的进程"
    exit 0
fi

# 强制杀死所有匹配进程
echo "找到进程: $pids，正在强制终止..."
kill -9 $pids 2>/dev/null
