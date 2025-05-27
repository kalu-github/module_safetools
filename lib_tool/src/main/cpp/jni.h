#include <jni.h>
#include <string.h>

#ifndef JNI_ONLOAD_H
#define MD5_H

jbyteArray JNI_OnLoad_GetFlag1(JNIEnv *env, jobject instance);

jbyteArray JNI_OnLoad_GetFlag2(JNIEnv *env, jobject instance);


jbyteArray JNI_OnLoad_Base64Decrypt(JNIEnv *env, jobject instance, jbyteArray jstr);

jbyteArray JNI_OnLoad_Base64Encrypt(JNIEnv *env, jobject instance, jbyteArray jstr);

jstring JNI_OnLoad_Md5(JNIEnv *env, jobject instance, jbyteArray jstr);

jstring JNI_OnLoad_HmacSha265(JNIEnv *env, jobject instance, jbyteArray data, jbyteArray key);

jbyteArray JNI_OnLoad_AesEcbDecrypt(JNIEnv *env, jobject instance, jbyteArray key, jbyteArray data, jboolean useBase64);

jbyteArray JNI_OnLoad_AesEcbEncrypt(JNIEnv *env, jobject instance, jbyteArray key, jbyteArray data, jboolean useBase64);

jbyteArray JNI_OnLoad_AesCbcDecrypt(JNIEnv *env, jobject instance, jbyteArray key, jbyteArray iv, jbyteArray data, jboolean useBase64);

jbyteArray JNI_OnLoad_AesCbcEncrypt(JNIEnv *env, jobject instance, jbyteArray key, jbyteArray iv, jbyteArray data, jboolean useBase64);

#endif