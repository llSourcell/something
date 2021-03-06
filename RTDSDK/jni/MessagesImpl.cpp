#include "MessagesImpl.h"
#include "TwilioIPMessagingClientContextDefines.h"

#include <twilio-jni/JNIEnvAttacher.h>
#include <twilio-jni/tw-jni.h>
#include <twilio-jni/tw-jni-compat.h>
#include <android/log.h>
#include "TwilioIPMessagingLogger.h"
#include "talk/app/webrtc/java/jni/jni_helpers.h"

#define TAG  "MessagesImpl(native)"

using namespace webrtc_jni;

ITMessagesPtr getMessagesPtrFromNativeHandle(JNIEnv *env, jobject obj) {
	ITMessagesPtr messages = nullptr;

	jlong nativeMessagesContext = tw_jni_fetch_long(env, obj, "nativeMessagesContextHandler");

	LOG_DEBUG(TAG,"getMessagesPtrFromNativeHandle : Checking nativeMessagesContext.");

	if (nativeMessagesContext == 0) {
			LOG_WARN(TAG, "nativeMessagesContext is null");
			return nullptr;
	} else {

		MessagesContext *messagesContextRecreate = reinterpret_cast<MessagesContext *>(nativeMessagesContext);
		LOG_DEBUG(TAG, "nativeMessagesContext context is recreated.");
		if(messagesContextRecreate == nullptr) {
			LOG_WARN(TAG,"Java_com_twilio_ipmessaging_impl_MessagesImpl_getMessagesNative : MessagesContextRecreate is NULL.");
			return 0;
		}

		if(messagesContextRecreate->messages == nullptr) {
			LOG_WARN(TAG,"Java_com_twilio_ipmessaging_impl_MessagesImpl_getMessagesNative : ITMessagesPtr is NULL.");
			return 0;
		}

		messages = messagesContextRecreate->messages;
	}

	return messages;
}


ITMessagePtr getMessagePtrFromNativeHandle(JNIEnv *env, jobject obj) {
	ITMessagePtr message = nullptr;

	jlong nativeMessageContext = tw_jni_fetch_long(env, obj, "nativeMessageContextHandle");

	LOG_DEBUG(TAG,"getMessagePtrFromNativeHandle : Checking nativeMessageContext.");

	if (nativeMessageContext == 0) {
			LOG_WARN(TAG, "nativeMessageContext is null");
			return nullptr;
	} else {
		MessageContext *messageContextRecreate = reinterpret_cast<MessageContext *>(nativeMessageContext);
		LOG_DEBUG(TAG, "nativeMessageContext is recreated.");
		if(messageContextRecreate == nullptr) {
			LOG_WARN(TAG,"Java_com_twilio_ipmessaging_impl_MessagesImpl_getMessagesNative : MessagesContextRecreate is NULL.");
			return 0;
		}
		if(messageContextRecreate->message == nullptr) {
			LOG_WARN(TAG, "Java_com_twilio_ipmessaging_impl_MessagesImpl_getMessagesNative : ITMessagesPtr is NULL.");
			return 0;
		}
		message = messageContextRecreate->message;
	}

	return message;
}



JNIEXPORT jobject JNICALL Java_com_twilio_ipmessaging_impl_MessagesImpl_createMessageNative
  (JNIEnv *env, jobject obj, jstring msgText) {
	jobject message;
	//Message (Ljava/lang/String;J)V
	ITMessagesPtr messages = getMessagesPtrFromNativeHandle(env, obj);

	if(messages != nullptr) {
		ITMessagePtr messageLocal = messages->createMessage();
		std::string msgTextStr = JavaToStdString(env, msgText);
		messageLocal->setBody(msgTextStr, [](TMResult result) {LOG_DEBUG(TAG,"Message setBody command processed");});

		jclass java_message_impl_cls = tw_jni_find_class(env, "com/twilio/ipmessaging/impl/MessageImpl");
		if(java_message_impl_cls != NULL) {
			LOG_WARN(TAG,"Found java_message_impl_cls class" );
		}

		MessageContext* messageContext_ = new MessageContext();
		messageContext_->message = messageLocal;
		jlong messageContextHandle = jlongFromPointer(messageContext_);

		std::string author = messageContext_->message->getAuthor();
		std::string body = messageContext_->message->getBody();
		std::string timestamp = messageContext_->message->getTimestamp();

		LOG_WARN(TAG,"author Name  : %s.", author.c_str() );
		LOG_WARN(TAG,"body is %s", body.c_str());

		jstring authorString = JavaStringFromStdString(env, author);
		jstring bodyString = JavaStringFromStdString(env, body);
		jstring timestampString = JavaStringFromStdString(env, timestamp);

		jmethodID construct = tw_jni_get_method_by_class(env, java_message_impl_cls, "<init>", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;J)V");
		message = tw_jni_new_object(env, java_message_impl_cls, construct, authorString, bodyString, timestampString, messageContextHandle);
		LOG_WARN(TAG,"Created Message Object.");

		return message;
	}

}

/*
 * Class:     com_twilio_ipmessaging_impl_MessagesImpl
 * Method:    sendMessageNative
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_MessagesImpl_sendMessageNative
  (JNIEnv *env, jobject obj, jobject messageObj, jobject listener) {

	ITMessagesPtr messages = getMessagesPtrFromNativeHandle(env, obj);
	if(messages != nullptr) {
		if(listener != nullptr) {
			jobject j_statusListener_ = env->NewGlobalRef(listener);
			jclass cls = (env)->GetObjectClass(j_statusListener_);
			jmethodID j_onSuccess_ = (env)->GetMethodID(cls, "onSuccess", "()V");
			jmethodID j_onError_ = (env)->GetMethodID(cls, "onError", "()V");

			ITMessagePtr message = getMessagePtrFromNativeHandle(env, messageObj);
			if(message != nullptr) {
				messages->send(message, [j_statusListener_,j_onSuccess_, j_onError_](TMResult result){
					JNIEnvAttacher jniAttacher;
					if (result == rtd::TMResult::kTMResultSuccess) {
						LOG_DEBUG(TAG, "Sent message is successful. Calling java listener.");
						//Call Java
						jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onSuccess_);
						jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
					} else {
						LOG_DEBUG(TAG, "Sent message failed");

						//Call Java
						jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onError_);
						jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
					}
				});
			} else {
				LOG_DEBUG(TAG, "Sent message DeleteGlobalRef.");
				env->DeleteGlobalRef(j_statusListener_);
			}
		} else {
			LOG_DEBUG(TAG, "Sending message. StatusListener is set to null.");
			ITMessagePtr message = getMessagePtrFromNativeHandle(env, messageObj);
			if(message != nullptr) {
				messages->send(message, nullptr);
			}
		}
	}
}

/*
 * Class:     com_twilio_ipmessaging_impl_MessagesImpl
 * Method:    removeMessageNative
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_MessagesImpl_removeMessageNative
  (JNIEnv *env, jobject obj, jobject messageObj, jobject listener) {

	ITMessagesPtr messages = getMessagesPtrFromNativeHandle(env, obj);
	if(messages != nullptr) {
		if(listener != nullptr) {
			jobject j_statusListener_ = env->NewGlobalRef(listener);
			jclass cls = (env)->GetObjectClass(j_statusListener_);
			jmethodID j_onSuccess_ = (env)->GetMethodID(cls, "onSuccess", "()V");
			jmethodID j_onError_ = (env)->GetMethodID(cls, "onError", "()V");

			ITMessagePtr message = getMessagePtrFromNativeHandle(env, messageObj);
			if(message != nullptr) {
				 messages->remove(message, [j_statusListener_,j_onSuccess_, j_onError_](TMResult result){
					JNIEnvAttacher jniAttacher;
					if (result == rtd::TMResult::kTMResultSuccess) {
						LOG_DEBUG(TAG, "Remove message is successful. Calling java listener.");
						//Call Java
						jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onSuccess_);
						jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
					} else {
						LOG_DEBUG(TAG, "Remove message failed");

						//Call Java
						jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onError_);
						jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
					}
				});
			} else {
				env->DeleteGlobalRef(j_statusListener_);
			}
		} else {
			LOG_DEBUG(TAG, "Removing message. StatusListener is set to null.");
			ITMessagePtr message = getMessagePtrFromNativeHandle(env, messageObj);
			if(message != nullptr) {
				messages->remove(message, nullptr);
			}
		}
	}
}


/*
 * Class:     com_twilio_ipmessaging_impl_MessagesImpl
 * Method:    getMessagesNative
 * Signature: (J)[Lcom/twilio/ipmessaging/Message;
 */
JNIEXPORT jlongArray JNICALL Java_com_twilio_ipmessaging_impl_MessagesImpl_getMessageIndexArrayNative
  (JNIEnv *env, jobject obj , jlong nativeMessagesContext){
	jlongArray messageIndexArray;
	if (nativeMessagesContext == 0) {
			LOG_WARN(TAG, "client context is null");
			return nullptr;
	} else {
		MessagesContext *MessagesContextRecreate = reinterpret_cast<MessagesContext *>(nativeMessagesContext);
		if(MessagesContextRecreate == nullptr) {
			LOG_WARN(TAG,"Java_com_twilio_ipmessaging_impl_MessagesImpl_getMessagesNative : MessagesContextRecreate is NULL.");
			return 0;
		}
		if(MessagesContextRecreate->messages == nullptr) {
			LOG_WARN(TAG,"Java_com_twilio_ipmessaging_impl_MessagesImpl_getMessagesNative : ITMessagesPtr is NULL.");
			return 0;
		}

		ITMessagesPtr messages = MessagesContextRecreate->messages;
		std::vector<ITMessagePtr> messageList;
		messages->getMessagesList(messageList);
		LOG_DEBUG(TAG,"Message count : %d",messageList.size());

		jlong buffer[messageList.size()];
		messageIndexArray = env->NewLongArray(messageList.size());

		for (int i= 0; i< messageList.size() ; i++ ) {
			ITMessagePtr messagePtr = messageList[i];
			uint64_t index;
			messagePtr->getMessageIndex(index);
			buffer[i] = index;
		}

		env->SetLongArrayRegion(messageIndexArray, 0, messageList.size(), buffer);
		return messageIndexArray;
	}
	return nullptr;
}



/*
 * Class:     com_twilio_ipmessaging_impl_MessagesImpl
 * Method:    getMessage
 * Signature: (JJ)Lcom/twilio/ipmessaging/Message;
 */
JNIEXPORT jobject JNICALL Java_com_twilio_ipmessaging_impl_MessagesImpl_getMessageByIndex
 (JNIEnv *env, jobject obj, jlong messsageindex, jlong nativeMessagesContext) {
	jobject message;
	if (nativeMessagesContext == 0) {
			LOG_WARN(TAG, "client context is null");
			return nullptr;
	} else {
		MessagesContext *MessagesContextRecreate = reinterpret_cast<MessagesContext *>(nativeMessagesContext);
		if(MessagesContextRecreate == nullptr) {
			LOG_WARN(TAG,"Java_com_twilio_ipmessaging_impl_MessagesImpl_getMessagesNative : MessagesContextRecreate is NULL.");
			return 0;
		}
		if(MessagesContextRecreate->messages == nullptr) {
			LOG_WARN(TAG,"Java_com_twilio_ipmessaging_impl_MessagesImpl_getMessagesNative : ITMessagesPtr is NULL.");
			return 0;
		}

		jclass java_message_impl_cls = tw_jni_find_class(env, "com/twilio/ipmessaging/impl/MessageImpl");
		if(java_message_impl_cls != NULL) {
			LOG_WARN(TAG, "Found java_message_impl_cls class" );
		}
		jmethodID construct = tw_jni_get_method_by_class(env, java_message_impl_cls, "<init>", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;J)V");

		ITMessagesPtr messages = MessagesContextRecreate->messages;
		if(messages != nullptr){
			ITMessagePtr messagePtr = messages->getMessageByIndex(messsageindex);
			MessageContext* messageContext_ = new MessageContext();
			messageContext_->message = messagePtr;
			jlong messageContextHandle = reinterpret_cast<jlong>(messageContext_);

			std::string author = messagePtr->getAuthor();
			std::string body = messagePtr->getBody();
			std::string timestamp = messagePtr->getTimestamp();

			LOG_WARN(TAG, "author Name  : %s.", author.c_str());
			LOG_WARN(TAG, "body is %s", body.c_str());

			jstring authorString = JavaStringFromStdString(env, author);
			jstring bodyString = JavaStringFromStdString(env, body);
			jstring timeStampString  = JavaStringFromStdString(env, timestamp);

			message = tw_jni_new_object(env, java_message_impl_cls, construct, authorString, bodyString, timeStampString, messageContextHandle);
			LOG_DEBUG(TAG,"Created Message Object.");
		}
	}
	return message;
}

