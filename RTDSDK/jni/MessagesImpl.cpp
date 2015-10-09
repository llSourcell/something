#include "MessagesImpl.h"
#include "TwilioIPMessagingClientContextDefines.h"

#include <twilio-jni/twilio-jni.h>
#include <android/log.h>

#define TAG  "MessagesImpl(native)"

jstring str2jstring(JNIEnv* env, const char* pat) {
	jclass strClass = (env)->FindClass("java/lang/String");
	jmethodID ctorID = (env)->GetMethodID(strClass, "<init>",
			"([BLjava/lang/String;)V");
	jbyteArray bytes = (env)->NewByteArray(strlen(pat));
	(env)->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*) pat);
	jstring encoding = (env)->NewStringUTF("UTF8");
	return (jstring) (env)->NewObject(strClass, ctorID, bytes, encoding);
}

jbyteArray convertStringToByteArray(JNIEnv* env, jclass clazz, jstring str)
{
	const char* nativeString = env->GetStringUTFChars(str, 0);
	size_t length = strlen(nativeString);

	jbyteArray array = env->NewByteArray(length);
	env->SetByteArrayRegion(array, 0, length, (const jbyte*)nativeString);

	env->ReleaseStringUTFChars(str, nativeString);

	return array;
}


std::string jstring2str(JNIEnv* env, jstring jstr) {
	char* rtn = NULL;
	jclass clsstring = env->FindClass("java/lang/String");
	jstring strencode = env->NewStringUTF("UTF8");
	jmethodID mid = env->GetMethodID(clsstring, "getBytes",
			"(Ljava/lang/String;)[B");
	jbyteArray barr = (jbyteArray) env->CallObjectMethod(jstr, mid, strencode);
	jsize alen = env->GetArrayLength(barr);
	jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
	if (alen > 0) {
		rtn = (char*) malloc(alen + 1);
		memcpy(rtn, ba, alen);
		rtn[alen] = 0;
	}
	env->ReleaseByteArrayElements(barr, ba, 0);
	std::string stemp(rtn);
	free(rtn);
	return stemp;
}

jstring JNICALL convertByteArrayToString(JNIEnv* env, jclass clazz, jbyteArray array)
{
	jsize length = env->GetArrayLength(array);
	jbyte* buffer = new jbyte[length+1];

	env->GetByteArrayRegion(array, 0, length, buffer);
	buffer[length] = '\0';

	jstring result = env->NewStringUTF((const char*)buffer);

	delete[] buffer;

	return result;
}


ITMessagesPtr getMessagesPtrFromNativeHandle(JNIEnv *env, jobject obj) {
	ITMessagesPtr messages = nullptr;
	LOGD(TAG,"getMessagesPtrFromNativeHandle : Checking nativeMessagesContext.");

	if (nativeMessagesContext == 0) {
			LOGW(TAG, "nativeMessagesContext is null");
			return nullptr;
	} else {

		MessagesContext *messagesContextRecreate = reinterpret_cast<MessagesContext *>(nativeMessagesContext);
		LOGD(TAG, "nativeMessagesContext context is recreated.");
		if(messagesContextRecreate == nullptr) {
			LOGW(TAG,"Java_com_twilio_ipmessaging_impl_MessagesImpl_getMessagesNative : MessagesContextRecreate is NULL.");
			return 0;
		}

		if(messagesContextRecreate->messages == nullptr) {
			LOGW(TAG,"Java_com_twilio_ipmessaging_impl_MessagesImpl_getMessagesNative : ITMessagesPtr is NULL.");
			return 0;
		}

		messages = messagesContextRecreate->messages;
	}

	return messages;
}


ITMessagePtr getMessagePtrFromNativeHandle(JNIEnv *env, jobject obj) {
	ITMessagePtr message = nullptr;

	jlong nativeMessageContext = tw_jni_fetch_long(env, obj, "nativeMessageContextHandle");

	LOGD(TAG,"getMessagePtrFromNativeHandle : Checking nativeMessageContext.");

	if (nativeMessageContext == 0) {
			LOGW(TAG, "nativeMessageContext is null");
			return nullptr;
	} else {
		MessageContext *messageContextRecreate = reinterpret_cast<MessageContext *>(nativeMessageContext);
		LOGD(TAG, "nativeMessageContext is recreated.");
		if(messageContextRecreate == nullptr) {
			LOGW(TAG,"Java_com_twilio_ipmessaging_impl_MessagesImpl_getMessagesNative : MessagesContextRecreate is NULL.");
			return 0;
		}
		if(messageContextRecreate->message == nullptr) {
			LOGW(TAG, "Java_com_twilio_ipmessaging_impl_MessagesImpl_getMessagesNative : ITMessagesPtr is NULL.");
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

		 const char *msgTextStr = jstring2str(env, msgText).c_str();
		//const char *msgTextStr = env->GetStringUTFChars(msgText, 0);

		messageLocal->setBody(msgTextStr, [](TMResult result) {LOGD(TAG,"Message setBody command processed");});

		jclass java_message_impl_cls = tw_jni_find_class(env, "com/twilio/ipmessaging/impl/MessageImpl");
		if(java_message_impl_cls != NULL) {
			LOGW(TAG,"Found java_message_impl_cls class" );
		}

		MessageContext* messageContext_ = new MessageContext();
		messageContext_->message = messageLocal;
		jlong messageContextHandle = reinterpret_cast<jlong>(messageContext_);

		const char* author = messageContext_->message->getAuthor().c_str();
		const char* body = messageContext_->message->getBody().c_str();
		const char* timestamp = messageContext_->message->getTimestamp().c_str();

		LOGW(TAG,"author Name  : %s.", author );
		LOGW(TAG,"body is %s", body);

		jstring authorString = env->NewStringUTF(author);
		jstring bodyString = env->NewStringUTF(body);
		jstring timestampString = env->NewStringUTF(timestamp);

		jmethodID construct = tw_jni_get_method_by_class(env, java_message_impl_cls, "<init>", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;J)V");
		message = tw_jni_new_object(env, java_message_impl_cls, construct, authorString, bodyString, timestampString, messageContextHandle);
		LOGW(TAG,"Created Message Object.");

		return message;
	}

}


/*
 * Class:     com_twilio_ipmessaging_impl_MessagesImpl
 * Method:    createMessageNative
 * Signature: (Ljava/lang/String;)Lcom/twilio/ipmessaging/Message;
 */
JNIEXPORT jobject JNICALL Java_com_twilio_ipmessaging_impl_MessagesImpl_createMessageNativeBuffer
  (JNIEnv *env, jobject obj, jbyteArray array) {
	jobject message;

	ITMessagesPtr messages = getMessagesPtrFromNativeHandle(env, obj);

	if(messages != nullptr) {

		jsize length = env->GetArrayLength(array);
		jbyte* buffer = new jbyte[length+1];
		env->GetByteArrayRegion(array, 0, length, buffer);
		buffer[length] = '\0';
		jstring msgText = env->NewStringUTF((const char*)buffer);
		delete[] buffer;

		ITMessagePtr messageLocal = messages->createMessage();
		//const char *msgTextStr = jstring2str(env, msgText).c_str();
		const char *msgTextStr = env->GetStringUTFChars(msgText, 0);

		messageLocal->setBody(msgTextStr, [](TMResult result) {});

		jclass java_message_impl_cls = tw_jni_find_class(env, "com/twilio/ipmessaging/impl/MessageImpl");
		if(java_message_impl_cls != NULL) {
			LOGW(TAG,"Found java_message_impl_cls class" );
		}

		MessageContext* messageContext_ = new MessageContext();
		messageContext_->message = messageLocal;
		jlong messageContextHandle = reinterpret_cast<jlong>(messageContext_);

		const char* author = messageContext_->message->getAuthor().c_str();
		const char* body = messageContext_->message->getBody().c_str();
		const char* timestamp = messageContext_->message->getTimestamp().c_str();

		LOGD(TAG,"author Name  : %s.", author );
		LOGD(TAG,"body is %s", body);

		jstring authorString = env->NewStringUTF(author);
		jstring bodyString = env->NewStringUTF(body);
		jstring timestampString = env->NewStringUTF(timestamp);

		jmethodID construct = tw_jni_get_method_by_class(env, java_message_impl_cls, "<init>", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;J)V");
		message = tw_jni_new_object(env, java_message_impl_cls, construct, authorString, bodyString, timestampString, messageContextHandle);
		LOGD(TAG,"Created Message Object.");

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
			 messages->send(message, [](TMResult result) {LOGD(TAG,"Messages send command processed");});
		}
	}
}

/*
 * Class:     com_twilio_ipmessaging_impl_MessagesImpl
 * Method:    removeMessageNative
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_MessagesImpl_removeMessageNative
  (JNIEnv *env, jobject obj, jobject messageObj) {

	ITMessagesPtr messages = getMessagesPtrFromNativeHandle(env, obj);
	if(messages != nullptr) {
		ITMessagePtr message = getMessagePtrFromNativeHandle(env, messageObj);
		if(message != nullptr) {
			 messages->remove(message, [](TMResult result) {LOGD(TAG,"Messages remove command processed");});
		}
	}
}

/*
 * Class:     com_twilio_ipmessaging_impl_MessagesImpl
 * Method:    getMessagesNative
 * Signature: (J)[Lcom/twilio/ipmessaging/Message;
 */
JNIEXPORT jobjectArray JNICALL Java_com_twilio_ipmessaging_impl_MessagesImpl_getMessagesNative
  (JNIEnv *env, jobject obj , jlong nativeMessagesContext1){
	jobject message;

	jlong nativeMessagesContext = tw_jni_fetch_long(env, obj, "nativeMessagesContextHandler");

	LOGD(TAG,"Java_com_twilio_ipmessaging_impl_MessagesImpl_getMessagesNative : Checking nativeMessagesContext.");

	if (nativeMessagesContext == 0) {
			LOGW(TAG, "client context is null");
			return nullptr;
	} else {

		MessagesContext *MessagesContextRecreate = reinterpret_cast<MessagesContext *>(nativeMessagesContext);

		LOGD(TAG, "client context is recreated.");

		if(MessagesContextRecreate == nullptr) {
			LOGW(TAG,"Java_com_twilio_ipmessaging_impl_MessagesImpl_getMessagesNative : MessagesContextRecreate is NULL.");
			return 0;
		}

		if(MessagesContextRecreate->messages == nullptr) {
			LOGW(TAG,"Java_com_twilio_ipmessaging_impl_MessagesImpl_getMessagesNative : ITMessagesPtr is NULL.");
			return 0;
		}

		ITMessagesPtr messages = MessagesContextRecreate->messages;

		std::vector<ITMessagePtr> messageList;
		messages->getMessagesList(messageList);

		jclass java_message_impl_cls = tw_jni_find_class(env, "com/twilio/ipmessaging/impl/MessageImpl");
		if(java_message_impl_cls != NULL) {
			LOGW(TAG, "Found java_message_impl_cls class" );
		}

		jmethodID construct = tw_jni_get_method_by_class(env, java_message_impl_cls, "<init>", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;J)V");
		jobjectArray messagesArray = (jobjectArray) env->NewObjectArray(messageList.size(),java_message_impl_cls, 0);

		for (int i= 0; i< messageList.size() ; i++ ) {
			ITMessagePtr messagePtr = messageList[i];

			MessageContext* messageContext_ = new MessageContext();
			messageContext_->message = messagePtr;
			jlong messageContextHandle = reinterpret_cast<jlong>(messageContext_);

			const char* author = messagePtr->getAuthor().c_str();
			const char* body = messagePtr->getBody().c_str();
			const char* timestamp = messagePtr->getTimestamp().c_str();

			LOGW(TAG, "author Name  : %s.", author );
			LOGW(TAG, "body is %s", body);

			jstring authorString = env->NewStringUTF(author);
			//jstring bodyString = env->NewStringUTF(body);
			jstring bodyString = str2jstring(env, body);
			jstring timeStampString  = env->NewStringUTF(timestamp);

			message = tw_jni_new_object(env, java_message_impl_cls, construct, authorString, bodyString, timeStampString, messageContextHandle );
			LOGD(TAG,"Created Message Object.");
			env->SetObjectArrayElement(messagesArray, i, message);
			LOGD(TAG, "Added object to array");
		}

		return messagesArray;
	}

	return nullptr;
}


