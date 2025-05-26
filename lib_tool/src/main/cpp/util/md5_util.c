#include <jni.h>
#include <string.h>
#include <stdio.h>
#include "openssl_md5.h"

JNIEXPORT jstring JNICALL
md5(JNIEnv *env, jclass clazz, jbyteArray src_) {

    return openssl_md5(env, src_);
}