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
	__android_log_print(ANDROID_LOG_ERROR, TAG, "GCM RegistrationObserverImpl::onRegStateChanged");

	//__android_log_print(ANDROID_LOG_ERROR, TAG, "GCM onRegStateChanged state: %s ", state);
	__android_log_print(ANDROID_LOG_ERROR, TAG, "GCM onRegStateChanged notification-id : %s ", nId.c_str());
	//__android_log_print(ANDROID_LOG_ERROR, TAG, "GCM onRegStateChanged type : %s ", type);

	__android_log_print(ANDROID_LOG_ERROR, TAG, "GCM token status question" );
	if( state == rtd::TNRegistrationStates::Registered ){
		__android_log_print(ANDROID_LOG_ERROR, TAG, "GCM token registered");
	}
	__android_log_print(ANDROID_LOG_ERROR, TAG, "GCM token status question completed" );


	JNIEnvAttacher jniAttacher;
	jniAttacher.get()->CallVoidMethod(j_statusListener_, j_onSuccess_);
}



void RegistrationObserverImpl::onError(const TNErrorDescription& message, TNError code)
{
	__android_log_print(ANDROID_LOG_INFO, TAG, "RegistrationObserverImpl::onError");
	__android_log_print(ANDROID_LOG_ERROR, TAG, "GCM RegistrationObserverImpl::onError");

	JNIEnvAttacher jniAttacher;
	jniAttacher.get()->CallVoidMethod(j_statusListener_, j_onError_);

}


void RegistrationObserverImpl::onFatalError(const TNErrorDescription& message, TNError code)
{
	__android_log_print(ANDROID_LOG_INFO, TAG, "RegistrationObserverImpl::oFatalnError");
	__android_log_print(ANDROID_LOG_ERROR, TAG, "GCM RegistrationObserverImpl::onFatalError");

	JNIEnvAttacher jniAttacher;
	jniAttacher.get()->CallVoidMethod(j_statusListener_, j_onError_);

}



