#include <android/log.h>
#include "logtool.h"
#include "debugtool.h"

void log(const char *src, ...) {

    int i = isdebug();
    if (i != 1)
        return;

    __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, src);
}