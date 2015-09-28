#include "MessageImpl.h"
#include "TwilioIPMessagingClientContextDefines.h"

#include <twilio-jni/twilio-jni.h>
#include <android/log.h>


/*
 * Class:     com_twilio_ipmessaging_impl_MessageImpl
 * Method:    getChannelSidNative
 * Signature: ()J
 */
JNIEXPORT jstring JNICALL Java_com_twilio_ipmessaging_impl_MessageImpl_getChannelSidNative
  (JNIEnv *env, jobject obj)
{
	jlong nativeMessageContext = tw_jni_fetch_long(env, obj, "nativeMessageHandle");
	ITMessagePtr messagePtr = nullptr;
	jstring sidString;


	LOGW( "getMessagePtrFromNativeHandle : Checking nativeMessageContext.");

	if (nativeMessageContext == 0) {
			__android_log_print(ANDROID_LOG_ERROR, TAG, "nativeMessageContext is null");
			return nullptr;
	} else {

		MessageContext *messageContextRecreate = reinterpret_cast<MessageContext *>(nativeMessageContext);

		__android_log_print(ANDROID_LOG_ERROR, TAG, "nativeMessageContext context is recreated.");

		if(messageContextRecreate == nullptr) {
			LOGW( "Java_com_twilio_ipmessaging_impl_MessageImpl_getChannelSidNative : MessageContextRecreate is NULL.");
			return 0;
		}

		if(messageContextRecreate->message == nullptr) {
			LOGW( "MessageContextRecreate : ITMessagesPtr is NULL.");
			return 0;
		}

		messagePtr = messageContextRecreate->message;
		ITMChannelPtr channelPtr = messagePtr->getChannel();
		const char* sid = channelPtr->getSid().c_str();
		sidString = env->NewStringUTF(sid);

	}
	return sidString;
}

