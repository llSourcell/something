//
//  ITMessages.h
//  RTDIPMessagingLib
//
//  Created by Allan Rank on 13/04/15.
//  Copyright (c) 2015 Twilio. All rights reserved.
//

#ifndef RTDIPMessagingLib_ITMessages_h
#define RTDIPMessagingLib_ITMessages_h

#include <vector>
#include "ITMTypes.h"

namespace rtd {

/** ITMessages
 */
class ITMessages
{
public:
    virtual ~ITMessages() {};
    
public:
    /** factory */
    virtual ITMessagePtr createMessage() = 0;
    /** getters */
    virtual void getMessagesList(std::vector<ITMessagePtr>& messages) const = 0;
    virtual ITMChannelPtr getChannel() = 0;
    /** actions */
    virtual void send(ITMessagePtr message, TMResultHandler handler) = 0;
    virtual void remove(ITMessagePtr message, TMResultHandler handler) = 0;
};

} //namespace rtd

#endif
