#include <jni.h>
#include <string.h>

#ifndef JNI_CIPHER_H
#define JNI_CIPHER_H

jstring aesEncode(JNIEnv *env, jobject instance, jstring jstr);

jstring aesDecode(JNIEnv *env, jobject instance, jstring jstr);

#endif