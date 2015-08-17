//
//  ITMClient.h
//  RTDIPMessagingLib
//
//  Created by Allan Rank on 08/04/15.
//  Copyright (c) 2015 Twilio. All rights reserved.
//

#ifndef RTDIPMessagingLib_ITMClient_h
#define RTDIPMessagingLib_ITMClient_h

#include <vector>
#include <string>

#include <Common/ITDConfiguration.h>
#include <Notification/ITNNotificationClient.h>

#include "ITMTypes.h"

namespace rtd {

/** Interface for client callback events.
 */
class ITMClientListener
{
public:
    virtual void onMessage(TMAction action, ITMessagePtr message) = 0;
    virtual void onChannel(TMAction action, ITMChannelPtr channel) = 0;
    virtual void onMember(TMAction action, ITMMemberPtr member) = 0;
    virtual void onError(const std::string& error) = 0;
    
    virtual void onToastNotification(const std::string& channelSid, const std::string& messageId) = 0;
    virtual void onToastSubscribed() = 0;
    virtual void onToastFailed(TNError code) = 0;
};

/** ITMClient is the initial object which application needs to create in order to use ip messaging product.
 */
class ITMClient
{
public:
    static ITMClientPtr createClient(const std::string& capabilityToken, ITMClientListenerPtr listener, ITDConfigurationProviderPtr configurationProvider, ITNNotificationClientPtr notificationClient, TMResultHandler handler);
    
    /////////////////////////////////
    /// FOR TESTING PURPOSES ONLY ///
    static std::string fetchCapabilityToken(const std::string& username, const std::string& password, const std::string& endpointId, ITDConfigurationProviderPtr configurationProvider);
    /////////////////////////////////
        
public:
    virtual ITMChannelsPtr getChannels() = 0;
    virtual void shutdown() = 0;
    
protected:
    virtual ~ITMClient() {};
};
    
} //namespace rtd

#endif
