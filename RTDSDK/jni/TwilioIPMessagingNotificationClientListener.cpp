//
//  TwilioIPMessagingNotificationClientListener.cpp
//  Twilio IP Messaging Client
//
//  Copyright © 2015 Twilio. All rights reserved.
//

#include "TwilioIPMessagingNotificationClientListener.h"

#include <iostream>
#include <stdio.h>

using namespace rtd;


void TwilioIPMessagingNotificationClientListener::onInited()
{
}

void TwilioIPMessagingNotificationClientListener::onShutdowned()
{
    std::unique_lock<std::mutex> l(_mutex);
    _shutdown.notify_one();
}
void TwilioIPMessagingNotificationClientListener::onError(const TNErrorDescription& message, TNError code)
{


}

void TwilioIPMessagingNotificationClientListener::waitShutdown()
{
    std::unique_lock<std::mutex> l(_mutex);
    _shutdown.wait(l);
}
