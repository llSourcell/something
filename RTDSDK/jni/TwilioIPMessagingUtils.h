#include <twilio-jni/twilio-jni.h>
#include <android/log.h>

#define TAG  "TwilioIMessagingUtils(native)"

ITMessagesPtr getMessagesPtrFromNativeHandle(JNIEnv *env, jobject obj) {
	ITMessagesPtr messages = nullptr;
	jlong nativeMessagesContext = tw_jni_fetch_long(env, obj, "nativeMessagesHandler");

	LOGD(TAG,"getMessagesPtrFromNativeHandle : Checking nativeMessagesContext.");

	if (nativeMessagesContext == 0) {
			LOG_W(TAG,"nativeMessagesContext is null");
			return nullptr;
	} else {

		MessagesContext *messagesContextRecreate = reinterpret_cast<MessagesContext *>(nativeMessagesContext);
		LOGD(TAG, "nativeMessagesContext context is recreated.");

		if(messagesContextRecreate == nullptr) {
			LOG_W( TAG, "MessagesContextRecreate is NULL.");
			return 0;
		}

		if(messagesContextRecreate->messages == nullptr) {
			LOGW( TAG, "ITMessagesPtr is NULL.");
			return 0;
		}

		messages = messagesContextRecreate->messages;
	}

	return messages;
}


ITMessagePtr getMessagePtrFromNativeHandle(JNIEnv *env, jobject obj) {
	ITMessagePtr message = nullptr;

	jlong nativeMessageContext = tw_jni_fetch_long(env, obj, "nativeMessageHandle");

	LOGD( TAG,"getMessagePtrFromNativeHandle : Checking nativeMessageContext.");

	if (nativeMessageContext == 0) {
			LOG_W(TAG, "nativeMessageContext is null");
			return nullptr;
	} else {

		MessageContext *messageContextRecreate = reinterpret_cast<MessageContext *>(nativeMessageContext);
		LOGD(TAG, "nativeMessageContext is recreated.");

		if(messageContextRecreate == nullptr) {
			LOG_W( TAG, "getMessagePtrFromNativeHandle : MessagesContextRecreate is NULL.");
			return 0;
		}

		if(messageContextRecreate->message == nullptr) {
			LOGW(TAG,"getMessagePtrFromNativeHandle : ITMessagesPtr is NULL.");
			return 0;
		}

		message = messageContextRecreate->message;
	}

	return message;
}

