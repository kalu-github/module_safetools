#include <jni.h>
#include <string.h>
#include <stdio.h>
#include "openssl_hmac_sha265.h"

JNIEXPORT jstring JNICALL
hmacSha265(JNIEnv *env, jclass clazz, jbyteArray input, jbyteArray key) {

    // logs("lib_hmac_sha265 => hmacSha265Encrypt => input_str = ", input_str);
    return openssl_hmac_sha265(env, key, input);
}