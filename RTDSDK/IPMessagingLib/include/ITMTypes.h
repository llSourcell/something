//
//  ITMTypes.h
//  RTDIPMessagingLib
//
//  Created by Allan Rank on 14/04/15.
//  Copyright (c) 2015 Twilio. All rights reserved.
//

#ifndef RTDIPMessagingLib_ITMTypes_h
#define RTDIPMessagingLib_ITMTypes_h

#include <memory>
#include <functional>

namespace rtd {

/** Forward declarations.
 */
class ITMClient;
class ITMChannels;
class ITMChannel;
class ITMessages;
class ITMessage;
class ITMMember;
class ITMMembers;
class ITMClientListener;

/** Defines action performed with the object.
 */
enum TMAction
{
    kTMActionAdded = 0,
    kTMActionChanged,
    kTMActionDeleted
};

/** Defines result returned to handler functions.
 */
enum TMResult
{
    kTMResultSuccess = 0,
    kTMResultFailure
};

/** Ptr and vector aliases.
 */
using ITMClientPtr = std::shared_ptr<ITMClient>;
using ITMChannelsPtr = std::shared_ptr<ITMChannels>;
using ITMChannelPtr = std::shared_ptr<ITMChannel>;
using ITMessagesPtr = std::shared_ptr<ITMessages>;
using ITMessagePtr = std::shared_ptr<ITMessage>;
using ITMMemberPtr = std::shared_ptr<ITMMember>;
using ITMMembersPtr = std::shared_ptr<ITMMembers>;
using ITMClientListenerPtr = std::shared_ptr<ITMClientListener>;

/** Function aliases.
 */
using TMResultHandler = std::function<void(TMResult)>;
}

#endif
