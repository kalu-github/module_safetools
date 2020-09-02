#include <jni.h>
#include <string.h>
#include "jnionload.h"
#include "../cipher/ciphertool.h"
#include "../tool/applicationtool.h"

JNIEXPORT JNICALL jint JNI_OnLoad(JavaVM *vm, void *reserved) {

    JNIEnv *env = NULL;

    if ((*vm)->GetEnv(vm, (void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    // chiper
    jclass cipher_clazz = (*env)->FindClass(env, "lib/kalu/encryption/cipher/CipherTool");
    static JNINativeMethod cipher_methods[] = {
            {"aesDecode", "(Ljava/lang/String;)Ljava/lang/String;", (void *) JNI_OnLoad_AesDecode},
            {"aesEncode", "(Ljava/lang/String;)Ljava/lang/String;", (void *) JNI_OnLoad_AesEncode},
    };

    jint cipher_size = sizeof(cipher_methods) / sizeof(cipher_methods[0]);
    if ((*env)->RegisterNatives(env, cipher_clazz, cipher_methods, cipher_size) != JNI_OK)
        return JNI_ERR;

    return JNI_VERSION_1_6;
}

jstring
JNI_OnLoad_AesDecode(JNIEnv *env, jobject instance, jstring jstr) {

    return aesDecode(env, instance, jstr);
}

jstring
JNI_OnLoad_AesEncode(JNIEnv *env, jobject instance, jstring jstr) {

    return aesEncode(env, instance, jstr);
}