//
//  INMessageReceiver.h
//  RTDNotificationLib
//
//  Created by Sergei Voronichev on 09/04/15.
//  Copyright (c) 2015 Twilio. All rights reserved.
//

#ifndef RTDNotificationLib_INMessageReceiver_h
#define RTDNotificationLib_INMessageReceiver_h

#include "TNNotificationCommon.h"

namespace rtd
{

/*!
 * Provide API to put incoming messages from different sources to Notification library.
 *
 * This API puts messages to the message queue, then workers take parse and rout them to subscribers.
 */
class ITNMessageReceiver
{
public:
    /*!
     * Used to put imcoming messages here from certain channel type \sa #rtd::TNChannelType.
     */
    virtual void ReceivedNotification(const TNChannelType& channelType, const TNRawMessage& message) = 0;
    virtual ~ITNMessageReceiver() = default;

    /*!
     * Used to process messages which were received before inited state. (Hack!)
     */
    virtual void ProcessDeferredQueue() = 0;

};
    
using ITNMessageReceiverPtr = std::shared_ptr<ITNMessageReceiver>;

}
#endif
