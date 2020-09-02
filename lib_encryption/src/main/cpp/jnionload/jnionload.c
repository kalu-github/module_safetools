#include <jni.h>
#include <string.h>
#include "jnionload.h"
#include "../cipher/ciphertool.h"
#include "../multidex/multidex.h"
#include "../tool/applicationtool.h"

JNIEXPORT JNICALL jint JNI_OnLoad(JavaVM *vm, void *reserved) {

    JNIEnv *env = NULL;

    if ((*vm)->GetEnv(vm, (void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    // multidex
    jclass multidex_clazz = (*env)->FindClass(env, "lib/kalu/encryption/multidex/BoostNative");
    static JNINativeMethod multidex_methods[] = {
            {"obtainCheckSum", "(Ljava/lang/String;)J",                    (void *) JNI_OnLoad_ObtainCheckSum},
            {"makeOptDexFile", "(Ljava/lang/String;Ljava/lang/String;)Z",  (void *) JNI_OnLoad_MakeOptDexFile},
            {"initialize",     "(ILjava/lang/Class;)Z",                    (void *) JNI_OnLoad_Initialize},
            {"recoverAction",  "()V",                                      (void *) JNI_OnLoad_RecoverAction},
            {"loadDirectDex", "(Ljava/lang/String;[B)Ljava/lang/Object;", (void *) JNI_OnLoad_LoadDirectDex},
    };
    jint multidex_size = sizeof(multidex_methods) / sizeof(multidex_methods[0]);
    if ((*env)->RegisterNatives(env, multidex_clazz, multidex_methods, multidex_size) != JNI_OK) {
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

jlong
JNI_OnLoad_ObtainCheckSum(JNIEnv *env, jobject instance, jstring jstr) {

    return obtainCheckSum(env, instance, jstr);
}

jobject
JNI_OnLoad_LoadDirectDex(JNIEnv *env, jobject instance, jstring jstr, jbyte jbyte) {

    return loadDirectDex(env, instance, jstr, jbyte);
}

void
JNI_OnLoad_RecoverAction(JNIEnv *env, jobject instance) {

    recoverAction(env, instance);
}

jboolean
JNI_OnLoad_MakeOptDexFile(JNIEnv *env, jobject instance, jstring filePath, jstring optFilePath) {

    return makeOptDexFile(env, instance, filePath, optFilePath);
}

jboolean
JNI_OnLoad_Initialize(JNIEnv *env, jobject instance, jint sdkVersion, jclass jcls) {

    return initialize(env, instance, sdkVersion, jcls);
}