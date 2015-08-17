//
//  INotificator.h
//  RTDNotificationLib
//
//  Created by Sergei Voronichev on 09/04/15.
//  Copyright (c) 2015 Twilio. All rights reserved.
//

#ifndef RTDNotificationLib_INotificator_h
#define RTDNotificationLib_INotificator_h

#include "TNNotificationCommon.h"

namespace rtd
{
/*!
 * It is owner of a subscription.
 *
 * Subscription invalidates when the value of ref counter for this object is 0.
 * \sa #rtd::ITNNotificationObserver
 */
class ITNSubscription
{
public:
    virtual ~ITNSubscription() = default;
};
using ITNSubscriptionPtr = std::shared_ptr<ITNSubscription>;

/*!
 * This API provides to subscribers a possibility to receive status of subscription and incoming messages.
 *
 * Note: all callbacks are called from internal threads. Don't block them for a long time.
 */
class ITNNotificationObserver
{
public:
    virtual ~ITNNotificationObserver() = default;

    /*!
     * Called to notify that subscription is ready.
     *
     * \param [in] type - message type wich was used to make subscription
     * \param [in] subsription - smart pointer to subscription. If ref counter become 0, subscription is removed (Notificator's Unsubscribe method is called automatically).
     */
    virtual void onSubscribed(const TNMessageType& type, ITNSubscriptionPtr subsription) = 0;

    /*!
     * Called to notify that new message accordin the message type has turned up via certain channel \sa #rtd::TNChannelType.
     */
    virtual void onReceivedNotification(const TNChannelType& channelType, const TNMessageType& type, const TNMessagePayload& payload) = 0;

    /*!
     * Called to notify about an Error in subscription flow.
     *
     * \sa #rtd::TNError.
     */
    virtual void onError(const TNErrorDescription& message, TNError code) = 0;
};
using ITNNotificationObserverPtr = std::shared_ptr<ITNNotificationObserver>;

/*!
 * API to create subscriptions for receiving an incoming messages according a message type.
 */
class ITNNotificator
{
public:
    virtual ~ITNNotificator() = default;
    /*!
     * Provide API to add an observer in oder to receive message according message type.
     *
     * \sa #rtd::ITNNotificationObserver.
     */
    virtual void Subscribe(const TNMessageType& type, const TNChannelTypeList& channels, ITNNotificationObserverPtr observer) = 0;
};
using ITNNotificatorPtr = std::shared_ptr<ITNNotificator>;

}
#endif
