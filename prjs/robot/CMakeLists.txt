cmake_minimum_required(VERSION 3.15)
project(robot_grpc)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --------------------------
# 自定义gRPC和Protobuf路径
# --------------------------
# 请修改为你的实际安装路径
set(CUSTOM_GRPC_PATH "../../output")              # gRPC安装根目录
set(CUSTOM_PROTOBUF_PATH "${CUSTOM_GRPC_PATH}")       # 如果Protobuf与gRPC一起安装，可复用此路径
# 如果Protobuf单独安装，取消下面一行注释并设置
# set(CUSTOM_PROTOBUF_PATH "/path/to/your/protobuf/install")

# 添加自定义库的查找路径
list(INSERT CMAKE_PREFIX_PATH 0 ${CUSTOM_GRPC_PATH} ${CUSTOM_PROTOBUF_PATH})

# 查找gRPC和Protobuf库（优先使用自定义路径）
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)

# 验证是否使用了自定义路径
message(STATUS "Using Protobuf include dir: ${Protobuf_INCLUDE_DIRS}")
message(STATUS "Using Protobuf library: ${Protobuf_LIBRARIES}")
message(STATUS "Using gRPC include dir: ${gRPC_INCLUDE_DIRS}")
message(STATUS "Using gRPC library: ${gRPC_LIBRARIES}")

# 指定proto文件
set(PROTO_FILE robot.proto)

# 生成proto相关文件
# 获取protoc和grpc_cpp_plugin的路径（优先使用自定义路径）
get_target_property(protobuf_location protobuf::protoc LOCATION)
get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)

# 生成.pb.h和.pb.cc文件
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/robot.pb.h ${CMAKE_CURRENT_BINARY_DIR}/robot.pb.cc
    COMMAND ${protobuf_location}
    ARGS --cpp_out=${CMAKE_CURRENT_BINARY_DIR}
         -I${CMAKE_CURRENT_SOURCE_DIR}
         ${CMAKE_CURRENT_SOURCE_DIR}/${PROTO_FILE}
    DEPENDS ${PROTO_FILE}
)

# 生成.grpc.pb.h和.grpc.pb.cc文件
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/robot.grpc.pb.h ${CMAKE_CURRENT_BINARY_DIR}/robot.grpc.pb.cc
    COMMAND ${protobuf_location}
    ARGS --grpc_out=${CMAKE_CURRENT_BINARY_DIR}
         --plugin=protoc-gen-grpc=${grpc_cpp_plugin_location}
         -I${CMAKE_CURRENT_SOURCE_DIR}
         ${CMAKE_CURRENT_SOURCE_DIR}/${PROTO_FILE}
    DEPENDS ${PROTO_FILE} ${CMAKE_CURRENT_BINARY_DIR}/robot.pb.h
)

# 生成的文件列表
set(GENERATED_FILES
    ${CMAKE_CURRENT_BINARY_DIR}/robot.pb.h
    ${CMAKE_CURRENT_BINARY_DIR}/robot.pb.cc
    ${CMAKE_CURRENT_BINARY_DIR}/robot.grpc.pb.h
    ${CMAKE_CURRENT_BINARY_DIR}/robot.grpc.pb.cc
)

# 包含二进制目录（生成的头文件在这里）
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${gRPC_INCLUDE_DIRS} ${Protobuf_INCLUDE_DIRS})

# 构建服务器
add_executable(robot_server
    app_server.cpp
    ${GENERATED_FILES}
)

# 构建客户端
add_executable(robot_client
    app_client.cpp
    ${GENERATED_FILES}
)

# 链接自定义库
target_link_libraries(robot_server
    gRPC::grpc++_reflection
    gRPC::grpc++
    protobuf::libprotobuf
    absl::flags_parse
    absl::log_initialize
)

target_link_libraries(robot_client
    gRPC::grpc++_reflection
    gRPC::grpc++
    protobuf::libprotobuf
    absl::flags_parse
    absl::log_initialize
)

# 可选：如果运行时需要找到自定义库，设置RPATH
set_target_properties(robot_server PROPERTIES
    INSTALL_RPATH "${CUSTOM_GRPC_PATH}/lib;${CUSTOM_PROTOBUF_PATH}/lib"
    BUILD_WITH_INSTALL_RPATH TRUE
)

set_target_properties(robot_client PROPERTIES
    INSTALL_RPATH "${CUSTOM_GRPC_PATH}/lib;${CUSTOM_PROTOBUF_PATH}/lib"
    BUILD_WITH_INSTALL_RPATH TRUE
)
