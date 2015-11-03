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
#include <Common/TLLoggerInitializer.h>
#include <Poco/Net/PrivateKeyPassphraseHandler.h>
#include <Poco/UUIDGenerator.h>
#include <twilio-jni/twilio-jni.h>
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


class LogListener: public ITLLogListener {
public :
	void onNewEntry(const char * c, const char* prefix) {
		__android_log_print(ANDROID_LOG_INFO, TAG, "%s, %s", c, prefix);
	}

};

static LogListener logger;


/*
 * Class:     com_twilio_ipmessaging_impl_TwilioIPMessagingSDKImpl
 * Method:    initNative
 * Signature: (Ljava/lang/String;Lcom/twilio/ipmessaging/impl/IPMessagingClientListenerInternal;)J
 */
JNIEXPORT jlong JNICALL Java_com_twilio_ipmessaging_impl_TwilioIPMessagingClientImpl_initNative
  (JNIEnv *env, jobject obj, jstring token, jobject listener) {

	LOGD( TAG, "Entered IPMessagingClientImpl_initNative()");

	rtd::ITDLogger::initialize(true);
	rtd::TLLogger::instance().SetLogListener(&logger);
	LOGD(TAG, "Set ITDLogger to true.");


	ITMClient::initialize();
	if (token == nullptr) {
		LOGE("token is null");
		return 0;
	}

	const char *tokenStr = env->GetStringUTFChars(token, 0);
	IPMessagingClientContext *clientParams_ = new IPMessagingClientContext();
	auto messagingListener = std::make_shared<TwilioIPMessagingClientListener>(env, obj, listener);

	LOGD(TAG,"Creating  config map");
	__android_log_print(ANDROID_LOG_INFO, TAG, "%s, %s", TEST_IPMESSAGING_SERVICE, TEST_REGISTRATION_SERVICE);
	configMap.insert(std::make_pair("RTDIPMessagingServiceAddr", TEST_IPMESSAGING_SERVICE));
	configMap.insert(std::make_pair("RTDRegistrationServiceAddr", TEST_REGISTRATION_SERVICE));
	configMap.insert(std::make_pair("RTDTwilsockServiceAddr", TEST_TWILSOCK_SERVICE));
	configMap.insert(std::make_pair("RTDDataServiceAddr", TEST_DATA_SERVICE));
	configMap.insert(std::make_pair("RTDSubscriptionServiceAddr", TEST_SUBSCRIPTIONS_SERVICE));

	LOGD(TAG,"Creating configurationProvider");
	auto configurationProvider = std::make_shared<TwilioIPMessagingConfigurationProvider>(configMap);
	if(configurationProvider == nullptr) {
		LOGD( TAG, "configurationProvider is NULL");
	}

	LOGD( TAG, "Creating notificationClientObserver");
	auto  notificationClientObserver = std::make_shared<TwilioIPMessagingNotificationClientListener>();
	if(notificationClientObserver == nullptr) {
		LOGD( TAG, "notificationClientObserver is NULL");
	}

	auto m_twilsock = rtd::TNTwilsockClientFactory::create(nullptr, tokenStr, configurationProvider);
	m_twilsock->connect();

	LOGD( TAG,"Creating ITNNotificationClientPtr");
	ITNNotificationClientPtr notificationClientPtr;
	notificationClientPtr = TNNotificationClientFactory::CreateNotificationClient(tokenStr,m_twilsock, configurationProvider);
	notificationClientPtr->Init(notificationClientObserver);

	if(notificationClientPtr == nullptr) {
		LOGD(TAG, "notificationClientPtr is NULL");
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

	LOGD( TAG,"Checking token validity.");

	if (token == nullptr) {
		LOG_E(TAG,"token is null");
		return 0;
	}

	const char *tokenStr = env->GetStringUTFChars(token, 0);
	LOGD(TAG,"Checking nativeClientParam.");

	if (nativeClientContext == 0) {
			LOG_W(TAG, "client context is null");
			return 0;
	} else {

		IPMessagingClientContext *clientParamsRecreate = reinterpret_cast<IPMessagingClientContext *>(nativeClientContext);
		LOGD(TAG,"client context is recreated.");

		if(clientParamsRecreate->messagingListener == nullptr) {
			LOG_W( TAG, "messagingListener is NULL.");
			return 0;
		}

		if(clientParamsRecreate->configurationProvider == nullptr) {
			LOG_W(TAG, "configurationProvider is NULL.");
			return 0;
		}

		if( clientParamsRecreate->notificationClient == nullptr) {
			LOG_W(TAG, "notificationClient is NULL.");
			return 0;
		}

		LOGD( TAG,"Creating the msgClient.");

		ITMClientPtr messagingClient = ITMClient::createClient(tokenStr, "Android",
				clientParamsRecreate->messagingListener,
				clientParamsRecreate->configurationProvider,
				clientParamsRecreate->notificationClient,
				([](TMResult result) { LOG_W( TAG,"Created the msgClient."); }));

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

	LOGW(TAG,"Java_com_twilio_ipmessaging_impl_TwilioIPMessagingClientImpl_getChannelsNative : Checking nativeClientContext.");
	if (nativeClientContext == 0) {
			LOGW(TAG,"client context is null");
			return 0;
	} else {

		IPMessagingClientContext *clientParamsRecreate = reinterpret_cast<IPMessagingClientContext *>(nativeClientContext);

		if(clientParamsRecreate != nullptr) {
			if(clientParamsRecreate->messagingClient != nullptr) {
				//get channels object//
				ITMChannelsPtr channelsLocal = clientParamsRecreate->messagingClient->getChannels();
				LOGW(TAG,"clientParamsRecreate->messagingClient is NOT null");
				while (channelsLocal == nullptr)
				{
					LOGW(TAG,"Messaging lib not ready, retrying...");
					Poco::Thread::sleep(1000);
					channelsLocal = clientParamsRecreate->messagingClient->getChannels();
				}

				ChannelsContext* channelsContext_ = new ChannelsContext();
				channelsContext_->channels = channelsLocal;
				jlong channelsContextHandle = reinterpret_cast<jlong>(channelsContext_);
				clientParamsRecreate->channels = channelsLocal;

				jclass java_channels_impl_cls = tw_jni_find_class(env, "com/twilio/ipmessaging/impl/ChannelsImpl");
				if(java_channels_impl_cls != nullptr) {
					LOGW(TAG, "Found java_channels_impl_cls class" );
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
  (JNIEnv *env, jobject obj, jstring token, jlong nativeClientContext) {

	if (nativeClientContext == 0) {
		LOGW(TAG,"client context is null");
		return 0;
	} else {
		const char *tokenStr = env->GetStringUTFChars(token, 0);
		IPMessagingClientContext *clientParamsRecreate = reinterpret_cast<IPMessagingClientContext *>(nativeClientContext);
		if(clientParamsRecreate != nullptr) {
			ITMClientPtr messagingClient = clientParamsRecreate->messagingClient;
			if(messagingClient != nullptr) {
				messagingClient->updateToken(tokenStr,[](TMResult result){
					if (result == rtd::TMResult::kTMResultSuccess) {
						__android_log_print(ANDROID_LOG_INFO, TAG, "updateToken is successful.");
					} else {
						__android_log_print(ANDROID_LOG_INFO, TAG, "updateToken failed.");
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
		LOGW(TAG,"client context is null");
		return 0;
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
