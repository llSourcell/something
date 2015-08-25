//
//  TwilioIPMessagingClientListener.cpp
//  Twilio IP Messaging Client
//
//  Copyright © 2015 Twilio. All rights reserved.
//

#include "TwilioIPMessagingClientListener.h"

using namespace rtd;

TwilioIPMessagingClientListener::TwilioIPMessagingClientListener(void* target):m_target(target)
{
}

TwilioIPMessagingClientListener::~TwilioIPMessagingClientListener()
{
    m_target = nullptr;
}

void
TwilioIPMessagingClientListener::setTarget(void* target)
{
    m_target = target;
}

void TwilioIPMessagingClientListener::onMessage(TMAction action, ITMessagePtr message)
{
   // std::cout << "app: onMessage" << std::endl;

    if (m_target)
    {
        //[(__bridge id<TwilioIPMessagingClientListenerDelegate>)m_target onMessageAction:action message:message];
    }
}

void TwilioIPMessagingClientListener::onChannel(TMAction action, ITMChannelPtr channel)
{
    //std::cout << "app: onChannel" << (action == TMAction::kTMActionDeleted ? " removed" : " added") << std::endl;

    if (m_target)
    {
        //[(__bridge id<TwilioIPMessagingClientListenerDelegate>)m_target onChannelAction:action channel:channel];
    }
}

void TwilioIPMessagingClientListener::onMember(TMAction action, ITMMemberPtr member)
{
    //std::cout << "app: onMember " << member->getUsername() << (action == TMAction::kTMActionDeleted ? " removed" : " added") << std::endl;

    if (m_target)
    {
       // [(__bridge id<TwilioIPMessagingClientListenerDelegate>)m_target onMemberAction:action member:member];
    }
}

void TwilioIPMessagingClientListener::onError(const std::string& error)
{
    //std::cout << "app: onError " << error << std::endl;

    if (m_target)
    {
       // [(__bridge id<TwilioIPMessagingClientListenerDelegate>)m_target onError:error];
    }
}

void TwilioIPMessagingClientListener::onToastNotification(const std::string& channelId, const std::string& messageId)
{
   // std::cout << "app: onToastNotification" << std::endl;

    if (m_target)
    {
        //[(__bridge id<TwilioIPMessagingClientListenerDelegate>)m_target onToastNotificationChannelId:channelId messageId:messageId];
    }
}

void TwilioIPMessagingClientListener::onToastSubscribed()
{
    //std::cout << "app: onToastSubscribed" << std::endl;

    if (m_target)
    {
       // [(__bridge id<TwilioIPMessagingClientListenerDelegate>)m_target onToastSubscribed];
    }
}

void TwilioIPMessagingClientListener::onToastFailed(TNError code)
{
    //std::cout << "app: onToastFailed" << std::endl;

    if (m_target)
    {
       // [(__bridge id<TwilioIPMessagingClientListenerDelegate>)m_target onToastFailedWithErrorCode:code];
    }


}


