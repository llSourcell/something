//
//  TwilioIPMessagingClientListener.cpp
//  Twilio IP Messaging Client
//
//  Copyright © 2015 Twilio. All rights reserved.
//

#include "TwilioIPMessagingClientListener.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__))


using namespace rtd;

TwilioIPMessagingClientListener::TwilioIPMessagingClientListener() {
}

TwilioIPMessagingClientListener::~TwilioIPMessagingClientListener()
{

}


void TwilioIPMessagingClientListener::onMessage(TMAction action, ITMessagePtr message)
{

}

void TwilioIPMessagingClientListener::onChannel(TMAction action, ITMChannelPtr channel)
{

}

void TwilioIPMessagingClientListener::onMember(TMAction action, ITMMemberPtr member)
{

}

void TwilioIPMessagingClientListener::onError(const std::string& error)
{
    //std::cout << "app: onError " << error << std::endl;

   /* if (m_target)
    {
       // [(__bridge id<TwilioIPMessagingClientListenerDelegate>)m_target onError:error];
    } */
}

void TwilioIPMessagingClientListener::onToastNotification(const std::string& channelId, const std::string& messageId)
{

}

void TwilioIPMessagingClientListener::onToastSubscribed()
{

}

void TwilioIPMessagingClientListener::onToastFailed(TNError code)
{


}


