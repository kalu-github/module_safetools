#include <jni.h>
#include <string.h>

#ifndef JNI_CIPHER_H
#define JNI_CIPHER_H

jstring aesEncode(JNIEnv *env, jobject instance, jstring jstr);

jstring aesEncodeMult(JNIEnv *env, jobject instance, jstring jstr, jboolean checkRoot,
                      jboolean checkEmulator, jboolean checkXposed, jboolean checkSignature);

jstring aesDecode(JNIEnv *env, jobject instance, jstring jstr);

jstring aesDecodeMult(JNIEnv *env, jobject instance, jstring jstr, jboolean checkRoot,
                      jboolean checkEmulator, jboolean checkXposed, jboolean checkSignature);

#endif