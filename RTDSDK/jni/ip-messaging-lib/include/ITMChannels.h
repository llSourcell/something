//
//  ITMChannels.h
//  RTDIPMessagingLib
//
//  Created by Allan Rank on 13/04/15.
//  Copyright (c) 2015 Twilio. All rights reserved.
//

#ifndef RTDIPMessagingLib_ITMChannels_h
#define RTDIPMessagingLib_ITMChannels_h

#include <vector>
#include <unordered_map>
#include "ITMTypes.h"

namespace rtd {

/** ITMChannels
 */
class ITMChannels
{
public:
    virtual ~ITMChannels() {};
    
public:
    /** factory */
    virtual ITMChannelPtr createChannel() = 0;
    /** getters */
    virtual void getMyChannelsList(std::vector<ITMChannelPtr>& channels) const = 0;
    virtual void getPublicChannelsList(std::vector<ITMChannelPtr>& channels) const = 0;
    virtual void getPublicChannelsFriendlyNameList(std::unordered_map<std::string, std::string>& channels) const = 0;
    virtual ITMChannelPtr getChannel(const std::string channelSid) = 0;
    /** actions */
    virtual void add(ITMChannelPtr channel, TMResultHandler handler) = 0;
};

} //namespace rtd

#endif
