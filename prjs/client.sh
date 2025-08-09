echo "================== http / ======================"
curl -v  --http2-prior-knowledge http://127.0.0.1:9000/
echo 
echo "================== grpc Hello ======================"
./hello/build/hello_client --target=localhost:9000
echo "================== grpc Robot ======================"
./robot/build/robot_client --target=localhost:9000
echo "========================================"


