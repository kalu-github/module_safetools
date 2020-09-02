#include <jni.h>
#include "roottool.h"
#include "logtool.h"

JNIEXPORT jint JNICALL check_is_root(JNIEnv *env) {

    int result = JNI_TRUE;
    log("check_is_root => step1");

    for (int i = 0; i < 8; i++) {

        jstring jStringSu;

        if (i == 0) {
            jStringSu = (jstring) (*env)->NewStringUTF(env, sbin_su);
        } else if (i == 1) {
            jStringSu = (jstring) (*env)->NewStringUTF(env, sbin_bin_su);
        } else if (i == 2) {
            jStringSu = (jstring) (*env)->NewStringUTF(env, sbin_xbin_su);
        } else if (i == 3) {
            jStringSu = (jstring) (*env)->NewStringUTF(env, data_local_xbin_su);
        } else if (i == 4) {
            jStringSu = (jstring) (*env)->NewStringUTF(env, data_local_bin_su);
        } else if (i == 5) {
            jStringSu = (jstring) (*env)->NewStringUTF(env, system_sd_xbin_su);
        } else if (i == 6) {
            jStringSu = (jstring) (*env)->NewStringUTF(env, system_bin_failsafe_su);
        } else {
            jStringSu = (jstring) (*env)->NewStringUTF(env, data_local_su);
        }
        log("check_is_root => step2");

        jclass jClassFile = (*env)->FindClass(env, "java/io/File");
        log("check_is_root => step3");

        jmethodID jMethodidInit = (*env)->GetMethodID(env, jClassFile, "<init>",
                                                      "(Ljava/lang/String;)V");
        log("check_is_root => step4");

        jobject jObjectFile = (*env)->NewObject(env, jClassFile, jMethodidInit, jStringSu);
        log("check_is_root => step5");

        jmethodID jMethodidExists = (*env)->GetMethodID(env, jClassFile, "exists",
                                                        "()Z");
        log("check_is_root => step6");

        jboolean jBooleanExists = (*env)->CallBooleanMethod(env, jObjectFile, jMethodidExists);
        log("check_is_root => step7");

        // 删除临时资源
        if (jObjectFile != NULL) {
            (*env)->DeleteLocalRef(env, jObjectFile);
        }

        // break
        if (jBooleanExists == JNI_TRUE) {

            log("check_is_root => 1");
            result = jBooleanExists;
            break;
        }

    }

    return result;
}