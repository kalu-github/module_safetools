#include <jni.h>
#include <string.h>
#include "ciphertool.h"
#include "../aes/aes.h"
#include "../md5/md5.h"
#include "../base64/base64.h"
#include "../tool/logtool.h"
#include "../tool/signaturetool.h"
#include "../tool/emulatortool.h"
#include "../tool/keytool.h"
#include "../tool/stringtool.h"
#include "../tool/xposedtool.h"
#include "../tool/roottool.h"

#define CBC 1
#define ECB 1

//Java_lib_kalu_core_cmake_cipher_CipherTool_aesEncode(JNIEnv *env, jobject instance, jstring jstr) {
JNIEXPORT jstring JNICALL
aesEncode(JNIEnv *env, jobject instance, jstring jstr) {
    log("ciphertool => aesEncode =>");
    return aesEncodeMult(env, instance, jstr, JNI_FALSE, JNI_FALSE, JNI_FALSE, JNI_FALSE);
}

JNIEXPORT jstring JNICALL
aesEncodeMult(JNIEnv *env, jobject instance, jstring jstr, jboolean checkRoot,
              jboolean checkEmulator, jboolean checkXposed, jboolean checkSignature) {

    jint status = JNI_TRUE;

    // checkRoot
    if (status == JNI_TRUE && checkRoot) {
        log("ciphertool => aesEncodeMult => checkRoot =>");
        status = check_is_root(env);
    }
    // checkEmulator
    if (status == JNI_TRUE && checkEmulator) {
        log("ciphertool => aesEncodeMult => checkEmulator =>");
        status = check_is_emulator(env);
    }
    // checkXposed
    if (status == JNI_TRUE && checkXposed) {
        log("ciphertool => aesEncodeMult => checkXposed =>");
        status = jni_check_xposed(env);
    }
    // checkSignature
    if (status == JNI_TRUE && checkSignature) {
        log("ciphertool => aesEncodeMult => checkSignature =>");
        status = jni_check_signature(env);
    }

    log("ciphertool => aesEncodeMult =>");

    // fail
    if (status != JNI_TRUE) {
        const char *message = "";
        log("ciphertool => aesEncodeMult => fail");
        return charToJstring(env, message);
    }
        // succ
    else {

        const char *str = (*env)->GetStringUTFChars(env, jstr, JNI_FALSE);
        logs("ciphertool => aesEncodeMult => olds = ", str);

        const char *key = getKey();
        logs("ciphertool => aesEncodeMult => key = ", key);

        const char *encode = AES_128_ECB_PKCS5Padding_Encrypt(str, key);
        (*env)->ReleaseStringUTFChars(env, jstr, str);
        logs("ciphertool => aesEncodeMult => news = ", encode);

        log("ciphertool => aesEncodeMult => succ");
        return charToJstring(env, encode);
    }
}

JNIEXPORT jstring JNICALL
aesDecode(JNIEnv *env, jobject instance, jstring jstr) {

    log("ciphertool => aesDecode =>");
    return aesDecodeMult(env, instance, jstr, JNI_FALSE, JNI_FALSE, JNI_FALSE, JNI_FALSE);
}

JNIEXPORT jstring JNICALL
aesDecodeMult(JNIEnv *env, jobject instance, jstring jstr, jboolean checkRoot,
              jboolean checkEmulator, jboolean checkXposed, jboolean checkSignature) {

    jint status = JNI_TRUE;

    // checkRoot
    if (status == JNI_TRUE && checkRoot) {
        log("ciphertool => aesDecodeMult => checkRoot =>");
        status = check_is_root(env);
    }
    // checkEmulator
    if (status == JNI_TRUE && checkEmulator) {
        log("ciphertool => aesDecodeMult => checkEmulator =>");
        status = check_is_emulator(env);
    }
    // checkXposed
    if (status == JNI_TRUE && checkXposed) {
        log("ciphertool => aesDecodeMult => checkXposed =>");
        status = jni_check_xposed(env);
    }
    // checkSignature
    if (status == JNI_TRUE && checkSignature) {
        log("ciphertool => aesDecodeMult => checkSignature =>");
        status = jni_check_signature(env);
    }

    log("ciphertool => aesDecodeMult =>");

    // fail
    if (status != JNI_TRUE) {
        const char *message = "";
        log("ciphertool => aesDecodeMult => fail");
        return charToJstring(env, message);
    }
        // succ
    else {

        const char *str = (*env)->GetStringUTFChars(env, jstr, JNI_FALSE);
        logs("ciphertool => aesDecodeMult => olds = ", str);

        const char *key = getKey();
        logs("ciphertool => aesDecodeMult => key = ", key);

        const char *decode = AES_128_ECB_PKCS5Padding_Decrypt(str, key);
        (*env)->ReleaseStringUTFChars(env, jstr, str);
        logs("ciphertool => aesDecodeMult => news = ", decode);

        log("ciphertool => aesDecodeMult => succ");
        return charToJstring(env, decode);
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