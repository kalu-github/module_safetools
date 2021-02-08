#include "jni.h"

#ifndef TOOL_XPOSED_H
#define TOOL_XPOSED_H

static const char *files_data = "/data/data/com.kalu.jnitools/files";
static const char *files_user = "/data/user/0/com.kalu.jnitools/files";
static const char *str_exposed = "io.va.exposed";
static const char *str_xposed = "xposed";
static const char *str_virtual = "virtual";

JNIEXPORT jint JNICALL jni_check_xposed(JNIEnv *env);

#endif