//
//  TwilioIPMessagingClientListener.cpp
//  Twilio IP Messaging Client
//
//  Copyright © 2015 Twilio. All rights reserved.
//

#include <jni.h>
#include <twilio-jni/twilio-jni.h>
#include "TwilioIPMessagingClientListener.h"
#include "TwilioIPMessagingClientContextDefines.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__))


using namespace rtd;


void handleChannelInvite(ITMChannelPtr channelPtr) {

}



TwilioIPMessagingClientListener::TwilioIPMessagingClientListener(JNIEnv* env,jobject j_ipmessagingclient, jobject j_ipmessagingclientListenerInternal ) {
	j_ipmessagingclientListenerInternal_ = env->NewGlobalRef(j_ipmessagingclientListenerInternal);
	j_ipmessagingclient_ == env->NewGlobalRef(j_ipmessagingclient);
	j_onMessageAdd_ = tw_jni_get_method(env, j_ipmessagingclientListenerInternal_, "onMessageAdd", "(Lcom/twilio/ipmessaging/Message;)V");
	j_onChannelInvite_ = tw_jni_get_method(env, j_ipmessagingclientListenerInternal_, "onChannelInvite", "(Lcom/twilio/ipmessaging/Channel;)V");
	env_ = env;
}

TwilioIPMessagingClientListener::~TwilioIPMessagingClientListener()
{

	if (j_ipmessagingclientListenerInternal_ != NULL) {
		//TODO - we should probably notify jobject that native handle is being destroyed
		env_->DeleteGlobalRef(j_ipmessagingclientListenerInternal_);
		j_ipmessagingclientListenerInternal_ = NULL;
	}

	if (j_ipmessagingclient_ != NULL) {
		env_->DeleteGlobalRef(j_ipmessagingclient_);
		j_ipmessagingclient_ = NULL;
	}


}


void TwilioIPMessagingClientListener::onMessage(TMAction action, ITMessagePtr messagePtr)
{
	LOGW("TwilioIPMessagingClientListener::onMessage changed 1");
	JNIEnvAttacher jniAttacher;
	switch (action) {
		case rtd::kTMActionAdded:
		{
			LOGW("Adding Message Object.");
			MessageContext* messageContext_ = new MessageContext();
			messageContext_->message = messagePtr;
			jlong messageContextHandle = reinterpret_cast<jlong>(messageContext_);

			const char* author = messagePtr->getAuthor().c_str();
			const char* body = messagePtr->getBody().c_str();


			LOGW("author Name  : %s.", author );
			LOGW("Message body is %s", body);

			jstring authorString = jniAttacher.get()->NewStringUTF(author);
			jstring bodyString = jniAttacher.get()->NewStringUTF(body);

			LOGW("jstring done");

			jclass java_message_impl_cls = tw_jni_find_class(jniAttacher.get(), "com/twilio/ipmessaging/impl/MessageImpl");
			if(java_message_impl_cls != NULL) {
				LOGW("Found java_message_impl_cls class" );
			}

			jmethodID construct = tw_jni_get_method_by_class(jniAttacher.get(), java_message_impl_cls, "<init>", "(Ljava/lang/String;Ljava/lang/String;J)V");
			LOGW("Creating Messsage Object : construct");
			jobject message = tw_jni_new_object(jniAttacher.get(), java_message_impl_cls, construct, authorString, bodyString, messageContextHandle );
			LOGW("Created Message Object, calling java");

			jniAttacher.get()->CallVoidMethod(j_ipmessagingclientListenerInternal_, j_onMessageAdd_, message);

			LOGW("Calling java");
			break;
		}
		case rtd::kTMActionChanged:
		{
			break;
		}
		case rtd::kTMActionDeleted:
		{
			break;
		}
	}
}

void TwilioIPMessagingClientListener::onChannel(TMAction action, ITMChannelPtr channelPtr)
{
	LOGW("TwilioIPMessagingClientListener::onChannel");
	switch (action) {
		case rtd::kTMActionAdded:
			if(channelPtr != nullptr) {
				switch (channelPtr->getStatus()) {
					case TMChannelStatus::kTMChannelStatusInvited:
					{
						LOGW("TwilioIPMessagingClientListener::onChannel invited");
						jobject channel;
						JNIEnvAttacher jniAttacher;

						jclass java_channel_impl_cls = tw_jni_find_class(jniAttacher.get(), "com/twilio/ipmessaging/impl/ChannelImpl");
						if(java_channel_impl_cls != nullptr) {
							LOGW("Found java_channel_impl_cls class" );
						}

						jmethodID construct = tw_jni_get_method_by_class(jniAttacher.get(), java_channel_impl_cls, "<init>", "(Ljava/lang/String;Ljava/lang/String;J)V");
						const char* sid = channelPtr->getSid().c_str();
						const char* name = channelPtr->getName().c_str();

						LOGW("Channel Name  : %s.", name );
						LOGW("Channel Sid %s", sid);

						ChannelContext* channelContext_ = new ChannelContext();
						channelContext_->channel = channelPtr;
						jlong channelContextHandle = reinterpret_cast<jlong>(channelContext_);


						jstring nameString = jniAttacher.get()->NewStringUTF(name);
						jstring sidString = jniAttacher.get()->NewStringUTF(sid);
						channel = tw_jni_new_object(jniAttacher.get(), java_channel_impl_cls, construct, nameString, sidString, channelContextHandle);
						LOGW("Created Channel Object.");

						jniAttacher.get()->CallVoidMethod(j_ipmessagingclientListenerInternal_, j_onChannelInvite_, channel);

						LOGW("Calling java");
						break;
					}
					case TMChannelStatus::kTMChannelStatusJoined:
						LOGW("TwilioIPMessagingClientListener::onChannel joined");
						break;
					case TMChannelStatus::kTMChannelStatusNotParticipating:
						LOGW("TwilioIPMessagingClientListener::onChannel notparticipating");
						break;
				}
			break;
		}
	}
}

void TwilioIPMessagingClientListener::onMember(TMAction action, ITMMemberPtr member)
{
	LOGW("TwilioIPMessagingClientListener::onMember");
	JNIEnvAttacher jniAttacher;
	switch (action) {
		case rtd::kTMActionAdded:
			ITMChannelPtr channel = member->getChannel();
			if(channel != nullptr) {
				switch (channel->getStatus()) {
					case TMChannelStatus::kTMChannelStatusInvited:
						LOGW("TwilioIPMessagingClientListener::onMember invited");
					case TMChannelStatus::kTMChannelStatusJoined:
						LOGW("TwilioIPMessagingClientListener::onMember joined");
					case TMChannelStatus::kTMChannelStatusNotParticipating:
						LOGW("TwilioIPMessagingClientListener::onMember notparticipating");
				}
			break;
		}
	}
}

void TwilioIPMessagingClientListener::onError(const std::string& error)
{
	LOGW("TwilioIPMessagingClientListener::onError");
}

void TwilioIPMessagingClientListener::onToastNotification(const std::string& channelId, const std::string& messageId)
{
	LOGW("TwilioIPMessagingClientListener::onToastNotification");

}

void TwilioIPMessagingClientListener::onToastSubscribed()
{
	LOGW("TwilioIPMessagingClientListener::onToastSubscribed");
}

void TwilioIPMessagingClientListener::onToastFailed(TNError code)
{
	LOGW("TwilioIPMessagingClientListener::onToastFailed");
}



