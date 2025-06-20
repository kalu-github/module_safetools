cmake_minimum_required(VERSION 3.1.0...3.20)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)
project(openssl)

# =============================
# 项目目录结构定义
# =============================
set(SOURCE_DIR ${PROJECT_SOURCE_DIR})
set(INCLUDE_DIR ${SOURCE_DIR}/include)
set(CRYPTO_DIR ${SOURCE_DIR}/crypto)
set(SSL_DIR ${SOURCE_DIR}/ssl)
set(APPS_DIR ${SOURCE_DIR}/apps)
set(CMAKE_DIR ${SOURCE_DIR}/cmake)

# =============================
# 安装目录结构定义
# =============================
set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "Installation prefix")
set(INSTALL_BIN_DIR bin CACHE PATH "Binary install directory")
set(INSTALL_LIB_DIR lib CACHE PATH "Library install directory")
set(INSTALL_INCLUDE_DIR include CACHE PATH "Header install directory")
set(INSTALL_DOC_DIR share/doc/openssl CACHE PATH "Documentation install directory")
set(INSTALL_PKGCONFIG_DIR ${INSTALL_LIB_DIR}/pkgconfig CACHE PATH "pkg-config files directory")
set(OPENSSLDIR "${CMAKE_INSTALL_PREFIX}/ssl" CACHE PATH "OpenSSL data directory")
set(ENGINESDIR "${CMAKE_INSTALL_PREFIX}/engines-1.1" CACHE PATH "OpenSSL engines directory")

# =============================
# 构建选项配置
# =============================
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_DIR})
option(WITH_APPS "Build applications" OFF)

# =============================
# 输出目录配置（优化 .a 文件生成路径）
# =============================
# 静态库（.a）统一输出到构建目录下的 output 目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)

# =============================
# 平台相关配置
# =============================
if (MSVC)
    include(MSVCRuntime)
    configure_msvc_runtime()
    set(OPENSSLDIR "C:/ssl")
    set(ENGINESDIR "C:/engines-1.1")
    add_definitions(-DOPENSSL_SYSNAME_WIN32)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
elseif (APPLE)
    set(CMAKE_MACOSX_RPATH ON)
    add_definitions(-DOPENSSL_SYSNAME_MACOSX)
elseif (WIN32 AND NOT CYGWIN)
    set(CMAKE_SHARED_LIBRARY_PREFIX "lib")
    set(CMAKE_STATIC_LIBRARY_PREFIX "lib")
    add_definitions(-DOPENSSL_SYSNAME_WIN32)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)

    if (BUILD_SHARED_LIBS)
        add_definitions(-D_WINDLL)
    endif ()
endif ()

if (MINGW)
    set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--export-all")
endif ()

# =============================
# 版本检测逻辑
# =============================
function(from_hex HEX DEC)
    string(TOUPPER "${HEX}" HEX)
    set(_res 0)
    string(LENGTH "${HEX}" _strlen)

    while (_strlen GREATER 0)
        math(EXPR _res "${_res} * 16")
        string(SUBSTRING "${HEX}" 0 1 NIBBLE)
        string(SUBSTRING "${HEX}" 1 -1 HEX)
        if (NIBBLE STREQUAL "A")
            math(EXPR _res "${_res} + 10")
        elseif (NIBBLE STREQUAL "B")
            math(EXPR _res "${_res} + 11")
        elseif (NIBBLE STREQUAL "C")
            math(EXPR _res "${_res} + 12")
        elseif (NIBBLE STREQUAL "D")
            math(EXPR _res "${_res} + 13")
        elseif (NIBBLE STREQUAL "E")
            math(EXPR _res "${_res} + 14")
        elseif (NIBBLE STREQUAL "F")
            math(EXPR _res "${_res} + 15")
        else ()
            math(EXPR _res "${_res} + ${NIBBLE}")
        endif ()

        string(LENGTH "${HEX}" _strlen)
    endwhile ()

    set(${DEC} ${_res} PARENT_SCOPE)
endfunction()

file(STRINGS "${INCLUDE_DIR}/openssl/opensslv.h" openssl_version_str
        REGEX "^#[\t ]*define[\t ]+OPENSSL_VERSION_NUMBER[\t ]+0x([0-9a-fA-F])+.*")

string(REGEX REPLACE "^.*OPENSSL_VERSION_NUMBER[\t ]+0x([0-9a-fA-F])([0-9a-fA-F][0-9a-fA-F])([0-9a-fA-F][0-9a-fA-F])([0-9a-fA-F][0-9a-fA-F])([0-9a-fA-F]).*$"
        "\\1;\\2;\\3;\\4;\\5" OPENSSL_VERSION_LIST "${openssl_version_str}")

list(GET OPENSSL_VERSION_LIST 0 OPENSSL_VERSION_MAJOR)
list(GET OPENSSL_VERSION_LIST 1 OPENSSL_VERSION_MINOR)
from_hex("${OPENSSL_VERSION_MINOR}" OPENSSL_VERSION_MINOR)
list(GET OPENSSL_VERSION_LIST 2 OPENSSL_VERSION_FIX)
from_hex("${OPENSSL_VERSION_FIX}" OPENSSL_VERSION_FIX)
list(GET OPENSSL_VERSION_LIST 3 OPENSSL_VERSION_PATCH)

if (NOT OPENSSL_VERSION_PATCH STREQUAL "00")
    from_hex("${OPENSSL_VERSION_PATCH}" _tmp)
    math(EXPR OPENSSL_VERSION_PATCH_ASCII "${_tmp} + 96")
    unset(_tmp)
    string(ASCII "${OPENSSL_VERSION_PATCH_ASCII}" OPENSSL_VERSION_PATCH_STRING)
endif ()

set(OPENSSL_VERSION "${OPENSSL_VERSION_MAJOR}.${OPENSSL_VERSION_MINOR}.${OPENSSL_VERSION_FIX}${OPENSSL_VERSION_PATCH_STRING}")
message(STATUS "OpenSSL version ${OPENSSL_VERSION}")

set(VERSION_MAJOR ${OPENSSL_VERSION_MAJOR})
set(VERSION_MINOR ${OPENSSL_VERSION_MINOR})
set(VERSION_PATCH ${OPENSSL_VERSION_FIX})

set(VERSION_STRING ${OPENSSL_VERSION})
set(LIB_VERSION ${VERSION_MAJOR}.${VERSION_MINOR})
set(LIB_SOVERSION ${VERSION_MAJOR}.${VERSION_MINOR})

# =============================
# 编译选项配置
# =============================
add_definitions(-DOPENSSL_NO_ASM)
add_definitions(-DOPENSSL_NO_STATIC_ENGINE)
add_definitions(-DOPENSSLDIR=\"${OPENSSLDIR}\" -DENGINESDIR=\"${ENGINESDIR}\")

# =============================
# 系统架构检测
# =============================
include(CheckTypeSize)
check_type_size("long" LONG_INT)
check_type_size("long long" LONG_LONG_INT)
check_type_size("int" INT)
if (HAVE_LONG_INT AND (${LONG_INT} EQUAL 8))
    set(SIXTY_FOUR_BIT_LONG ON)
elseif (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(SIXTY_FOUR_BIT ON)
else ()
    set(THIRTY_TWO_BIT ON)
endif ()

if (MSVC OR (WIN32 AND MINGW AND NOT CYGWIN))
    set(OPENSSL_EXPORT_VAR_AS_FUNCTION 1)
endif ()

# =============================
# 头文件配置与安装
# =============================
# 复制内部头文件到构建目录
file(COPY ${INCLUDE_DIR}/internal DESTINATION include)
file(COPY ${INCLUDE_DIR}/crypto DESTINATION include)
file(COPY ${INCLUDE_DIR}/openssl DESTINATION include)

# 生成配置头文件
file(READ ${SOURCE_DIR}/opensslconf.h.cmake CONF)
set(CONF "
#define OPENSSL_NO_MD2
#define OPENSSL_NO_RC5
#define OPENSSL_NO_RFC3779
#define OPENSSL_NO_EC_NISTP_64_GCC_128
${CONF}")
file(WRITE ${PROJECT_BINARY_DIR}/opensslconf.h.cmake "${CONF}")

configure_file(${PROJECT_BINARY_DIR}/opensslconf.h.cmake
        ${PROJECT_BINARY_DIR}/include/openssl/opensslconf.h)

# 收集头文件
file(GLOB PUBLIC_HEADERS "${PROJECT_BINARY_DIR}/include/openssl/*.h")
install(DIRECTORY ${INCLUDE_DIR}/openssl DESTINATION ${INSTALL_INCLUDE_DIR})
install(DIRECTORY ${INCLUDE_DIR}/crypto DESTINATION ${INSTALL_INCLUDE_DIR})
install(DIRECTORY ${INCLUDE_DIR}/internal DESTINATION ${INSTALL_INCLUDE_DIR})

# =============================
# 子目录构建
# =============================
add_subdirectory(${CRYPTO_DIR})
add_subdirectory(${SSL_DIR})

if (WITH_APPS AND NOT ANDROID AND NOT IOS)
    add_subdirectory(${APPS_DIR})

    # 处理 c_rehash 脚本
    file(READ ${APPS_DIR}/c_rehash.cmake C_REHASH)
    string(REPLACE "@OPENSSLDIR@" "${OPENSSLDIR}" C_REHASH "${C_REHASH}")
    string(REPLACE "@CMAKE_INSTALL_PREFIX@" "${CMAKE_INSTALL_PREFIX}" C_REHASH "${C_REHASH}")
    file(WRITE ${PROJECT_BINARY_DIR}/c_rehash "${C_REHASH}")

    # 安装配置文件和目录
    install(FILES
            ${APPS_DIR}/openssl.cnf
            ${APPS_DIR}/ct_log_list.cnf
            DESTINATION ${OPENSSLDIR})
    install(CODE "FILE(MAKE_DIRECTORY \$ENV{DESTDIR}${OPENSSLDIR}/certs)")
    install(CODE "FILE(MAKE_DIRECTORY \$ENV{DESTDIR}${OPENSSLDIR}/misc)")
    install(CODE "FILE(MAKE_DIRECTORY \$ENV{DESTDIR}${OPENSSLDIR}/private)")
    install(FILES ${PROJECT_BINARY_DIR}/c_rehash DESTINATION ${INSTALL_BIN_DIR})
endif ()

# =============================
# 文档与资源安装
# =============================
install(FILES
        FAQ LICENSE README README.ENGINE
        DESTINATION ${INSTALL_DOC_DIR})
install(DIRECTORY doc DESTINATION ${INSTALL_DOC_DIR})

# =============================
# pkg-config 文件生成
# =============================
set(prefix "${CMAKE_INSTALL_PREFIX}")
set(includedir "include")
set(libdir "lib")
get_target_property(libcrypto crypto OUTPUT_NAME)
get_target_property(libssl ssl OUTPUT_NAME)
configure_file(openssl.pc.in.cmake openssl.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/openssl.pc
        DESTINATION ${INSTALL_PKGCONFIG_DIR})

# =============================
# 卸载目标
# =============================
configure_file(
        ${CMAKE_DIR}/cmake_uninstall.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake IMMEDIATE @ONLY
)

if (${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
    add_custom_target(
            "uninstall"
            ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
    )
endif ()

# =============================
# 打包配置
# =============================
set(CPACK_GENERATOR ZIP TGZ)
set(CPACK_PACKAGE_NAME "${CMAKE_PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${VERSION_STRING}")

include(CPack)