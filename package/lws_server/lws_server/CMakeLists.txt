cmake_minimum_required(VERSION 3.10)
project(LwsServer VERSION 1.0 LANGUAGES C)

# 定义可执行文件
add_executable(lws_server lws_server.c)

# 链接库
target_link_libraries(lws_server PRIVATE
    websockets
    cjson
)

# 安装规则
install(TARGETS lws_server DESTINATION bin)
