#include <jni.h>
#include <string.h>

#ifndef JNI_ONLOAD_H
#define JNI_ONLOAD_H

////////////////////////////////////////////////////////////////////////////////

jstring JNI_OnLoad_AesDecode(JNIEnv *env, jobject instance, jstring jstr);

jstring JNI_OnLoad_AesEncode(JNIEnv *env, jobject instance, jstring jstr);

////////////////////////////////////////////////////////////////////////////////

jobject JNI_OnLoad_LoadDirectDex(JNIEnv *env, jobject instance, jstring jstr, jbyte jbyte);

jlong JNI_OnLoad_ObtainCheckSum(JNIEnv *env, jobject instance, jstring jstr);

void JNI_OnLoad_RecoverAction(JNIEnv *env, jobject instance);

jboolean JNI_OnLoad_MakeOptDexFile(JNIEnv *env, jobject instance, jstring filePath, jstring optFilePath);

jboolean JNI_OnLoad_Initialize(JNIEnv *env, jobject instance, jint sdkVersion, jclass runtimeExceptionClass);

////////////////////////////////////////////////////////////////////////////////

#endif