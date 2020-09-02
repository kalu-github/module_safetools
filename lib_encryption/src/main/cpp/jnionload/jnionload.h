#include <jni.h>
#include <string.h>

#ifndef JNI_ONLOAD_H
#define JNI_ONLOAD_H

jstring JNI_OnLoad_AesDecode(JNIEnv *env, jobject instance, jstring jstr);

jstring JNI_OnLoad_AesEncode(JNIEnv *env, jobject instance, jstring jstr);

#endif