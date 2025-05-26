#include <string>
#include "common_util.h"
#include "openssl_md5.h"
#include "md5.h"

extern "C"
jstring openssl_md5(JNIEnv *env, jbyteArray input) {

    jbyte *input_str = env->GetByteArrayElements(input, nullptr);
    LOGD("openssl_md5 -> input_str = %s", input_str);

    jsize input_str_len = env->GetArrayLength(input);
    LOGD("openssl_md5 -> input_str_len = %d", input_str_len);

    char buff[3] = {'\0'};
    char hex[33] = {'\0'};
    unsigned char digest[MD5_DIGEST_LENGTH];

//    MD5((const unsigned char *) src, src_Len, digest);

    MD5_CTX ctx;
    MD5_Init(&ctx);
//    LOGD("MD5->进行MD5信息摘要运算");
    MD5_Update(&ctx, input_str, input_str_len);
    MD5_Final(digest, &ctx);

    strcpy(hex, "");
//    LOGD("MD5->把哈希值按%%02x格式定向到缓冲区");
    for (int i = 0; i != sizeof(digest); i++) {
        sprintf(buff, "%02x", digest[i]);
        strcat(hex, buff);
    }

    LOGD("openssl_md5 -> result_str = %s", hex);

//    LOGD("MD5->从jni释放数据指针");
    env->ReleaseByteArrayElements(input, input_str, 0);

    return env->NewStringUTF(hex);
}


