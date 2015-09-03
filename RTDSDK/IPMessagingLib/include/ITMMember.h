//
//  ITMMember.h
//  RTDIPMessagingLib
//
//  Created by Allan Rank on 09/04/15.
//  Copyright (c) 2015 Twilio. All rights reserved.
//

#ifndef RTDIPMessagingLib_ITMMember_h
#define RTDIPMessagingLib_ITMMember_h

#include "ITMTypes.h"

namespace rtd {

/** ITMMember
 */
class ITMMember
{
public:
    virtual ~ITMMember() {}
    
public:
    /** getters */
    virtual const std::string& getUsername() = 0;
    virtual ITMChannelPtr getChannel() = 0;
    virtual ITMMembersPtr getMembers() = 0;
    virtual const std::string& getSid() = 0;
};

} //namespace rtd

#endif
