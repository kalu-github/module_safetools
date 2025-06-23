cmake_minimum_required(VERSION 3.22.1)

project("openssl_kit")

# 设置构建类型
set(CMAKE_BUILD_TYPE "Release")

# 1. 栈保护机制
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fstack-protector-strong")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fstack-protector-strong")

# 2. 地址空间布局随机化
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

# 3. 忽略警告
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror=unknown-warning-option")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=unknown-warning-option")

# 正确配置头文件路径
include_directories(
        openssl-1.1.1w/include
        core
        util
)

# 添加OpenSSL子项目
add_subdirectory(openssl-1.1.1w)

# 创建共享库
add_library(
        openssl_kit
        SHARED

        # core
        core/openssl_aes.cpp
        core/openssl_md5.cpp
        core/openssl_base64.cpp
        core/openssl_hmac_sha265.cpp

        # util
        util/common_util.c
        util/aes_util.c
        util/md5_util.c
        util/base64_util.c
        util/hmac_sha265_util.c

        # jni
        jni.c)

# 链接OpenSSL库
target_link_libraries(
        openssl_kit

        # 根据OpenSSL官方CMake配置，使用正确的库名
        crypto
        ssl

        # 系统库
        log)