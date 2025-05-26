#include <jni.h>
#include <string.h>

#ifndef JNI_ONLOAD_H
#define MD5_H

jbyteArray JNI_OnLoad_GetFlag1(JNIEnv *env, jobject instance);

jbyteArray JNI_OnLoad_GetFlag2(JNIEnv *env, jobject instance);


jbyteArray JNI_OnLoad_Base64Decrypt(JNIEnv *env, jobject instance, jbyteArray jstr);

jbyteArray JNI_OnLoad_Base64Encrypt(JNIEnv *env, jobject instance, jbyteArray jstr);

jbyteArray JNI_OnLoad_AesDecrypt(JNIEnv *env, jobject instance, jbyteArray data, jbyteArray key, jboolean useBase64);

jbyteArray JNI_OnLoad_AesEncrypt(JNIEnv *env, jobject instance, jbyteArray data, jbyteArray key, jboolean useBase64);

jstring JNI_OnLoad_Md5(JNIEnv *env, jobject instance, jbyteArray jstr);

jstring JNI_OnLoad_HmacSha265(JNIEnv *env, jobject instance, jbyteArray data, jbyteArray key);

#endif