
#ifndef _INCLUDE_CLOWN_ANDROID_JNI_LOG_H
#define _INCLUDE_CLOWN_ANDROID_JNI_LOG_H

#include <android/log.h>

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,"sw-jni", __VA_ARGS__) // 定义LOGD类型
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,"sw-jni",__VA_ARGS__) // 定义LOGI类型
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,"sw-jni",__VA_ARGS__) // 定义LOGW类型
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,"sw-jni",__VA_ARGS__) // 定义LOGE类型
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,"sw-jni",__VA_ARGS__) // 定义LOGF类型

#endif