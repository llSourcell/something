//
//  RTDNotificationCommon.h
//  RTDNotificationLib
//
//  Created by Sergei Voronichev on 09/04/15.
//  Copyright (c) 2015 Twilio. All rights reserved.
//

#ifndef RTDNotificationLib_RTDNotificationCommon_h
#define RTDNotificationLib_RTDNotificationCommon_h

#include <memory>
#include <string>
#include <list>
#include <sstream>


namespace rtd
{
    using TNString = std::string;
    using TNMessageType = TNString;
    using TNMessagePayload = TNString;
    using TNNotificationID = TNString;
    using TNRawMessage = TNString;
    using TNTwilioToken = TNString;
    using TNErrorDescription = TNString;
    using TNUri = TNString;

    enum class TNChannelType : unsigned int
    {
        APNS, //!< channel type for receiving push notification via APNs service.
        GCM, //!< channel type for receiving push notification via GCM service.
        TWS, //!< channel type for receiving push notification via twilsock service.
        Unknown,
    };
    using TNChannelTypeList = std::list<TNChannelType>;

    enum class TNEError : unsigned int
    {
        PocoError, //!< Poco exception occured.
        StdExc, //!< STD exception occured.
        IncorrectState, //!< Used component is in wrong state.
        NotImplemented, //!< Method has not been implemented yet.
        WrongRegLocation, //!< Received wrong or empty register location from ERS.
        WrongChannelType, //!< Tried to use wrong channel type. \sa #rtd::TNChannelType.
        WrongAnswer, //!< Wrong answer from a service.
        TwilsockError, //!< Error is inside twilsock transport.
        CannotParsIncomingMessage, //!< Parser can not parse incoming message.
        CannotRouteIncomingMessage, //!< Router can not route incoming message.
        CannotMakeSubscription, //!< Notification client can not create a subscription.
        CannotRemoveSubscription, //!< Notification client can not remove subscription.
        UnknownMessageType, //!< Try to work with unknown message type.
        EmptyMessageTypeList, //!< Try to create/update registration with empty message type list.
        Unknown,
        
    };

    /*!
     * Union to make it easy convert enume to integer and back.
     */
    union TNError
    {
        TNError(TNEError e) : error(e){}
        TNError(unsigned int e) : nError(e){}

        TNEError error;
        unsigned int nError;
    };
}

std::stringstream& operator<<(std::stringstream& stream, const rtd::TNChannelType& entry);

#endif
