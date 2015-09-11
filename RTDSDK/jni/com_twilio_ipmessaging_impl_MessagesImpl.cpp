#include "com_twilio_ipmessaging_impl_MessagesImpl.h"
#include "TwilioIPMessagingClientContextDefines.h"

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
 * Class:     com_twilio_ipmessaging_impl_MessagesImpl
 * Method:    createMessageNative
 * Signature: (Ljava/lang/String;)Lcom/twilio/ipmessaging/Message;
 */
JNIEXPORT jobject JNICALL Java_com_twilio_ipmessaging_impl_MessagesImpl_createMessageNative
  (JNIEnv *env, jobject obj, jstring msgText) {
	jobject message;
	//Message (Ljava/lang/String;J)V
	ITMessagesPtr messages = getMessagesPtrFromNativeHandle(env, obj);

	if(messages != nullptr) {
		ITMessagePtr messageLocal = messages->createMessage();
		const char *msgTextStr = env->GetStringUTFChars(msgText, 0);

		messageLocal->setBody(msgTextStr, [](TMResult result) {});

		jclass java_message_impl_cls = tw_jni_find_class(env, "com/twilio/ipmessaging/impl/MessageImpl");
		if(java_message_impl_cls != NULL) {
			LOGW("Found java_message_impl_cls class" );
		}

		MessageContext* messageContext_ = new MessageContext();
		messageContext_->message = messageLocal;

		jlong messageContextHandle = reinterpret_cast<jlong>(messageContext_);
		const char* author = messageContext_->message->getAuthor().c_str();
		const char* body = messageContext_->message->getBody().c_str();

		LOGW("author Name  : %s.", author );
		LOGW("body is %s", body);

		jstring authorString = env->NewStringUTF(author);
		jstring bodyString = env->NewStringUTF(body);

		jmethodID construct = tw_jni_get_method_by_class(env, java_message_impl_cls, "<init>", "(Ljava/lang/String;Ljava/lang/String;J)V");
		message = tw_jni_new_object(env, java_message_impl_cls, construct, authorString, bodyString, messageContextHandle);
		LOGW("Created Message Object.");

		return message;
	}

}

/*
 * Class:     com_twilio_ipmessaging_impl_MessagesImpl
 * Method:    sendMessageNative
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_MessagesImpl_sendMessageNative
  (JNIEnv *env, jobject obj, jobject messageObj) {

	ITMessagesPtr messages = getMessagesPtrFromNativeHandle(env, obj);
	if(messages != nullptr) {
		ITMessagePtr message = getMessagePtrFromNativeHandle(env, messageObj);
		if(message != nullptr) {
			 messages->send(message, nullptr);
		}
	}
}

/*
 * Class:     com_twilio_ipmessaging_impl_MessagesImpl
 * Method:    getMessagesNative
 * Signature: (J)[Lcom/twilio/ipmessaging/Message;
 */
JNIEXPORT jobjectArray JNICALL Java_com_twilio_ipmessaging_impl_MessagesImpl_getMessagesNative
  (JNIEnv *env, jobject obj){

	jobject message;

	jlong nativeMessagesContext = tw_jni_fetch_long(env, obj, "nativeMessagesHandler");

	LOGW( "Java_com_twilio_ipmessaging_impl_MessagesImpl_getMessagesNative : Checking nativeMessagesContext.");

	if (nativeMessagesContext == 0) {
			__android_log_print(ANDROID_LOG_ERROR, TAG, "client context is null");
			return nullptr;
	} else {

		MessagesContext *MessagesContextRecreate = reinterpret_cast<MessagesContext *>(nativeMessagesContext);

		__android_log_print(ANDROID_LOG_ERROR, TAG, "client context is recreated.");

		if(MessagesContextRecreate == nullptr) {
			LOGW( "Java_com_twilio_ipmessaging_impl_MessagesImpl_getMessagesNative : MessagesContextRecreate is NULL.");
			return 0;
		}

		if(MessagesContextRecreate->messages == nullptr) {
			LOGW( "Java_com_twilio_ipmessaging_impl_MessagesImpl_getMessagesNative : ITMessagesPtr is NULL.");
			return 0;
		}

		ITMessagesPtr messages = MessagesContextRecreate->messages;

		std::vector<ITMessagePtr> messageList;
		messages->getMessagesList(messageList);

		jclass java_message_impl_cls = tw_jni_find_class(env, "com/twilio/ipmessaging/impl/MessageImpl");
		if(java_message_impl_cls != NULL) {
			LOGW("Found java_message_impl_cls class" );
		}

		//jmethodID construct = tw_jni_get_method_by_class(env, java_message_impl_cls, "<init>", "(Ljava/lang/String;Ljava/lang/String;)V");
		jmethodID construct = tw_jni_get_method_by_class(env, java_message_impl_cls, "<init>", "(Ljava/lang/String;Ljava/lang/String;J)V");
		jobjectArray messagesArray = (jobjectArray) env->NewObjectArray(messageList.size(),java_message_impl_cls, 0);

		for (int i= 0; i< messageList.size() ; i++ ) {
			ITMessagePtr messagePtr = messageList[i];

			MessageContext* messageContext_ = new MessageContext();
			messageContext_->message = messagePtr;
			jlong messageContextHandle = reinterpret_cast<jlong>(messageContext_);

			const char* author = messagePtr->getAuthor().c_str();
			const char* body = messagePtr->getBody().c_str();

			LOGW("author Name  : %s.", author );
			LOGW("body is %s", body);

			jstring authorString = env->NewStringUTF(author);
			jstring bodyString = env->NewStringUTF(body);

			message = tw_jni_new_object(env, java_message_impl_cls, construct, authorString, bodyString, messageContextHandle );
			LOGW("Created Message Object.");
			env->SetObjectArrayElement(messagesArray, i, message);
			LOGW("Added object to array");
		}

		if(messagesArray != NULL) {
			LOGW("messagesArray is NOT NULL ");
		} else {
			LOGW("messagesArray is  NULL *********");
		}

		return messagesArray;
	}

	return nullptr;
}


