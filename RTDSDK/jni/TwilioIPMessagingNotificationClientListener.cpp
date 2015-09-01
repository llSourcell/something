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
    LOGW("NotificationClientListener: initiated");
}

void TwilioIPMessagingNotificationClientListener::onShutdowned()
{
    LOGW("NotificationClientListener: shut down");
    std::unique_lock<std::mutex> l(_mutex);
    _shutdown.notify_one();
    LOGW("NotificationClientListener: shut down exit");
}
void TwilioIPMessagingNotificationClientListener::onError(const TNErrorDescription& message, TNError code)
{
    LOGW("NotificationClientListener: errorCode %d", code.nError );
    //const char *msgStr = env->GetStringUTFChars(std::string(message), 0);

    //__android_log_print(ANDROID_LOG_ERROR, "TwilioIPMessagingNotificationClientListener", std::string(message));
    //LOGW(msgStr);

}

void TwilioIPMessagingNotificationClientListener::waitShutdown()
{
    std::unique_lock<std::mutex> l(_mutex);
    _shutdown.wait(l);
}
