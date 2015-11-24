//
//  TwilioIPMessagingClientListener.cpp
//  Twilio IP Messaging Client
//
//  Copyright © 2015 Twilio. All rights reserved.
//

#include <jni.h>
#include <twilio-jni/JNIEnvAttacher.h>
#include <twilio-jni/tw-jni.h>
#include <twilio-jni/tw-jni-compat.h>
#include "TwilioIPMessagingClientListener.h"
#include "TwilioIPMessagingClientContextDefines.h"
#include "TwilioIPMessagingLogger.h"
#define TAG  "TwilioIPMessagingClientListener(native)"

using namespace rtd;

jobject createChannelObject(ITMChannelPtr channelPtr) {
	jobject channel;
	JNIEnvAttacher jniAttacher;
	int status = 0;
	int type = 0;

	jclass java_channel_impl_cls = tw_jni_find_class(jniAttacher.get(), "com/twilio/ipmessaging/impl/ChannelImpl");
	if(java_channel_impl_cls != nullptr) {
		LOG_WARN(TAG,"Found java_channel_impl_cls class" );
	}

	const char* sid = channelPtr->getSid().c_str();
	const char* name = channelPtr->getFriendlyName().c_str();
	switch (channelPtr->getStatus()) {
		case TMChannelStatus::kTMChannelStatusInvited:
			status = 0;
			break;
		case TMChannelStatus::kTMChannelStatusJoined:
			status = 1;
			break;
		case TMChannelStatus::kTMChannelStatusNotParticipating:
			status = 2;
			break;
		default:
			break;
	}
	switch (channelPtr->getType()) {
		case TMChannelType::kTMChannelTypePublic:
			type = 0;
			break;
		case TMChannelType::kTMChannelTypePrivate:
			type = 1;
			break;
		default:
			break;
	}

	ChannelContext* channelContext_ = new ChannelContext();
	channelContext_->channel = channelPtr;
	jlong channelContextHandle = reinterpret_cast<jlong>(channelContext_);

	LOG_WARN(TAG, "channelContextHandle is %d ", channelContextHandle);

	jstring nameString = jniAttacher.get()->NewStringUTF(name);
	jstring sidString = jniAttacher.get()->NewStringUTF(sid);
	jmethodID construct = tw_jni_get_method_by_class(jniAttacher.get(), java_channel_impl_cls, "<init>", "(Ljava/lang/String;Ljava/lang/String;JII)V");
	channel = tw_jni_new_object(jniAttacher.get(), java_channel_impl_cls, construct, nameString, sidString, channelContextHandle, status, type);
	LOG_DEBUG(TAG, "Created Channel Object.");
	//jniAttacher.get()->NewGlobalRef(channel);

	return channel;
}


TwilioIPMessagingClientListener::TwilioIPMessagingClientListener(JNIEnv* env,jobject j_ipmessagingclient, jobject j_ipmessagingclientListenerInternal ) {
	j_ipmessagingclientListenerInternal_ = env->NewGlobalRef(j_ipmessagingclientListenerInternal);
	j_ipmessagingclient_ == env->NewGlobalRef(j_ipmessagingclient);
	j_onMessageAdd_ = tw_jni_get_method(env, j_ipmessagingclientListenerInternal_, "onMessageAdd", "(Lcom/twilio/ipmessaging/Message;)V");
	j_onMessageChange_ = tw_jni_get_method(env, j_ipmessagingclientListenerInternal_, "onMessageChange", "(Lcom/twilio/ipmessaging/Message;)V");
	j_onMessageDelete_ = tw_jni_get_method(env, j_ipmessagingclientListenerInternal_, "onMessageDelete", "(Lcom/twilio/ipmessaging/Message;)V");
	j_onChannelInvite_ = tw_jni_get_method(env, j_ipmessagingclientListenerInternal_, "onChannelInvite", "(Lcom/twilio/ipmessaging/Channel;)V");
	j_onChannelAdd_ = tw_jni_get_method(env, j_ipmessagingclientListenerInternal_, "onChannelAdd", "(Lcom/twilio/ipmessaging/impl/ChannelImpl;)V");
	j_onChannelChanged_=tw_jni_get_method(env, j_ipmessagingclientListenerInternal_, "onChannelChange", "(Lcom/twilio/ipmessaging/impl/ChannelImpl;)V");
	j_onChannelDeleted_= tw_jni_get_method(env, j_ipmessagingclientListenerInternal_, "onChannelDelete", "(Lcom/twilio/ipmessaging/impl/ChannelImpl;)V");
	j_onAttributesChange_ = tw_jni_get_method(env, j_ipmessagingclientListenerInternal_, "onAttributesChange", "(Ljava/lang/String;)V");
    j_onMemberJoin_ = tw_jni_get_method(env, j_ipmessagingclientListenerInternal_, "onMemberJoin", "(Lcom/twilio/ipmessaging/Member;Lcom/twilio/ipmessaging/Channel;)V");;
    j_onMemberChange_ = tw_jni_get_method(env, j_ipmessagingclientListenerInternal_, "onMemberChange", "(Lcom/twilio/ipmessaging/Member;Lcom/twilio/ipmessaging/Channel;)V");
    j_onMemberDelete_ = tw_jni_get_method(env, j_ipmessagingclientListenerInternal_, "onMemberDelete", "(Lcom/twilio/ipmessaging/Member;Lcom/twilio/ipmessaging/Channel;)V");
    j_onTypingStarted_ = tw_jni_get_method(env, j_ipmessagingclientListenerInternal_, "onTypingStarted", "(Lcom/twilio/ipmessaging/Channel;Lcom/twilio/ipmessaging/Member;)V");
    j_onTypingEnded_ = tw_jni_get_method(env, j_ipmessagingclientListenerInternal_, "onTypingEnded", "(Lcom/twilio/ipmessaging/Channel;Lcom/twilio/ipmessaging/Member;)V");
    j_onError_ = tw_jni_get_method(env, j_ipmessagingclientListenerInternal_, "onError", "(ILjava/lang/String;)V");
    j_onChannelSync_ = tw_jni_get_method(env, j_ipmessagingclientListenerInternal_, "onChannelSync", "(Lcom/twilio/ipmessaging/impl/ChannelImpl;)V");
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
	LOG_DEBUG(TAG,"TwilioIPMessagingClientListener::onMessage");

	JNIEnvAttacher jniAttacher;
	LOG_DEBUG(TAG, "Adding Message Object.");
	MessageContext* messageContext_ = new MessageContext();
	messageContext_->message = messagePtr;
	jlong messageContextHandle = reinterpret_cast<jlong>(messageContext_);

	const char* author = messagePtr->getAuthor().c_str();
	const char* body = messagePtr->getBody().c_str();
	const char* timestamp = messagePtr->getTimestamp().c_str();
	const char* sid = messagePtr->getSid().c_str();

	LOG_DEBUG(TAG, "author Name  : %s.", author );
	LOG_DEBUG(TAG, "Message body is %s", body);
	LOG_DEBUG(TAG, "Message sid is %s", sid);
	LOG_DEBUG(TAG, "MessageSid is: %s", sid);

	jstring authorString = jniAttacher.get()->NewStringUTF(author);
	jstring bodyString = jniAttacher.get()->NewStringUTF(body);
	jstring timeStampString  = jniAttacher.get()->NewStringUTF(timestamp);
	jstring sidString  = jniAttacher.get()->NewStringUTF(sid);

	jclass java_message_impl_cls = tw_jni_find_class(jniAttacher.get(), "com/twilio/ipmessaging/impl/MessageImpl");
	if(java_message_impl_cls != NULL) {
		LOG_WARN(TAG, "Found java_message_impl_cls class" );
	}

	jmethodID construct = tw_jni_get_method_by_class(jniAttacher.get(), java_message_impl_cls, "<init>", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;J)V");
	LOG_DEBUG(TAG,"Creating Messsage Object : construct");
	jobject message = tw_jni_new_object(jniAttacher.get(), java_message_impl_cls, construct, authorString, bodyString, timeStampString, messageContextHandle);
	jmethodID setSid = tw_jni_get_method_by_class(jniAttacher.get(), java_message_impl_cls, "setSid", "(Ljava/lang/String;)V");
	jniAttacher.get()->CallVoidMethod(message, setSid, sidString);
	LOG_DEBUG(TAG,"Created Message Object, calling java");

	switch (action) {
		case rtd::kTMActionAdded:
		{
			jniAttacher.get()->CallVoidMethod(j_ipmessagingclientListenerInternal_, j_onMessageAdd_, message);
			LOG_DEBUG(TAG, "Called java");
			break;
		}
		case rtd::kTMActionChanged:
		{
			jniAttacher.get()->CallVoidMethod(j_ipmessagingclientListenerInternal_, j_onMessageChange_, message);
			LOG_DEBUG(TAG, "Called java");
			break;
		}
		case rtd::kTMActionDeleted:
		{
			jniAttacher.get()->CallVoidMethod(j_ipmessagingclientListenerInternal_, j_onMessageDelete_, message);
			LOG_DEBUG(TAG, "Called java");
			break;
		}
	}
}

void TwilioIPMessagingClientListener::onChannel(TMAction action, ITMChannelPtr channelPtr)
{
	LOG_WARN(TAG,"TwilioIPMessagingClientListener::onChannel");
	LOG_DEBUG(TAG, "TwilioIPMessagingClientListener::onChannel");
	switch (action) {
		case rtd::kTMActionAdded:
		{
			if(channelPtr != nullptr) {
				switch (channelPtr->getStatus()) {
					case TMChannelStatus::kTMChannelStatusInvited:
					{
						LOG_WARN(TAG,"TwilioIPMessagingClientListener::onChannel invited");
						JNIEnvAttacher jniAttacher;
						jobject channel = createChannelObject(channelPtr);
						jniAttacher.get()->CallVoidMethod(j_ipmessagingclientListenerInternal_, j_onChannelInvite_, channel);
						LOG_WARN(TAG,"Calling java");
						break;
					}
					case TMChannelStatus::kTMChannelStatusJoined:
						LOG_WARN(TAG,"TwilioIPMessagingClientListener::onChannel joined");
						LOG_DEBUG(TAG, "TwilioIPMessagingClientListener::onChannel - joined");
						break;
					case TMChannelStatus::kTMChannelStatusNotParticipating:
					{
						LOG_WARN(TAG,"TwilioIPMessagingClientListener::onChannel notparticipating");
						LOG_DEBUG(TAG, "TwilioIPMessagingClientListener::onChannel - notparticipating");
						JNIEnvAttacher jniAttacher;
						jobject channel = createChannelObject(channelPtr);
						jniAttacher.get()->CallVoidMethod(j_ipmessagingclientListenerInternal_, j_onChannelAdd_, channel);
						break;
					}
				}
			}
		}
		case rtd::kTMActionChanged:
		{
			LOG_WARN(TAG, "onChannel::kTMActionChanged");
			JNIEnvAttacher jniAttacher;
			jobject channel = createChannelObject(channelPtr);
			jniAttacher.get()->CallVoidMethod(j_ipmessagingclientListenerInternal_, j_onChannelChanged_, channel);
			break;
		}
		case rtd::kTMActionDeleted:
		{
			LOG_WARN(TAG,"onChannel::kTMActionDeleted");
			LOG_DEBUG(TAG, "TwilioIPMessagingClientListener::onChannel - deleted");
			JNIEnvAttacher jniAttacher;
			jobject channel = createChannelObject(channelPtr);
			jniAttacher.get()->CallVoidMethod(j_ipmessagingclientListenerInternal_, j_onChannelDeleted_, channel);
			break;
		}
	}
}

void TwilioIPMessagingClientListener::onMember(TMAction action, ITMMemberPtr memberPtr)
{
    LOG_DEBUG(TAG,"TwilioIPMessagingClientListener::onMember");
    JNIEnvAttacher jniAttacher;
    jobject member, channel;
    
   jclass java_member_impl_cls = tw_jni_find_class(jniAttacher.get(), "com/twilio/ipmessaging/impl/MemberImpl");
    if(java_member_impl_cls != nullptr) {
        LOG_WARN(TAG,"Found java_member_impl_cls class" );
    }
    
    jmethodID construct = tw_jni_get_method_by_class(jniAttacher.get(), java_member_impl_cls, "<init>", "(Ljava/lang/String;Ljava/lang/String;J)V");
    const char* sid = memberPtr->getSid().c_str();
    const char* name = memberPtr->getUsername().c_str();
    LOG_DEBUG(TAG,"Member Name  : %s.", name );
    LOG_DEBUG(TAG,"Member Sid %s", sid);
    
    MemberContext* memberContext_ = new MemberContext();
    memberContext_->member = memberPtr;
    jlong memberContextHandle = reinterpret_cast<jlong>(memberContext_);
    jstring nameString = jniAttacher.get()->NewStringUTF(name);
    jstring memberSidString = jniAttacher.get()->NewStringUTF(sid);

    member = tw_jni_new_object(jniAttacher.get(), java_member_impl_cls, construct, memberSidString, nameString, memberContextHandle);
    LOG_WARN(TAG,"Created Member Object.");
    ITMChannelPtr channelPtr = memberPtr->getChannel();
	channel = createChannelObject(channelPtr);
    
    switch (action) {
		case rtd::kTMActionAdded: {
			LOG_WARN(TAG, "TwilioIPMessagingClientListener::onMember added");
			jniAttacher.get()->CallVoidMethod(j_ipmessagingclientListenerInternal_,
					j_onMemberJoin_, member, channel);
			break;
		}
		case rtd::kTMActionDeleted: {
			LOG_WARN(TAG, "TwilioIPMessagingClientListener::onMember deleted");
			jniAttacher.get()->CallVoidMethod(j_ipmessagingclientListenerInternal_,
					j_onMemberDelete_, member, channel);
			break;
		}
		case rtd::kTMActionChanged: {
			LOG_WARN(TAG, "TwilioIPMessagingClientListener::onMember changed");
			jniAttacher.get()->CallVoidMethod(j_ipmessagingclientListenerInternal_,
					j_onMemberChange_, member, channel);
			break;
		}
	}
    //jniAttacher.get()->DeleteGlobalRef(channel);
}

void TwilioIPMessagingClientListener::onError(const std::string& error)
{
	LOG_WARN(TAG,"TwilioIPMessagingClientListener::onError");
	JNIEnvAttacher jniAttacher;
	jint errorCode = 200;
	jstring errorMessage = jniAttacher.get()->NewStringUTF(error.c_str());
	jniAttacher.get()->CallVoidMethod(j_ipmessagingclientListenerInternal_, j_onError_, errorCode, errorMessage);
}

void TwilioIPMessagingClientListener::onToastNotification(const std::string& channelId, const std::string& messageId)
{
	LOG_WARN(TAG,"TwilioIPMessagingClientListener::onToastNotification");

}

void TwilioIPMessagingClientListener::onToastSubscribed()
{
	LOG_WARN(TAG,"TwilioIPMessagingClientListener::onToastSubscribed");
}

void TwilioIPMessagingClientListener::onToastFailed(TNError code)
{
	LOG_WARN(TAG,"TwilioIPMessagingClientListener::onToastFailed");
}

void TwilioIPMessagingClientListener::onTyping(TMTypingAction action, ITMChannelPtr channelPtr, ITMMemberPtr memberPtr)
{
    LOG_WARN(TAG,"onTyping TwilioIPMessagingClientListener");
    LOG_DEBUG(TAG, "TwilioIPMessagingClientListener::onTyping");

    JNIEnvAttacher jniAttacher;
	jobject member, channel;

    jclass java_member_impl_cls = tw_jni_find_class(jniAttacher.get(), "com/twilio/ipmessaging/impl/MemberImpl");
	if(java_member_impl_cls != nullptr) {
		LOG_WARN(TAG,"Found java_member_impl_cls class" );
	}

	jmethodID construct = tw_jni_get_method_by_class(jniAttacher.get(), java_member_impl_cls, "<init>", "(Ljava/lang/String;Ljava/lang/String;J)V");
	const char* sid = memberPtr->getSid().c_str();
	const char* name = memberPtr->getUsername().c_str();
	LOG_DEBUG(TAG,"Member Name  : %s.", name );
	LOG_DEBUG(TAG,"Member Sid %s", sid);

	MemberContext* memberContext_ = new MemberContext();
	memberContext_->member = memberPtr;
	jlong memberContextHandle = reinterpret_cast<jlong>(memberContext_);
	jstring nameString = jniAttacher.get()->NewStringUTF(name);
	jstring memberSidString = jniAttacher.get()->NewStringUTF(sid);

	member = tw_jni_new_object(jniAttacher.get(), java_member_impl_cls, construct, memberSidString, nameString, memberContextHandle);
	LOG_WARN(TAG,"Created Member Object.");
	 LOG_DEBUG(TAG, "onTyping:: created message object.");

	channel = createChannelObject(channelPtr);
    LOG_DEBUG(TAG, "onTyping:: created channel object.");
    switch (action) {
   		case rtd::TMTypingAction::kTMTypingActionStarted: {
   			LOG_DEBUG(TAG, "onTyping:: calling j_onTypingStarted_.");
   			jniAttacher.get()->CallVoidMethod(j_ipmessagingclientListenerInternal_,j_onTypingStarted_, channel, member);
   			break;
   		}
   		case rtd::TMTypingAction::kTMTypingActionEnded:{
   			LOG_DEBUG(TAG, "onTyping:: calling j_onTypingEnded_.");
   			jniAttacher.get()->CallVoidMethod(j_ipmessagingclientListenerInternal_,j_onTypingEnded_, channel, member);
   			break;
   		}
   	}
}


void TwilioIPMessagingClientListener::onChannelSynchronization(
		TMSynchronization event, ITMChannelPtr channelPtr) {
	if (event == TMSynchronization::kTMSynchronizationCompleted) {
		LOG_DEBUG(TAG, "onChannelSynchronization::kTMSynchronizationCompleted");
		auto messages = channelPtr->getMessages();
		messages->fetchLastMessages(
				[this,channelPtr, messages](TMResult result, std::vector<uint64_t> indexes) {
					if (result == TMResult::kTMResultSuccess) {
						LOG_DEBUG(TAG, "onChannelSynchronization::fetchLastMessages::kTMResultSuccess");
						JNIEnvAttacher jniAttacher;
						jobject channel;
						auto messages = channelPtr->getMessages();
						if (messages == nullptr) {
							return;
						} else {
							channel = createChannelObject(channelPtr);
							LOG_DEBUG(TAG, "onChannelSynchronization::calling channelListener:j_onChannelSync_.");
							jniAttacher.get()->CallVoidMethod(j_ipmessagingclientListenerInternal_,j_onChannelSync_, channel);
						}
					}
				}, 2000000000);
	}
}



