#include "jni.h"

#ifndef TOOL_ROOT_H
#define TOOL_ROOT_H

static const char *sbin_su = "/sbin/su";
static const char *sbin_bin_su = "/system/bin/su";
static const char *sbin_xbin_su = "/system/xbin/su";
static const char *data_local_xbin_su = "/data/local/xbin/su";
static const char *data_local_bin_su = "/data/local/bin/su";
static const char *system_sd_xbin_su = "/system/sd/xbin/su";
static const char *system_bin_failsafe_su = "/system/bin/failsafe/su";
static const char *data_local_su = "/data/local/su";

JNIEXPORT jint JNICALL check_is_root(JNIEnv *env);

#endif