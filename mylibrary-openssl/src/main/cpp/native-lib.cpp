#include <jni.h>
#include <string>
#include "openssl/md5.h"
#include "androidJniLog.h"
#include "openssl/ossl_typ.h"
#include "openssl/evp.h"
#include "openssl/opensslv.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_sw_openssl_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    LOGI("openssl version:%s", OpenSSL_version(OPENSSL_VERSION));
    return env->NewStringUTF(hello.c_str());
}

const unsigned char *aesKey = (unsigned char *) "1234567890asdzxc";
extern "C" JNIEXPORT jstring JNICALL
Java_com_sw_openssl_MainActivity_jniMD5(
        JNIEnv* env,
        jobject /* this */,
        jstring text) {
    const char* data = env->GetStringUTFChars(text, nullptr);
    unsigned char md[MD5_DIGEST_LENGTH];
    unsigned char buf[MD5_DIGEST_LENGTH * 2 + 1];
    //1. 初始化
    MD5_CTX md5Ctx;
    int ret = MD5_Init(&md5Ctx);
    if (1 != ret) {
        LOGE("MD5_Init failed...");
    }
    //2. 添加数据
    ret = MD5_Update(&md5Ctx, (const void *) data, strlen((char *) data));
    if (1 != ret) {
        LOGE("MD5_Update failed...");
    }
    //3. 计算结果
    ret = MD5_Final(md, &md5Ctx);
    if (1 != ret) {
        LOGE("MD5_Final failed...");
    }
    //4. 输出结果
    memset(buf, 0, MD5_DIGEST_LENGTH * 2 + 1);
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        sprintf((char *) &buf[i * 2], "%02x", md[i]);
    }
    LOGI("MD5 释放指针...");
    env->ReleaseStringUTFChars(text, data);
    return env->NewStringUTF(reinterpret_cast<const char *>(buf));
}

extern "C" JNIEXPORT jbyteArray JNICALL
Java_com_sw_openssl_MainActivity_aesEncode(JNIEnv *env, jobject, jbyteArray src_) {
    LOGI("AES->对称密钥，也就是说加密和解密用的是同一个密钥");
    jbyte *src = env->GetByteArrayElements(src_, nullptr);
    jsize src_Len = env->GetArrayLength(src_);

    int outLen = 0, cipherText_len;

    auto *out = (unsigned char *) malloc((src_Len / 16 + 1) * 16);
    //清空内存空间
    memset(out, 0, (src_Len / 16 + 1) * 16);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);
    LOGI("AES->指定加密算法，初始化加密key/iv");
    EVP_CipherInit_ex(ctx, EVP_aes_128_ecb(), nullptr, aesKey, nullptr, 1);
    LOGI("AES->进行加密操作");
    EVP_CipherUpdate(ctx, out, &outLen, (const unsigned char *) src, src_Len);
    cipherText_len = outLen;

    LOGI("AES->结束加密操作");
    EVP_CipherFinal_ex(ctx, out + outLen, &outLen);
    cipherText_len += outLen;

    LOGI("AES->EVP_CIPHER_CTX_cleanup");
    EVP_CIPHER_CTX_cleanup(ctx);

    LOGI("AES->从jni释放数据指针");
    env->ReleaseByteArrayElements(src_, src, 0);

    jbyteArray cipher = env->NewByteArray(cipherText_len);
    LOGI("AES->在堆中分配ByteArray数组对象成功，将拷贝数据到数组中");
    env->SetByteArrayRegion(cipher, 0, cipherText_len, (jbyte *) out);
    LOGI("AES->释放内存");
    free(out);

    return cipher;
}


extern "C" JNIEXPORT jbyteArray JNICALL
Java_com_sw_openssl_MainActivity_aesDecode(JNIEnv *env, jobject, jbyteArray src_) {
    LOGI("AES->对称密钥，也就是说加密和解密用的是同一个密钥");
    jbyte *src = env->GetByteArrayElements(src_, nullptr);
    jsize src_Len = env->GetArrayLength(src_);

    int outLen = 0, plaintext_len;

    auto *out = (unsigned char *) malloc(src_Len);
    memset(out, 0, src_Len);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);
    LOGI("AES->指定解密算法，初始化解密key/iv");
    EVP_CipherInit_ex(ctx, EVP_aes_128_ecb(), nullptr, (const unsigned char *) aesKey, nullptr, 0);
    LOGI("AES->进行解密操作");
    EVP_CipherUpdate(ctx, out, &outLen, (const unsigned char *) src, src_Len);
    plaintext_len = outLen;

    LOGI("AES->结束解密操作");
    EVP_CipherFinal_ex(ctx, out + outLen, &outLen);
    plaintext_len += outLen;

    LOGI("AES->EVP_CIPHER_CTX_cleanup");
    EVP_CIPHER_CTX_cleanup(ctx);

    LOGI("AES->从jni释放数据指针");
    env->ReleaseByteArrayElements(src_, src, 0);

    jbyteArray cipher = env->NewByteArray(plaintext_len);
    LOGI("AES->在堆中分配ByteArray数组对象成功，将拷贝数据到数组中");
    env->SetByteArrayRegion(cipher, 0, plaintext_len, (jbyte *) out);
    LOGI("AES->释放内存");
    free(out);

    return cipher;
}


