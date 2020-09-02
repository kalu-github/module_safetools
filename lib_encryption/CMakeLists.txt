cmake_minimum_required(VERSION 3.4.1)           #普通版本的clang

#编译警告
add_definitions(
        -Wno-format-security)

add_library(
        # Sets the name of the library.
        kalu-encryption

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).

        # tool
        src/main/cpp/tool/applicationtool.c
        src/main/cpp/tool/applicationtool.h
        src/main/cpp/tool/stringtool.c
        src/main/cpp/tool/stringtool.h
        src/main/cpp/tool/keytool.c
        src/main/cpp/tool/keytool.h
        src/main/cpp/tool/signaturetool.h
        src/main/cpp/tool/signaturetool.c
        src/main/cpp/tool/emulatortool.h
        src/main/cpp/tool/emulatortool.c
        src/main/cpp/tool/debugtool.h
        src/main/cpp/tool/debugtool.c
        src/main/cpp/tool/logtool.h
        src/main/cpp/tool/logtool.c
        src/main/cpp/tool/contexttool.h
        src/main/cpp/tool/contexttool.c
        src/main/cpp/tool/packagetool.h
        src/main/cpp/tool/packagetool.c
        src/main/cpp/tool/xposedtool.h
        src/main/cpp/tool/xposedtool.c
        src/main/cpp/tool/roottool.h
        src/main/cpp/tool/roottool.c

        # aes
        src/main/cpp/aes/aes.h
        src/main/cpp/aes/aes.c

        # base64
        src/main/cpp/base64/base64.h
        src/main/cpp/base64/base64.c

        # md5
        src/main/cpp/md5/md5.h
        src/main/cpp/md5/md5.c

        # cipher
        src/main/cpp/cipher/ciphertool.c
        src/main/cpp/cipher/ciphertool.h

        # multidex
        src/main/cpp/multidex/multidex.cpp
        src/main/cpp/multidex/multidex.h

        # jnionload
        src/main/cpp/jnionload/jnionload.c
        src/main/cpp/jnionload/jnionload.h)

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
        kalu-encryption

        # included in the NDK.
        ${log-lib}

        # included in the NDK.
        ${z-lib})
