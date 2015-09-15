//
//  TwilioIPMessagingClientListener.cpp
//  Twilio IP Messaging Client
//
//  Copyright © 2015 Twilio. All rights reserved.
//

#include <jni.h>
#include "TwilioIPMessagingClientListener.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__))


using namespace rtd;

TwilioIPMessagingClientListener::TwilioIPMessagingClientListener(JNIEnv* env,jobject obj, jobject j_ipmessagingclient) {
}

TwilioIPMessagingClientListener::~TwilioIPMessagingClientListener()
{

}


void TwilioIPMessagingClientListener::onMessage(TMAction action, ITMessagePtr message)
{
	LOGW("TwilioIPMessagingClientListener::onMessage");

}

void TwilioIPMessagingClientListener::onChannel(TMAction action, ITMChannelPtr channel)
{
	LOGW("TwilioIPMessagingClientListener::onChannel");

}

void TwilioIPMessagingClientListener::onMember(TMAction action, ITMMemberPtr member)
{
	LOGW("TwilioIPMessagingClientListener::onMember");

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


