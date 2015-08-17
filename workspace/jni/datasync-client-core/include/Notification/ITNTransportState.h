//
//  ITNTransportState.h
//  RTDNotificationLib
//
//  Created by Sergei Voronichev on 29/04/15.
//  Copyright (c) 2015 Twilio. All rights reserved.
//

#ifndef RTDNotificationLib_ITNTransportState_h
#define RTDNotificationLib_ITNTransportState_h

#include "TNNotificationCommon.h"

namespace rtd
{
    enum class TNTransportStates
    {
        Disabled, //!< Transport for receiving notifications for notification lib is not working.
        Enabled //!< Transport for receiving notifications for notification lib is working.
    };

    /*!
     * API to receive an information about transport state changes.
     *
     * Note: all callbacks are called from internal threads. Don't block them for a long time.
     */
    class ITNTransportStateObserver
    {
    public:
        virtual ~ITNTransportStateObserver() = default;

        /*!
         * Called when main transport connection changes its state.
         *
         * Note: now it is working only for twilsock!
         * \param [in] state - state of the transport connection. \sa #rtd::TNTransportStates.
         * \param [in] channelType - chennel type for wich transport connection state changed. (only twilsock supports now).
         */
        virtual void onTransportStateChanged(TNTransportStates state, TNChannelType channelType) = 0;
    };
    using ITNTransportStateObserverPtr = std::shared_ptr<ITNTransportStateObserver>;

    /*!
     * API to control transport state observers for notification library.
     */
    class ITNTransportState
    {
    public:
        virtual ~ITNTransportState() = default;

        /*!
         * Used to add a new observer to notification library.
         *
         * \sa #rtd::ITNTransportStateObserver.
         */
        virtual void AddTransportObserver(ITNTransportStateObserverPtr) = 0;

        /*!
         * Used to remove an observer from notification library.
         *
         * \sa #rtd::ITNTransportStateObserver.
         */
        virtual void RemoveTransportObserver(ITNTransportStateObserverPtr) = 0;
    };
    using ITNTransportStatePtr = std::shared_ptr<ITNTransportState>;
}

#endif
