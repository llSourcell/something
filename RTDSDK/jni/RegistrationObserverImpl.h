//
//  RegistrationObserverImpl.h
//
//
//  Copyright © 2015 Twilio. All rights reserved.
//

#ifndef RegistrationObserverImpl_cpp
#define RegistrationObserverImpl_cpp

#include <Poco/Foundation.h>
#include <android/log.h>

#include "ITMClient.h"
#include "ITMChannel.h"
#include "ITMMember.h"
#include "ITMessages.h"
#include "ITMessage.h"

using namespace rtd;


class RegistrationObserverImpl : public ITNRegistrationObserver
{
public:
	RegistrationObserverImpl(JNIEnv* env,jobject obj,jobject j_listener);
	virtual ~RegistrationObserverImpl();
    void onRegStateChanged(const TNChannelType& type, const TNNotificationID& nId, TNRegistrationStates state);
    void onError(const TNErrorDescription& message, TNError code);
    void onFatalError(const TNErrorDescription& message, TNError code);

private:
    jobject j_statusListener_;
	jmethodID j_onSuccess_;
	jmethodID j_onError_;
	JNIEnv* env_;
};
#endif /* RegistrationObserverImpl_cpp */
