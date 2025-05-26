#include <jni.h>
#include <string.h>

#ifndef AES_UTIL_H
#define AES_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

jbyteArray aesEncrypt(JNIEnv *env, jobject obj, jbyteArray input, jbyteArray key, jboolean useBase64);

jbyteArray aesDecrypt(JNIEnv *env, jobject obj, jbyteArray input, jbyteArray key, jboolean useBase64);

#ifdef __cplusplus
}
#endif

#endif // AES_UTIL_H