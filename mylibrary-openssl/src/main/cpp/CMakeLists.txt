cmake_minimum_required(VERSION 3.22.1)

project("openssl_cmake")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        openssl_cmake

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        native-lib.cpp)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)


# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

# 导入头文件
target_include_directories(openssl_cmake PRIVATE openssl-1.1.1w/include)
# 添加openssl
add_subdirectory(openssl-1.1.1w)

target_link_libraries( # Specifies the target library.
        openssl_cmake

        #添加依赖库
        crypto
        ssl
        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})