#include <jni.h>
#include <string.h>
#include "applicationtool.h"
#include "contexttool.h"
#include "packagetool.h"

jstring getPackageName(JNIEnv *env) {

    // step1: 获取application
    jobject jApplication = getApplication(env);

    // step2: Context的类
    jclass jContext = getContext(env);

    // step3: 得到 getPackageName 方法的 ID
    jmethodID jMethodidGetPackageName = (*env)->GetMethodID(env, jContext, getmethod_getpackagename,
                                                            getmethod_getpackagename_clazz);

    // step4: 获得当前应用的包名
    jstring jPackageName = (*env)->CallObjectMethod(env, jApplication, jMethodidGetPackageName);

    // step5：释放临时变量
    if (jApplication != NULL) {
        (*env)->DeleteLocalRef(env, jApplication);
    }

    return jPackageName;
}

jobject getPackageInfo(JNIEnv *env) {

    // step0: 获取application
    jobject application = getApplication(env);

    // step1: Context的类
    jclass jcontext = getContext(env);

    // step2: 得到 getPackageManager 方法的 ID
    jmethodID methodID_getPackageManager = (*env)->GetMethodID(env, jcontext,
                                                               getmethod_getpackagemanager,
                                                               getmethod_getpackagemanager_clazz);

    // 获得PackageManager对象
    jobject packageManager = (*env)->CallObjectMethod(env, application,
                                                      methodID_getPackageManager);

    //	// 获得 PackageManager 类
    jclass pm_clazz = (*env)->GetObjectClass(env, packageManager);
//
//	// 得到 getPackageName 方法的 ID
    jmethodID methodID_pack = (*env)->GetMethodID(env, jcontext, getmethod_getpackagename,
                                                  getmethod_getpackagename_clazz);

    // 获得当前应用的包名
    jstring jpackagename = (*env)->CallObjectMethod(env, application, methodID_pack);

    // 得到 getPackageInfo 方法的 ID
    jmethodID methodID_pm = (*env)->GetMethodID(env, pm_clazz, getmethod_getpackageinfo,
                                                getmethod_getpackageinfo_clazz);

    // 获得PackageInfo
    jobject packageInfo = (*env)->CallObjectMethod(env, packageManager, methodID_pm,
                                                   jpackagename, 64);

    return packageInfo;

}