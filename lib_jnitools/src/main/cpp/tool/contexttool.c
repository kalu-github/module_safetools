#include <jni.h>
#include <string.h>
#include "applicationtool.h"

jclass getContext(JNIEnv *env) {

    // step1: 获取application
    jobject application = getApplication(env);

    // step2: Context的类
    jclass jcontext = (*env)->GetObjectClass(env, application);

    // step3：释放临时变量
    (*env)->DeleteLocalRef(env, application);

    // step4: return
    return jcontext;
}