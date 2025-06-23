#include "evp.h"
#include "openssl_des.h"
#include "des.h"
#include "string"
#include "common_util.h"


/**
 * des3cbc加密
 * @param env
 * @param keys_
 * @param iv_
 * @param src_
 * @return
 */
jbyteArray des3CbcEncrypt(JNIEnv *env, jbyteArray keys_, jbyteArray iv_, jbyteArray src_) {
    LOGD("DES->对称密钥，也就是说加密和解密用的是同一个密钥");
    jbyte *keys = env->GetByteArrayElements(keys_, nullptr);
    jbyte *src = env->GetByteArrayElements(src_, nullptr);
    jbyte *iv = env->GetByteArrayElements(iv_, nullptr);
    jsize src_Len = env->GetArrayLength(src_);

    int outlen = 0, cipherText_len = 0;
    //计算加密后数据长度并分配内存空间
    auto *out = (unsigned char *) malloc((src_Len / 8 + 1) * 8);
    //清空内存空间
    memset(out, 0, (src_Len / 8 + 1) * 8);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    EVP_CIPHER_CTX_init(ctx);
    //设置padding
    //EVP_CIPHER_CTX_set_padding(ctx,EVP_PADDING_PKCS7);

    LOGD("DES->指定加密算法，初始化加密key/iv");
    EVP_EncryptInit_ex(ctx, EVP_des_ede_cbc(), nullptr, (const unsigned char *) keys,
                       reinterpret_cast<const unsigned char *>(iv));
    LOGD("DES->对数据进行加密运算");
    EVP_EncryptUpdate(ctx, out, &outlen, (const unsigned char *) src, src_Len);
    cipherText_len = outlen;

    LOGD("DES->结束加密运算");
    EVP_EncryptFinal_ex(ctx, out + outlen, &outlen);
    cipherText_len += outlen;

    LOGD("DES->EVP_CIPHER_CTX_cleanup");
    EVP_CIPHER_CTX_cleanup(ctx);

    LOGD("DES->从jni释放数据指针");
    env->ReleaseByteArrayElements(keys_, keys, 0);
    env->ReleaseByteArrayElements(src_, src, 0);
    env->ReleaseByteArrayElements(iv_, iv, 0);

    jbyteArray cipher = env->NewByteArray(cipherText_len);
    LOGD("DES->在堆中分配ByteArray数组对象成功，将拷贝数据到数组中");
    env->SetByteArrayRegion(cipher, 0, cipherText_len, (jbyte *) out);
    LOGD("DES->释放内存");
    free(out);
    return cipher;
}

/**
 * des3cbc解密
 * @param env
 * @param keys_
 * @param iv_
 * @param src_
 * @return
 */
jbyteArray des3CbcDecrypt(JNIEnv *env, jbyteArray keys_, jbyteArray iv_, jbyteArray src_) {
    LOGD("DES->对称密钥，也就是说加密和解密用的是同一个密钥");
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
    //EVP_CIPHER_CTX_set_padding(ctx,EVP_PADDING_PKCS7);

    LOGD("DES->指定解密算法，初始化解密key/iv");
    EVP_DecryptInit_ex(ctx, EVP_des_ede3_cbc(), nullptr, (const unsigned char *) keys,
                       (const unsigned char *) iv);
    LOGD("DES->对数据进行解密运算");
    EVP_DecryptUpdate(ctx, out, &outlen, (const unsigned char *) src, src_Len);
    plaintext_len = outlen;

    LOGD("DES->结束解密运算");
    EVP_DecryptFinal_ex(ctx, out + outlen, &outlen);
    plaintext_len += outlen;

    LOGD("DES->EVP_CIPHER_CTX_cleanup");
    EVP_CIPHER_CTX_cleanup(ctx);

    LOGD("DES->从jni释放数据指针");
    env->ReleaseByteArrayElements(keys_, keys, 0);
    env->ReleaseByteArrayElements(src_, src, 0);
    env->ReleaseByteArrayElements(iv_, iv, 0);

    jbyteArray cipher = env->NewByteArray(plaintext_len);
    LOGD("DES->在堆中分配ByteArray数组对象成功，将拷贝数据到数组中");
    env->SetByteArrayRegion(cipher, 0, plaintext_len, (jbyte *) out);
    LOGD("DES->释放内存");
    free(out);

    return cipher;
}
