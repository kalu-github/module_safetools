#include "jni.h"

#ifndef TOOL_XPOSED_H
#define TOOL_XPOSED_H

static const char *files_data = "/data/data/com.kalu.encryption/files";
static const char *files_user = "/data/user/0/com.kalu.encryption/files";
static const char *exposed = "io.va.exposed";
static const char *xposed = "xposed";
static const char *virtual = "virtual";

JNIEXPORT jint JNICALL check_is_xposed(JNIEnv *env);

#endif