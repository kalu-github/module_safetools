#include <jni.h>
#include <string.h>
#include "jnionload.h"
#include "../cipher/ciphertool.h"
#include "../tool/xposedtool.h"
#include "../tool/emulatortool.h"
#include "../tool/roottool.h"
#include "../tool/signaturetool.h"

JNIEXPORT JNICALL jint JNI_OnLoad(JavaVM *vm, void *reserved) {

    JNIEnv *env = NULL;

    if ((*vm)->GetEnv(vm, (void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    // ToolUtil
    jclass cipher_clazz = (*env)->FindClass(env, "lib/kalu/jnisafetools/SafeTools");
    static JNINativeMethod cipher_methods[] = {
            {"aesDecode",      "(Ljava/lang/String;)Ljava/lang/String;",     JNI_OnLoad_AesDecode},
            {"aesDecodeMult",  "(Ljava/lang/String;ZZZZ)Ljava/lang/String;", JNI_OnLoad_AesDecodeMult},
            {"aesEncode",      "(Ljava/lang/String;)Ljava/lang/String;",     JNI_OnLoad_AesEncode},
            {"aesEncodeMult",  "(Ljava/lang/String;ZZZZ)Ljava/lang/String;", JNI_OnLoad_AesEncodeMult},
            {"checkEmulator",  "()Z",                                        JNI_OnLoad_CheckEmulator},
            {"checkXposed",    "()Z",                                        JNI_OnLoad_CheckXposed},
            {"checkRoot",      "()Z",                                        JNI_OnLoad_CheckRoot},
            {"checkSignature", "()Z",                                        JNI_OnLoad_CheckSignature},
    };

    jint cipher_size = sizeof(cipher_methods) / sizeof(cipher_methods[0]);
    if ((*env)->RegisterNatives(env, cipher_clazz, cipher_methods, cipher_size) != JNI_OK)
        return JNI_ERR;

    return JNI_VERSION_1_6;
}

jboolean
JNI_OnLoad_CheckSignature(JNIEnv *env, jobject instance) {

    jint status = jni_check_signature(env);
    return status == JNI_TRUE;
}

jboolean
JNI_OnLoad_CheckRoot(JNIEnv *env, jobject instance) {

    jint status = check_is_root(env);
    return status == JNI_TRUE;
}

jboolean
JNI_OnLoad_CheckEmulator(JNIEnv *env, jobject instance) {

    jint status = check_is_emulator(env);
    return status == JNI_TRUE;
}

jboolean
JNI_OnLoad_CheckXposed(JNIEnv *env, jobject instance) {

    jint status = jni_check_xposed(env);
    return status == JNI_TRUE;
}

jstring
JNI_OnLoad_AesDecode(JNIEnv *env, jobject instance, jstring jstr) {

    return aesDecode(env, instance, jstr);
}

jstring
JNI_OnLoad_AesDecodeMult(JNIEnv *env, jobject instance, jstring jstr, jboolean checkRoot,
                         jboolean checkEmulator, jboolean checkXposed, jboolean checkSignature) {

    return aesDecodeMult(env, instance, jstr, checkRoot, checkEmulator, checkXposed,
                         checkSignature);
}

jstring
JNI_OnLoad_AesEncode(JNIEnv *env, jobject instance, jstring jstr) {

    return aesEncode(env, instance, jstr);
}

jstring
JNI_OnLoad_AesEncodeMult(JNIEnv *env, jobject instance, jstring jstr, jboolean checkRoot,
                         jboolean checkEmulator, jboolean checkXposed, jboolean checkSignature) {

    return aesEncodeMult(env, instance, jstr, checkRoot, checkEmulator, checkXposed,
                         checkSignature);
}