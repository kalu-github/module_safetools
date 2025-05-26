#include <jni.h>
#include <string.h>
#include <malloc.h>
#include "base64_util.h"
#include "openssl_base64.h"

JNIEXPORT jbyteArray JNICALL
base64Encrypt(JNIEnv *env, jobject instance, jbyteArray data) {
//
//    char *input_str = jbyteArrayToConstChar(env, data);
//  //  logs("lib_base64 => base64Encrypt => input_str = ", input_str);
//
//    char *result_str = base64_encrypt(input_str, (int) strlen(input_str));
//  //  logs("lib_base64 => base64Encrypt => result_str = ", result_str);
//
//    jbyteArray result_jbyte = constCharToJByteArray(env, result_str, strlen(result_str));
//    free(input_str);
//    free(result_str);

    return openssl_base64_encrypt(env, data);
}

JNIEXPORT jbyteArray JNICALL
base64Decrypt(JNIEnv *env, jobject instance, jbyteArray data) {

//    char *input_str = jbyteArrayToConstChar(env, data);
//  //  logs("lib_base64 => base64Decrypt => input_str = ", input_str);
//
//    size_t result_str_len;
//    unsigned char *result_str = base64_decrypt(input_str, &result_str_len);
//   // logs("lib_base64 => base64Decrypt => result_str = ", (char *) result_str);
//
//    jbyteArray result_jbyte = constCharToJByteArray(env, (char *) result_str, result_str_len);
//    free(input_str);
//    free(result_str);

    return openssl_base64_decrypt(env, data);
}