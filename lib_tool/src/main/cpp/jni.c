#include <jni.h>
#include <string.h>
#include "jni.h"
#include "aes_util.h"
#include "util/md5_util.h"
#include "base64_util.h"
#include "hmac_sha265_util.h"
#include "common_util.h"

JNIEXPORT JNICALL

jint JNI_OnLoad(JavaVM *vm, void *reserved) {

    JNIEnv *env = NULL;

    if ((*vm)->GetEnv(vm, (void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    // ToolUtil
    jclass cipher_clazz = (*env)->FindClass(env, "lib/kalu/tool/Box");
    static JNINativeMethod cipher_methods[] = {
            {"_getFlag1",      "()[B",                     JNI_OnLoad_GetFlag1},
            {"_getFlag2",      "()[B",                     JNI_OnLoad_GetFlag2},
            {"_aesEcbDecrypt", "([B[BZ)[B",                JNI_OnLoad_AesEcbDecrypt},
            {"_aesEcbEncrypt", "([B[BZ)[B",                JNI_OnLoad_AesEcbEncrypt},
            {"_aesCbcDecrypt", "([B[B[BZ)[B",              JNI_OnLoad_AesCbcDecrypt},
            {"_aesCbcEncrypt", "([B[B[BZ)[B",              JNI_OnLoad_AesCbcEncrypt},
            {"_base64Decrypt", "([B)[B",                   JNI_OnLoad_Base64Decrypt},
            {"_base64Encrypt", "([B)[B",                   JNI_OnLoad_Base64Encrypt},
            {"_md5",           "([B)Ljava/lang/String;",   JNI_OnLoad_Md5},
            {"_hamcSha265",    "([B[B)Ljava/lang/String;", JNI_OnLoad_HmacSha265},
    };

    jint cipher_size = sizeof(cipher_methods) / sizeof(cipher_methods[0]);
    if ((*env)->RegisterNatives(env, cipher_clazz, cipher_methods, cipher_size) != JNI_OK)
        return JNI_ERR;

    return JNI_VERSION_1_6;
}

jbyteArray
JNI_OnLoad_GetFlag1(JNIEnv *env, jobject instance) {
    return getFlag1(env, instance);
}

jbyteArray
JNI_OnLoad_GetFlag2(JNIEnv *env, jobject instance) {
    return getFlag2(env, instance);
}

jbyteArray
JNI_OnLoad_Base64Decrypt(JNIEnv *env, jobject instance, jbyteArray jstr) {
    return base64Decrypt(env, instance, jstr);
}

jbyteArray
JNI_OnLoad_Base64Encrypt(JNIEnv *env, jobject instance, jbyteArray jstr) {
    return base64Encrypt(env, instance, jstr);
}

jstring
JNI_OnLoad_Md5(JNIEnv *env, jobject instance, jbyteArray jstr) {
    return md5(env, instance, jstr);
}

jstring
JNI_OnLoad_HmacSha265(JNIEnv *env, jobject instance, jbyteArray data, jbyteArray key) {
    return hmacSha265(env, instance, data, key);
}

jbyteArray
JNI_OnLoad_AesEcbDecrypt(JNIEnv *env, jobject instance, jbyteArray key, jbyteArray data,
                         jboolean useBase64) {
    return aesEcbDecrypt(env, instance, key, data, useBase64);
}

jbyteArray
JNI_OnLoad_AesEcbEncrypt(JNIEnv *env, jobject instance, jbyteArray key, jbyteArray data,
                         jboolean useBase64) {
    return aesEcbEncrypt(env, instance, key, data, useBase64);
}

jbyteArray
JNI_OnLoad_AesCbcDecrypt(JNIEnv *env, jobject instance, jbyteArray key, jbyteArray iv,
                         jbyteArray data, jboolean useBase64) {
    return aesCbcDecrypt(env, instance, key, iv, data, useBase64);
}

jbyteArray
JNI_OnLoad_AesCbcEncrypt(JNIEnv *env, jobject instance, jbyteArray key, jbyteArray iv,
                         jbyteArray data, jboolean useBase64) {
    return aesCbcEncrypt(env, instance, key, iv, data, useBase64);
}