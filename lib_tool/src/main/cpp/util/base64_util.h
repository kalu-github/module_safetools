#include <jni.h>
#include <string.h>

#ifndef JNI_BASE64_H
#define JNI_BASE64_H

jbyteArray base64Encrypt(JNIEnv *env, jobject instance, jbyteArray jstr);

jbyteArray base64Decrypt(JNIEnv *env, jobject instance, jbyteArray jstr);

#endif