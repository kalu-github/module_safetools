#include <jni.h>
#include <string.h>

#ifndef JNI_MULTIDEX_H
#define JNI_MULTIDEX_H

jlong obtainCheckSum(JNIEnv *env, jobject instance, jstring jstr);

jobject loadDirectDex(JNIEnv *env, jobject instance, jstring jstr, jbyte jbyte);

void recoverAction(JNIEnv *env, jobject instance);

jboolean makeOptDexFile(JNIEnv *env, jobject instance, jstring filePath, jstring optFilePath);

jboolean initialize(JNIEnv *env, jobject instance, jint sdkVersion, jclass runtimeExceptionClass);

#endif