#ifndef OPENSSL_MD5_H
#define OPENSSL_MD5_H


#include <jni.h>

#ifdef  __cplusplus
extern "C" {
#endif

jstring openssl_md5(JNIEnv *env, jbyteArray input);

#ifdef  __cplusplus
}
#endif

#endif //OPENSSL_MD5_H
