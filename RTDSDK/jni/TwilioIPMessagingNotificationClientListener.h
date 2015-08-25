//
//  TwilioIPMessagingNotificationClientListener.hpp
//  Twilio IP Messaging Client
//
//  Copyright © 2015 Twilio. All rights reserved.
//

#ifndef TwilioIPMessagingNotificationClientListener_cpp
#define TwilioIPMessagingNotificationClientListener_cpp

#include <Notification/ITNNotificationClient.h>

using namespace rtd;

class TwilioIPMessagingNotificationClientListener : public ITNNotificationClientObserver
{
public:
    void onInited();
    void onShutdowned();
    void onError(const TNErrorDescription& message, TNError code);

public:
    void waitShutdown();

private:
    std::condition_variable _shutdown;
    std::mutex _mutex;
};

#endif /* TwilioIPMessagingNotificationClientListener_cpp */
