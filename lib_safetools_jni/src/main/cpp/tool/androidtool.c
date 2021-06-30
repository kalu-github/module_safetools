#include <sys/system_properties.h>
#include <stdlib.h>
#include "jni.h"
#include "logtool.h"
#include "stringtool.h"
#include "androidtool.h"

int get_ro_build_version_sdk() {
    // 1. 获取 SDK 版本号 , 存储于 C 字符串 sdk_verison_str 中
    char version[128] = "0";
    // 获取当前系统SDK版本
    __system_property_get("ro.build.version.sdk", version);
    //将版本号转为 int 值
    int number = atoi(version);
    return number;
}

int isdebug() {

//#ifdef NDK_DEBUG //android.mk 编译情况下
//    LOGI("is NDK_DEBUG. \n");
//       return 1;
//#endif
//
//#ifdef CMAKE_BUILD_TYPE// cmakelist 编译情况下
//    LOGI("is CMAKE_BUILD_TYPE. \n");
//   return 1;
//#endif

// 0 关闭日志 1 打开日志
    return 1;
}

/////////////////////////////////////////////////////////////////////////////

jclass getClass(JNIEnv *env, jobject object) {

//    log("androidtool => getClass => ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓");

    jclass jclass_object = (*env)->GetObjectClass(env, object);
//    log("androidtool => getClass => jclass_object");

//    log("androidtool => getClass => ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑");
    return jclass_object;
}

//jobject getContext(JNIEnv *env){
//
//    log("androidtool => getContext => --------------");
//    log("androidtool => getContext => ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓");
//
//    jclass jclass_android$app$ActivityThread = (*env)->FindClass(env, "android/app/ActivityThread");
//    log("androidtool => getContext => jclass_android$app$ActivityThread");
//
//    jmethodID jmethodId_currentApplication = (*env)->GetStaticMethodID(env,
//                                                                       jclass_android$app$ActivityThread,
//                                                                       "currentApplication",
//                                                                       "()Landroid/app/Application;");
//    log("androidtool => getContext => jmethodId_currentApplication");
//
//    jobject jobject_context = (*env)->CallStaticObjectMethod(env, jclass_android$app$ActivityThread,
//                                                             jmethodId_currentApplication);
//    log("androidtool => getContext => jobject_context");
//
//    log("androidtool => getContext => ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑");
//    log("androidtool => getContext => --------------");
//    return jobject_context;
//}

jobject getContext(JNIEnv *env) {

//    log("androidtool => getContext => ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓");

    jclass jclass_android$app$ActivityThread = (*env)->FindClass(env, "android/app/ActivityThread");
//    log("androidtool => getContext => jclass_android$app$ActivityThread");

    jmethodID jmethodId_currentActivityThread = (*env)->GetStaticMethodID(env,
                                                                          jclass_android$app$ActivityThread,
                                                                          "currentActivityThread",
                                                                          "()Landroid/app/ActivityThread;");
//    log("androidtool => getContext => jmethodId_currentActivityThread");

    jobject jobject_currentActivityThread = (*env)->CallStaticObjectMethod(env,
                                                                           jclass_android$app$ActivityThread,
                                                                           jmethodId_currentActivityThread);
//    log("androidtool => getContext => jobject_currentActivityThread");

    jmethodID jmethodId_getApplication = (*env)->GetMethodID(env, jclass_android$app$ActivityThread,
                                                             "getApplication",
                                                             "()Landroid/app/Application;");
//    log("androidtool => getContext => jmethodId_getApplication");

    jobject jobject_context = (*env)->CallObjectMethod(env, jobject_currentActivityThread,
                                                       jmethodId_getApplication);
//    log("androidtool => getContext => jobject_context");

//    log("androidtool => getContext => ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑");
    return jobject_context;
}

char *getFilesDirAbsolutePath(JNIEnv *env) {

//    log("androidtool => getFilesDirAbsolutePath => ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓");

    jobject jobject_context = getContext(env);
    log("androidtool => getFilesDirAbsolutePath => jobject_context");

    jclass jclass_context = (*env)->GetObjectClass(env, jobject_context);
    log("androidtool => getFilesDirAbsolutePath => jclass_context");

    jmethodID jmethodId_getFilesDir = (*env)->GetMethodID(env, jclass_context, "getFilesDir",
                                                          "()Ljava/io/File;");
    log("androidtool => getFilesDirAbsolutePath => jmethodId_getFilesDir");

    jobject jobject_file = (*env)->CallObjectMethod(env, jobject_context, jmethodId_getFilesDir);
    log("androidtool => getFilesDirAbsolutePath => jobject_file");

    jclass jclass_file = (*env)->GetObjectClass(env, jobject_file);
    log("androidtool => getFilesDirAbsolutePath => jclass_file");

    jmethodID jmethodId_getAbsolutePath = (*env)->GetMethodID(env, jclass_file, "getAbsolutePath",
                                                              "()Ljava/lang/String;");
    log("androidtool => getFilesDirAbsolutePath => jmethodId_getAbsolutePath");

    jstring jstring_getAbsolutePath = (*env)->CallObjectMethod(env, jobject_file,
                                                               jmethodId_getAbsolutePath);
    log("androidtool => getFilesDirAbsolutePath => jstring_getAbsolutePath");

    const char *absolutePath = stringToJchar(env, jstring_getAbsolutePath);
    logs("androidtool => getFilesDirAbsolutePath => absolutePath = ", absolutePath);

    char str[128];
    strcpy(str, absolutePath);
    logs("androidtool => getFilesDirAbsolutePath => str = ", str);

    // 释放1
    (*env)->ReleaseStringUTFChars(env, jstring_getAbsolutePath, absolutePath);
    log("androidtool => getFilesDirAbsolutePath => 释放->absolutePath");
    // 释放2
    // (*env)->DeleteLocalRef(env, jstring_getAbsolutePath);
    log("androidtool => getFilesDirAbsolutePath => 释放->jstring_getAbsolutePath");
    // (*env)->DeleteLocalRef(env, jclass_file);
    log("androidtool => getFilesDirAbsolutePath => 释放->jclass_file");
    // (*env)->DeleteLocalRef(env, jobject_file);

    log("androidtool => getFilesDirAbsolutePath => ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑");

    return str;
}

//////////////////////////////////////////////////////////////////////////

jstring getPackageName(JNIEnv *env) {

    // step1: 获取application
    jobject jApplication = getContext(env);

    // step2: Context的类
    jclass jclass_context = getClass(env, jApplication);

    // step3: 得到 getPackageName 方法的 ID
    jmethodID jMethodidGetPackageName = (*env)->GetMethodID(env, jclass_context, "getPackageName",
                                                            "()Ljava/lang/String;");

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
    jobject application = getContext(env);

    // step1: Context的类
    jclass jcontext = getClass(env, application);

    // step2: 得到 getPackageManager 方法的 ID
    jmethodID methodID_getPackageManager = (*env)->GetMethodID(env, jcontext,
                                                               "getPackageManager",
                                                               "()Landroid/content/pm/PackageManager;");

    // 获得PackageManager对象
    jobject packageManager = (*env)->CallObjectMethod(env, application,
                                                      methodID_getPackageManager);

    //	// 获得 PackageManager 类
    jclass pm_clazz = (*env)->GetObjectClass(env, packageManager);
//
//	// 得到 getPackageName 方法的 ID
    jmethodID methodID_pack = (*env)->GetMethodID(env, jcontext, "getPackageName",
                                                  "()Ljava/lang/String;");

    // 获得当前应用的包名
    jstring jpackagename = (*env)->CallObjectMethod(env, application, methodID_pack);

    // 得到 getPackageInfo 方法的 ID
    jmethodID methodID_pm = (*env)->GetMethodID(env, pm_clazz, "getPackageInfo",
                                                "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");

    // 获得PackageInfo
    jobject packageInfo = (*env)->CallObjectMethod(env, packageManager, methodID_pm,
                                                   jpackagename, 64);

    return packageInfo;

}

/////////////////////////////////////////////////////////////////////////////////


jlong getMainThreadId(JNIEnv *env) {

    log("androidtool => getMainThreadId => ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓");

    // 判断当前线程
    jobject jobject_context = getContext(env);
    log("androidtool => getMainThreadId => jobject_context");

    jclass jclass_context = getClass(env, jobject_context);
    log("androidtool => getMainThreadId => jclass_context");

    //  getApplicationContext().getMainLooper()
    jmethodID jmethodId_getMainLooper = (*env)->GetMethodID(env, jclass_context,
                                                            "getMainLooper",
                                                            "()Landroid/os/Looper;");
    log("androidtool => getMainThreadId => jmethodId_getMainLooper");

    jobject jobject_getMainLooper = (*env)->CallObjectMethod(env, jobject_context,
                                                             jmethodId_getMainLooper);
    log("androidtool => getMainThreadId => jobject_getMainLooper");

    jclass jclass_getMainLooper = getClass(env, jobject_getMainLooper);
    log("androidtool => getMainThreadId => jclass_getMainLooper");

    //  getApplicationContext().getMainLooper().getThread();
    jmethodID jmethodId_getThread = (*env)->GetMethodID(env, jclass_getMainLooper, "getThread",
                                                        "()Ljava/lang/Thread;");
    log("androidtool => getMainThreadId => jmethodId_getThread");

    jobject jobject_getThread = (*env)->CallObjectMethod(env, jobject_getMainLooper,
                                                         jmethodId_getThread);
    log("androidtool => getMainThreadId => jobject_getThread");

    jclass jclass_getThread = getClass(env, jobject_getThread);
    log("androidtool => getMainThreadId => jclass_getThread");

    // getApplicationContext().getMainLooper().getThread().getId();
    jmethodID jmethodId_getId = (*env)->GetMethodID(env, jclass_getThread, "getId",
                                                    "()J");
    log("androidtool => getMainThreadId => jmethodId_getId_app");

    jlong jlong_getId = (*env)->CallLongMethod(env, jobject_getThread, jmethodId_getId);
    log("androidtool => getMainThreadId => jlong_getId");

    log("androidtool => getMainThreadId => ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑");
    return jlong_getId;
}

jlong getCurrentThreadId(JNIEnv *env) {

    log("androidtool => getCurrentThreadId => ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓");

    // Thread.currentThread();
    jclass jclass_java$lang$Thread = (*env)->FindClass(env, "java/lang/Thread");
    log("androidtool => getCurrentThreadId => jclass_java$lang$Thread");

    jmethodID jmethodId_currentThread = (*env)->GetStaticMethodID(env, jclass_java$lang$Thread,
                                                                  "currentThread",
                                                                  "()Ljava/lang/Thread;");
    log("androidtool => getCurrentThreadId => jmethodId_currentThread");

    jobject jobject_currentThread = (*env)->CallStaticObjectMethod(env, jclass_java$lang$Thread,
                                                                   jmethodId_currentThread);
    log("androidtool => getCurrentThreadId => jobject_currentThread");

    jclass jclass_currentThread = getClass(env, jobject_currentThread);
    log("androidtool => getCurrentThreadId => jclass_currentThread");

    // Thread.currentThread().getId();
    jmethodID jmethodId_getId = (*env)->GetMethodID(env, jclass_currentThread, "getId",
                                                    "()J");
    log("androidtool => getCurrentThreadId => jmethodId_getId");

    jlong jlong_getId = (*env)->CallLongMethod(env, jobject_currentThread,
                                               jmethodId_getId);
    log("androidtool => getCurrentThreadId => jlong_getId");

    log("androidtool => getCurrentThreadId => ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑");
    return jlong_getId;
}

void runLooperPrepare(JNIEnv *env) {

    log("androidtool => runLooperPrepare => ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓");

    jclass jclass_android$os$Looper = (*env)->FindClass(env, "android/os/Looper");
    jmethodID jmethodId_prepare = (*env)->GetStaticMethodID(env, jclass_android$os$Looper,
                                                            "prepare", "()V");
    (*env)->CallStaticVoidMethod(env, jclass_android$os$Looper, jmethodId_prepare);

    log("androidtool => runLooperPrepare => ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑");
}

void runLooperLoop(JNIEnv *env) {
    log("androidtool => runLooperLoop => ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓");

    jclass jclass_android$os$Looper = (*env)->FindClass(env, "android/os/Looper");
    jmethodID jmethodId_loop = (*env)->GetStaticMethodID(env, jclass_android$os$Looper, "loop",
                                                         "()V");
    (*env)->CallStaticVoidMethod(env, jclass_android$os$Looper, jmethodId_loop);

    log("androidtool => runLooperLoop => ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑");
}

void toast(JNIEnv *env, const char *message) {

    log("androidtool => toast => ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓");

//    jclass myclass = (*env)->FindClass(env, "lib/kalu/encryption/ToolUtil");
//    jmethodID mid = (*env)->GetStaticMethodID(env, myclass, "makeText",
//                                              "(Landroid/content/Context;Ljava/lang/String;)V");
//
//    jobject params1 = getApplication(env);
//    jstring params2 = charToJstring(env, message);
//
//    (*env)->CallStaticVoidMethod(env, object, mid, params1, params2);

    jlong jlong_getMainThreadId = getMainThreadId(env);
    log("androidtool => toast => jlong_getMainThreadId");

    jlong jlong_getCurrentThreadId = getCurrentThreadId(env);
    log("androidtool => toast => jlong_getCurrentThreadId");

    // 分线程, Looper.prepare();
    if (jlong_getMainThreadId != jlong_getCurrentThreadId) {

        log("androidtool => toast => 分线程1");
        log("androidtool => toast => Looper.prepare()");

        runLooperPrepare(env);

    } else {
        log("androidtool => toast => 主线程1");
    }

    jclass cls = (*env)->FindClass(env, "android/widget/Toast");
    if (NULL == cls)
        return;

    jmethodID mid_makeText = (*env)->GetStaticMethodID(env, cls, "makeText",
                                                       "(Landroid/content/Context;Ljava/lang/CharSequence;I)Landroid/widget/Toast;");
    if (NULL == mid_makeText)
        return;

    jobject context = getContext(env);
    if (NULL == context)
        return;

    jstring text = charToJstring(env, message);
    if (NULL == text)
        return;

    jobject toast = (*env)->CallStaticObjectMethod(env, cls, mid_makeText, context, text, 0);
    if (NULL == toast)
        return;

    jmethodID jmethodId_show = (*env)->GetMethodID(env, cls, "show", "()V");
    if (NULL == jmethodId_show)
        return;

    (*env)->CallVoidMethod(env, toast, jmethodId_show);

    /////////////////////////////////////////

    // 分线程, Looper.loop();
    if (jlong_getMainThreadId != jlong_getCurrentThreadId) {

        log("androidtool => toast => 分线程2");
        log("androidtool => toast => Looper.loop()");

        runLooperLoop(env);

    } else {
        log("androidtool => toast => 主线程2");
    }

    log("androidtool => toast => ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑");
}