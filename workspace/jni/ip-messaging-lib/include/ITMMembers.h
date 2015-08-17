//
//  ITMMembers.h
//  RTDIPMessagingLib
//
//  Created by Allan Rank on 13/04/15.
//  Copyright (c) 2015 Twilio. All rights reserved.
//

#ifndef RTDIPMessagingLib_ITMMembers_h
#define RTDIPMessagingLib_ITMMembers_h

#include <vector>
#include "ITMTypes.h"

namespace rtd {

/** ITMMembers
 */
class ITMMembers
{
public:
    virtual ~ITMMembers() {};    

public:
    /** factory */
    virtual ITMMemberPtr createMember(const std::string& username) = 0;
    /** getters */
    virtual void getMembersList(std::vector<ITMMemberPtr>& members) const = 0;
    virtual ITMChannelPtr getChannel() = 0;
    /** actions */
    virtual void add(ITMMemberPtr member, TMResultHandler handler) = 0;
    virtual void invite(ITMMemberPtr member, TMResultHandler handler) = 0;
    virtual void remove(ITMMemberPtr member, TMResultHandler handler) = 0;
};

} //namespace rtd

#endif
