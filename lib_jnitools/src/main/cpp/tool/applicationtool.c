#include <jni.h>
#include "applicationtool.h"

jobject getApplication(JNIEnv *env) {

    // step1:
    jobject jApplication = NULL;

    // step2:
    jclass jActivityThread = (*env)->FindClass(env, "android/app/ActivityThread");

    // step3:
    if (jActivityThread != NULL) {

        jmethodID jMethodidCurrentApplication = (*env)->GetStaticMethodID(env, jActivityThread,
                                                                "currentApplication",
                                                                "()Landroid/app/Application;");
        if (jMethodidCurrentApplication != NULL) {
            jApplication = (*env)->CallStaticObjectMethod(env, jActivityThread,
                                                          jMethodidCurrentApplication);
        }
    }

    if (jActivityThread != NULL) {
        (*env)->DeleteLocalRef(env, jActivityThread);
    }

    return jApplication;
}

//jmethodID getJmethodidApplication(JNIEnv *env, jstring jst){
//
//    // step1:
//    jobject jApplication = NULL;
//
//    // step2:
//    jclass jActivityThread = (*env)->FindClass(env, "android/app/ActivityThread");
//
//    // step3:
//    if (jActivityThread != NULL) {
//
//        jmethodID jMethodidCurrentApplication = (*env)->GetStaticMethodID(env, jActivityThread,
//                                                                          "currentApplication",
//                                                                          "()Landroid/app/Application;");
//        if (jMethodidCurrentApplication != NULL) {
//            jApplication = (*env)->CallStaticObjectMethod(env, jActivityThread,
//                                                          jMethodidCurrentApplication);
//        }
//    }
//
//}

//jobject getApplicationContext(JNIEnv *env) {
//
//    // step1:获取Activity Thread的实例对象
//    jclass activityThread = (*env)->FindClass(env,"android/app/ActivityThread");
//
//    // step2:
//    jmethodID currentActivityThread = (*env)->GetStaticMethodID(env,activityThread, "currentActivityThread", "()Landroid/app/ActivityThread;");
//
//    // step3:
//    jobject at = (*env)->CallStaticObjectMethod(env,activityThread, currentActivityThread);
//
//    // step4:获取Application，也就是全局的Context
//    jmethodID getApplication = (*env)->GetMethodID(env,activityThread, "getApplication", "()Landroid/app/Application;");
//
//    // step5:
//    jobject context = (*env)->CallObjectMethod(env,at, getApplication);
//
//    return context;
//}