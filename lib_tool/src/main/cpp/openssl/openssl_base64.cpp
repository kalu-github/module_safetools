#include <evp.h>
#include <bio.h>
#include <buffer.h>
#include <string>
#include <iostream>
#include "openssl_base64.h"

/**
 * base64编码
 * @param input 待编码内容
 * @param length  待编码内容长度
 * @param newLine  是否换行
 * @return  编码后内容
 */
extern "C" JNIEXPORT jbyteArray JNICALL
openssl_base64_encrypt(JNIEnv *env, jbyteArray src_) {
    // 获取输入数据
    jbyte *input = env->GetByteArrayElements(src_, nullptr);
    jsize inputLength = env->GetArrayLength(src_);

    if (input == nullptr || inputLength == 0) {
        return nullptr;
    }

    // 创建Base64编码上下文
    BIO *b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL); // 不添加换行符

    BUF_MEM *bptr = nullptr;
    BIO *bmem = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, bmem);

    // 写入数据并刷新缓冲区
    int written = BIO_write(b64, input, inputLength);
    if (written <= 0) {
        BIO_free_all(b64);
        env->ReleaseByteArrayElements(src_, input, JNI_ABORT);
        return nullptr;
    }

    if (BIO_flush(b64) != 1) {
        BIO_free_all(b64);
        env->ReleaseByteArrayElements(src_, input, JNI_ABORT);
        return nullptr;
    }

    // 获取内存指针和长度
    BIO_get_mem_ptr(b64, &bptr);

    // 创建Java字节数组
    jbyteArray result = env->NewByteArray(bptr->length);
    if (result != nullptr) {
        env->SetByteArrayRegion(result, 0, bptr->length, (const jbyte *) bptr->data);
    }

    // 释放资源
    BIO_free_all(b64);
    env->ReleaseByteArrayElements(src_, input, JNI_ABORT);

    return result;
}

/**
 * base64解码
 * @param input 待解码内容
 * @param length  待解码内容长度
 * @param newLine  是否换行
 * @return  解码后内容
 */
extern "C" JNIEXPORT jbyteArray JNICALL
openssl_base64_decrypt(JNIEnv *env, jbyteArray src_) {
    // 获取输入数据
    jbyte *input = env->GetByteArrayElements(src_, nullptr);
    jsize input_length = env->GetArrayLength(src_);

    if (input == nullptr || input_length <= 0) {
        return nullptr;
    }

    // 计算解码后最大长度
    int max_output_length = (input_length * 3) / 4 + 1;
    unsigned char *output = (unsigned char *) malloc(max_output_length);

    if (output == nullptr) {
        env->ReleaseByteArrayElements(src_, input, JNI_ABORT);
        return nullptr;
    }

    // 创建Base64解码上下文
    BIO *b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

    BIO *bmem = BIO_new_mem_buf(input, input_length);
    bmem = BIO_push(b64, bmem);

    // 执行解码并获取实际长度
    int actual_output_length = BIO_read(bmem, output, max_output_length);

    // 释放OpenSSL资源
    BIO_free_all(bmem);

    // 释放JNI资源
    env->ReleaseByteArrayElements(src_, input, JNI_ABORT);

    if (actual_output_length <= 0) {
        free(output);
        return nullptr;
    }

    // 创建Java字节数组并设置内容
    jbyteArray result = env->NewByteArray(actual_output_length);
    if (result != nullptr) {
        env->SetByteArrayRegion(result, 0, actual_output_length, (const jbyte *) output);
    }

    // 释放本地内存
    free(output);

    return result;
}