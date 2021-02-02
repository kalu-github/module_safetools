#include <jni.h>
#include <string.h>
#include "ciphertool.h"
#include "../aes/aes.h"
#include "../md5/md5.h"
#include "../base64/base64.h"
#include "../tool/logtool.h"
#include "../tool/signaturetool.h"
#include "../tool/emulatortool.h"
#include "../tool/applicationtool.h"
#include "../tool/keytool.h"
#include "../tool/stringtool.h"
#include "../tool/xposedtool.h"
#include "../tool/roottool.h"

#define CBC 1
#define ECB 1

/**
 * ase 加密信息
 * @param env
 * @param instance
 * @param jstr
 * @return
 */
jstring aesEncodeString(JNIEnv *env, jobject instance, jstring jstr) {

    const char *str = (*env)->GetStringUTFChars(env, jstr, JNI_FALSE);
    log("ciphertool[succ] => aesEncode olds = ");
    log(str);

    const char *key = getKey();
    log("ciphertool[succ] => aesEncode key = ");
    log(key);

    const char *encode = AES_128_ECB_PKCS5Padding_Encrypt(str, key);
    (*env)->
            ReleaseStringUTFChars(env, jstr, str
    );
    log("ciphertool[succ] => aesEncode news = ");
    log(encode);
    return
            charToJstring(env, encode
            );
}

/**
 * ase 解密信息
 * @param env
 * @param instance
 * @param jstr
 * @return
 */
jstring aesDecodeString(JNIEnv *env, jobject instance, jstring jstr) {

    log("ciphertool[fail] => aesDecode = start");

    const char *str = (*env)->GetStringUTFChars(env, jstr, JNI_FALSE);
    log("ciphertool[succ] => aesDecode olds = ");
    log(str);

    const char *key = getKey();
    log("ciphertool[succ] => aesDecode key = ");
    log(key);

    const char *decode = AES_128_ECB_PKCS5Padding_Decrypt(str, key);
    (*env)->ReleaseStringUTFChars(env, jstr, str);
    log("ciphertool[succ] => aesDecode news = ");
    log(decode);

    return charToJstring(env, decode);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

//Java_lib_kalu_core_cmake_cipher_CipherTool_aesEncode(JNIEnv *env, jobject instance, jstring jstr) {
JNIEXPORT jstring JNICALL
aesEncode(JNIEnv *env, jobject instance, jstring jstr) {
    return aesEncodeString(env, instance, jstr);
}

JNIEXPORT jstring JNICALL
aesEncodeMult(JNIEnv *env, jobject instance, jstring jstr, jboolean checkRoot,
              jboolean checkEmulator, jboolean checkXposed, jboolean checkSignature) {

    log("ciphertool[fail] => aesEncode = start");

    // root
    if (check_is_root(env) != JNI_TRUE) {
        log("ciphertool[fail] => aesEncode = decode error => root");
    }
        // xposed
    else if (check_is_xposed(env) != JNI_TRUE) {
        const char *message = "xposed";
        log("ciphertool[fail] => aesEncode = decode error => xposed");
        return charToJstring(env, message);
    }
        // 模拟器
    else if (check_is_emulator(env) != JNI_TRUE) {
        const char *message = "";
        log("ciphertool[fail] => aesEncode = decode error => emulator");
        return charToJstring(env, message);
    }
        // 签名
    else if (check_signature(env) != JNI_TRUE) {
        const char *message = "";
        log("ciphertool[fail] => aesEncode = decode error => signature");
        return charToJstring(env, message);
    }
        // succ
    else {
        return aesEncodeString(env, instance, jstr);
    }
}

//Java_lib_kalu_core_cmake_cipher_CipherTool_aesDecode(JNIEnv *env, jobject instance, jstring jstr) {
JNIEXPORT jstring JNICALL
aesDecode(JNIEnv *env, jobject instance, jstring jstr) {
    return aesDecodeString(env, instance, jstr);
}

//Java_lib_kalu_core_cmake_cipher_CipherTool_aesDecode(JNIEnv *env, jobject instance, jstring jstr) {
JNIEXPORT jstring JNICALL
aesDecodeMult(JNIEnv *env, jobject instance, jstring jstr, jboolean checkRoot,
              jboolean checkEmulator, jboolean checkXposed, jboolean checkSignature) {

    log("ciphertool[fail] => aesDecode = start");

    // root
    if (check_is_root(env) != JNI_TRUE) {
        log("ciphertool[fail] => aesDecode = decode error => root");
    }
        // xposed
    else if (check_is_xposed(env) != JNI_TRUE) {
        const char *message = "";
        log("ciphertool[fail] => aesDecode = decode error => xposed");
        return charToJstring(env, message);
    }
        // 模拟器
    else if (check_is_emulator(env) != JNI_TRUE) {
        const char *message = "";
        log("ciphertool[fail] => aesDecode = decode error => emulator");
        return charToJstring(env, message);
    }
        // 签名
    else if (check_signature(env) != JNI_TRUE) {
        const char *message = "";
        log("ciphertool[fail] => aesDecode = decode error => signature");
        return charToJstring(env, message);
    }
        // succ
    else {
        return aesDecodeString(env, instance, jstr);
    }
}

//JNIEXPORT jstring JNICALL
//Java_lib_kalu_core_cmake_cipher_CipherTool_base64Encode(JNIEnv *env, jobject instance, jstring jstr) {
//
//    const char *str = (*env)->GetStringUTFChars(env, jstr, JNI_FALSE);
//    const char *strencode = b64_encode(str, strlen(str));
//    (*env)->ReleaseStringUTFChars(env, jstr, str);
//    // return (*env)->NewStringUTF(env, message);
//    return charToJstring(env, strencode);
//}

//JNIEXPORT jstring JNICALL
//Java_lib_kalu_core_cmake_cipher_CipherTool_base64Decode(JNIEnv *env, jobject instance, jstring jstr) {
//
//    const char *str = (*env)->GetStringUTFChars(env, jstr, JNI_FALSE);
//    const char *strdecode = b64_decode(str, strlen(str));
//    (*env)->ReleaseStringUTFChars(env, jstr, str);
//    // return (*env)->NewStringUTF(env, message);
//    return charToJstring(env, strdecode);
//}

//JNIEXPORT jstring JNICALL
//Java_lib_kalu_core_cmake_cipher_CipherTool_md532Encode(JNIEnv *env, jobject instance, jstring jst) {
//
//    const char *cstr = (*env)->GetStringUTFChars(env, jst, 0);
//    MD5_CTX md5Ctx = {0};
//    MD5Init(&md5Ctx);
//    MD5Update(&md5Ctx, cstr, strlen(cstr));
//    unsigned char dest[16] = {0};
//    MD5Final(dest, &md5Ctx);
//    (*env)->ReleaseStringUTFChars(env, jst, cstr);
//
//    int i;
//    char destination[32] = {0};
//    for (i = 0; i < 16; i++) {
//        sprintf(destination, "%s%02x", destination, dest[i]);
//    }
//    // LOGI("%s", destination);
//    // return (*env)->NewStringUTF(env, message);
//    return charToJstring(env, destination);
//}