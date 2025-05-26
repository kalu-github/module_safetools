#include <jni.h>
#include <string.h>

#ifndef LIB_HMAC_SHA265_H
#define LIB_HMAC_SHA265_H

jstring hmacSha265(JNIEnv *env, jobject instance, jstring input, jstring key);

#endif //LIB_HMAC_SHA265_H
