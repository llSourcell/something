//
//  INotificationClient.h
//  RTDNotificationLib
//
//  Created by Sergei Voronichev on 09/04/15.
//  Copyright (c) 2015 Twilio. All rights reserved.
//

#ifndef RTDNotificationLib_INotificationClient_h
#define RTDNotificationLib_INotificationClient_h

#include "ITNMessageReceiver.h"
#include "ITNNotificator.h"
#include "ITNRegistrar.h"
#include "ITNTransportState.h"

#include "../Common/ITDConfiguration.h"

namespace rtd
{
    /*!
     * API to provide information about a notification client state changes.
     *
     * Note: all callbacks are called from internal threads. Don't block them for a long time.
     */
    class ITNNotificationClientObserver
    {
    public:
        virtual ~ITNNotificationClientObserver() = default;

        /*!
         * Called when notification client is inited and ready to work.
         */
        virtual void onInited() = 0;

        /*!
         * Called when notification client is finally closed.
         */
        virtual void onShutdowned() = 0;

        /*!
         * Called to notify about an Error in a notification client work.
         *
         * \sa #rtd::TNError.
         */
        virtual void onError(const TNErrorDescription& message, TNError code) = 0;
    };
    using ITNNotificationClientObserverPtr = std::shared_ptr<ITNNotificationClientObserver>;

    /*!
     * This API is main entery point to use notification library.
     */
    class ITNNotificationClient
        : public ITNMessageReceiver
        , public ITNNotificator
        , public ITNRegistrar
        , public ITNTransportState
    {
    public:
        virtual ~ITNNotificationClient() = default;

        /*!
         * API to initialize notification client.
         *
         * \param [in] observer - an observer to follow up notification client states. \sa #rtd::ITNNotificationClientObserver.
         */
        virtual void Init(ITNNotificationClientObserverPtr observer) = 0;

        /*!
         * API to shutdown notification client.
         */
        virtual void Shutdown() = 0;

        /*!
         * API to re-initialize notification library for a new twilio capability token.
         */
        virtual void SetToken(const TNTwilioToken& newTwToken) = 0;
    };
    using ITNNotificationClientPtr = std::shared_ptr<ITNNotificationClient>;

    /*!
     * Simple factory to create an instance of notification client.
     */
    class TNNotificationClientFactory
    {
    public:
        /*!
         * Creates an instance of Notification client.
         *
         * \param [in] twToken - twilio user token for authentification issue.
         * \param [in] conf - a reference to configuration provider object. \sa #rtd::ITDConfigurationProvider.
         * (Now notification client uses two parameters from configuration provider: "RTDTwilsockServiceAddr", "RTDRegistrationServiceAddr").
         */
        static ITNNotificationClientPtr CreateNotificationClient(const TNTwilioToken& twToken, ITDConfigurationProviderPtr conf = nullptr);
    };
}

#endif
