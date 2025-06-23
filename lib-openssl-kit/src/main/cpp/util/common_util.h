#include <jni.h>
#include <string.h>
#include <android/log.h>

#ifndef COMMON_UTIL_H
#define COMMON_UTIL_H

#define DEBUG 0 //日志开关，1为开，其它为关
#if(DEBUG == 1)
#define LOG_TAG "jni_tool"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE,LOG_TAG,__VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#else
#define LOGV(...) NULL
#define LOGD(...) NULL
#define LOGI(...) NULL
#define LOGE(...) NULL
#endif

jbyteArray getFlag1(JNIEnv *env, jobject obj);

jbyteArray getFlag2(JNIEnv *env, jobject obj);

#endif // COMMON_UTIL_H