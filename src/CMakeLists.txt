cmake_minimum_required(VERSION 3.15)
set(PROJECT_NAME "src_module")

# 递归搜索
file(GLOB_RECURSE H_SRCS "*.h")
file(GLOB_RECURSE CXX_SRCS "*.cpp")

list(APPEND SRCS ${H_SRCS} ${CXX_SRCS})
# 生成链接库
add_library(${PROJECT_NAME} STATIC ${SRCS})