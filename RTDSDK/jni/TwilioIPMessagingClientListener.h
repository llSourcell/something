//
//  TwilioIPMessagingClientListener.hpp
//  Twilio IP Messaging Client
//
//  Copyright © 2015 Twilio. All rights reserved.
//

#ifndef TwilioIPMessagingClientListener_cpp
#define TwilioIPMessagingClientListener_cpp

#include <Poco/Foundation.h>
#include <android/log.h>

#include "ITMClient.h"
#include "ITMChannel.h"
#include "ITMMember.h"
#include "ITMessages.h"
#include "ITMessage.h"

using namespace rtd;


class TwilioIPMessagingClientListener : public ITMClientListener
{
public:
    TwilioIPMessagingClientListener(JNIEnv* env,jobject obj,jobject j_listenert);
    virtual ~TwilioIPMessagingClientListener();

    void onMessage(TMAction action, ITMessagePtr message);
    void onChannel(TMAction action, ITMChannelPtr channel);
    void onMember(TMAction action, ITMMemberPtr member);
    void onError(const std::string& error);

    void onChannelSynchronization(TMSynchronization event, ITMChannelPtr channel);

    void onToastNotification(const std::string& channelId, const std::string& messageId);
    void onToastSubscribed();
    void onToastFailed(TNError code);
    void onTyping(TMTypingAction action, ITMChannelPtr channel, ITMMemberPtr member);

private:
    jobject j_ipmessagingclient_;
    jobject j_ipmessagingclientListenerInternal_;
	jmethodID j_onMessageAdd_;
	jmethodID j_onMessageChange_;
	jmethodID j_onMessageDelete_;
	jmethodID j_onChannelInvite_;
	jmethodID j_onChannelAdd_;
	jmethodID j_onChannelChanged_;
	jmethodID j_onChannelDeleted_;
	jmethodID j_onAttributesChange_;
    jmethodID j_onMemberJoin_;
    jmethodID j_onMemberChange_;
    jmethodID j_onMemberDelete_;
    jmethodID j_onTypingStarted_;
    jmethodID j_onTypingEnded_;
	jmethodID j_onError_;
	jmethodID j_onChannelSync_;
	JNIEnv* env_;
};

#endif /* TwilioIPMessagingClientListener_cpp */
