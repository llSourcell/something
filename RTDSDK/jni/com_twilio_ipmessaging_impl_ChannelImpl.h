/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_twilio_ipmessaging_impl_ChannelImpl */

#ifndef _Included_com_twilio_ipmessaging_impl_ChannelImpl
#define _Included_com_twilio_ipmessaging_impl_ChannelImpl
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_twilio_ipmessaging_impl_ChannelImpl
 * Method:    joinChannel
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_ChannelImpl_joinChannel
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_twilio_ipmessaging_impl_ChannelImpl
 * Method:    levaeChannel
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_ChannelImpl_leaveChannel
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_twilio_ipmessaging_impl_ChannelImpl
 * Method:    destroyChannel
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_ChannelImpl_destroyChannel
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_twilio_ipmessaging_impl_ChannelImpl
 * Method:    getStatus
 *
 */
JNIEXPORT jint JNICALL Java_com_twilio_ipmessaging_impl_ChannelImpl_getStatus
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_twilio_ipmessaging_impl_ChannelImpl
 * Method:    getMessage
 *
 */
JNIEXPORT jobjectArray JNICALL Java_com_twilio_ipmessaging_impl_ChannelImpl_getMessages
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_twilio_ipmessaging_impl_ChannelImpl
 * Method:    getMessagesObject
 *
 */
JNIEXPORT jobject JNICALL Java_com_twilio_ipmessaging_impl_ChannelImpl_getMessagesObject
  (JNIEnv *, jobject, jstring);


/*
 * Class:     com_twilio_ipmessaging_impl_ChannelImpl
 * Method:    updateChannelName
 *
 */

JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_ChannelImpl_updateChannelName
(JNIEnv *, jobject, jstring, jstring);


/*
 * Class:     com_twilio_ipmessaging_impl_ChannelImpl
 * Method:    getMembers
 *
 */
JNIEXPORT jobjectArray JNICALL Java_com_twilio_ipmessaging_impl_ChannelImpl_getMembers
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_twilio_ipmessaging_impl_ChannelImpl
 * Method:    updateChannelAttributes
 *
 */
JNIEXPORT jobjectArray JNICALL Java_com_twilio_ipmessaging_impl_ChannelImpl_updateChannelAttributes
  (JNIEnv *, jobject, jstring, jobject);



#ifdef __cplusplus
}
#endif
#endif