/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_twilio_ipmessaging_impl_MessagesImpl */

#ifndef _Included_MessagesImpl
#define _Included_MessagesImpl
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
 * Method:    createMessageNativeBuffer
 * Signature: (Ljava/lang/String;)Lcom/twilio/ipmessaging/Message;
 */
JNIEXPORT jobject JNICALL Java_com_twilio_ipmessaging_impl_MessagesImpl_createMessageNativeBuffer
(JNIEnv *, jobject, jbyteArray);

/*
 * Class:     com_twilio_ipmessaging_impl_MessagesImpl
 * Method:    sendMessageNative
 * Signature: (Lcom/twilio/ipmessaging/Message;)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_MessagesImpl_sendMessageNative
  (JNIEnv *, jobject, jobject, jobject);


/*
 * Class:     com_twilio_ipmessaging_impl_MessagesImpl
 * Method:    removeMessageNative
 * Signature: (Lcom/twilio/ipmessaging/Message;)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_MessagesImpl_removeMessageNative
  (JNIEnv *, jobject, jobject, jobject);

/*
 * Class:     com_twilio_ipmessaging_impl_MessagesImpl
 * Method:    getMessageIndexArrayNative
 * Signature: (J)[J
 */
JNIEXPORT jlongArray JNICALL Java_com_twilio_ipmessaging_impl_MessagesImpl_getMessageIndexArrayNative
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_twilio_ipmessaging_impl_MessagesImpl
 * Method:    getMessage
 * Signature: (JJ)Lcom/twilio/ipmessaging/Message;
 */
JNIEXPORT jobject JNICALL Java_com_twilio_ipmessaging_impl_MessagesImpl_getMessageByIndex
  (JNIEnv *, jobject, jlong, jlong);


#ifdef __cplusplus
}
#endif
#endif
