#include <jni.h>
#include <string.h>
#include <stdbool.h>
#include <sys/system_properties.h>
#include <stdlib.h>
#include "toasttool.h"
#include "contexttool.h"
#include "applicationtool.h"
#include "stringtool.h"

void toast(JNIEnv *env, jobject object, const char *message) {

    jclass myclass = (*env)->FindClass(env, "lib/kalu/encryption/ToolUtil");
    jmethodID mid = (*env)->GetStaticMethodID(env, myclass, "makeText",
                                              "(Landroid/content/Context;Ljava/lang/String;)V");

    jobject params1 = getApplication(env);
    jstring params2 = charToJstring(env, message);

    (*env)->CallStaticVoidMethod(env, object, mid, params1, params2);

//    jclass myclass = (*env)->FindClass(env, "android/widget/Toast");
//    jmethodID mid1 = (*env)->GetStaticMethodID(env, myclass, "makeText",
//                                               "(Landroid/content/Context;Ljava/lang/CharSequence;I)Landroid/widget/Toast;");
//
//    jobject params1 = getApplication(env);
//    jstring params2 = charToJstring(env, message);
//    jint params3 = 0;
//
//    jobject obj = (*env)->CallStaticObjectMethod(env, object, mid1, params1, params2, params3);
//
//    jmethodID mid2 = (*env)->GetMethodID(env, myclass, "show", "()V");
//    (*env)->CallVoidMethod(env, obj, mid2);
}