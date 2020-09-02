#include <jni.h>
#include <string.h>
#include "../base64/base64.h"
#include "keytool.h"

//__attribute__((section (".mytext")))//隐藏字符表 并没有什么卵用 只是针对初阶hacker的一个小方案而已
const char *getKey() {
    int n = 0;
    char s[23];//"@CaitcLibKaluCipherTool";

    s[n++] = '@';
    s[n++] = 'C';
    s[n++] = 'a';
    s[n++] = 'i';
    s[n++] = 't';
    s[n++] = 'c';
    s[n++] = 'L';
    s[n++] = 'i';
    s[n++] = 'b';
    s[n++] = 'K';
    s[n++] = 'a';
    s[n++] = 'l';
    s[n++] = 'u';
    s[n++] = 'C';
    s[n++] = 'i';
    s[n++] = 'p';
    s[n++] = 'h';
    s[n++] = 'e';
    s[n++] = 'r';
    s[n++] = 'T';
    s[n++] = 'o';
    s[n++] = 'o';
    s[n++] = 'l';

    const char *encode_str = s + 1;
    return b64_decode(encode_str, strlen(encode_str));
}