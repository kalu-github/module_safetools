#ifndef OPENSSL_HMAC_SHA265_H
#define OPENSSL_HMAC_SHA265_H


#include <jni.h>

#ifdef  __cplusplus
extern "C" {
#endif

jstring openssl_hmac_sha265(JNIEnv *env, jbyteArray jkey, jbyteArray jdata);

#ifdef  __cplusplus
}
#endif

#endif //OPENSSL_HMAC_SHA265_H
