#include <jni.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>
#include "openssl_aes.h"
#include "openssl_base64.h"
#include "common_util.h"

JNIEXPORT jbyteArray

JNICALL
aesEcbEncrypt(JNIEnv *env, jclass clazz, jbyteArray key, jbyteArray input, jboolean useBase64) {
    LOGD("aes_util -> aesEncrypt -> useBase64 = %b", useBase64);
    if (useBase64) {
        jbyteArray result_aes = openssl_aes128_ecb_encrypt(env, key, input);
        return openssl_base64_encrypt(env, result_aes);
    } else {
        return openssl_aes128_ecb_encrypt(env, key, input);
    }
}

JNIEXPORT jbyteArray

JNICALL
aesEcbDecrypt(JNIEnv *env, jclass clazz, jbyteArray key, jbyteArray input, jboolean useBase64) {
    LOGD("aes_util -> aesDecrypt -> useBase64 = %b", useBase64);
    if (useBase64) {
        jbyteArray result_base64 = openssl_base64_decrypt(env, input);
        return openssl_aes128_ecb_decrypt(env, key, result_base64);
    } else {
        return openssl_aes128_ecb_decrypt(env, key, input);
    }
}

JNIEXPORT jbyteArray

JNICALL
aesCbcEncrypt(JNIEnv *env, jclass clazz, jbyteArray key, jbyteArray iv, jbyteArray input,
              jboolean useBase64) {
    LOGD("aes_util -> aesEncrypt -> useBase64 = %b", useBase64);
    if (useBase64) {
        jbyteArray result_aes = openssl_aes128_cbc_encrypt(env, key, iv, input);
        return openssl_base64_encrypt(env, result_aes);
    } else {
        return openssl_aes128_cbc_encrypt(env, key, iv, input);
    }
}

JNIEXPORT jbyteArray

JNICALL
aesCbcDecrypt(JNIEnv *env, jclass clazz, jbyteArray key, jbyteArray iv, jbyteArray input,
              jboolean useBase64) {
    LOGD("aes_util -> aesDecrypt -> useBase64 = %b", useBase64);
    if (useBase64) {
        jbyteArray result_base64 = openssl_base64_decrypt(env, input);
        return openssl_aes128_cbc_decrypt(env, key, iv, result_base64);
    } else {
        return openssl_aes128_cbc_decrypt(env, key, iv, input);
    }
}