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
    std::cout << "NotificationClientListener: inited" << std::endl;
}
void TwilioIPMessagingNotificationClientListener::onShutdowned()
{
    std::cout << "NotificationClientListener: shut down" << std::endl;
    std::unique_lock<std::mutex> l(_mutex);
    _shutdown.notify_one();
    std::cout << "NotificationClientListener: shut down exit" << std::endl;
}
void TwilioIPMessagingNotificationClientListener::onError(const TNErrorDescription& message, TNError code)
{
    std::cout << "NotificationClientListener: error " << code.nError << " - " << message << std::endl;
}

void TwilioIPMessagingNotificationClientListener::waitShutdown()
{
    std::unique_lock<std::mutex> l(_mutex);
    _shutdown.wait(l);
}
