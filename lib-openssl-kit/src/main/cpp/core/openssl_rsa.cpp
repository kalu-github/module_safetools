#include <jni.h>
#include <string>
#include <rsa.h>
#include <pem.h>
#include "openssl_rsa.h"
#include "common_util.h"

#define KEY_LENGTH  1024


/**
 * RSA公钥加密
 * @param env
 * @param publicKeys_
 * @param src_
 * @return
 */
jbyteArray publicKeyEncrypt(JNIEnv *env, jbyteArray publicKeys_, jbyteArray src_) {
    LOGD("RSA->非对称密码算法，也就是说该算法需要一对密钥，使用其中一个加密，则需要用另一个才能解密");
    jbyte *keys = env->GetByteArrayElements(publicKeys_, nullptr);
    jbyte *src = env->GetByteArrayElements(src_, nullptr);
    jsize src_Len = env->GetArrayLength(src_);

    int ret = 0, src_flen = 0, cipherText_offset = 0, desText_len = 0, src_offset = 0;

    RSA *rsa = nullptr;
    BIO *keybio = nullptr;

    LOGD("RSA->从字符串读取RSA公钥");
    keybio = BIO_new_mem_buf(keys, -1);
    LOGD("RSA->从bio结构中得到RSA结构");
    rsa = PEM_read_bio_RSA_PUBKEY(keybio, nullptr, nullptr, nullptr);
    LOGD("RSA->释放BIO");
    BIO_free_all(keybio);

    int flen = RSA_size(rsa);
    //计算加密后数据长度并分配内存空间
    desText_len = flen * (src_Len / (flen - 11) + 1);

    auto *srcOrigin = (unsigned char *) malloc(src_Len);
    auto *cipherText = (unsigned char *) malloc(flen);
    auto *desText = (unsigned char *) malloc(desText_len);
    memset(desText, 0, desText_len);

    memset(srcOrigin, 0, src_Len);
    memcpy(srcOrigin, src, src_Len);

    LOGD("RSA->对数据进行公钥加密运算");
    //RSA_PKCS1_PADDING最大加密长度：128-11；RSA_NO_PADDING最大加密长度：128
    for (int i = 0; i <= src_Len / (flen - 11); i++) {
        src_flen = (i == src_Len / (flen - 11)) ? src_Len % (flen - 11) : flen - 11;
        if (src_flen == 0) {
            break;
        }

        memset(cipherText, 0, flen);
        ret = RSA_public_encrypt(src_flen, srcOrigin + src_offset, cipherText, rsa,
                                 RSA_PKCS1_PADDING);
        LOGD("ret  %d ", ret);
        memcpy(desText + cipherText_offset, cipherText, ret);
        cipherText_offset += ret;
        src_offset += src_flen;
    }

    RSA_free(rsa);
    LOGD("RSA->CRYPTO_cleanup_all_ex_data");
    CRYPTO_cleanup_all_ex_data();

    LOGD("RSA->从jni释放数据指针");
    env->ReleaseByteArrayElements(publicKeys_, keys, 0);
    env->ReleaseByteArrayElements(src_, src, 0);

    jbyteArray cipher = env->NewByteArray(cipherText_offset);
    LOGD("RSA->在堆中分配ByteArray数组对象成功，将拷贝数据到数组中");
    env->SetByteArrayRegion(cipher, 0, cipherText_offset, (jbyte *) desText);
    LOGD("RSA->释放内存");
    free(srcOrigin);
    free(cipherText);
    free(desText);

    return cipher;
}

/**
 * RSA 私钥加密
 * @param env
 * @param privateKeys_
 * @param src_
 * @return
 */
jbyteArray privateKeyEncrypt(JNIEnv *env, jbyteArray privateKeys_, jbyteArray src_) {
    LOGD("RSA->非对称密码算法，也就是说该算法需要一对密钥，使用其中一个加密，则需要用另一个才能解密");
    jbyte *keys = env->GetByteArrayElements(privateKeys_, nullptr);
    jbyte *src = env->GetByteArrayElements(src_, nullptr);
    jsize src_Len = env->GetArrayLength(src_);

    int ret = 0, src_flen = 0, cipherText_offset = 0, desText_len = 0, src_offset = 0;

    RSA *rsa = nullptr;
    BIO *keybio = nullptr;

    LOGD("RSA->从字符串读取RSA私钥");
    keybio = BIO_new_mem_buf(keys, -1);
    LOGD("RSA->从bio结构中得到RSA结构");
    rsa = PEM_read_bio_RSAPrivateKey(keybio, nullptr, nullptr, nullptr);
    LOGD("RSA->释放BIO");
    BIO_free_all(keybio);

    int flen = RSA_size(rsa);
    desText_len = flen * (src_Len / (flen - 11) + 1);

    auto *srcOrigin = (unsigned char *) malloc(src_Len);
    auto *cipherText = (unsigned char *) malloc(flen);
    auto *desText = (unsigned char *) malloc(desText_len);
    memset(desText, 0, desText_len);

    memset(srcOrigin, 0, src_Len);
    memcpy(srcOrigin, src, src_Len);

    LOGD("RSA->对数据进行私钥加密运算");
    //RSA_PKCS1_PADDING最大加密长度：128-11；RSA_NO_PADDING最大加密长度：128
    for (int i = 0; i <= src_Len / (flen - 11); i++) {
        src_flen = (i == src_Len / (flen - 11)) ? src_Len % (flen - 11) : flen - 11;
        if (src_flen == 0) {
            break;
        }

        memset(cipherText, 0, flen);
        ret = RSA_private_encrypt(src_flen, srcOrigin + src_offset, cipherText, rsa,
                                  RSA_PKCS1_PADDING);

        memcpy(desText + cipherText_offset, cipherText, ret);
        cipherText_offset += ret;
        src_offset += src_flen;
    }

    RSA_free(rsa);
    LOGD("RSA->CRYPTO_cleanup_all_ex_data");
    CRYPTO_cleanup_all_ex_data();

    LOGD("RSA->从jni释放数据指针");
    env->ReleaseByteArrayElements(privateKeys_, keys, 0);
    env->ReleaseByteArrayElements(src_, src, 0);

    jbyteArray cipher = env->NewByteArray(cipherText_offset);
    LOGD("RSA->在堆中分配ByteArray数组对象成功，将拷贝数据到数组中");
    env->SetByteArrayRegion(cipher, 0, cipherText_offset, (jbyte *) desText);
    LOGD("RSA->释放内存");
    free(srcOrigin);
    free(cipherText);
    free(desText);

    return cipher;
}

/**
 * RSA 私钥解密
 * @param privateKeys_
 * @param src_
 * @return
 */
jbyteArray privateKeyDecrypt(JNIEnv *env, jbyteArray privateKeys_, jbyteArray src_) {
    LOGD("RSA->非对称密码算法，也就是说该算法需要一对密钥，使用其中一个加密，则需要用另一个才能解密");
    jbyte *keys = env->GetByteArrayElements(privateKeys_, nullptr);
    jbyte *src = env->GetByteArrayElements(src_, nullptr);
    jsize src_Len = env->GetArrayLength(src_);

    int ret = 0, src_flen = 0, plaintext_offset = 0, descText_len = 0, src_offset = 0;

    RSA *rsa = nullptr;
    BIO *keybio = nullptr;

    LOGD("RSA->从字符串读取RSA私钥");
    keybio = BIO_new_mem_buf(keys, -1);
    LOGD("RSA->从bio结构中得到RSA结构");
    rsa = PEM_read_bio_RSAPrivateKey(keybio, nullptr, nullptr, nullptr);
    LOGD("RSA->释放BIO");
    BIO_free_all(keybio);

    int flen = RSA_size(rsa);
    descText_len = (flen - 11) * (src_Len / flen + 1);

    auto *srcOrigin = (unsigned char *) malloc(src_Len);
    auto *plaintext = (unsigned char *) malloc(flen - 11);
    auto *desText = (unsigned char *) malloc(descText_len);
    memset(desText, 0, descText_len);

    memset(srcOrigin, 0, src_Len);
    memcpy(srcOrigin, src, src_Len);

    LOGD("RSA->对数据进行私钥解密运算");
    //一次性解密数据最大字节数RSA_size
    for (int i = 0; i <= src_Len / flen; i++) {
        if (i == src_Len / flen) {
            src_flen = src_Len % flen;
        } else {
            src_flen = flen;
        }
        if (src_flen == 0) {
            break;
        }

        memset(plaintext, 0, flen - 11);
        ret = RSA_private_decrypt(src_flen, srcOrigin + src_offset, plaintext, rsa,
                                  RSA_PKCS1_PADDING);

        memcpy(desText + plaintext_offset, plaintext, ret);
        plaintext_offset += ret;
        src_offset += src_flen;
    }

    RSA_free(rsa);
    LOGD("RSA->CRYPTO_cleanup_all_ex_data");
    CRYPTO_cleanup_all_ex_data();

    LOGD("RSA->从jni释放数据指针");
    env->ReleaseByteArrayElements(privateKeys_, keys, 0);
    env->ReleaseByteArrayElements(src_, src, 0);

    jbyteArray cipher = env->NewByteArray(plaintext_offset);
    LOGD("RSA->在堆中分配ByteArray数组对象成功，将拷贝数据到数组中");
    env->SetByteArrayRegion(cipher, 0, plaintext_offset, (jbyte *) desText);
    LOGD("RSA->释放内存");
    free(srcOrigin);
    free(plaintext);
    free(desText);

    return cipher;
}

/**
 * RSA 公钥解密
 * @param publicKeys_
 * @param src_
 * @return
 */
jbyteArray publicKeyDecrypt(JNIEnv *env, jbyteArray publicKeys_, jbyteArray src_) {
    LOGD("RSA->非对称密码算法，也就是说该算法需要一对密钥，使用其中一个加密，则需要用另一个才能解密");
    jbyte *keys = env->GetByteArrayElements(publicKeys_, nullptr);
    jbyte *src = env->GetByteArrayElements(src_, nullptr);
    jsize src_Len = env->GetArrayLength(src_);

    int ret = 0, src_flen = 0, plaintext_offset = 0, desText_len = 0, src_offset = 0;

    RSA *rsa = nullptr;
    BIO *keybio = nullptr;

    LOGD("RSA->从字符串读取RSA公钥");
    keybio = BIO_new_mem_buf(keys, -1);
    LOGD("RSA->从bio结构中得到RSA结构");
    rsa = PEM_read_bio_RSA_PUBKEY(keybio, nullptr, nullptr, nullptr);
    LOGD("RSA->释放BIO");
    BIO_free_all(keybio);

    int flen = RSA_size(rsa);
    desText_len = (flen - 11) * (src_Len / flen + 1);

    auto *srcOrigin = (unsigned char *) malloc(src_Len);
    auto *plaintext = (unsigned char *) malloc(flen - 11);
    auto *desText = (unsigned char *) malloc(desText_len);
    memset(desText, 0, desText_len);

    memset(srcOrigin, 0, src_Len);
    memcpy(srcOrigin, src, src_Len);

    LOGD("RSA->对数据进行公钥解密运算");
    //一次性解密数据最大字节数RSA_size
    for (int i = 0; i <= src_Len / flen; i++) {
        src_flen = (i == src_Len / flen) ? src_Len % flen : flen;
        if (src_flen == 0) {
            break;
        }

        memset(plaintext, 0, flen - 11);
        ret = RSA_public_decrypt(src_flen, srcOrigin + src_offset, plaintext, rsa,
                                 RSA_PKCS1_PADDING);

        memcpy(desText + plaintext_offset, plaintext, ret);
        plaintext_offset += ret;
        src_offset += src_flen;
    }

    RSA_free(rsa);
    LOGD("RSA->CRYPTO_cleanup_all_ex_data");
    CRYPTO_cleanup_all_ex_data();

    LOGD("RSA->从jni释放数据指针");
    env->ReleaseByteArrayElements(publicKeys_, keys, 0);
    env->ReleaseByteArrayElements(src_, src, 0);

    jbyteArray cipher = env->NewByteArray(plaintext_offset);
    LOGD("RSA->在堆中分配ByteArray数组对象成功，将拷贝数据到数组中");
    env->SetByteArrayRegion(cipher, 0, plaintext_offset, (jbyte *) desText);
    LOGD("RSA->释放内存");
    free(srcOrigin);
    free(plaintext);
    free(desText);

    return cipher;
}

/**
 * RSA 私钥加签
 * @param env
 * @param privateKeys_
 * @param src_
 * @return
 */
jbyteArray privateKeySign(JNIEnv *env, jbyteArray privateKeys_, jbyteArray src_) {
    LOGD("RSA->非对称密码算法，也就是说该算法需要一对密钥，使用其中一个加密，则需要用另一个才能解密");
    jbyte *keys = env->GetByteArrayElements(privateKeys_, nullptr);
    jbyte *src = env->GetByteArrayElements(src_, nullptr);
    jsize src_Len = env->GetArrayLength(src_);

    unsigned int siglen = 0;
    unsigned char digest[SHA_DIGEST_LENGTH];

    RSA *rsa = nullptr;
    BIO *keybio = nullptr;

    LOGD("RSA->从字符串读取RSA公钥");
    keybio = BIO_new_mem_buf(keys, -1);
    LOGD("RSA->从bio结构中得到RSA结构");
    rsa = PEM_read_bio_RSAPrivateKey(keybio, nullptr, nullptr, nullptr);
    LOGD("RSA->释放BIO");
    BIO_free_all(keybio);

    auto *sign = (unsigned char *) malloc(129);
    memset(sign, 0, 129);

    LOGD("RSA->对数据进行摘要运算");
    SHA1((const unsigned char *) src, src_Len, digest);
    LOGD("RSA->对摘要进行RSA私钥加密");
    RSA_sign(NID_sha1, digest, SHA_DIGEST_LENGTH, sign, &siglen, rsa);

    RSA_free(rsa);
    LOGD("RSA->CRYPTO_cleanup_all_ex_data");
    CRYPTO_cleanup_all_ex_data();

    LOGD("RSA->从jni释放数据指针");
    env->ReleaseByteArrayElements(privateKeys_, keys, 0);
    env->ReleaseByteArrayElements(src_, src, 0);

    jbyteArray cipher = env->NewByteArray(siglen);
    LOGD("RSA->在堆中分配ByteArray数组对象成功，将拷贝数据到数组中");
    env->SetByteArrayRegion(cipher, 0, siglen, (jbyte *) sign);
    LOGD("RSA->释放内存");
    free(sign);

    return cipher;
}

/**
 * RSA 公钥验签
 * @param env
 * @param publicKeys_
 * @param src_  内容原文
 * @param sign_ 加签内容
 * @return
 */
jint
publicKeyVerify(JNIEnv *env, jbyteArray publicKeys_, jbyteArray src_, jbyteArray sign_) {
    LOGD("RSA->非对称密码算法，也就是说该算法需要一对密钥，使用其中一个加密，则需要用另一个才能解密");
    jbyte *keys = env->GetByteArrayElements(publicKeys_, nullptr);
    jbyte *src = env->GetByteArrayElements(src_, nullptr);
    jbyte *sign = env->GetByteArrayElements(sign_, nullptr);

    jsize src_Len = env->GetArrayLength(src_);
    jsize siglen = env->GetArrayLength(sign_);

    int ret;
    unsigned char digest[SHA_DIGEST_LENGTH];

    RSA *rsa = nullptr;
    BIO *keybio = nullptr;

    LOGD("RSA->从字符串读取RSA公钥");
    keybio = BIO_new_mem_buf(keys, -1);
    LOGD("RSA->从bio结构中得到RSA结构");
    rsa = PEM_read_bio_RSA_PUBKEY(keybio, nullptr, nullptr, nullptr);
    LOGD("RSA->释放BIO");
    BIO_free_all(keybio);

    LOGD("RSA->对数据进行摘要运算");
    SHA1((const unsigned char *) src, src_Len, digest);
    LOGD("RSA->对摘要进行RSA公钥验证");
    ret = RSA_verify(NID_sha1, digest, SHA_DIGEST_LENGTH, (const unsigned char *) sign, siglen,
                     rsa);

    RSA_free(rsa);
    LOGD("RSA->CRYPTO_cleanup_all_ex_data");
    CRYPTO_cleanup_all_ex_data();

    LOGD("RSA->从jni释放数据指针");
    env->ReleaseByteArrayElements(publicKeys_, keys, 0);
    env->ReleaseByteArrayElements(src_, src, 0);
    env->ReleaseByteArrayElements(sign_, sign, 0);

    return ret;
}

void generateRSAKey() {
    // 公私密钥对
    size_t pri_len;
    size_t pub_len;
    char *pri_key = nullptr;
    char *pub_key = nullptr;
    RSA *keypair = RSA_new();
    BIGNUM *bne = BN_new();
    BN_set_word(bne, RSA_F4);
    // 生成密钥对
    RSA_generate_key_ex(keypair, KEY_LENGTH, bne, nullptr);

    BIO *pri = BIO_new(BIO_s_mem());
    BIO *pub = BIO_new(BIO_s_mem());

    PEM_write_bio_RSAPrivateKey(pri, keypair, nullptr, nullptr, 0, nullptr, nullptr);
    PEM_write_bio_RSAPublicKey(pub, keypair);

    // 获取长度
    pri_len = BIO_pending(pri);
    pub_len = BIO_pending(pub);

    // 密钥对读取到字符串
    pri_key = (char *) malloc(pri_len + 1);
    pub_key = (char *) malloc(pub_len + 1);

    BIO_read(pri, pri_key, pri_len);
    BIO_read(pub, pub_key, pub_len);

    pri_key[pri_len] = '\0';
    pub_key[pub_len] = '\0';

    // 内存释放
    RSA_free(keypair);
    BN_free(bne);
    BIO_free_all(pub);
    BIO_free_all(pri);

    LOGD("openssl pkcs1 类型私钥  ：\n%s", pri_key);
    //注 ： 这里实际生成的公钥是pkcs1类型的 java使用的是pkcs8 需要转换
    LOGD("openssl pkcs1 类型公钥  ：\n%s", pub_key);

    free(pri_key);
    free(pub_key);
}