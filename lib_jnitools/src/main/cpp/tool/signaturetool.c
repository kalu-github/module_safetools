#include <string.h>
#include <android/log.h>
#include <jni.h>
#include "signaturetool.h"
#include "logtool.h"
#include "androidtool.h"

jint check_signature(JNIEnv *env, jobject object) {

    // step1-2: 获得当前应用的包名
    jstring jPackageName = getPackageName(env);
    const char *packagename = (*env)->GetStringUTFChars(env, jPackageName, 0);

    // step1-2: 包名是否正确
    if (strcmp(packagename, app_packagename) != 0)
        return JNI_FALSE;

    // step2: 根据jniFlag动态判断签名文件hashcode
//    jclass jClassBuildConfig = (*env)->FindClass(env, "lib/kalu/core/BuildConfig");
//    jfieldID jFileidJniFlag = (*env)->GetStaticFieldID(env, jClassBuildConfig, "jniFlag",
//                                                       "Ljava/lang/String;");
//    jstring jStringJniFlag = (jstring) (*env)->GetStaticObjectField(env, jClassBuildConfig,
//                                                                    jFileidJniFlag);
//    const char *jniFlag = (*env)->GetStringUTFChars(env, jStringJniFlag, 0);

    // step3: app签名文件hashcode
    jobject jObjectPackageInfo = getPackageInfo(env);
    jclass jClassPackageinfo = (*env)->GetObjectClass(env, jObjectPackageInfo);
    jfieldID jFileidSignatures = (*env)->GetFieldID(env, jClassPackageinfo, getmethod_signatures,
                                                    getmethod_signatures_clazz);
    jobjectArray jObjectArraySignatures = (jobjectArray) (*env)->GetObjectField(env,
                                                                                jObjectPackageInfo,
                                                                                jFileidSignatures);
    //Signature数组中取出第一个元素
    jobject jObjectSignature = (*env)->GetObjectArrayElement(env, jObjectArraySignatures, 0);
    //读signature的hashcode
    jclass jClassSignature = (*env)->GetObjectClass(env, jObjectSignature);
    jmethodID jMethodidHashcode = (*env)->GetMethodID(env, jClassSignature, getmethod_hashcode,
                                                      getmethod_hashcode_clazz);
    jint jIntHashcode = (*env)->CallIntMethod(env, jObjectSignature, jMethodidHashcode);

    // 释放临时变量
    if (jObjectPackageInfo != NULL) {
        (*env)->DeleteLocalRef(env, jObjectPackageInfo);
    }
    if (jObjectArraySignatures != NULL) {
        (*env)->DeleteLocalRef(env, jObjectArraySignatures);
    }
    if (jObjectSignature != NULL) {
        (*env)->DeleteLocalRef(env, jObjectSignature);
    }

    // net 签名文件hashcode
    if (jIntHashcode == app_signature_hashcode_net) {
        log("check_signature => net 签名文件hashcode");
        return JNI_TRUE;
    }
        // dev 签名文件hashcode
    else if (jIntHashcode == app_signature_hashcode_dev) {
        log("check_signature => dev 签名文件hashcode");
        return JNI_TRUE;
    }
        // fail
    else {
        log("check_signature => 签名信息不匹配");
//        const char *message = "错误：签名信息不匹配";
//        toast(env, object, message);
        return JNI_FALSE;
    }
}