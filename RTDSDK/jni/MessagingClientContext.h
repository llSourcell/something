//  TwilioIPMessagingConfigurationProvider.hpp
//  Twilio IP Messaging Client
//
//  Copyright © 2015 Twilio. All rights reserved.
//

#ifndef MessagingClient_h
#define MessagingClient_h

#include <string>


/** MessagingClient
 */
class MessagingClient
{
public:
    virtual ~MessagingClient() {};
    MessagingClient();

public:
    /** getters */
    std::shared_ptr<TwilioIPMessagingNotificationClientListener> getNotificationClientListener() = 0;
    std::shared_ptr<TwilioIPMessagingConfigurationProvider> getConfigurationProvider() = 0;
    std::shared_ptr<TwilioIPMessagingClientListener> getClientListener() = 0;
    ITNNotificationClientPtr getITNNotificationClientPtr() = 0;
    ITMClientPtr getITMClientPtr() = 0;
    ITMChannelsPtr getITMChannelsPtr() = 0;
    /** setters */
    virtual void setNotificationClientListener(std::shared_ptr<TwilioIPMessagingNotificationClientListener> listener) = 0;
    virtual void setConfigurationProvider(std::shared_ptr<TwilioIPMessagingConfigurationProvider> provider) = 0;
    virtual void setIPMessagingClientListener(std::shared_ptr<TwilioIPMessagingClientListener> listener) = 0;
    virtual void setITNNotificationClientPtr(ITNNotificationClientPtr notificationClientPointer) = 0;
    virtual void setITMClientPtr(ITMClientPtr clientPtr) = 0;
    virtual void setITMChannelsPtr(ITMChannelsPtr listener) = 0;

private:
    std::shared_ptr<TwilioIPMessagingNotificationClientListener>  notificationClientObserver;
    std::shared_ptr<TwilioIPMessagingConfigurationProvider> configurationProvider;
    std::shared_ptr<TwilioIPMessagingClientListener> messagingListener;
    ITNNotificationClientPtr notificationClient;
    ITMClientPtr messagingClient;
    ITMChannelsPtr channels;

};

#endif /* MessagingClient_h */


/*
 *
 * // structure with all the shared pointer
typedef struct ClientContext {
	std::shared_ptr<TwilioIPMessagingNotificationClientListener>  notificationClientObserver;
	std::shared_ptr<TwilioIPMessagingConfigurationProvider> configurationProvider;
	std::shared_ptr<TwilioIPMessagingClientListener> messagingListener;
	ITNNotificationClientPtr notificationClient;
	ITMClientPtr messagingClient;
	ITMChannelsPtr channels;
	std::map<const char*,ITMChannelPtr>   channelMap;

} ClientContext;

*/
 */
