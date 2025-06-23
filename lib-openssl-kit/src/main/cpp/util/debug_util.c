#include <jni.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <android/log.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "common_util.h"

//// 检测调试器附加（基于ptrace反制）
//void antiDebugPtrace() {
//    pid_t childPid = fork();
//    if (childPid == 0) {
//        // 子进程尝试ptrace父进程，若成功则说明有调试器存在
//        if (ptrace(PTRACE_ATTACH, getppid(), 0, 0) == 0) {
//            ptrace(PTRACE_DETACH, getppid(), 0, 0);
//            exit(0);
//        }
//        exit(1);
//    } else {
//        int status;
//        waitpid(childPid, &status, 0);
//        if (WEXITSTATUS(status) == 0) {
//            LOGE("Debugger detected via ptrace attach!");
//            exit(1);
//        }
//    }
//}
//
//// 检测/proc文件系统访问（适用于Android 5.0+）
//bool checkDebugFS() {
//    char path[64];
//    sprintf(path, "/proc/%d/maps", getpid());
//
//    FILE* file = fopen(path, "r");
//    if (!file) return false;
//
//    char line[256];
//    while (fgets(line, sizeof(line), file)) {
//        // 检查是否存在调试相关的文件映射（如gdbserver）
//        if (strstr(line, "gdb") || strstr(line, "lldb") || strstr(line, "debug")) {
//            fclose(file);
//            return true;
//        }
//    }
//    fclose(file);
//    return false;
//}

// 检查是否被调试（ptrace方式）
bool isBeingDebuggedByPtrace() {
    // ptrace(PTRACE_TRACEME, 0, 0, 0)返回0表示未被调试，非0表示被调试
    return ptrace(PTRACE_TRACEME, 0, 0, 0) < 0;
}

// 检查/proc/self/status中的TracerPid字段
bool isBeingDebuggedByTracerPid() {
    char path[64];
    sprintf(path, "/proc/%d/status", getpid());

    FILE *file = fopen(path, "r");
    if (!file) {
        LOGE("Failed to open status file");
        return false;
    }

    char line[128];
    while (fgets(line, sizeof(line), file)) {
        // 查找"TracerPid:"字段，值为0表示未被调试
        if (strncmp(line, "TracerPid:", 9) == 0) {
            int pid = atoi(&line[9]);
            fclose(file);
            return pid != 0;
        }
    }

    fclose(file);
    return false;
}

// 检查调试器相关进程
bool checkDebuggerProcess() {
    // 示例：检查常见调试工具进程名
    char cmdline[128];
    sprintf(cmdline, "/proc/%d/cmdline", getpid());

    int fd = open(cmdline, O_RDONLY);
    if (fd < 0) {
        LOGE("Failed to open cmdline file");
        return false;
    }

    int len = read(fd, cmdline, sizeof(cmdline) - 1);
    close(fd);

    if (len > 0) {
        cmdline[len] = '\0';
        // 检查是否包含调试器关键词（如"gdb"、"lldb"）
        if (strstr(cmdline, "gdb") || strstr(cmdline, "lldb") ||
            strstr(cmdline, "android_server") || strstr(cmdline, "strace")) {
            return true;
        }
    }
    return false;
}

bool check() {
    if (isBeingDebuggedByPtrace() || isBeingDebuggedByTracerPid() || checkDebuggerProcess()) {
        LOGE("Debugger detected! Exiting...");
        return false;
    } else {
        return true;
    }
}
