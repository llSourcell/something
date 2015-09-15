#include <twilio-jni/twilio-jni.h>
#include <android/log.h>


#define TAG  "RTD_TESTS"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__))

ITMessagesPtr getMessagesPtrFromNativeHandle(JNIEnv *env, jobject obj) {
	ITMessagesPtr messages = nullptr;

	jlong nativeMessagesContext = tw_jni_fetch_long(env, obj, "nativeMessagesHandler");

	LOGW( "getMessagesPtrFromNativeHandle : Checking nativeMessagesContext.");

	if (nativeMessagesContext == 0) {
			__android_log_print(ANDROID_LOG_ERROR, TAG, "nativeMessagesContext is null");
			return nullptr;
	} else {

		MessagesContext *messagesContextRecreate = reinterpret_cast<MessagesContext *>(nativeMessagesContext);

		__android_log_print(ANDROID_LOG_ERROR, TAG, "nativeMessagesContext context is recreated.");

		if(messagesContextRecreate == nullptr) {
			LOGW( "Java_com_twilio_ipmessaging_impl_MessagesImpl_getMessagesNative : MessagesContextRecreate is NULL.");
			return 0;
		}

		if(messagesContextRecreate->messages == nullptr) {
			LOGW( "Java_com_twilio_ipmessaging_impl_MessagesImpl_getMessagesNative : ITMessagesPtr is NULL.");
			return 0;
		}

		messages = messagesContextRecreate->messages;
	}

	return messages;
}


ITMessagePtr getMessagePtrFromNativeHandle(JNIEnv *env, jobject obj) {
	ITMessagePtr message = nullptr;

	jlong nativeMessageContext = tw_jni_fetch_long(env, obj, "nativeMessageHandle");

	LOGW( "getMessagePtrFromNativeHandle : Checking nativeMessageContext.");

	if (nativeMessageContext == 0) {
			__android_log_print(ANDROID_LOG_ERROR, TAG, "nativeMessageContext is null");
			return nullptr;
	} else {

		MessageContext *messageContextRecreate = reinterpret_cast<MessageContext *>(nativeMessageContext);

		__android_log_print(ANDROID_LOG_ERROR, TAG, "nativeMessageContext is recreated.");

		if(messageContextRecreate == nullptr) {
			LOGW( "Java_com_twilio_ipmessaging_impl_MessagesImpl_getMessagesNative : MessagesContextRecreate is NULL.");
			return 0;
		}

		if(messageContextRecreate->message == nullptr) {
			LOGW( "Java_com_twilio_ipmessaging_impl_MessagesImpl_getMessagesNative : ITMessagesPtr is NULL.");
			return 0;
		}

		message = messageContextRecreate->message;
	}

	return message;
}

/*
ClientContext* getClientContextPtrFromNativeHandle(JNIEnv *env, jobject obj) {
	ClientContext *clientParamsRecreate = nullptr;

	jlong nativeClientContext = tw_jni_fetch_long(env, obj, "nativeClientParam");

	LOGW( "getMessagingClientPtrFromNativeHandle : Checking nativeClientContext.");

	if (nativeClientContext == 0) {
		__android_log_print(ANDROID_LOG_ERROR, TAG, "nativeClientContext is null");
		return nullptr;
	} else {
		clientParamsRecreate = reinterpret_cast<ClientContext *>(nativeClientContext);
	}

	return clientParamsRecreate;
}

*/

