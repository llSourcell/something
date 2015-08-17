//
//  IRegistrar.h
//  RTDNotificationLib
//
//  Created by Sergei Voronichev on 09/04/15.
//  Copyright (c) 2015 Twilio. All rights reserved.
//

#ifndef RTDNotificationLib_IRegistrar_h
#define RTDNotificationLib_IRegistrar_h

#include "TNNotificationCommon.h"

namespace rtd
{

enum class TNRegistrationStates
{
    Unregistered, //!< Registartion session is in unregistered state and do not try to establishe registration.
    Registering, //!< Registartion session is in unregistered but is trying to establishe registration.
    Registered //!< Registration is in active state. Client received confirmation from server.
};

/*!
 * API to provide information about a registrarion state changes
 *
 * Note: all callbacks are called from internal threads. Don't block them for a long time.
 */
class ITNRegistrationObserver
{
public:
    virtual ~ITNRegistrationObserver() = default;

    /*!
     * Called when registartion FSM changes its state.
     *
     * \param [in] type - channel type for which registration state has changed. \sa #rtd::TNChannelType
     * \param [in] nId - notification id for which registration state has changed.
     * \param [in] state - current registration state. \sa #rtd::TNRegistrationStates.
     */
    virtual void onRegStateChanged(const TNChannelType& type, const TNNotificationID& nId, TNRegistrationStates state) = 0;

    /*!
     * Called to notify about an Error in registration flow.
     *
     * \sa #rtd::TNError.
     */
    virtual void onError(const TNErrorDescription& message, TNError code) = 0;
};
using ITNRegistrationObserverPtr = std::shared_ptr<ITNRegistrationObserver>;

/*!
 * API to start register/unregister procedures and set observer if it's necessary.
 */
class ITNRegistrar
{
public:
    virtual ~ITNRegistrar() = default;
    /*!
     * Provide possibility to make registration on registration service (ERS).
     *
     * Could be used to retry performing registration after a failed attempt. Otherwise next attempt will start by timer.
     * \param [in] type - what chennel type does notification id belong. \sa #rtd::TNChannelType.
     * \param [in] nId - notification id for certain channel.
     * \param [in] observer - an observer for receiving registration state changes. \sa #rtd::ITNRegistrationObserver.
     */
    virtual void Register(const TNChannelType& type, const TNNotificationID& nId, ITNRegistrationObserverPtr observer) = 0;

    /*!
     * Provide possibility to remove registration from registration service (ERS).
     *
     * \param [in] type - what chennel type does notification id belong. \sa #rtd::TNChannelType.
     * \param [in] nId - notification id for certain channel.
     * \param [in] observer - an observer for receiving registration state changes. \sa #rtd::ITNRegistrationObserver.
     */
    virtual void Unregister(const TNChannelType& type, const TNNotificationID& nId, ITNRegistrationObserverPtr observer) = 0;
};
using ITNRegistrarPtr = std::shared_ptr<ITNRegistrar>;

}

#endif
