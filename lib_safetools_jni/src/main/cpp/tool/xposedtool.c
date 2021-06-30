#include <jni.h>
#include <string.h>
#include <stdbool.h>
#include <sys/system_properties.h>
#include <stdlib.h>
#include "xposedtool.h"
#include "logtool.h"
#include "stringtool.h"
#include "androidtool.h"

JNIEXPORT jint JNICALL jni_check_xposed(JNIEnv *env) {

    // virtual-xposed- 不需要root, app缓存目录问题-ContextWrapper
    log("xposedtool => jni_check_xposed => ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓");
    log("xposedtool => jni_check_xposed => start");

    // 状态
    int status = JNI_TRUE;

    const char *path = getFilesDirAbsolutePath(env);
    logs("xposedtool => jni_check_xposed => path = ", path);

    // 缓存路径不正确
    if (strcmp(path, files_user) != 0 && strcmp(path, files_data) != 0) {

        log("xposedtool => jni_check_xposed => fail, 缓存路径不正确");
        status = JNI_FALSE;
    }
        // 缓存路径发现特殊字符
    else if (strstr(path, str_exposed) || strstr(path, str_xposed) || strstr(path, str_virtual)) {

        log("xposedtool => jni_check_xposed => fail, 缓存路径发现特殊字符");
        status = JNI_FALSE;
    }
        // 缓存路径是否存在特殊class, 特殊method
    else {

    }

    if (status == JNI_TRUE) {
        log("xposedtool => jni_check_xposed => succ");
    } else {
        log("xposedtool => jni_check_xposed => fail");
    }

    // 释放
    // (*env)->ReleaseStringUTFChars(env, jstring_getAbsolutePath, path);

    log("xposedtool => jni_check_xposed => ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑");
    return status;
}