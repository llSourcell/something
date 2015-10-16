#include "MessageImpl.h"
#include "TwilioIPMessagingClientContextDefines.h"

#include <twilio-jni/twilio-jni.h>
#include <android/log.h>

#define TAG  "MessageImpl(native)"

/*
 * Class:     com_twilio_ipmessaging_impl_MessageImpl
 * Method:    getChannelSidNative
 * Signature: ()J
 */
JNIEXPORT jstring JNICALL Java_com_twilio_ipmessaging_impl_MessageImpl_getChannelSidNative
  (JNIEnv *env, jobject obj, jlong nativeMessageContext)
{
	ITMessagePtr messagePtr = nullptr;
	jstring sidString;

	LOGD(TAG,"getMessagePtrFromNativeHandle : Checking nativeMessageContext.");
	if (nativeMessageContext == 0) {
		LOGW(TAG, "nativeMessageContext is null");
		return nullptr;
	} else {
		MessageContext *messageContextRecreate = reinterpret_cast<MessageContext *>(nativeMessageContext);
		LOGD(TAG, "nativeMessageContext context is recreated.");

		if(messageContextRecreate == nullptr) {
			LOGW(TAG,"Java_com_twilio_ipmessaging_impl_MessageImpl_getChannelSidNative : MessageContextRecreate is null.");
			return 0;
		}

		if(messageContextRecreate->message == nullptr) {
			LOGW(TAG,"MessageContextRecreate : ITMessagesPtr is null.");
			return 0;
		}

		messagePtr = messageContextRecreate->message;
		ITMChannelPtr channelPtr = messagePtr->getChannel();
		const char* sid = channelPtr->getSid().c_str();
		sidString = env->NewStringUTF(sid);

	}
	return sidString;
}

