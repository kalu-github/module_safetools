#include <string>
#include <jni.h>
#include "ossl_typ.h"
#include "evp.h"
#include "buffer.h"
#include "common_util.h"

/* Padding modes */
#define EVP_PADDING_PKCS7       1
#define EVP_PADDING_ISO7816_4   2
#define EVP_PADDING_ANSI923     3
#define EVP_PADDING_ISO10126    4
#define EVP_PADDING_ZERO        5


/**
 * AES-ECB-128加密
 * @param env
 * @param keys_
 * @param iv_
 * @param src_
 * @return
 */
extern "C"
jbyteArray
openssl_aes128_ecb_encrypt(JNIEnv *env, jbyteArray input_key, jbyteArray input_data) {

    if (!input_key) {
        LOGD("openssl_aes -> openssl_aes128_ecb_encrypt -> input_key = null");
        return nullptr;
    }

    if (!input_data) {
        LOGD("openssl_aes -> openssl_aes128_ecb_encrypt -> input_data = null");
        return nullptr;
    }

    // 验证密钥
    jbyte *input_key_str = env->GetByteArrayElements(input_key, nullptr);
    LOGD("openssl_aes -> openssl_aes128_ecb_encrypt -> input_key_str = %s", input_key_str);
    jsize input_key_len = env->GetArrayLength(input_key);
    LOGD("openssl_aes -> openssl_aes128_ecb_encrypt -> input_key_len = %d", input_key_len);
    if (input_key_len != 16) {
        LOGD("openssl_aes -> openssl_aes128_ecb_encrypt -> 密钥长度必须为16字节");
        env->ReleaseByteArrayElements(input_key, input_key_str, JNI_ABORT);
        return nullptr;
    }

    // 加密内容
    jbyte *input_data_str = env->GetByteArrayElements(input_data, nullptr);
    LOGD("openssl_aes -> openssl_aes128_ecb_encrypt -> input_data_str = %s", input_data_str);
    jsize input_data_len = env->GetArrayLength(input_data);
    LOGD("openssl_aes -> openssl_aes128_ecb_encrypt -> input_data_len = %d", input_data_len);

    // 计算加密后数据长度并分配内存
    int maxCipherLen = input_data_len + 16; // 考虑PKCS7填充
    unsigned char *cipherData = (unsigned char *) malloc(maxCipherLen);
    if (!cipherData) {
        LOGD("openssl_aes -> openssl_aes128_ecb_encrypt -> 内存分配失败");
        env->ReleaseByteArrayElements(input_key, input_key_str, JNI_ABORT);
        env->ReleaseByteArrayElements(input_data, input_data_str, JNI_ABORT);
        return nullptr;
    }
    memset(cipherData, 0, maxCipherLen);

    // 初始化加密上下文
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        LOGD("openssl_aes -> openssl_aes128_ecb_encrypt -> 无法创建加密上下文");
        free(cipherData);
        env->ReleaseByteArrayElements(input_key, input_key_str, JNI_ABORT);
        env->ReleaseByteArrayElements(input_data, input_data_str, JNI_ABORT);
        return nullptr;
    }
    EVP_CIPHER_CTX_init(ctx);

    // 设置加密参数
    EVP_CIPHER_CTX_set_padding(ctx, EVP_PADDING_PKCS7);

    // 初始化加密
    if (!EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), nullptr, (const unsigned char *) input_key_str,
                            nullptr)) {
        LOGD("openssl_aes -> openssl_aes128_ecb_encrypt -> 初始化加密失败");
        EVP_CIPHER_CTX_free(ctx);
        free(cipherData);
        env->ReleaseByteArrayElements(input_key, input_key_str, JNI_ABORT);
        env->ReleaseByteArrayElements(input_data, input_data_str, JNI_ABORT);
        return nullptr;
    }

    // 执行加密
    int outlen = 0, cipherText_len = 0;
    if (!EVP_EncryptUpdate(ctx, cipherData, &outlen, (const unsigned char *) input_data_str,
                           input_data_len)) {
        LOGD("openssl_aes -> openssl_aes128_ecb_encrypt -> 加密更新失败");
        EVP_CIPHER_CTX_free(ctx);
        free(cipherData);
        env->ReleaseByteArrayElements(input_key, input_key_str, JNI_ABORT);
        env->ReleaseByteArrayElements(input_data, input_data_str, JNI_ABORT);
        return nullptr;
    }
    cipherText_len = outlen;

    // 完成加密 (处理填充)
    if (!EVP_EncryptFinal_ex(ctx, cipherData + outlen, &outlen)) {
        LOGD("openssl_aes -> openssl_aes128_ecb_encrypt -> 加密最终块失败");
        EVP_CIPHER_CTX_free(ctx);
        free(cipherData);
        env->ReleaseByteArrayElements(input_key, input_key_str, JNI_ABORT);
        env->ReleaseByteArrayElements(input_data, input_data_str, JNI_ABORT);
        return nullptr;
    }
    cipherText_len += outlen;

    // 清理加密上下文
    EVP_CIPHER_CTX_free(ctx);

    // 准备返回结果
    jbyteArray result = env->NewByteArray(cipherText_len);
    if (result) {
        env->SetByteArrayRegion(result, 0, cipherText_len, (jbyte *) cipherData);
    } else {
        LOGD("openssl_aes -> openssl_aes128_ecb_encrypt -> 创建加密结果数组失败");
    }

    // 释放JNI资源和加密数据
    env->ReleaseByteArrayElements(input_key, input_key_str, JNI_ABORT);
    env->ReleaseByteArrayElements(input_data, input_data_str, JNI_ABORT);
    free(cipherData);

    // 检查是否成功创建结果数组
    if (!result) {
        LOGD("openssl_aes -> openssl_aes128_ecb_encrypt -> 创建结果数组失败");
        return nullptr;
    }

    LOGD("openssl_aes -> openssl_aes128_ecb_encrypt -> 加密成功，结果长度: %d",
         env->GetArrayLength(result));
    return result;
}


extern "C"
jbyteArray
openssl_aes128_ecb_decrypt(JNIEnv *env, jbyteArray input_key, jbyteArray input_data) {

    if (!input_key) {
        LOGD("openssl_aes -> openssl_aes128_ecb_decrypt -> input_key = null");
        return nullptr;
    }

    if (!input_data) {
        LOGD("openssl_aes -> openssl_aes128_ecb_decrypt -> input_data = null");
        return nullptr;
    }

    // 验证密钥
    jbyte *input_key_str = env->GetByteArrayElements(input_key, nullptr);
    LOGD("openssl_aes -> openssl_aes128_ecb_decrypt -> input_key_str = %s", input_key_str);
    jsize input_key_len = env->GetArrayLength(input_key);
    LOGD("openssl_aes -> openssl_aes128_ecb_decrypt -> input_key_len = %d", input_key_len);
    if (input_key_len != 16) {
        LOGD("openssl_aes -> openssl_aes128_ecb_decrypt -> 密钥长度必须为16字节");
        env->ReleaseByteArrayElements(input_key, input_key_str, JNI_ABORT);
        return nullptr;
    }

    // 加密内容
    jbyte *input_data_str = env->GetByteArrayElements(input_data, nullptr);
    LOGD("openssl_aes -> openssl_aes128_ecb_decrypt -> input_data_str = %s", input_data_str);
    jsize input_data_len = env->GetArrayLength(input_data);
    LOGD("openssl_aes -> openssl_aes128_ecb_decrypt -> input_data_len = %d", input_data_len);

    // 检查密钥长度 (AES-128 需要16字节密钥)
    jsize keyLen = env->GetArrayLength(input_key);
    if (keyLen != 16) {
        LOGD("openssl_aes -> openssl_aes128_ecb_decrypt -> 密钥长度必须为16字节");
        env->ReleaseByteArrayElements(input_key, input_key_str, JNI_ABORT);
        env->ReleaseByteArrayElements(input_data, input_data_str, JNI_ABORT);
        return nullptr;
    }

    // 处理解码
    unsigned char *inputData = (unsigned char *) input_data_str;
    int inputLen = input_data_len;

    // 分配解密输出缓冲区 (可能比输入大，因为PKCS7填充)
    int maxOutputLen = inputLen + 16; // 最大可能的填充
    auto *out = (unsigned char *) malloc(maxOutputLen);
    if (!out) {
        LOGD("openssl_aes -> openssl_aes128_ecb_decrypt -> 内存分配失败");
        env->ReleaseByteArrayElements(input_key, input_key_str, JNI_ABORT);
        env->ReleaseByteArrayElements(input_data, input_data_str, JNI_ABORT);
        return nullptr;
    }
    memset(out, 0, maxOutputLen);

    // 初始化加密上下文
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        LOGD("openssl_aes -> openssl_aes128_ecb_decrypt -> 无法创建加密上下文");
        free(out);
        env->ReleaseByteArrayElements(input_key, input_key_str, JNI_ABORT);
        env->ReleaseByteArrayElements(input_data, input_data_str, JNI_ABORT);
        return nullptr;
    }
    EVP_CIPHER_CTX_init(ctx);

    // 设置解密参数
    EVP_CIPHER_CTX_set_padding(ctx, EVP_PADDING_PKCS7);

    // 初始化解密
    if (!EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), nullptr, (const unsigned char *) input_key_str,
                            nullptr)) {
        LOGD("openssl_aes -> openssl_aes128_ecb_decrypt -> 初始化解密失败");
        EVP_CIPHER_CTX_cleanup(ctx);
        EVP_CIPHER_CTX_free(ctx);
        free(out);
        env->ReleaseByteArrayElements(input_key, input_key_str, JNI_ABORT);
        env->ReleaseByteArrayElements(input_data, input_data_str, JNI_ABORT);
        return nullptr;
    }

    // 执行解密
    int outlen = 0, plaintext_len = 0;
    if (!EVP_DecryptUpdate(ctx, out, &outlen, inputData, inputLen)) {
        LOGD("openssl_aes -> openssl_aes128_ecb_decrypt -> 解密更新失败");
        EVP_CIPHER_CTX_cleanup(ctx);
        EVP_CIPHER_CTX_free(ctx);
        free(out);
        env->ReleaseByteArrayElements(input_key, input_key_str, JNI_ABORT);
        env->ReleaseByteArrayElements(input_data, input_data_str, JNI_ABORT);
        return nullptr;
    }
    plaintext_len = outlen;

    // 完成解密 (处理填充)
    if (!EVP_DecryptFinal_ex(ctx, out + outlen, &outlen)) {
        LOGD("openssl_aes -> openssl_aes128_ecb_decrypt -> 解密最终块失败，可能填充不正确");
        EVP_CIPHER_CTX_cleanup(ctx);
        EVP_CIPHER_CTX_free(ctx);
        free(out);
        env->ReleaseByteArrayElements(input_key, input_key_str, JNI_ABORT);
        env->ReleaseByteArrayElements(input_data, input_data_str, JNI_ABORT);
        return nullptr;
    }
    plaintext_len += outlen;

    // 清理资源
    EVP_CIPHER_CTX_cleanup(ctx);
    EVP_CIPHER_CTX_free(ctx);

    // 释放JNI资源
    env->ReleaseByteArrayElements(input_key, input_key_str, JNI_ABORT);
    env->ReleaseByteArrayElements(input_data, input_data_str, JNI_ABORT);

    // 创建Java字节数组并返回结果
    jbyteArray result = env->NewByteArray(plaintext_len);
    if (!result) {
        LOGD("openssl_aes -> openssl_aes128_ecb_decrypt -> 创建结果数组失败");
        free(out);
        return nullptr;
    }

    env->SetByteArrayRegion(result, 0, plaintext_len, (jbyte *) out);
    free(out);

    LOGD("openssl_aes -> openssl_aes128_ecb_decrypt -> 解密成功，结果长度: %d", plaintext_len);
    return result;
}


/**
 * AES-CBC-128加密
 * @param env
 * @param keys_
 * @param iv_
 * @param src_
 * @return
 */
extern "C"
jbyteArray
openssl_aes128_cbc_encrypt(JNIEnv *env, jbyteArray keys_, jbyteArray iv_, jbyteArray src_) {
    LOGD("AES CBC:      对称密钥，也就是说加密和解密用的是同一个密钥");
    jbyte *keys = env->GetByteArrayElements(keys_, nullptr);
    jbyte *src = env->GetByteArrayElements(src_, nullptr);
    jbyte *iv = env->GetByteArrayElements(iv_, nullptr);
    jsize src_Len = env->GetArrayLength(src_);

    int outlen = 0, cipherText_len = 0;
    //计算加密后数据长度并分配内存空间
    auto *out = (unsigned char *) malloc((src_Len / 16 + 1) * 16);
    //清空内存空间
    memset(out, 0, (src_Len / 16 + 1) * 16);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);

    //设置padding
    EVP_CIPHER_CTX_set_padding(ctx, EVP_PADDING_PKCS7);

    LOGD("AES CBC:      指定加密算法，初始化加密key/iv");
    EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, (const unsigned char *) keys,
                       reinterpret_cast<const unsigned char *>(iv));
    LOGD("AES CBC:      对数据进行加密运算");
    EVP_EncryptUpdate(ctx, out, &outlen, (const unsigned char *) src, src_Len);
    cipherText_len = outlen;

    LOGD("AES CBC:      结束加密运算");
    EVP_EncryptFinal_ex(ctx, out + outlen, &outlen);
    cipherText_len += outlen;

    LOGD("AES CBC:      EVP_CIPHER_CTX_cleanup");
    EVP_CIPHER_CTX_cleanup(ctx);

    LOGD("AES CBC:      从jni释放数据指针");
    env->ReleaseByteArrayElements(keys_, keys, 0);
    env->ReleaseByteArrayElements(src_, src, 0);
    env->ReleaseByteArrayElements(iv_, iv, 0);

    jbyteArray cipher = env->NewByteArray(cipherText_len);
    LOGD("AES CBC:      在堆中分配ByteArray数组对象成功，将拷贝数据到数组中");
    env->SetByteArrayRegion(cipher, 0, cipherText_len, (jbyte *) out);
    LOGD("AES CBC:      释放内存");
    free(out);

    return cipher;
}

/**
 * AES-CBC-128解密
 * @param env
 * @param keys_
 * @param iv_
 * @param src_
 * @return
 */
extern "C"
jbyteArray
openssl_aes128_cbc_decrypt(JNIEnv *env, jbyteArray keys_, jbyteArray iv_, jbyteArray src_) {
    LOGD("AES CBC:      对称密钥，也就是说加密和解密用的是同一个密钥");
    jbyte *keys = env->GetByteArrayElements(keys_, nullptr);
    jbyte *src = env->GetByteArrayElements(src_, nullptr);
    jbyte *iv = env->GetByteArrayElements(iv_, nullptr);
    jsize src_Len = env->GetArrayLength(src_);

    int outlen = 0, plaintext_len = 0;

    auto *out = (unsigned char *) malloc(src_Len);
    memset(out, 0, src_Len);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);

    //设置padding
    EVP_CIPHER_CTX_set_padding(ctx, EVP_PADDING_PKCS7);

    LOGD("AES CBC:      指定解密算法，初始化解密key/iv");
    EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), nullptr, (const unsigned char *) keys,
                       (const unsigned char *) iv);
    LOGD("AES CBC:      对数据进行解密运算");
    EVP_DecryptUpdate(ctx, out, &outlen, (const unsigned char *) src, src_Len);
    plaintext_len = outlen;

    LOGD("AES CBC:      结束解密运算");
    EVP_DecryptFinal_ex(ctx, out + outlen, &outlen);
    plaintext_len += outlen;

    LOGD("AES CBC:      EVP_CIPHER_CTX_cleanup");
    EVP_CIPHER_CTX_cleanup(ctx);

    LOGD("AES CBC:      从jni释放数据指针");
    env->ReleaseByteArrayElements(keys_, keys, 0);
    env->ReleaseByteArrayElements(src_, src, 0);
    env->ReleaseByteArrayElements(iv_, iv, 0);

    jbyteArray cipher = env->NewByteArray(plaintext_len);
    LOGD("AES CBC:      在堆中分配ByteArray数组对象成功，将拷贝数据到数组中");
    env->SetByteArrayRegion(cipher, 0, plaintext_len, (jbyte *) out);
    LOGD("AES CBC:      释放内存");
    free(out);

    return cipher;
}