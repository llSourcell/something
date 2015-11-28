#include <jni.h>
#include <iostream>
#include <string.h>
#include <map>
#include <utility>
#include <android/log.h>
#include <array>
#include <Notification/ITNNotificationClient.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/Net/Context.h>
#include <Poco/Net/PrivateKeyPassphraseHandler.h>
#include <Poco/UUIDGenerator.h>
#include <twilio-jni/JNIEnvAttacher.h>
#include <twilio-jni/tw-jni.h>
#include <twilio-jni/tw-jni-compat.h>

#include <Twilsock/ITNTwilsockClient.h>

#include "ITMClient.h"
#include "ITMChannels.h"
#include "ITMChannel.h"
#include "ITMessages.h"
#include "ITMessage.h"
#include "ITMMembers.h"
#include "ITMMember.h"
#include "IPMessagingClientImpl.h"
#include "TwilioIPMessagingClientContextDefines.h"
#include "TwilioIPMessagingLogger.h"


#define TAG  "IPMessagingClient(native)"
#define PRODUCTION 1
#define STAGE 0
#define DEV 0
#define WITH_SSL 1

using namespace rtd;

std::map<std::string, std::string> configMap;

#if PRODUCTION
#define TEST_IPMESSAGING_SERVICE "https://aim.twilio.com"
#define TEST_REGISTRATION_SERVICE "https://ers.twilio.com"
#define TEST_TWILSOCK_SERVICE "https://tsock.twilio.com"
#define TEST_DATA_SERVICE "https://cds.twilio.com"
#define TEST_SUBSCRIPTIONS_SERVICE "https://cds.twilio.com"
#elif STAGE
#define TEST_IPMESSAGING_SERVICE "https://aim.stage-us1.twilio.com"
#define TEST_REGISTRATION_SERVICE "https://ers.stage-us1.twilio.com"
#define TEST_TWILSOCK_SERVICE "https://tsock.stage-us1.twilio.com"
#define TEST_DATA_SERVICE "https://cds.stage-us1.twilio.com"
#define TEST_SUBSCRIPTIONS_SERVICE "https://cds.stage-us1.twilio.com"
#elif DEV
#define TEST_IPMESSAGING_SERVICE "https://aim.dev-us1.twilio.com"
#define TEST_REGISTRATION_SERVICE "https://ers.dev-us1.twilio.com"
#define TEST_TWILSOCK_SERVICE "https://tsock.dev-us1.twilio.com"
#define TEST_DATA_SERVICE "https://cds.dev-us1.twilio.com"
#define TEST_SUBSCRIPTIONS_SERVICE "https://cds.dev-us1.twilio.com"
#endif


/*
 * Class:     com_twilio_ipmessaging_impl_TwilioIPMessagingSDKImpl
 * Method:    initNative
 * Signature: (Ljava/lang/String;Lcom/twilio/ipmessaging/impl/IPMessagingClientListenerInternal;)J
 */
JNIEXPORT jlong JNICALL Java_com_twilio_ipmessaging_impl_TwilioIPMessagingClientImpl_initNative
  (JNIEnv *env, jobject obj, jstring token, jobject listener) {

	LOG_DEBUG( TAG, "Entered IPMessagingClientImpl_initNative()");

	ITMClient::initialize();
	if (token == nullptr) {
		LOG_ERROR(TAG, "token is null");
		return 0;
	}

	const char *tokenStr = env->GetStringUTFChars(token, 0);
	IPMessagingClientContext *clientParams_ = new IPMessagingClientContext();
	auto messagingListener = std::make_shared<TwilioIPMessagingClientListener>(env, obj, listener);

	LOG_DEBUG(TAG,"Creating  config map");
	LOG_DEBUG(TAG, "%s, %s", TEST_IPMESSAGING_SERVICE, TEST_REGISTRATION_SERVICE);
	configMap.insert(std::make_pair("RTDIPMessagingServiceAddr", TEST_IPMESSAGING_SERVICE));
	configMap.insert(std::make_pair("RTDRegistrationServiceAddr", TEST_REGISTRATION_SERVICE));
	configMap.insert(std::make_pair("RTDTwilsockServiceAddr", TEST_TWILSOCK_SERVICE));
	configMap.insert(std::make_pair("RTDDataServiceAddr", TEST_DATA_SERVICE));
	configMap.insert(std::make_pair("RTDSubscriptionServiceAddr", TEST_SUBSCRIPTIONS_SERVICE));

	LOG_DEBUG(TAG,"Creating configurationProvider");
	auto configurationProvider = std::make_shared<TwilioIPMessagingConfigurationProvider>(configMap);
	if(configurationProvider == nullptr) {
		LOG_DEBUG( TAG, "configurationProvider is NULL");
	}

	LOG_DEBUG( TAG, "Creating notificationClientObserver");
	auto  notificationClientObserver = std::make_shared<TwilioIPMessagingNotificationClientListener>();
	if(notificationClientObserver == nullptr) {
		LOG_DEBUG( TAG, "notificationClientObserver is NULL");
	}

	auto m_twilsock = rtd::TNTwilsockClientFactory::create(nullptr, tokenStr, configurationProvider);
	m_twilsock->connect();

	LOG_DEBUG( TAG,"Creating ITNNotificationClientPtr");
	ITNNotificationClientPtr notificationClientPtr;
	notificationClientPtr = TNNotificationClientFactory::CreateNotificationClient(tokenStr,m_twilsock, configurationProvider);
	notificationClientPtr->Init(notificationClientObserver);

	if(notificationClientPtr == nullptr) {
		LOG_DEBUG(TAG, "notificationClientPtr is NULL");
	}

	clientParams_->messagingListener = messagingListener;
	clientParams_->configurationProvider = configurationProvider;
	clientParams_->notificationClientObserver = notificationClientObserver;
	clientParams_->notificationClient = notificationClientPtr;
	clientParams_->twilsock = m_twilsock;

	return reinterpret_cast<jlong>(clientParams_);

}


JNIEXPORT jlong JNICALL Java_com_twilio_ipmessaging_impl_TwilioIPMessagingClientImpl_createMessagingClient
  (JNIEnv *env, jobject obj, jstring token, jlong nativeClientContext) {

	LOG_DEBUG( TAG,"Checking token validity.");

	if (token == nullptr) {
		LOG_ERROR(TAG,"token is null");
		return 0;
	}

	const char *tokenStr = env->GetStringUTFChars(token, 0);
	LOG_DEBUG(TAG,"Checking nativeClientParam.");

	if (nativeClientContext == 0) {
			LOG_WARN(TAG, "client context is null");
			return 0;
	} else {

		IPMessagingClientContext *clientParamsRecreate = reinterpret_cast<IPMessagingClientContext *>(nativeClientContext);
		LOG_DEBUG(TAG,"client context is recreated.");

		if(clientParamsRecreate->messagingListener == nullptr) {
			LOG_WARN( TAG, "messagingListener is NULL.");
			return 0;
		}

		if(clientParamsRecreate->configurationProvider == nullptr) {
			LOG_WARN(TAG, "configurationProvider is NULL.");
			return 0;
		}

		if( clientParamsRecreate->notificationClient == nullptr) {
			LOG_WARN(TAG, "notificationClient is NULL.");
			return 0;
		}

		LOG_DEBUG( TAG,"Creating the msgClient.");

		ITMClientPtr messagingClient = ITMClient::createClient(tokenStr, "Android",
				clientParamsRecreate->messagingListener,
				clientParamsRecreate->configurationProvider,
				clientParamsRecreate->notificationClient,
				([](TMResult result) { LOG_WARN( TAG,"Created the msgClient."); }));

		clientParamsRecreate->messagingClient = messagingClient;

	}

	return 0;
}



/*
 * Class:     com_twilio_ipmessaging_impl_TwilioIPMessagingClientImpl
 * Method:    getChannelsNative
 * Signature: ()Lcom/twilio/ipmessaging/Channels;
 */
JNIEXPORT jobject JNICALL Java_com_twilio_ipmessaging_impl_TwilioIPMessagingClientImpl_getChannelsNative
  (JNIEnv *env, jobject obj, jlong nativeClientContext) {
	jobject channels = nullptr;

	LOG_WARN(TAG,"Java_com_twilio_ipmessaging_impl_TwilioIPMessagingClientImpl_getChannelsNative : Checking nativeClientContext.");
	if (nativeClientContext == 0) {
			LOG_WARN(TAG,"client context is null");
			return 0;
	} else {

		IPMessagingClientContext *clientParamsRecreate = reinterpret_cast<IPMessagingClientContext *>(nativeClientContext);

		if(clientParamsRecreate != nullptr) {
			if(clientParamsRecreate->messagingClient != nullptr) {
				//get channels object//
				ITMChannelsPtr channelsLocal = clientParamsRecreate->messagingClient->getChannels();
				LOG_WARN(TAG,"clientParamsRecreate->messagingClient is NOT null");
				while (channelsLocal == nullptr)
				{
					LOG_WARN(TAG,"Messaging lib not ready, retrying...");
					Poco::Thread::sleep(1000);
					channelsLocal = clientParamsRecreate->messagingClient->getChannels();
				}

				ChannelsContext* channelsContext_ = new ChannelsContext();
				channelsContext_->channels = channelsLocal;
				jlong channelsContextHandle = reinterpret_cast<jlong>(channelsContext_);
				clientParamsRecreate->channels = channelsLocal;

				jclass java_channels_impl_cls = tw_jni_find_class(env, "com/twilio/ipmessaging/impl/ChannelsImpl");
				if(java_channels_impl_cls != nullptr) {
					LOG_WARN(TAG, "Found java_channels_impl_cls class" );
				}

				jmethodID construct = tw_jni_get_method_by_class(env, java_channels_impl_cls, "<init>", "(Lcom/twilio/ipmessaging/impl/TwilioIPMessagingClientImpl;J)V");
				channels = tw_jni_new_object(env, java_channels_impl_cls, construct, obj, channelsContextHandle);
			}
		}
	}
	return channels;
}


/*
 * Class:     com_twilio_ipmessaging_impl_TwilioIPMessagingClientImpl
 * Method:    updateToken
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_TwilioIPMessagingClientImpl_updateToken
  (JNIEnv *env, jobject obj, jstring token, jlong nativeClientContext, jobject listener) {

	if (nativeClientContext == 0) {
		LOG_WARN(TAG,"client context is null");
	} else {
		const char *tokenStr = env->GetStringUTFChars(token, 0);
		IPMessagingClientContext *clientParamsRecreate = reinterpret_cast<IPMessagingClientContext *>(nativeClientContext);
		if(clientParamsRecreate != nullptr) {
			jobject j_statusListener_ = env->NewGlobalRef(listener);
			jclass cls = (env)->GetObjectClass(j_statusListener_);
			jmethodID j_onSuccess_ = (env)->GetMethodID(cls, "onSuccess", "()V");
			jmethodID j_onError_ = (env)->GetMethodID(cls, "onError", "()V");

			ITMClientPtr messagingClient = clientParamsRecreate->messagingClient;
			if(messagingClient != nullptr) {
				messagingClient->updateToken(tokenStr,[j_statusListener_,j_onSuccess_, j_onError_](TMResult result){
					JNIEnvAttacher jniAttacher;
					if (result == rtd::TMResult::kTMResultSuccess) {
						LOG_DEBUG(TAG, "updateToken is successful. Calling java listener.");
						//Call Java
						jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onSuccess_);
						jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
					} else {
						LOG_WARN(TAG, "updateToken channel failed. Calling java listener.");
						//Call Java
						jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onError_);
						jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
					}
				});
			}
		}

	}
}


/*
 * Class:     com_twilio_ipmessaging_impl_TwilioIPMessagingClientImpl
 * Method:    shutDownNative
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_TwilioIPMessagingClientImpl_shutDownNative
  (JNIEnv *env, jobject obj, jlong nativeClientContext) {
	if (nativeClientContext == 0) {
		LOG_WARN(TAG,"client context is null");
	} else {
		IPMessagingClientContext *clientParamsRecreate = reinterpret_cast<IPMessagingClientContext *>(nativeClientContext);
		if(clientParamsRecreate != nullptr) {
			ITMClientPtr messagingClient = clientParamsRecreate->messagingClient;
			if(messagingClient != nullptr) {
				messagingClient->shutdown();
			}

			ITNNotificationClientPtr notificationClient = clientParamsRecreate->notificationClient;
			if(notificationClient != nullptr) {
				notificationClient->Shutdown();
			}

			std::shared_ptr<TwilioIPMessagingNotificationClientListener> notificationClientObserver = clientParamsRecreate->notificationClientObserver;
			if (notificationClientObserver != nullptr) {
				notificationClientObserver.reset();
				notificationClientObserver = nullptr;
			}
			if (notificationClientObserver != nullptr) {
				notificationClientObserver->waitShutdown();
			}

			ITNTwilsockClientPtr twilsock = clientParamsRecreate->twilsock;
			if (twilsock != nullptr) {
				twilsock->stop();
			}

			if (messagingClient != nullptr) {
				messagingClient.reset();
				messagingClient = nullptr;
			}

			if (notificationClient != nullptr) {
				notificationClient.reset();
				notificationClient = nullptr;
			}

			std::shared_ptr<TwilioIPMessagingClientListener> messagingListener = clientParamsRecreate->messagingListener;
			if (messagingListener != nullptr) {
				messagingListener.reset();
				messagingListener = nullptr;
			}
		}
	}

}
