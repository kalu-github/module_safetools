#include <jni.h>
#include <string.h>

#ifndef TOOL_PACKAGEMANAGER_H
#define TOOL_PACKAGEMANAGER_H

static const char *getmethod_getpackagemanager = "getPackageManager";
static const char *getmethod_getpackagemanager_clazz = "()Landroid/content/pm/PackageManager;";

static const char *getmethod_getpackageinfo = "getPackageInfo";
static const char *getmethod_getpackageinfo_clazz = "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;";

static const char *getmethod_getpackagename = "getPackageName";
static const char *getmethod_getpackagename_clazz = "()Ljava/lang/String;";

jstring getPackageName(JNIEnv *env);

jobject getPackageInfo(JNIEnv *env);

#endif