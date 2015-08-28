//
//  TwilioIPMessagingClientListener.hpp
//  Twilio IP Messaging Client
//
//  Copyright © 2015 Twilio. All rights reserved.
//

#ifndef TwilioIPMessagingClientListener_cpp
#define TwilioIPMessagingClientListener_cpp

#include <Poco/Foundation.h>

#include <Common/TLLogger.h>

#include "ITMClient.h"
#include "ITMChannel.h"
#include "ITMMember.h"
#include "ITMessages.h"
#include "ITMessage.h"

using namespace rtd;

class TwilioIPMessagingClientListener : public ITMClientListener
{
public:
    TwilioIPMessagingClientListener();
    virtual ~TwilioIPMessagingClientListener();

    void onMessage(TMAction action, ITMessagePtr message);
    void onChannel(TMAction action, ITMChannelPtr channel);
    void onMember(TMAction action, ITMMemberPtr member);
    void onError(const std::string& error);

    void onToastNotification(const std::string& channelId, const std::string& messageId);
    void onToastSubscribed();
    void onToastFailed(TNError code);

};

#endif /* TwilioIPMessagingClientListener_cpp */
