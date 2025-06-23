
#ifndef OPENSSL_DES_H
#define OPENSSL_DES_H

#include "jni.h"

#ifdef  __cplusplus
extern "C" {
#endif

/**
* des3cbc加密
*/
jbyteArray des3CbcEncrypt(JNIEnv *env, jbyteArray keys_, jbyteArray iv_, jbyteArray src_);

/**
 * des3cbc解密
 */
jbyteArray des3CbcDecrypt(JNIEnv *env, jbyteArray keys_, jbyteArray iv_, jbyteArray src_);

#ifdef  __cplusplus
}
#endif

#endif //OPENSSL_DES_H
