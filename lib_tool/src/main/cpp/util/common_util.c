#include <jni.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include "common_util.h"

JNIEXPORT jbyteArray

JNICALL
getFlag1(JNIEnv *env, jclass clazz) {
    // 定义密钥字符串
    const char *key_str = "123456";
    const int key_str_len = strlen(key_str);

    // 创建Java字节数组
    jbyteArray result = (*env)->NewByteArray(env, key_str_len);
    if (result == NULL) {
        // 处理内存分配失败的情况
        (*env)->ThrowNew(env, (*env)->FindClass(env, "java/lang/OutOfMemoryError"),
                         "Failed to allocate byte array");
        return NULL;
    }

    // 复制数据到Java字节数组
    (*env)->SetByteArrayRegion(env, result, 0, key_str_len, (const jbyte *) key_str);

    // 检查是否发生异常
    if ((*env)->ExceptionCheck(env)) {
        (*env)->ExceptionDescribe(env);
        (*env)->DeleteLocalRef(env, result);
        return NULL;
    }

    return result;
}

JNIEXPORT jbyteArray

JNICALL
getFlag2(JNIEnv *env, jclass clazz) {
    // 定义密钥字符串（设为静态常量以防止修改）
    static const char key_str[] = "654321";
    const int key_str_len = sizeof(key_str) - 1; // 使用sizeof更安全，自动计算长度

    // 检查JNI环境是否有效
    if (env == NULL) {
        return NULL;
    }

    // 创建Java字节数组
    jbyteArray result = (*env)->NewByteArray(env, key_str_len);
    if (result == NULL) {
        // 处理内存分配失败
        (*env)->ThrowNew(env, (*env)->FindClass(env, "java/lang/OutOfMemoryError"),
                         "Failed to allocate byte array for getFlag2");
        return NULL;
    }

    // 复制数据到Java字节数组
    (*env)->SetByteArrayRegion(env, result, 0, key_str_len, (const jbyte *) key_str);

    // 检查异常
    if ((*env)->ExceptionCheck(env)) {
        (*env)->ExceptionDescribe(env);
        (*env)->DeleteLocalRef(env, result);
        return NULL;
    }

    return result;
}