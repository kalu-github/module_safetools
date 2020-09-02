#include <stddef.h>
#include "emulatortool.h"
#include "jni.h"
#include "logtool.h"
#include <string.h>

int startsWith(const char *str, const char *pre) {
    size_t lenpre = strlen(pre);
    size_t lenstr = strlen(str);
    int result = lenstr < lenpre ? -1 : strncmp(pre, str, lenpre);
    if (result == 0)
        return 1;
    else
        return 0;
}

int contains(const char *str, char *key) {

    char *re;
    re = strstr(str, key);//第二个参数必须带*
    if (re)
        return 1;
    else
        return 0;
}

int equals(const char *str, char *key) {
    return strcmp(str, key) == 0;
}


JNIEXPORT jint JNICALL check_is_emulator(JNIEnv *env) {

//    if(isdebug()==1)//debug模式不校验
//        return 1;

// 这里是模仿java代码的检查是否为模拟器 必要参数 context

    jclass clazz = (*env)->FindClass(env, "android/os/Build");

    //FINGERPRINT
    jfieldID fid = (*env)->GetStaticFieldID(env, clazz, "FINGERPRINT", "Ljava/lang/String;");
    jstring fingerprintstr = (*env)->GetStaticObjectField(env, clazz, fid);
    const char *fingerprintchars = (*env)->GetStringUTFChars(env, fingerprintstr, JNI_FALSE);
    (*env)->ReleaseStringUTFChars(env, fingerprintstr, fingerprintchars);
    //MODEL
    jfieldID mid = (*env)->GetStaticFieldID(env, clazz, "MODEL", "Ljava/lang/String;");
    jstring modelstr = (*env)->GetStaticObjectField(env, clazz, mid);
    const char *modelchars = (*env)->GetStringUTFChars(env, modelstr, JNI_FALSE);
    (*env)->ReleaseStringUTFChars(env, modelstr, modelchars);
    //MANUFACTURER
    jfieldID manid = (*env)->GetStaticFieldID(env, clazz, "MANUFACTURER", "Ljava/lang/String;");
    jstring manufacturerstr = (*env)->GetStaticObjectField(env, clazz, manid);
    const char *manufacturerchars = (*env)->GetStringUTFChars(env, manufacturerstr, JNI_FALSE);
    (*env)->ReleaseStringUTFChars(env, manufacturerstr, manufacturerchars);
    //PRODUCT
    jfieldID pid = (*env)->GetStaticFieldID(env, clazz, "PRODUCT", "Ljava/lang/String;");
    jstring productstr = (*env)->GetStaticObjectField(env, clazz, pid);
    const char *productchars = (*env)->GetStringUTFChars(env, productstr, JNI_FALSE);
    (*env)->ReleaseStringUTFChars(env, productstr, productchars);
    //BRAND
    jfieldID bid = (*env)->GetStaticFieldID(env, clazz, "BRAND", "Ljava/lang/String;");
    jstring brandstr = (*env)->GetStaticObjectField(env, clazz, bid);
    const char *brandchars = (*env)->GetStringUTFChars(env, brandstr, JNI_FALSE);
    (*env)->ReleaseStringUTFChars(env, brandstr, brandchars);
    //DEVICE
    jfieldID did = (*env)->GetStaticFieldID(env, clazz, "DEVICE", "Ljava/lang/String;");
    jstring devicestr = (*env)->GetStaticObjectField(env, clazz, did);
    const char *devicechars = (*env)->GetStringUTFChars(env, devicestr, JNI_FALSE);
    (*env)->ReleaseStringUTFChars(env, devicestr, devicechars);

//    log("fingerprintchars: %s", fingerprintchars);
//    log("modelchars: %s", modelchars);
//    log("manufacturerchars: %s", manufacturerchars);
//    log("productchars: %s", productchars);
//    log("brandchars: %s", brandchars);
    if (contains(fingerprintchars, "Android")) {//是模拟器 不通过
        return JNI_FALSE;
    }
    if (contains(fingerprintchars, "unknown")) {//是模拟器 不通过
        return JNI_FALSE;
    }

    if (contains(modelchars, "google_sdk")) {
        return JNI_FALSE;
    }
    if (contains(modelchars, "Emulator")) {
        return JNI_FALSE;
    }
    if (contains(modelchars, "Android SDK built for x86")) {
        return JNI_FALSE;
    }

    if (contains(manufacturerchars, "Genymotion")) {
        return JNI_FALSE;
    }

    if (contains(brandchars, "generic") && startsWith(devicechars, "generic")) {
        return JNI_FALSE;
    }

    if (contains(productchars, "google_sdk")) {
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

