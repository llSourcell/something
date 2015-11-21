#include "MessageImpl.h"
#include "TwilioIPMessagingClientContextDefines.h"
#include "TwilioIPMessagingLogger.h"

#include <twilio-jni/JNIEnvAttacher.h>
#include <twilio-jni/tw-jni.h>
#include <twilio-jni/tw-jni-compat.h>
#include <android/log.h>


#define TAG  "MessageImpl(native)"

using namespace rtd;

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

	LOG_DEBUG(TAG,"getMessagePtrFromNativeHandle : Checking nativeMessageContext.");
	if (nativeMessageContext == 0) {
		LOG_WARN(TAG, "nativeMessageContext is null");
	} else {
		MessageContext *messageContextRecreate = reinterpret_cast<MessageContext *>(nativeMessageContext);
		LOG_DEBUG(TAG, "nativeMessageContext context is recreated.");

		if(messageContextRecreate == nullptr) {
			LOG_WARN(TAG,"Java_com_twilio_ipmessaging_impl_MessageImpl_getChannelSidNative : MessageContextRecreate is null.");
		}

		if(messageContextRecreate->message == nullptr) {
			LOG_WARN(TAG,"MessageContextRecreate : ITMessagesPtr is null.");
		}

		messagePtr = messageContextRecreate->message;
		ITMChannelPtr channelPtr = messagePtr->getChannel();
		const char* sid = channelPtr->getSid().c_str();
		sidString = env->NewStringUTF(sid);

	}
	return sidString;
}


/*
 * Class:     com_twilio_ipmessaging_impl_MessageImpl
 * Method:    setBodyNative
 * Signature: (JLjava/lang/String;Lcom/twilio/ipmessaging/Constants/StatusListener;)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_MessageImpl_setBodyNative
(JNIEnv *env, jobject obj, jlong nativeMessageContext, jstring msgText	, jobject listener) {
	ITMessagePtr messagePtr = nullptr;
	jstring sidString;

	LOG_DEBUG(TAG,"getMessagePtrFromNativeHandle : Checking nativeMessageContext.");
	if (nativeMessageContext == 0) {
		LOG_WARN(TAG, "nativeMessageContext is null");
	} else {
		MessageContext *messageContextRecreate = reinterpret_cast<MessageContext *>(nativeMessageContext);
		LOG_DEBUG(TAG, "nativeMessageContext context is recreated.");

		if(messageContextRecreate == nullptr) {
			LOG_WARN(TAG,"Java_com_twilio_ipmessaging_impl_MessageImpl_getChannelSidNative : MessageContextRecreate is null.");
		}

		if(messageContextRecreate->message == nullptr) {
			LOG_WARN(TAG,"MessageContextRecreate : ITMessagesPtr is null.");
		}

		messagePtr = messageContextRecreate->message;
		if(messagePtr != nullptr) {
			const char* msgTextStr = env->GetStringUTFChars(msgText, 0);

			if(listener != nullptr) {
				jobject j_statusListener_ = env->NewGlobalRef(listener);
				jclass cls = (env)->GetObjectClass(j_statusListener_);
				jmethodID j_onSuccess_ = (env)->GetMethodID(cls, "onSuccess", "()V");
				jmethodID j_onError_ = (env)->GetMethodID(cls, "onError", "()V");

				messagePtr->setBody(msgTextStr, [j_statusListener_,j_onSuccess_, j_onError_](TMResult result) {
					JNIEnvAttacher jniAttacher;
					if (result == rtd::TMResult::kTMResultSuccess) {
						LOG_DEBUG(TAG, "Successfully set body of the message. Calling java listener.");
						//Call Java
						jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onSuccess_);
						jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
					} else {
						LOG_DEBUG(TAG, "Failed to set body of the message.");

						//Call Java
						jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onError_);
						jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
					}
				});
			} else {
				messagePtr->setBody(msgTextStr, nullptr);
			}
		}
	}

}

