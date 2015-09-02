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

#include <twilio-jni/twilio-jni.h>

#include "ITMClient.h"
#include "ITMChannels.h"
#include "ITMChannel.h"
#include "ITMessages.h"
#include "ITMessage.h"
#include "ITMMembers.h"
#include "ITMMember.h"
#include "TestRTDJNI.h"

#include "TwilioIPMessagingConfigurationProvider.h"
#include "TwilioIPMessagingNotificationClientListener.h"
#include "TwilioIPMessagingClientListener.h"

#define TAG  "RTD_TESTS"
#define PRODUCTION 1
#define WITH_SSL 1

using namespace rtd;

std::map<std::string, std::string> configMap;

#if PRODUCTION
#define TEST_IPMESSAGING_SERVICE "https://aim.twilio.com"
#define TEST_REGISTRATION_SERVICE "https://ers.twilio.com"
#define TEST_TWILSOCK_SERVICE "https://tsock.twilio.com"
#define TEST_DATA_SERVICE "https://cds.twilio.com"
#define TEST_SUBSCRIPTIONS_SERVICE "https://cds.twilio.com"
#else
#define TEST_IPMESSAGING_SERVICE "https://aim.dev-us1.twilio.com"
#define TEST_REGISTRATION_SERVICE "https://ers.dev-us1.twilio.com"
#define TEST_TWILSOCK_SERVICE "https://tsock.dev-us1.twilio.com"
#define TEST_DATA_SERVICE "https://cds.dev-us1.twilio.com"
#define TEST_SUBSCRIPTIONS_SERVICE "https://cds.dev-us1.twilio.com"
#endif



// structure with all the shared pointer
typedef struct ClientContext {
	std::shared_ptr<TwilioIPMessagingNotificationClientListener>  notificationClientObserver;
	std::shared_ptr<TwilioIPMessagingConfigurationProvider> configurationProvider;
	std::shared_ptr<TwilioIPMessagingClientListener> messagingListener;
	ITNNotificationClientPtr notificationClient;
} ClientContext;



jobject j_rtd_test_obj_;
ITNNotificationClientPtr notificationClient;
ClientContext *clientParams_;



void resultHandler(TMResult result) {

}

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__))

JNIEXPORT jlong JNICALL Java_com_twilio_example_TestRTDJNI_init(JNIEnv *env, jobject obj, jstring token) {

	LOGW( "Java_com_twilio_example_TestRTDJNI_init() : Initializing ssl.");

	//rtd::ITDLogger::initialize(true);

	LOGW( "Java_com_twilio_example_TestRTDJNI_init() : set ITDLogger to true.");

#if WITH_SSL
	Poco::Net::initializeSSL();
	LOGW( "Java_com_twilio_example_TestRTDJNI_init() : done initializeSSL()");
	auto context = new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "", "", "", Poco::Net::Context::VERIFY_NONE, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
	LOGW( "Java_com_twilio_example_TestRTDJNI_init() : done getting context");
	Poco::Net::SSLManager::instance().initializeClient(nullptr, nullptr, context);
	LOGW( "Java_com_twilio_example_TestRTDJNI_init() : done Poco::Net::SSLManager::instance()");
#endif

	LOGW( "Java_com_twilio_example_TestRTDJNI_init() : Checking if token is null");
	if (token == NULL) {
		__android_log_print(ANDROID_LOG_ERROR, TAG, "token is null");
		return 0;
	}

	const char *tokenStr = env->GetStringUTFChars(token, 0);

	LOGW( "Java_com_twilio_example_TestRTDJNI_init : Creating a Global Reference of top Java object");
	j_rtd_test_obj_ = env->NewGlobalRef(obj);

	clientParams_ = new ClientContext();

	LOGW( "Java_com_twilio_example_TestRTDJNI_init : Creating messagingListener");
	auto messagingListener = std::make_shared<TwilioIPMessagingClientListener>();

	LOGW( "Java_com_twilio_example_TestRTDJNI_init : Creating  config map");
	configMap.insert(std::make_pair("RTDIPMessagingServiceAddr", TEST_IPMESSAGING_SERVICE));
	configMap.insert(std::make_pair("RTDRegistrationServiceAddr", TEST_REGISTRATION_SERVICE));
	configMap.insert(std::make_pair("RTDTwilsockServiceAddr", TEST_TWILSOCK_SERVICE));
	configMap.insert(std::make_pair("RTDDataServiceAddr", TEST_DATA_SERVICE));
	configMap.insert(std::make_pair("RTDSubscriptionServiceAddr", TEST_SUBSCRIPTIONS_SERVICE));

	LOGW( "Java_com_twilio_example_TestRTDJNI_init : Creating configurationProvider");
	auto configurationProvider = std::make_shared<TwilioIPMessagingConfigurationProvider>(configMap);

	if(configurationProvider == NULL) {
		LOGW( "Java_com_twilio_example_TestRTDJNI_init : configurationProvider is NULL");
	} else {
		LOGW( "Java_com_twilio_example_TestRTDJNI_init : configurationProvider is NOT NULL");
	}

	LOGW( "Java_com_twilio_example_TestRTDJNI_init : Creating notificationClientObserver");
	auto  notificationClientObserver = std::make_shared<TwilioIPMessagingNotificationClientListener>();
	if(notificationClientObserver == NULL) {
		LOGW( "Java_com_twilio_example_TestRTDJNI_init : notificationClientObserver is NULL");
	} else {
		LOGW( "Java_com_twilio_example_TestRTDJNI_init : notificationClientObserver is NOT NULL");
	}


	LOGW( "Java_com_twilio_example_TestRTDJNI_init : Creating ITNNotificationClientPtr");
	ITNNotificationClientPtr notificationClientPtr;
	notificationClientPtr = TNNotificationClientFactory::CreateNotificationClient(tokenStr,configurationProvider);
	notificationClientPtr->Init(notificationClientObserver);

	if(notificationClientPtr == NULL) {
		LOGW( "Java_com_twilio_example_TestRTDJNI_init : notificationClientPtr is NULL");
	} else {
		LOGW( "Java_com_twilio_example_TestRTDJNI_init : notificationClientPtr is NOT NULL");
	}

	//sleep(1000);

	clientParams_->messagingListener = messagingListener;
	clientParams_->configurationProvider = configurationProvider;
	clientParams_->notificationClientObserver = notificationClientObserver;
	clientParams_->notificationClient = notificationClientPtr;

	LOGW( "Java_com_twilio_example_TestRTDJNI_init : Setting nativeClientParam");
	tw_jni_set_long(env, obj, "nativeClientParam", (jlong)clientParams_);

	return reinterpret_cast<jlong>(clientParams_);

}


JNIEXPORT void JNICALL Java_com_twilio_example_TestRTDJNI_shutDown(JNIEnv *env, jobject obj) {
	LOGW( "Deleteing the Global Ref.");
	if(j_rtd_test_obj_ != NULL) {
		env->DeleteGlobalRef(j_rtd_test_obj_);
		j_rtd_test_obj_ = NULL;
	}
}



JNIEXPORT jlong JNICALL Java_com_twilio_example_TestRTDJNI_createMessagingClient(JNIEnv *env, jobject obj, jstring token) {

	LOGW( "Java_com_twilio_example_TestRTDJNI_createMessagingClient : Checking token validity.");

	if (token == NULL) {
		__android_log_print(ANDROID_LOG_ERROR, TAG, "token is null");
		return 0;
	}
	const char *tokenStr = env->GetStringUTFChars(token, 0);

	jlong nativeClientContext = tw_jni_fetch_long(env, obj, "nativeClientParam");

	LOGW( "Java_com_twilio_example_TestRTDJNI_createMessagingClient : Checking nativeClientContext.");

	if (nativeClientContext == NULL) {
			__android_log_print(ANDROID_LOG_ERROR, TAG, "token is null");
			return 0;
	} else {

		if(clientParams_->messagingListener == NULL ) {
			LOGW( "Java_com_twilio_example_TestRTDJNI_createMessagingClient : messagingListener is NULL.");
			return 0;
		}

		if(clientParams_->configurationProvider == NULL) {
			LOGW( "Java_com_twilio_example_TestRTDJNI_createMessagingClient : configurationProvider is NULL.");
			return 0;
		}

		if( clientParams_->notificationClient == NULL) {
			LOGW( "Java_com_twilio_example_TestRTDJNI_createMessagingClient : notificationClient is NULL.");
			return 0;
		}

		LOGW( "Java_com_twilio_example_TestRTDJNI_createMessagingClient : Creating the msgClient.");

		ITMClientPtr messagingClient = ITMClient::createClient(tokenStr,
														   clientParams_->messagingListener,
														   clientParams_->configurationProvider,
														   clientParams_->notificationClient,
														   NULL);
			                                               //([](TMResult result) { LOGW( "Java_com_twilio_example_TestRTDJNI_getMessagingClient : Client init.");}));


		//get channels object//////////////////////////////////////
		ITMChannelsPtr channels = messagingClient->getChannels();
		while (channels == nullptr)
		{
			LOGW("app: messaging lib not ready, retrying...");
			Poco::Thread::sleep(1000);
			channels = messagingClient->getChannels();
		}


	}

	return 0;//reinterpret_cast<jlong>(messagingClient.get());
}



