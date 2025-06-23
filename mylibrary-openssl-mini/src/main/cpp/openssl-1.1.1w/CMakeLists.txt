# =============================
# OpenSSL项目CMake构建脚本
# 版本：1.0.0
# 说明：包含语法修正、功能优化和平台兼容性改进
# =============================

# 最低CMake版本要求（明确版本范围）
cmake_minimum_required(VERSION 3.1.0...3.20)

# 禁用Cygwin传统Win32模式
set(CMAKE_LEGACY_CYGWIN_WIN32 0)

# 项目初始化
project(openssl LANGUAGES C)

# -----------------------------
# 项目目录结构定义（统一注释风格）
# -----------------------------
set(SOURCE_DIR ${PROJECT_SOURCE_DIR})
set(INCLUDE_DIR ${SOURCE_DIR}/include)
set(CRYPTO_DIR ${SOURCE_DIR}/crypto)
set(SSL_DIR ${SOURCE_DIR}/ssl)
set(APPS_DIR ${SOURCE_DIR}/apps)
set(CMAKE_DIR ${SOURCE_DIR}/cmake)

# 检查关键目录是否存在
if (NOT EXISTS ${INCLUDE_DIR})
  message(FATAL_ERROR "头文件目录不存在: ${INCLUDE_DIR}")
endif ()

# -----------------------------
# 安装目录结构定义
# -----------------------------
# 安装前缀（允许通过命令行修改）
set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "安装前缀，默认安装到/usr/local")

# 子目录配置
set(INSTALL_BIN_DIR bin CACHE PATH "二进制文件安装目录")
set(INSTALL_LIB_DIR lib CACHE PATH "库文件安装目录")
set(INSTALL_INCLUDE_DIR include CACHE PATH "头文件安装目录")
set(INSTALL_DOC_DIR share/doc/openssl CACHE PATH "文档安装目录")
set(INSTALL_PKGCONFIG_DIR ${INSTALL_LIB_DIR}/pkgconfig CACHE PATH "pkg-config文件目录")

# OpenSSL特定目录
set(OPENSSLDIR "${CMAKE_INSTALL_PREFIX}/ssl" CACHE PATH "OpenSSL数据目录")
set(ENGINESDIR "${CMAKE_INSTALL_PREFIX}/engines-1.1" CACHE PATH "OpenSSL引擎目录")

# -----------------------------
# 构建选项基础配置
# -----------------------------
# 禁止修改源文件和在源目录内构建
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# 添加自定义CMake模块路径
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_DIR})

# 应用程序构建选项（默认关闭，可通过-DWITH_APPS=ON启用）
option(WITH_APPS "构建应用程序" OFF)

# -----------------------------
# 输出目录配置（优化静态库生成路径）
# -----------------------------
# 静态库(.a)统一输出到构建目录下的output目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/output)

# -----------------------------
# 编译类型配置（添加Debug/Release模式支持）
# -----------------------------
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "构建类型" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug;Release;RelWithDebInfo;MinSizeRel")
endif ()

# -----------------------------
# 平台相关配置（增强兼容性）
# -----------------------------
if (MSVC)
  # MSVC编译器配置
  include(MSVCRuntime)
  configure_msvc_runtime()

  # Windows特定目录
  set(OPENSSLDIR "C:/ssl" CACHE PATH "Windows下OpenSSL数据目录")
  set(ENGINESDIR "C:/engines-1.1" CACHE PATH "Windows下OpenSSL引擎目录")

  # 编译定义
  add_definitions(-DOPENSSL_SYSNAME_WIN32)
  add_definitions(-DWIN32_LEAN_AND_MEAN)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)

  # Unicode支持
  add_definitions(-DUNICODE -D_UNICODE)

  elif(APPLE)
  # MacOSX配置
  set(CMAKE_MACOSX_RPATH ON)
  set(CMAKE_INSTALL_NAME_DIR ${CMAKE_INSTALL_PREFIX}/lib)
  add_definitions(-DOPENSSL_SYSNAME_MACOSX)

  elif(CYGWIN)
  # Cygwin平台支持
  add_definitions(-DOPENSSL_SYSNAME_CYGWIN)
  set(OPENSSLDIR "/usr/local/ssl" CACHE PATH "Cygwin下OpenSSL数据目录")

  elif(WIN32 AND NOT CYGWIN)
  # MinGW等Windows非Cygwin环境
  set(CMAKE_SHARED_LIBRARY_PREFIX "lib")
  set(CMAKE_STATIC_LIBRARY_PREFIX "lib")

  add_definitions(-DOPENSSL_SYSNAME_WIN32)
  add_definitions(-DWIN32_LEAN_AND_MEAN)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)

  if (BUILD_SHARED_LIBS)
    add_definitions(-D_WINDLL)
  endif ()

  # MinGW特殊处理
  if (MINGW)
    set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--export-all")
  endif ()
endif ()

# -----------------------------
# 版本检测逻辑（增强健壮性）
# -----------------------------
# 十六进制转十进制函数（添加错误处理）
function(from_hex HEX DEC)
  string(TOUPPER "${HEX}" HEX)
  if (HEX STREQUAL "")
    message(FATAL_ERROR "十六进制字符串为空，无法转换")
  endif ()

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
    elseif (NIBBLE MATCHES "[0-9]")
      math(EXPR _res "${_res} + ${NIBBLE}")
    else ()
      message(FATAL_ERROR "无效的十六进制字符: ${NIBBLE}")
    endif ()

    string(LENGTH "${HEX}" _strlen)
  endwhile ()

  set(${DEC} ${_res} PARENT_SCOPE)
endfunction()

# 提取版本号（添加文件存在检查）
if (NOT EXISTS ${INCLUDE_DIR}/openssl/opensslv.h)
  message(FATAL_ERROR "版本头文件不存在: ${INCLUDE_DIR}/openssl/opensslv.h")
endif ()

file(STRINGS "${INCLUDE_DIR}/openssl/opensslv.h" openssl_version_str
        REGEX "^#[\t ]*define[\t ]+OPENSSL_VERSION_NUMBER[\t ]+0x([0-9a-fA-F])+.*")

if (openssl_version_str STREQUAL "")
  message(FATAL_ERROR "无法从opensslv.h中提取版本号")
endif ()

# 解析版本号（添加正则表达式健壮性）
string(REGEX REPLACE "^.*OPENSSL_VERSION_NUMBER[\t ]+0x([0-9a-fA-F])([0-9a-fA-F][0-9a-fA-F])([0-9a-fA-F][0-9a-fA-F])([0-9a-fA-F][0-9a-fA-F])([0-9a-fA-F]).*$"
        "\\1;\\2;\\3;\\4;\\5" OPENSSL_VERSION_LIST "${openssl_version_str}")

# 提取版本组件（添加存在性检查）
list(LENGTH OPENSSL_VERSION_LIST VERSION_LIST_LENGTH)
if (VERSION_LIST_LENGTH LESS 4)
  message(FATAL_ERROR "版本号解析失败，格式不正确")
endif ()

list(GET OPENSSL_VERSION_LIST 0 OPENSSL_VERSION_MAJOR)
list(GET OPENSSL_VERSION_LIST 1 OPENSSL_VERSION_MINOR)
from_hex("${OPENSSL_VERSION_MINOR}" OPENSSL_VERSION_MINOR)
list(GET OPENSSL_VERSION_LIST 2 OPENSSL_VERSION_FIX)
from_hex("${OPENSSL_VERSION_FIX}" OPENSSL_VERSION_FIX)
list(GET OPENSSL_VERSION_LIST 3 OPENSSL_VERSION_PATCH)

# 处理补丁版本（添加默认值）
set(OPENSSL_VERSION_PATCH_STRING "")
if (NOT OPENSSL_VERSION_PATCH STREQUAL "" AND NOT OPENSSL_VERSION_PATCH STREQUAL "00")
  from_hex("${OPENSSL_VERSION_PATCH}" _tmp)
  math(EXPR OPENSSL_VERSION_PATCH_ASCII "${_tmp} + 96")
  unset(_tmp)
  string(ASCII "${OPENSSL_VERSION_PATCH_ASCII}" OPENSSL_VERSION_PATCH_STRING)
endif ()

# 生成版本字符串
set(OPENSSL_VERSION "${OPENSSL_VERSION_MAJOR}.${OPENSSL_VERSION_MINOR}.${OPENSSL_VERSION_FIX}${OPENSSL_VERSION_PATCH_STRING}")
message(STATUS "检测到OpenSSL版本: ${OPENSSL_VERSION}")

# 导出版本变量
set(VERSION_MAJOR ${OPENSSL_VERSION_MAJOR})
set(VERSION_MINOR ${OPENSSL_VERSION_MINOR})
set(VERSION_PATCH ${OPENSSL_VERSION_FIX})
set(VERSION_STRING ${OPENSSL_VERSION})
set(LIB_VERSION ${VERSION_MAJOR}.${VERSION_MINOR})
set(LIB_SOVERSION ${VERSION_MAJOR}.${VERSION_MINOR})

# -----------------------------
# 编译选项配置（优化功能裁剪）
# -----------------------------
# 基础编译选项（按需调整）
#add_definitions(-DOPENSSL_NO_asm)          # 禁用汇编优化（可选，视性能需求）
add_definitions(-DOPENSSLDIR=\"${OPENSSLDIR}\") # 让编译后的程序在运行时知道 OpenSSL 的配置文件或库文件路径（例如，用于查找 CA 证书、加密库等）。
add_definitions(-DENGINESDIR=\"${ENGINESDIR}\") # 指定 OpenSSL 引擎（如硬件加速模块）的路径，用于加载第三方加密引擎。

# 仅编译静态库（可选）
add_definitions(-DOPENSSL_NO_STATIC_ENGINE)

# 禁用动态共享对象
add_definitions(-DOPENSSL_NO_DSO)

# 禁用OpenSSL引擎支持
#add_definitions(-DOPENSSL_NO_ENGINE)
#add_definitions(-DOPENSSL_NO_STATIC_ENGINE)

add_definitions(-DOPENSSL_NO_ZLIB)
add_definitions(-DOPENSSL_NO_ZLIB_DYNAMIC)

# zlib依赖
option(ENABLE_ZLIB "启用zlib支持" OFF)
if (NOT ENABLE_ZLIB)
  add_definitions(-DOPENSSL_NO_ZLIB)
  add_definitions(-DOPENSSL_NO_ZLIB_DYNAMIC)
else ()
  find_package(ZLIB REQUIRED)
  include_directories(${ZLIB_INCLUDE_DIRS})
  link_libraries(${ZLIB_LIBRARIES})
endif ()
#
## 算法和功能裁剪（分类配置，保留常用功能）
## 禁用过时或低安全性算法
#add_definitions(-DOPENSSL_NO_MD2)    # 禁用MD2算法
#add_definitions(-DOPENSSL_NO_MD4)    # 禁用MD4算法
#add_definitions(-DOPENSSL_NO_RC2)    # 禁用RC2算法
#add_definitions(-DOPENSSL_NO_RC4)    # 禁用RC4算法
#add_definitions(-DOPENSSL_NO_RC5)    # 禁用RC5算法
#add_definitions(-DOPENSSL_NO_SHA1)   # 禁用SHA1（保留SHA2/sha3用于HMAC-SHA）
#
## 禁用较少使用的算法
#add_definitions(-DOPENSSL_NO_GOST)   # 禁用GOST算法
#add_definitions(-DOPENSSL_NO_IDEA)   # 禁用IDEA算法
#add_definitions(-DOPENSSL_NO_SEED)   # 禁用SEED算法
#add_definitions(-DOPENSSL_NO_CAMELLIA) # 禁用Camellia算法
#
## 禁用特定协议功能
#add_definitions(-DOPENSSL_NO_KRB5)   # 禁用Kerberos 5支持
#add_definitions(-DOPENSSL_NO_OCSP)   # 禁用OCSP功能
#add_definitions(-DOPENSSL_NO_PKCS12) # 禁用PKCS#12格式
#add_definitions(-DOPENSSL_NO_PKCS7)  # 禁用PKCS#7格式
#
## 保留常用加密算法（默认启用）
## add_definitions(-DOPENSSL_NO_RSA)    # 如需禁用RSA，取消注释
## add_definitions(-DOPENSSL_NO_AES)    # 如需禁用AES，取消注释
## add_definitions(-DOPENSSL_NO_ECC)    # 如需禁用ECC，取消注释

# -----------------------------
# 系统架构检测
# -----------------------------
include(CheckTypeSize)
check_type_size("long" LONG_INT)
check_type_size("long long" LONG_LONG_INT)
check_type_size("int" INT)

if (HAVE_LONG_INT AND ${LONG_INT} EQUAL 8)
  set(SIXTY_FOUR_BIT_LONG ON)
  message(STATUS "检测到64位长整型")
elseif (CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(SIXTY_FOUR_BIT ON)
  message(STATUS "检测到64位系统")
else ()
  set(THIRTY_TWO_BIT ON)
  message(STATUS "检测到32位系统")
endif ()

# Windows平台导出变量配置
if (MSVC OR (WIN32 AND MINGW AND NOT CYGWIN))
  set(OPENSSL_EXPORT_VAR_AS_FUNCTION 1)
endif ()

# -----------------------------
# 头文件配置与安装（增强健壮性）
# -----------------------------
# 复制内部头文件到构建目录（添加存在性检查）
if (EXISTS ${INCLUDE_DIR}/internal)
  file(COPY ${INCLUDE_DIR}/internal DESTINATION include)
endif ()
if (EXISTS ${INCLUDE_DIR}/crypto)
  file(COPY ${INCLUDE_DIR}/crypto DESTINATION include)
endif ()
if (EXISTS ${INCLUDE_DIR}/openssl)
  file(COPY ${INCLUDE_DIR}/openssl DESTINATION include)
endif ()

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

# 安装头文件（添加目录创建）
install(DIRECTORY ${INCLUDE_DIR}/openssl DESTINATION ${INSTALL_INCLUDE_DIR})
install(DIRECTORY ${INCLUDE_DIR}/crypto DESTINATION ${INSTALL_INCLUDE_DIR})
if (EXISTS ${INCLUDE_DIR}/internal)
  install(DIRECTORY ${INCLUDE_DIR}/internal DESTINATION ${INSTALL_INCLUDE_DIR})
endif ()

# -----------------------------
# 子目录构建
# -----------------------------
add_subdirectory(${CRYPTO_DIR})
add_subdirectory(${SSL_DIR})

if (WITH_APPS AND NOT ANDROID AND NOT IOS)
  add_subdirectory(${APPS_DIR})

  # 处理c_rehash脚本
  if (EXISTS ${APPS_DIR}/c_rehash.cmake)
    file(READ ${APPS_DIR}/c_rehash.cmake C_REHASH)
    string(REPLACE "@OPENSSLDIR@" "${OPENSSLDIR}" C_REHASH "${C_REHASH}")
    string(REPLACE "@CMAKE_INSTALL_PREFIX@" "${CMAKE_INSTALL_PREFIX}" C_REHASH "${C_REHASH}")
    file(WRITE ${PROJECT_BINARY_DIR}/c_rehash "${C_REHASH}")
    execute_process(COMMAND chmod +x ${PROJECT_BINARY_DIR}/c_rehash)
  endif ()

  # 安装配置文件和目录
  if (EXISTS ${APPS_DIR}/openssl.cnf)
    install(FILES ${APPS_DIR}/openssl.cnf ${APPS_DIR}/ct_log_list.cnf
            DESTINATION ${OPENSSLDIR})
  endif ()

  install(CODE "FILE(MAKE_DIRECTORY \$ENV{DESTDIR}${OPENSSLDIR}/certs)")
  install(CODE "FILE(MAKE_DIRECTORY \$ENV{DESTDIR}${OPENSSLDIR}/misc)")
  install(CODE "FILE(MAKE_DIRECTORY \$ENV{DESTDIR}${OPENSSLDIR}/private)")

  if (EXISTS ${PROJECT_BINARY_DIR}/c_rehash)
    install(FILES ${PROJECT_BINARY_DIR}/c_rehash DESTINATION ${INSTALL_BIN_DIR})
  endif ()
endif ()

# -----------------------------
# 文档与资源安装
# -----------------------------
# 安装文档文件（添加存在性检查）
file(GLOB DOC_FILES ${SOURCE_DIR}/FAQ ${SOURCE_DIR}/LICENSE ${SOURCE_DIR}/README ${SOURCE_DIR}/README.ENGINE)
if (DOC_FILES)
  install(FILES ${DOC_FILES} DESTINATION ${INSTALL_DOC_DIR})
endif ()

if (EXISTS ${SOURCE_DIR}/doc)
  install(DIRECTORY ${SOURCE_DIR}/doc DESTINATION ${INSTALL_DOC_DIR})
endif ()

# -----------------------------
# pkg-config文件生成（完善版本信息）
# -----------------------------
set(prefix "${CMAKE_INSTALL_PREFIX}")
set(includedir "include")
set(libdir "lib")

# 获取库目标名称
get_target_property(libcrypto crypto OUTPUT_NAME)
get_target_property(libssl ssl OUTPUT_NAME)

## 生成pkg-config模板（添加版本信息）
configure_file(openssl.pc.in.cmake ${CMAKE_CURRENT_BINARY_DIR}/openssl.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/openssl.pc
        DESTINATION ${INSTALL_PKGCONFIG_DIR})

## -----------------------------
## 卸载目标
## -----------------------------
## 生成卸载脚本
#configure_file(
#        ${CMAKE_DIR}/cmake_uninstall.cmake.in
#        ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake IMMEDIATE @ONLY
#)
#
#if (${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
#  add_custom_target(
#          "uninstall"
#          ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
#          COMMENT "执行卸载操作"
#  )
#endif ()

# -----------------------------
# 打包配置（完善版本信息）
# -----------------------------
set(CPACK_GENERATOR ZIP TGZ)
set(CPACK_PACKAGE_NAME "${CMAKE_PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${VERSION_STRING}")

# 添加打包描述信息
set(CPACK_PACKAGE_VENDOR "OpenSSL Project")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "OpenSSL加密库")
set(CPACK_RESOURCE_FILE_LICENSE "${SOURCE_DIR}/LICENSE")

include(CPack)

# -----------------------------
# 构建完成提示
# -----------------------------
message(STATUS "OpenSSL构建配置完成")
message(STATUS "版本: ${OPENSSL_VERSION}")
message(STATUS "安装前缀: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "构建类型: ${CMAKE_BUILD_TYPE}")
message(STATUS "应用程序构建: ${WITH_APPS}")