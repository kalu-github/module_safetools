cmake_minimum_required(VERSION 3.10.2)

# 编译Release(Release Debug)
set(CMAKE_BUILD_TYPE "Release")

# 编译警告
add_definitions(-Wno-format-security)
#
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Os -Wall -s -fPIC")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Os -Wall -s -fPIC")

# 头文件
include_directories(

        openssl
        openssl/include

        util)

add_library(
        openssl-crypto
        STATIC
        IMPORTED)
set_target_properties(
        openssl-crypto
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/openssl/${ANDROID_ABI}/lib/libcrypto.a)
add_library(
        openssl-ssl
        STATIC
        IMPORTED)
set_target_properties(
        openssl-ssl
        PROPERTIES IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/openssl/${ANDROID_ABI}/lib/libssl.a)

add_library(
        # Sets the name of the library.
        tool

        # Sets the library as a shared library.
        SHARED

        # openssl
        openssl/openssl_aes.cpp
        openssl/openssl_md5.cpp
        openssl/openssl_base64.cpp
        openssl/openssl_hmac_sha265.cpp

        # util
        util/common_util.c
        util/aes_util.c
        util/md5_util.c
        util/base64_util.c
        util/hmac_sha265_util.c

        # jni
        jni.c)

# 搜索指定的预构建库并将路径存储为变量。 因为默认情况下，CMake在搜索路径中包含系统库，
# 所以您只需要指定公共NDK库的名称你想添加。 在完成构建之前，CMake验证库存在。

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

find_library( # Sets the name of the path variable.
        z-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        z)


# 指定CMake应链接到目标库的库。 您可以链接多个库，例如在此构建脚本中定义的库，预构建的第三方库或系统库。

target_link_libraries(
        # custom
        tool

        openssl-ssl
        openssl-crypto

        ${log-lib}
        ${z-lib})