#include <jni.h>
#include <string.h>

#ifndef AES_UTIL_H
#define AES_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

jbyteArray aesEcbEncrypt(JNIEnv *env, jobject obj, jbyteArray key, jbyteArray input, jboolean useBase64);

jbyteArray aesEcbDecrypt(JNIEnv *env, jobject obj, jbyteArray key, jbyteArray input, jboolean useBase64);

jbyteArray aesCbcEncrypt(JNIEnv *env, jobject obj, jbyteArray key, jbyteArray iv, jbyteArray input, jboolean useBase64);

jbyteArray aesCbcDecrypt(JNIEnv *env, jobject obj, jbyteArray key, jbyteArray iv, jbyteArray input, jboolean useBase64);

#ifdef __cplusplus
}
#endif

#endif // AES_UTIL_H