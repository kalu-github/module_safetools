#ifndef OPENSSL_BASE64_H
#define OPENSSL_BASE64_H

#include <jni.h>

#ifdef  __cplusplus
extern "C" {
#endif


jbyteArray openssl_base64_encrypt(JNIEnv *env, jbyteArray src_);

jbyteArray openssl_base64_decrypt(JNIEnv *env, jbyteArray src_);

#ifdef  __cplusplus
}
#endif

#endif //OPENSSL_BASE64_H
