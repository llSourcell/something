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
	} else {
		MessageContext *messageContextRecreate = reinterpret_cast<MessageContext *>(nativeMessageContext);
		LOGD(TAG, "nativeMessageContext context is recreated.");

		if(messageContextRecreate == nullptr) {
			LOGW(TAG,"Java_com_twilio_ipmessaging_impl_MessageImpl_getChannelSidNative : MessageContextRecreate is null.");
		}

		if(messageContextRecreate->message == nullptr) {
			LOGW(TAG,"MessageContextRecreate : ITMessagesPtr is null.");
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

	LOGD(TAG,"getMessagePtrFromNativeHandle : Checking nativeMessageContext.");
	if (nativeMessageContext == 0) {
		LOGW(TAG, "nativeMessageContext is null");
	} else {
		MessageContext *messageContextRecreate = reinterpret_cast<MessageContext *>(nativeMessageContext);
		LOGD(TAG, "nativeMessageContext context is recreated.");

		if(messageContextRecreate == nullptr) {
			LOGW(TAG,"Java_com_twilio_ipmessaging_impl_MessageImpl_getChannelSidNative : MessageContextRecreate is null.");
		}

		if(messageContextRecreate->message == nullptr) {
			LOGW(TAG,"MessageContextRecreate : ITMessagesPtr is null.");
		}

		messagePtr = messageContextRecreate->message;
		if(messagePtr != nullptr) {
			const char* msgTextStr = env->GetStringUTFChars(msgText, 0);

			if(listener != nullptr) {
				jobject j_statusListener_ = env->NewGlobalRef(listener);
				jmethodID j_onSuccess_ = tw_jni_get_method(env, j_statusListener_, "onSuccess", "()V");
				jmethodID j_onError_ = tw_jni_get_method(env, j_statusListener_, "onError", "()V");

				messagePtr->setBody(msgTextStr, [j_statusListener_,j_onSuccess_, j_onError_](TMResult result){
					JNIEnvAttacher jniAttacher;
					if (result == rtd::TMResult::kTMResultSuccess) {
						__android_log_print(ANDROID_LOG_INFO, TAG, "Successfully set body of the message. Calling java listener.");
						//Call Java
						jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onSuccess_);
						jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
					} else {
						__android_log_print(ANDROID_LOG_INFO, TAG, "Failed to set body of the message.");

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

