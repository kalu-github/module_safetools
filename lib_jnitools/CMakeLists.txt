cmake_minimum_required(VERSION 3.10.2)

# 编译Release(Release Debug)
set(CMAKE_BUILD_TYPE "Release")

# 编译警告
add_definitions(-Wno-format-security)

# 头文件
include_directories(
        src/main/cpp/aes
        src/main/cpp/base64
        src/main/cpp/cipher
        src/main/cpp/jnionload
        src/main/cpp/md5
        src/main/cpp/tool)

add_library(
        # Sets the name of the library.
        tools

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).

        # tool
        src/main/cpp/tool/applicationtool.c
        src/main/cpp/tool/stringtool.c
        src/main/cpp/tool/keytool.c
        src/main/cpp/tool/signaturetool.c
        src/main/cpp/tool/emulatortool.c
        src/main/cpp/tool/debugtool.c
        src/main/cpp/tool/logtool.c
        src/main/cpp/tool/contexttool.c
        src/main/cpp/tool/packagetool.c
        src/main/cpp/tool/xposedtool.c
        src/main/cpp/tool/roottool.c

        # aes
        src/main/cpp/aes/aes.c

        # base64
        src/main/cpp/base64/base64.c

        # md5
        src/main/cpp/md5/md5.c

        # cipher
        src/main/cpp/cipher/ciphertool.c

        # jnionload
        src/main/cpp/jnionload/jnionload.c)

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
        tools

        # included in the NDK.
        ${log-lib}

        # included in the NDK.
        ${z-lib})
