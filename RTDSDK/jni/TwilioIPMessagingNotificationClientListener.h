//
//  TwilioIPMessagingNotificationClientListener.hpp
//  Twilio IP Messaging Client
//
//  Copyright © 2015 Twilio. All rights reserved.
//

#ifndef TwilioIPMessagingNotificationClientListener_cpp
#define TwilioIPMessagingNotificationClientListener_cpp

#include <Notification/ITNNotificationClient.h>
#include <android/log.h>
#include <jni.h>

using namespace rtd;


class TwilioIPMessagingNotificationClientListener : public ITNNotificationClientObserver
{
public:
    void onInited();
    void onShutdowned();
    void onError(const TNErrorDescription& message, TNError code);
    void waitShutdown();

private:
    std::condition_variable _shutdown;
    std::mutex _mutex;
    JNIEnv* env_;
};

#endif /* TwilioIPMessagingNotificationClientListener_cpp */
