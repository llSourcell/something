//
//  TwilioIPMessagingClientListener.cpp
//  Twilio IP Messaging Client
//
//  Copyright © 2015 Twilio. All rights reserved.
//

#include <jni.h>
#include <twilio-jni/twilio-jni.h>
#include "RegistrationObserverImpl.h"
#include "TwilioIPMessagingClientContextDefines.h"
#define TAG  "RegistrationObserverImpl(native)"

using namespace rtd;



RegistrationObserverImpl::RegistrationObserverImpl(JNIEnv* env,jobject j_ipmessagingclient, jobject j_statusListener ) {
	j_statusListener_ = env->NewGlobalRef(j_statusListener);
	j_onSuccess_ = tw_jni_get_method(env, j_statusListener_, "onSuccess", "()V");
	j_onError_ = tw_jni_get_method(env, j_statusListener_, "onError", "()V");
	env_ = env;
}

RegistrationObserverImpl::~RegistrationObserverImpl()
{
	if (j_statusListener_ != NULL) {
		env_->DeleteGlobalRef(j_statusListener_);
		j_statusListener_ = NULL;
	}
}


void RegistrationObserverImpl::onRegStateChanged(const TNChannelType& type, const TNNotificationID& nId, TNRegistrationStates state)
{
	__android_log_print(ANDROID_LOG_INFO, TAG, "RegistrationObserverImpl::onRegStateChanged");

	JNIEnvAttacher jniAttacher;
	jniAttacher.get()->CallVoidMethod(j_statusListener_, j_onSuccess_);
}



void RegistrationObserverImpl::onError(const TNErrorDescription& message, TNError code)
{
	__android_log_print(ANDROID_LOG_INFO, TAG, "RegistrationObserverImpl::onError");

	JNIEnvAttacher jniAttacher;
	jniAttacher.get()->CallVoidMethod(j_statusListener_, j_onError_);

}


void RegistrationObserverImpl::onFatalError(const TNErrorDescription& message, TNError code)
{
	__android_log_print(ANDROID_LOG_INFO, TAG, "RegistrationObserverImpl::oFatalnError");

	JNIEnvAttacher jniAttacher;
	jniAttacher.get()->CallVoidMethod(j_statusListener_, j_onError_);

}



