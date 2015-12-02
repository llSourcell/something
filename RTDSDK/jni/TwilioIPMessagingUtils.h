#include <twilio-jni/JNIEnvAttacher.h>
#include <twilio-jni/tw-jni.h>
#include <twilio-jni/tw-jni-compat.h>
#include <android/log.h>

#define TAG  "TwilioIMessagingUtils(native)"

ITMessagesPtr getMessagesPtrFromNativeHandle(JNIEnv *env, jobject obj) {
	ITMessagesPtr messages = nullptr;
	jlong nativeMessagesContext = tw_jni_fetch_long(env, obj, "nativeMessagesHandler");

	LOG_DEBUG(TAG,"getMessagesPtrFromNativeHandle : Checking nativeMessagesContext.");

	if (nativeMessagesContext == 0) {
			LOG_WARN(TAG,"nativeMessagesContext is null");
			return nullptr;
	} else {

		MessagesContext *messagesContextRecreate = reinterpret_cast<MessagesContext *>(nativeMessagesContext);
		LOG_DEBUG(TAG, "nativeMessagesContext context is recreated.");

		if(messagesContextRecreate == nullptr) {
			LOG_WARN( TAG, "MessagesContextRecreate is NULL.");
			return 0;
		}

		if(messagesContextRecreate->messages == nullptr) {
			LOG_WARN( TAG, "ITMessagesPtr is NULL.");
			return 0;
		}

		messages = messagesContextRecreate->messages;
	}

	return messages;
}


ITMessagePtr getMessagePtrFromNativeHandle(JNIEnv *env, jobject obj) {
	ITMessagePtr message = nullptr;

	jlong nativeMessageContext = tw_jni_fetch_long(env, obj, "nativeMessageHandle");

	LOG_DEBUG( TAG,"getMessagePtrFromNativeHandle : Checking nativeMessageContext.");

	if (nativeMessageContext == 0) {
			LOG_WARN(TAG, "nativeMessageContext is null");
			return nullptr;
	} else {

		MessageContext *messageContextRecreate = reinterpret_cast<MessageContext *>(nativeMessageContext);
		LOG_DEBUG(TAG, "nativeMessageContext is recreated.");

		if(messageContextRecreate == nullptr) {
			LOG_WARN( TAG, "getMessagePtrFromNativeHandle : MessagesContextRecreate is NULL.");
			return 0;
		}

		if(messageContextRecreate->message == nullptr) {
			LOG_WARN(TAG,"getMessagePtrFromNativeHandle : ITMessagesPtr is NULL.");
			return 0;
		}

		message = messageContextRecreate->message;
	}

	return message;
}


