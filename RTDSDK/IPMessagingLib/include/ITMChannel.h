//
//  ITMChannel.h
//  RTDIPMessagingLib
//
//  Created by Allan Rank on 09/04/15.
//  Copyright (c) 2015 Twilio. All rights reserved.
//

#ifndef RTDIPMessagingLib_ITMChannel_h
#define RTDIPMessagingLib_ITMChannel_h

#include "ITMTypes.h"

namespace rtd {

enum TMChannelStatus
{
    kTMChannelStatusInvited = 0,
    kTMChannelStatusJoined,
    kTMChannelStatusNotParticipating
};

enum TMChannelType
{
    kTMChannelTypePublic = 0,
    kTMChannelTypePrivate
};

/** ITMChannel
 */
class ITMChannel
{
public:
    virtual ~ITMChannel() {};
    
public:
    /** getters */
    virtual ITMMembersPtr getMembers() = 0;
    virtual ITMessagesPtr getMessages() = 0;
    virtual const std::string& getName() = 0;
    virtual const std::string& getAttributes() = 0;
    virtual TMChannelStatus getStatus() = 0;
    virtual const std::string& getSid() = 0;
    virtual TMChannelType getType() = 0;
    /** setters */
    virtual void setName(const std::string& name, TMResultHandler handler) = 0;
    virtual void setAttributes(const std::string& attributes, TMResultHandler handler) = 0;
    virtual void setType(TMChannelType type, TMResultHandler handler) = 0;
    /** actions */
    virtual void join(TMResultHandler handler) = 0;
    virtual void leave(TMResultHandler handler) = 0;
    virtual void destroy(TMResultHandler handler) = 0;
    virtual void declineInvitation(TMResultHandler handler) = 0;
};

} //namespace rtd

#endif
