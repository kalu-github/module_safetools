#include <jni.h>
#include <string.h>

#ifndef JNI_ONLOAD_H
#define JNI_ONLOAD_H

jstring JNI_OnLoad_AesDecode(JNIEnv *env, jobject instance, jstring jstr);

jstring JNI_OnLoad_AesDecodeMult(JNIEnv *env, jobject instance, jstring jstr, jboolean checkRoot, jboolean checkEmulator, jboolean checkXposed, jboolean checkSignature);

jstring JNI_OnLoad_AesEncode(JNIEnv *env, jobject instance, jstring jstr);

jstring JNI_OnLoad_AesEncodeMult(JNIEnv *env, jobject instance, jstring jstr, jboolean checkRoot, jboolean checkEmulator, jboolean checkXposed, jboolean checkSignature);

jboolean JNI_OnLoad_CheckEmulator(JNIEnv *env, jobject instance);

jboolean JNI_OnLoad_CheckXposed(JNIEnv *env, jobject instance);

jboolean JNI_OnLoad_CheckRoot(JNIEnv *env, jobject instance);

jboolean JNI_OnLoad_CheckSignature(JNIEnv *env, jobject instance);

#endif