cmake_minimum_required(VERSION 3.15)
set(PROJECT_NAME "cpp_notes")
project(${PROJECT_NAME})

option(PRINT_CMAKE_LOG "print cmake log" ON)

# 设置可执行文件保存的路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

if (PRINT_CMAKE_LOG)
    message("--------------------------------")
    message(STATUS ${PROJECT_NAME}, " BUILD_TYPE: " ${CMAKE_BUILD_TYPE})
endif (PRINT_CMAKE_LOG)

#set(INC_DIR include)
## 包含头文件目录
#include_directories(${INC_DIR})

# 相对src目录下搜索
#file(GLOB_RECURSE SRC_LIST RELATIVE "src" "*.cpp")

# 把当前目录下的所有源文件添加到SRC_LIST中
aux_source_directory(. SRC_LIST)
# list追加变量的值
list(APPEND SRC_LIST main.cpp)

add_subdirectory(src)

# 添加一个可执行程序
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME}
        src_module)
