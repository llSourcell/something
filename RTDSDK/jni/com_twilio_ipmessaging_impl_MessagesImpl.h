/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_twilio_ipmessaging_impl_MessagesImpl */

#ifndef _Included_com_twilio_ipmessaging_impl_MessagesImpl
#define _Included_com_twilio_ipmessaging_impl_MessagesImpl
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_twilio_ipmessaging_impl_MessagesImpl
 * Method:    createMessageNative
 * Signature: (Ljava/lang/String;)Lcom/twilio/ipmessaging/Message;
 */
JNIEXPORT jobject JNICALL Java_com_twilio_ipmessaging_impl_MessagesImpl_createMessageNative
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_twilio_ipmessaging_impl_MessagesImpl
 * Method:    sendMessageNative
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_MessagesImpl_sendMessageNative
  (JNIEnv *, jobject, jobject);

/*
 * Class:     com_twilio_ipmessaging_impl_MessagesImpl
 * Method:    getMessagesNative
 * Signature: (J)[Lcom/twilio/ipmessaging/Message;
 */
JNIEXPORT jobjectArray JNICALL Java_com_twilio_ipmessaging_impl_MessagesImpl_getMessagesNative
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif