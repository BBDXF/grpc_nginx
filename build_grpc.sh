#! /usr/bin/bash

sudo apt install -y build-essential autoconf libtool pkg-config net-tools

mkdir output
export MY_INSTALL_DIR=$HOME/myrpc/output
export PATH="$MY_INSTALL_DIR/bin:$PATH"

read -p "If your can visit github, enter to continue ... or config proxy then use this script!"

# proxy setting
#export https_proxy=http://192.168.2.94:7897 http_proxy=http://192.168.2.94:7897 all_proxy=socks5://192.168.2.94:7897

# clone grpc
# git clone --recurse-submodules -b v1.74.0 --depth 1 --shallow-submodules https://github.com/grpc/grpc
git clone --depth 1  https://github.com/grpc/grpc
git submodule status
git submodule update --recursive

# build & install
cd grpc
mkdir -p cmake/build
pushd cmake/build
cmake -DgRPC_INSTALL=ON \
      -DgRPC_BUILD_TESTS=OFF \
      -DCMAKE_CXX_STANDARD=17 \
      -DCMAKE_INSTALL_PREFIX=$MY_INSTALL_DIR \
      ../..
make -j $(nproc)
make install
popd

# build helloworld
cd examples/cpp/helloworld
mkdir -p cmake/build
pushd cmake/build
cmake -DCMAKE_PREFIX_PATH=$MY_INSTALL_DIR ../..
make -j $(nproc)
popd


# https://grpc.org.cn/docs/languages/cpp/quickstart/#install-grpc