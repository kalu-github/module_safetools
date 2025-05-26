
#ifndef OPENSSL_RSA_H
#define OPENSSL_RSA_H

#include "jni.h"

#ifdef  __cplusplus
extern "C" {
#endif

jbyteArray publicKeyEncrypt(JNIEnv *env, jbyteArray publicKeys_, jbyteArray src_);

jbyteArray privateKeyEncrypt(JNIEnv *env, jbyteArray privateKeys_, jbyteArray src_);

jbyteArray privateKeyDecrypt(JNIEnv *env, jbyteArray privateKeys_, jbyteArray src_);

jbyteArray publicKeyDecrypt(JNIEnv *env, jbyteArray publicKeys_, jbyteArray src_);

jbyteArray privateKeySign(JNIEnv *env, jbyteArray privateKeys_, jbyteArray src_);

jint publicKeyVerify(JNIEnv *env, jbyteArray publicKeys_, jbyteArray src_, jbyteArray sign_);

void generateRSAKey();


#ifdef  __cplusplus
}
#endif

#endif //OPENSSL_RSA_H
