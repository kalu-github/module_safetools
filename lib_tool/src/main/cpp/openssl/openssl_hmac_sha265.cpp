#include <jni.h>
#include <hmac.h>
#include <sha.h>
#include <cstring>

extern "C"
jstring openssl_hmac_sha265(JNIEnv *env, jbyteArray jkey, jbyteArray jdata) {
    // 1. 获取输入数据
    jbyte *key_bytes = env->GetByteArrayElements(jkey, nullptr);
    jsize key_len = env->GetArrayLength(jkey);

    jbyte *data_bytes = env->GetByteArrayElements(jdata, nullptr);
    jsize data_len = env->GetArrayLength(jdata);

    if (!key_bytes || !data_bytes) {
        env->ThrowNew(env->FindClass("java/lang/IllegalArgumentException"), "Input data is null");
        if (key_bytes) env->ReleaseByteArrayElements(jkey, key_bytes, JNI_ABORT);
        if (data_bytes) env->ReleaseByteArrayElements(jdata, data_bytes, JNI_ABORT);
        return nullptr;
    }

    // 2. 计算HMAC-SHA256
    unsigned char digest[SHA256_DIGEST_LENGTH];
    unsigned int digest_len = SHA256_DIGEST_LENGTH;

    HMAC(EVP_sha256(),
         key_bytes, key_len,
         reinterpret_cast<const unsigned char *>(data_bytes), data_len,
         digest, &digest_len);

    // 3. 释放输入资源
    env->ReleaseByteArrayElements(jkey, key_bytes, JNI_ABORT);
    env->ReleaseByteArrayElements(jdata, data_bytes, JNI_ABORT);

    // 4. 将结果转换为十六进制字符串
    constexpr int hex_str_len = SHA256_DIGEST_LENGTH * 2 + 1; // 每个字节对应2个字符+终止符
    char hex_str[hex_str_len];

    for (int i = 0; i < digest_len; ++i) {
        sprintf(&hex_str[i * 2], "%02x", static_cast<unsigned int>(digest[i]));
    }

    // 5. 创建Java字符串并返回
    jstring result = env->NewStringUTF(hex_str);

    // 检查异常（如内存不足）
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
        return nullptr;
    }

    return result;
}