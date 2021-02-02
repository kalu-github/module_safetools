#include <jni.h>
#include <string.h>
#include <stdbool.h>
#include <sys/system_properties.h>
#include <stdlib.h>
#include "xposedtool.h"
#include "logtool.h"
#include "applicationtool.h"
#include "stringtool.h"

JNIEXPORT jint JNICALL check_is_xposed(JNIEnv *env) {

    // virtual-xposed- 不需要root, app缓存目录问题-ContextWrapper
    log("check_is_xposed => step1-0");

    // step1-1
    jobject jObjectApplication = getApplication(env);
    log("check_is_xposed => step1-1");
    // step1-2: File filesDir = getFilesDir();
    jclass jClassApplication = (*env)->GetObjectClass(env, jObjectApplication);
    log("check_is_xposed => step1-2-1");
    jmethodID jMethodidGetFilesDir = (*env)->GetMethodID(env, jClassApplication, "getFilesDir",
                                                         "()Ljava/io/File;");
    log("check_is_xposed => step1-2-2");
    jobject jObjectFile = (*env)->CallObjectMethod(env, jObjectApplication, jMethodidGetFilesDir);
    log("check_is_xposed => step1-2-3");
    // step1-3: String absolutePath = filesDir.getAbsolutePath();
    jclass jClassFile = (*env)->GetObjectClass(env, jObjectFile);
    log("check_is_xposed => step1-3-1");
    jmethodID jMethodidGetAbsolutePath = (*env)->GetMethodID(env, jClassFile, "getAbsolutePath",
                                                             "()Ljava/lang/String;");
    log("check_is_xposed => step1-3-2");
    jstring jStringPath = (*env)->CallObjectMethod(env, jObjectFile, jMethodidGetAbsolutePath);
    log("check_is_xposed => step1-3-3");
    const char *path = stringToJchar(env, jStringPath);
    log("check_is_xposed => step1-3-4");
    log(path);

    // 释放临时变量
    if (jObjectApplication != NULL) {
        (*env)->DeleteLocalRef(env, jObjectApplication);
    }
    if (jObjectFile != NULL) {
        (*env)->DeleteLocalRef(env, jObjectFile);
    }

    // no - 字符串是否相同
    if (strcmp(path, files_user) != 0 && strcmp(path, files_data) != 0) {
        if (jStringPath != NULL) {
            (*env)->ReleaseStringUTFChars(env, jStringPath, path);
        }
        return JNI_FALSE;
    }

//    // 1. 获取 SDK 版本号 , 存储于 C 字符串 sdk_verison_str 中
//    char sdk[128] = "0";
//    // 获取版本号方法
//    __system_property_get("ro.build.version.sdk", sdk);
//    //将版本号转为 int 值
//    int sdk_verison = atoi(sdk);
//    log(sdk);
//
//    // no - 字符串是否相同
//    if (sdk_verison >= 23) {
//        log(">=23");
//        log(files_m);
//
//        if (strcmp(path, files_m) != 0) {
//            if (jStringPath != NULL) {
//                (*env)->ReleaseStringUTFChars(env, jStringPath, path);
//            }
//            return JNI_FALSE;
//        }
//
//    } else {
//        log("<23");
//        log(files);
//        if (strcmp(path, files) != 0) {
//            if (jStringPath != NULL) {
//                (*env)->ReleaseStringUTFChars(env, jStringPath, path);
//            }
//            return JNI_FALSE;
//        }
//    }

//    // no - 字符串是否包含关键字
//    if (strstr(path, exposed)) {
//        if (jStringPath != NULL) {
//            (*env)->ReleaseStringUTFChars(env, jStringPath, path);
//        }
//        return JNI_FALSE;
//    }
//
//    // no - 字符串是否包含关键字
//    if (strstr(path, virtual)) {
//        if (jStringPath != NULL) {
//            (*env)->ReleaseStringUTFChars(env, jStringPath, path);
//        }
//        return JNI_FALSE;
//    }
//
//    // no - 字符串是否包含关键字
//    if (strstr(path, xposed)) {
//        if (jStringPath != NULL) {
//            (*env)->ReleaseStringUTFChars(env, jStringPath, path);
//        }
//        return JNI_FALSE;
//    }

    log("check_is_xposed => step2-0");
    //找到ClassLoader类
    jclass classloaderClass = (*env)->FindClass(env, "java/lang/ClassLoader");

    log("check_is_xposed => step2-1");
    //找到ClassLoader类中的静态方法getSystemClassLoader
    jmethodID getSysLoaderMethod = (*env)->GetStaticMethodID(env, classloaderClass,
                                                             "getSystemClassLoader",
                                                             "()Ljava/lang/ClassLoader;");

    log("check_is_xposed => step2-2");
    //调用ClassLoader中的getSystemClassLoader方法，返回ClassLoader对象
    jobject classLoader = (*env)->CallStaticObjectMethod(env, classloaderClass, getSysLoaderMethod);

    log("check_is_xposed => step2-3");
    //DexClassLoader：能够加载自定义的jar/apk/dex
    //PathClassLoader：只能加载系统中已经安装过的apk
    jclass dexLoaderClass = (*env)->FindClass(env, "dalvik/system/DexClassLoader");

    log("check_is_xposed => step2-4");
    //找到ClassLoader中的方法loadClass
    jmethodID loadClass = (*env)->GetMethodID(env, dexLoaderClass, "loadClass",
                                              "(Ljava/lang/String;)Ljava/lang/Class;");

    log("check_is_xposed => step2-5");
    //调用DexClassLoader的loadClass方法，加载需要调用的类
    // system/framework/XposedBridge.jar
    jstring dir = (*env)->NewStringUTF(env, "de.robv.android.xposed.XposedBridge");

    log("check_is_xposed => step2-6");
    jclass targetClass = (jclass) (*env)->CallObjectMethod(env, classLoader, loadClass, dir);

    // succ, jni调用异常, 即不存在xposed
    if ((*env)->ExceptionCheck(env)) {  // 检查JNI调用是否有引发异常

        log("check_is_xposed => step2-7 true");
        (*env)->ExceptionDescribe(env);
        (*env)->ExceptionClear(env);        // 清除引发的异常，在Java层不会打印异常的堆栈信息
        // (*env)->ThrowNew(env, (*env)->FindClass(env, "java/lang/Exception"), "JNI抛出的异常！");

//        jobject jObjectApplication = getApplication(env);
//        jclass  jObjectContext = getApplicationContext(env);
//
//        jstring jStringMsg = (jstring) (*env)->NewStringUTF(env, "This mesage is from C");
//        jclass jClassToast = (*env)->FindClass(env, "lib/kalu/core/util/ToastUtil");
//        jmethodID jMethodidShow = (*env)->GetStaticMethodID(env, jClassToast, "showToast",
//                                                            "(Landroid/content/Context;Ljava/lang/String;)V");
//        (*env)->CallStaticVoidMethod(env, jObjectApplication, jMethodidShow, jObjectContext, jStringMsg);

        return JNI_TRUE;
    }
        // fail, 手机存在xposed
    else {
        log("check_is_xposed => step2-7 false");

        if (targetClass != NULL) {

            log("check_is_xposed => step2-8-1");
            jfieldID disableHooksFiled = (*env)->GetStaticFieldID(env, targetClass, "disableHooks",
                                                                  "Z");
            (*env)->SetStaticBooleanField(env, targetClass, disableHooksFiled, true);
            jfieldID runtimeFiled = (*env)->GetStaticFieldID(env, targetClass, "runtime", "I");
            (*env)->SetStaticIntField(env, targetClass, runtimeFiled, 2);
            return JNI_FALSE;
        } else {

            log("check_is_xposed => step2-8-2");
            return JNI_TRUE;
        }
    }
}