#include "debugtool.h"


int isdebug() {

//#ifdef NDK_DEBUG //android.mk 编译情况下
//    LOGI("is NDK_DEBUG. \n");
//       return 1;
//#endif
//
//#ifdef CMAKE_BUILD_TYPE// cmakelist 编译情况下
//    LOGI("is CMAKE_BUILD_TYPE. \n");
//   return 1;
//#endif

// 0 关闭日志 1 打开日志
    return 1;
}