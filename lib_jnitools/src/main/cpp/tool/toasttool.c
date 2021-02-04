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

//    jclass myclass = (*env)->FindClass(env, "lib/kalu/encryption/ToolUtil");
//    jmethodID mid = (*env)->GetStaticMethodID(env, myclass, "makeText",
//                                              "(Landroid/content/Context;Ljava/lang/String;)V");
//
//    jobject params1 = getApplication(env);
//    jstring params2 = charToJstring(env, message);
//
//    (*env)->CallStaticVoidMethod(env, object, mid, params1, params2);

    jclass cls = (*env)->FindClass(env, "android/widget/Toast");
    if (NULL == cls)
        return;

    jmethodID mid_makeText = (*env)->GetStaticMethodID(env, cls, "makeText",
                                                       "(Landroid/content/Context;Ljava/lang/CharSequence;I)Landroid/widget/Toast;");
    if (NULL == mid_makeText)
        return;

    jobject context = getApplication(env);
    if (NULL == context)
        return;

    jstring text = charToJstring(env, message);
    if (NULL == text)
        return;

    jobject toast = (*env)->CallStaticObjectMethod(env, cls, mid_makeText, context, text, 0);
    if (NULL == toast)
        return;

    jmethodID mid_show = (*env)->GetMethodID(env, cls, "show", "()V");
    if (NULL == mid_show)
        return;

    (*env)->CallVoidMethod(env, toast, mid_show);
}