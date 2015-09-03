//
//  ITMessage.h
//  RTDIPMessagingLib
//
//  Created by Allan Rank on 09/04/15.
//  Copyright (c) 2015 Twilio. All rights reserved.
//

#ifndef RTDIPMessagingLib_ITMessage_h
#define RTDIPMessagingLib_ITMessage_h

#include "ITMTypes.h"

namespace rtd {

/** ITMessage
 */
class ITMessage
{
public:
    virtual ~ITMessage() {};
    
public:
    /** getters */
    virtual const std::string& getAuthor() = 0;
    virtual const std::string& getBody() = 0;
    virtual const std::string& getTimestamp() = 0;
    virtual ITMChannelPtr getChannel() = 0;
    virtual ITMessagesPtr getMessages() = 0;
    virtual const std::string& getSid() = 0;
    /** setters */
    virtual void setBody(const std::string& body, TMResultHandler handler) = 0;
};

} //namespace rtd

#endif
