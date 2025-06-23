#ifndef OPENSSL_AES_H
#define OPENSSL_AES_H


#include <jni.h>

#ifdef  __cplusplus
extern "C" {
#endif

jbyteArray
openssl_aes128_ecb_encrypt(JNIEnv *env, jbyteArray input_key, jbyteArray input_data);

jbyteArray
openssl_aes128_ecb_decrypt(JNIEnv *env, jbyteArray input_key, jbyteArray input_data);

jbyteArray
openssl_aes128_cbc_encrypt(JNIEnv *env, jbyteArray keys_, jbyteArray iv_, jbyteArray src_);

jbyteArray
openssl_aes128_cbc_decrypt(JNIEnv *env, jbyteArray keys_, jbyteArray iv_, jbyteArray src_);


#ifdef  __cplusplus
}
#endif

#endif //OPENSSL_AES_H
