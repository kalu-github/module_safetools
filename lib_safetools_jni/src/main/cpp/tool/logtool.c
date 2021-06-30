#include <android/log.h>
#include "logtool.h"
#include "androidtool.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void log(const char *src, ...) {

    int i = isdebug();
    if (i != 1)
        return;

    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, src);
}

void logs(const char *str1, const char *str2) {

    int i = isdebug();
    if (i != 1)
        return;

    char *str = (char *) malloc(strlen(str1) + strlen(str2));
    strcpy(str, str1);
    strcat(str, str2);

    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, str);
}