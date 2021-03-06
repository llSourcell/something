/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_twilio_ipmessaging_impl_ChannelsImpl */

#ifndef _Included_ChannelsImpl
#define _Included_ChannelsImpl
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:     com_twilio_ipmessaging_impl_ChannelsImpl
 * Method:    createChannelNativeWithListener
 * Signature: (Ljava/lang/String;IJLcom/twilio/ipmessaging/TwilioIPMessagingClient/CreateChannelListener;)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_ChannelsImpl_createChannelNativeWithListener
  (JNIEnv *, jobject, jstring, jint, jlong, jobject);

/*
 * Class:     com_twilio_ipmessaging_impl_ChannelsImpl
 * Method:    createChannelNativeWithListener
 * Signature: (Ljava/lang/String;IJLcom/twilio/ipmessaging/TwilioIPMessagingClient/CreateChannelListener;)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_ChannelsImpl_createChannelNativeWithListenerWithSDKListener
  (JNIEnv *, jobject, jstring, jint, jlong, jobject, jobject, jobject);

/*
 * Class:     com_twilio_ipmessaging_impl_ChannelsImpl
 * Method:    createChannelNative
 * Signature: (Ljava/lang/String;IJ)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_ChannelsImpl_createChannelNative
  (JNIEnv *, jobject, jstring, jint, jlong);

/*
 * Class:     com_twilio_ipmessaging_impl_ChannelsImpl
 * Method:    createChannelNative
 * Signature: (Ljava/lang/String;IJ)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_ChannelsImpl_createChannelNativeWithSDKListener
  (JNIEnv *, jobject, jstring, jint, jlong, jobject, jobject);

/*
 * Class:     com_twilio_ipmessaging_impl_ChannelsImpl
 * Method:    createChannelWithOptionsNative
 * Signature: (Ljava/lang/String;Ljava/lang/String;IJ)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_ChannelsImpl_createChannelWithOptionsNative
  (JNIEnv *, jobject, jstring, jstring, jstring, jint, jlong, jobject, jobject);;

/*
 * Class:     com_twilio_ipmessaging_impl_ChannelsImpl
 * Method:    createChannelNativeWithOptionsWithListener
 * Signature: (Ljava/lang/String;Ljava/lang/String;IJLcom/twilio/ipmessaging/Constants/CreateChannelListener;)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_ChannelsImpl_createChannelNativeWithOptionsWithListener
  (JNIEnv *, jobject, jstring, jstring, jstring, jint, jlong, jobject, jobject, jobject);

/*
 * Class:     com_twilio_ipmessaging_impl_ChannelsImpl
 * Method:    getChannelNative
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT jobject JNICALL Java_com_twilio_ipmessaging_impl_ChannelsImpl_getChannelNative
  (JNIEnv *, jobject, jstring, jlong);

/*
 * Class:     com_twilio_ipmessaging_impl_ChannelsImpl
 * Method:    getChannelsNative
 * Signature: (J)[Lcom/twilio/ipmessaging/Channel;
 */
JNIEXPORT jobjectArray JNICALL Java_com_twilio_ipmessaging_impl_ChannelsImpl_getChannelsNative
  (JNIEnv *, jobject, jlong, jobject);


/*
 * Class:     com_twilio_ipmessaging_impl_ChannelsImpl
 * Method:    getChannelSidArrayNative
 * Signature: (J)[Lcom/twilio/ipmessaging/Channel;
 */
JNIEXPORT jobjectArray JNICALL Java_com_twilio_ipmessaging_impl_ChannelsImpl_getChannelSidArrayNative
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_twilio_ipmessaging_impl_ChannelsImpl
 * Method:    getChannelNativeWithUniqueName
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT jobject JNICALL Java_com_twilio_ipmessaging_impl_ChannelsImpl_getChannelNativeWithUniqueName
  (JNIEnv *, jobject, jstring, jlong);

#ifdef __cplusplus
}
#endif
#endif
