
# 定义要执行的命令数组
commands=(
    "./hello/build/hello_server --target=localhost:9090 &"
    "./robot/build/robot_server --target=localhost:9091 &"
    "ps -aux|grep _server"
)
for index in "${!commands[@]}"; do
    # 计算命令序号（从1开始）
    cmd_num=$((index + 1))
    
    # 打印当前执行的命令信息
    echo "正在执行第 $cmd_num 条命令: ${commands[$index]}"
    
    # 执行命令并捕获退出码
    eval "${commands[$index]}"

done

sudo nginx -s stop
sudo nginx -c ~/myrpc/prjs/nginx_grpc.conf

