#! /usr/bin/bash

# add grpc bin
export MY_INSTALL_DIR=$HOME/myrpc/output
export PATH="$MY_INSTALL_DIR/bin:$PATH"

PRJ_NAME="hello"

# 如果提供了第一个参数，则使用该参数作为项目名称
if [ $# -ge 1 ] && [ -n "$1" ]; then
    PRJ_NAME="$1"
fi

# 导出项目目录环境变量
export HELLO_DIR="$HOME/myrpc/prjs/$PRJ_NAME"

# 1. proto file
# hello.pb.cc      hello.pb.h       # proto protobuf source files
# hello.grpc.pb.cc hello.grpc.pb.h  # network, server and stub grpc source files
# protoc --cpp_out=$HELLO_DIR \
#        --grpc_out=$HELLO_DIR \
#        --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` \
#        --proto_path=$HELLO_DIR \
#        hello.proto
# 2. server 
# 3. client


# cmake
cd $HELLO_DIR
mkdir -p build
pushd build
cmake ..
make -j $(nproc)
popd

