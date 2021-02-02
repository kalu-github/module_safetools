#include <jni.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include "stringtool.h"
#include "logtool.h"

// char to string
jstring charToJstring(JNIEnv *env, const char *str) {

    jsize len = strlen(str);
    // 定义java String类 strClass
    jclass strClass = (*env)->FindClass(env, "java/lang/String");
    //设置String, 保存语言类型,用于byte数组转换至String时的参数
    jstring encoding = (*env)->NewStringUTF(env, "utf-8");
//    jstring encoding = (*env)->NewStringUTF(env, "GB2312");
    // 获取java String类方法String(byte[],String)的构造器,用于将本地byte[]数组转换为一个新String
    jmethodID ctorID = (*env)->GetMethodID(env, strClass, "<init>", "([BLjava/lang/String;)V");
    // 建立byte数组
    jbyteArray bytes = (*env)->NewByteArray(env, len);
    // 将char* 转换为byte数组
    (*env)->SetByteArrayRegion(env, bytes, 0, len, (jbyte *) str);
    //将byte数组转换为java String,并输出
    return (jstring) (*env)->NewObject(env, strClass, ctorID, bytes, encoding);
}

// string to char(把java的字符串转换成c的字符串,使用反射)
char *stringToJchar(JNIEnv *env, jstring jstr) {
    char *rtn = NULL;
    // 1:先找到字节码文件
    jclass clsstring = (*env)->FindClass(env, "java/lang/String");
    jstring strencode = (*env)->NewStringUTF(env, "utf-8");
//    jstring strencode = (*env)->NewStringUTF(env, "GB2312");
    // 2:通过字节码文件找到方法ID
    jmethodID mid = (*env)->GetMethodID(env, clsstring, "getBytes", "(Ljava/lang/String;)[B");
    // 3:通过方法id，调用方法
    jbyteArray barr = (jbyteArray) (*env)->CallObjectMethod(env, jstr, mid,
                                                            strencode); // String .getByte("GB2312");
    // 4:得到数据的长度
    jsize alen = (*env)->GetArrayLength(env, barr);
    // 5：得到数据的首地址
    jbyte *ba = (*env)->GetByteArrayElements(env, barr, JNI_FALSE);
    // 6:得到C语言的字符串
    if (alen > 0) {
        rtn = (char *) malloc(alen + 1); //"\0"
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    (*env)->ReleaseByteArrayElements(env, barr, ba, 0); //
    return rtn;
}

int stringToInt(JNIEnv *env, char *src) {
    int ints = atoi(src);
    return ints;
}