#include <jni.h>
#include <string.h>
#include <malloc.h>

#ifndef TOOL_STRING_H
#define TOOL_STRING_H

jstring charToJstring(JNIEnv *envPtr, const char *src);

// string to char(把java的字符串转换成c的字符串,使用反射)
char *stringToJchar(JNIEnv *env, jstring jstr);

int stringToInt(JNIEnv *env, char *src);

#endif