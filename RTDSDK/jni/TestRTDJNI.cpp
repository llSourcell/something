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

// Implementation of native method testRTD() of TestRTDJNI class


JNIEXPORT void JNICALL Java_com_twilio_example_TestRTDJNI_testRTD(JNIEnv *env, jobject thisObj, jstring token) {

	if (token == NULL) {
			__android_log_print(ANDROID_LOG_ERROR, TAG, "token is null");
			return;
	}

	const char *capabilityToken = env->GetStringUTFChars(token, 0);

	LOGW("Java_com_twilio_example_TestRTDJNI_testRTD 1");

	std::array<const char*, sizeof("TMessaging")> filters = { "TMessaging" };

	LOGW("Java_com_twilio_example_TestRTDJNI_testRTD 2");

	//rtd::ITDLogger::initialize(true, filters.data(), filters.size());

	LOGW( "Java_com_twilio_example_TestRTDJNI_testRTD 3");

	std::string userName = "kumkum";


#if WITH_SSL
	Poco::Net::initializeSSL();
	auto context = new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "", "", "", Poco::Net::Context::VERIFY_NONE, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
	Poco::Net::SSLManager::instance().initializeClient(nullptr, nullptr, context);
#endif

	LOGW( "Java_com_twilio_example_TestRTDJNI_testRTD 4");
	configMap.insert(std::make_pair("RTDIPMessagingServiceAddr", TEST_IPMESSAGING_SERVICE));
	configMap.insert(std::make_pair("RTDRegistrationServiceAddr", TEST_REGISTRATION_SERVICE));
	configMap.insert(std::make_pair("RTDTwilsockServiceAddr", TEST_TWILSOCK_SERVICE));
	configMap.insert(std::make_pair("RTDDataServiceAddr", TEST_DATA_SERVICE));
	configMap.insert(std::make_pair("RTDSubscriptionServiceAddr", TEST_SUBSCRIPTIONS_SERVICE));

	LOGW( "Java_com_twilio_example_TestRTDJNI_testRTD 5: capability Token");
	auto configurationProvider = std::make_shared<TwilioIPMessagingConfigurationProvider>(configMap);
	LOGW( "Java_com_twilio_example_TestRTDJNI_testRTD 6");

	LOGW( "Java_com_twilio_example_TestRTDJNI_testRTD 8: notificationClientObserver");
	std::shared_ptr<TwilioIPMessagingNotificationClientListener>  notificationClientObserver = std::make_shared<TwilioIPMessagingNotificationClientListener>();


	LOGW( "Java_com_twilio_example_TestRTDJNI_testRTD 7 : ITNNotificationClientPtr");
	ITNNotificationClientPtr notificationClient;
	notificationClient = TNNotificationClientFactory::CreateNotificationClient(capabilityToken,configurationProvider);
	notificationClient->Init(notificationClientObserver);

	LOGW( "Java_com_twilio_example_TestRTDJNI_testRTD 9: TwilioIPMessagingClientListener");
	std::shared_ptr<TwilioIPMessagingClientListener> messagingListener;
	messagingListener = std::make_shared<TwilioIPMessagingClientListener>();

	//auto messagingListener = std::make_shared<MessagingListener>();

	LOGW( "Java_com_twilio_example_TestRTDJNI_testRTD 10 : TwilioIPMessagingClientListener");
	ITMClientPtr messagingClient = ITMClient::createClient(capabilityToken,
		                                                   messagingListener,
		                                                   configurationProvider,
		                                                   notificationClient,
		                                                   ([](TMResult result) { }));
		                                                  //[](TMResult result){ LOGW( "INIT : Java_com_twilio_example_TestRTDJNI_testRTD 10 : TwilioIPMessagingClientListener");});

	LOGW( "Java_com_twilio_example_TestRTDJNI_testRTD 11 : TwilioIPMessagingClientListener");

	sleep(999);
   return;

}


JNIEXPORT jlong JNICALL Java_com_twilio_example_TestRTDJNI_init(JNIEnv *env, jobject obj, jstring token) {

	LOGW( "Java_com_twilio_example_TestRTDJNI_init() : Initializing ssl");

#if WITH_SSL
	Poco::Net::initializeSSL();
	auto context = new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "", "", "", Poco::Net::Context::VERIFY_NONE, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
	Poco::Net::SSLManager::instance().initializeClient(nullptr, nullptr, context);
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


JNIEXPORT jlong JNICALL Java_com_twilio_example_TestRTDJNI_getMessagingListener(JNIEnv *env, jobject obj) {

	LOGW( "Creating Java_com_twilio_example_TestRTDJNI_getMessagingListener: messagingListener");

	auto messagingListener = std::make_shared<TwilioIPMessagingClientListener>();
	return reinterpret_cast<jlong>(messagingListener.get());
}



JNIEXPORT jlong JNICALL Java_com_twilio_example_TestRTDJNI_getConfigurationProvider(JNIEnv *env, jobject obj) {

	LOGW( "Java_com_twilio_example_TestRTDJNI_getConfigurationProvider : Creating  configurationProvider");
	configMap.insert(std::make_pair("RTDIPMessagingServiceAddr", TEST_IPMESSAGING_SERVICE));
	configMap.insert(std::make_pair("RTDRegistrationServiceAddr", TEST_REGISTRATION_SERVICE));
	configMap.insert(std::make_pair("RTDTwilsockServiceAddr", TEST_TWILSOCK_SERVICE));
	configMap.insert(std::make_pair("RTDDataServiceAddr", TEST_DATA_SERVICE));
	configMap.insert(std::make_pair("RTDSubscriptionServiceAddr", TEST_SUBSCRIPTIONS_SERVICE));

	LOGW( "Java_com_twilio_example_TestRTDJNI_getConfigurationProvider 5: capability Token");
	auto configurationProvider = std::make_shared<TwilioIPMessagingConfigurationProvider>(configMap);
	LOGW( "Java_com_twilio_example_TestRTDJNI_getConfigurationProvider 6");

	jlong t = reinterpret_cast<jlong>(configurationProvider.get());

	__android_log_print(ANDROID_LOG_WARN, "LOG_TAG", "Printing Native Configuration Provider reference : %d",(int)t);

	return reinterpret_cast<jlong>(configurationProvider.get()) ;
}


JNIEXPORT jlong JNICALL Java_com_twilio_example_TestRTDJNI_getNotificationClientObserver(JNIEnv *env, jobject obj) {

	LOGW( "Java_com_twilio_example_TestRTDJNI_getNotificationClientObserver : Creating notificationClientObserver");

	LOGW( "Java_com_twilio_example_TestRTDJNI_getNotificationClient : notificationClientObserver 2");
	auto  notificationClientObserver = std::make_shared<TwilioIPMessagingNotificationClientListener>();

	jlong t = reinterpret_cast<jlong>(notificationClientObserver.get());

	__android_log_print(ANDROID_LOG_WARN, "LOG_TAG", "Printing Native Notification Client Observer reference : %d",(int)t);

	return t ;
}


JNIEXPORT jlong JNICALL Java_com_twilio_example_TestRTDJNI_getNotificationClient(JNIEnv *env, jobject obj, jstring token, jlong nativeCfgProvider, jlong nativeNotificationClientObserver, jlong nativeMsgClientListener) {


	if (token == NULL) {
		LOGW( "Java_com_twilio_example_TestRTDJNI_getNotificationClient : Creating notificationClient, token is null ");
		return 0;
	}
	const char *tokenStr = env->GetStringUTFChars(token, 0);


	LOGW( "Java_com_twilio_example_TestRTDJNI_getNotificationClient : Creating notificationClient ");

	__android_log_print(ANDROID_LOG_WARN, "LOG_TAG", "Printing Native Configuration provider reference : %d",(int)nativeCfgProvider);

	__android_log_print(ANDROID_LOG_WARN, "LOG_TAG", "Printing Native Notification Client Observer reference : %d",(int)nativeNotificationClientObserver);

	LOGW( "Java_com_twilio_example_TestRTDJNI_getNotificationClient : Creating nativeCfgProvider from the native reference");

	TwilioIPMessagingConfigurationProvider* t = (TwilioIPMessagingConfigurationProvider*) nativeCfgProvider;
	std::shared_ptr<TwilioIPMessagingConfigurationProvider> configurationProvider(t);

	LOGW( "Java_com_twilio_example_TestRTDJNI_getNotificationClient : Creating TwilioIPMessagingNotificationClientListener from the native reference");

	TwilioIPMessagingNotificationClientListener* l = (TwilioIPMessagingNotificationClientListener*) nativeNotificationClientObserver;
	std::shared_ptr<TwilioIPMessagingNotificationClientListener> notificationClientObserver(l);

	LOGW( "Java_com_twilio_example_TestRTDJNI_getNotificationClient : Creating ITNNotificationClientPtr from the native reference");

	//ITNNotificationClientPtr notificationClient;
    notificationClient = TNNotificationClientFactory::CreateNotificationClient(tokenStr,configurationProvider);
	notificationClient->Init(notificationClientObserver);

	LOGW( "Java_com_twilio_example_TestRTDJNI_getNotificationClient : Created ITNNotificationClientPtr from the native reference");


	return reinterpret_cast<jlong>(&notificationClient);//(jlong)(notificationClient.get());reinterpret_cast<jlong>(messagingClient.get());//
}



JNIEXPORT jlong JNICALL Java_com_twilio_example_TestRTDJNI_getMessagingClient(JNIEnv *env, jobject obj, jstring token, jlong nativeMsgClientListener, jlong nativeConfigProvider, jlong nativeNotificationClient) {

	LOGW( "Java_com_twilio_example_TestRTDJNI_getMessagingClient : Creating the msgClient.");


	if (token == NULL) {
		__android_log_print(ANDROID_LOG_ERROR, TAG, "token is null");
		return 0;
	}
	const char *tokenStr = env->GetStringUTFChars(token, 0);

	LOGW( "Java_com_twilio_example_TestRTDJNI_getMessagingClient : Creating the TwilioIPMessagingClientListener.");

	TwilioIPMessagingClientListener* mc = (TwilioIPMessagingClientListener*)nativeMsgClientListener;
	std::shared_ptr<TwilioIPMessagingClientListener> messagingListener(mc);

	LOGW( "Java_com_twilio_example_TestRTDJNI_getMessagingClient : Creating the TwilioIPMessagingConfigurationProvider.");

	TwilioIPMessagingConfigurationProvider* cp = (TwilioIPMessagingConfigurationProvider*) nativeConfigProvider;
	std::shared_ptr<TwilioIPMessagingConfigurationProvider> configurationProvider(cp);

	LOGW( "Java_com_twilio_example_TestRTDJNI_getMessagingClient : Creating the ITNNotificationClientPtr.");

	ITNNotificationClientPtr& notificationClient = (ITNNotificationClientPtr&)nativeNotificationClient;
	ITNNotificationClientPtr notificationClientPtr =  notificationClient;
	//std::shared_ptr<ITNNotificationClientPtr> notificationClient(nc);


	LOGW( "Java_com_twilio_example_TestRTDJNI_getMessagingClient : Creating the ITMClient::createClient.");

	ITMClientPtr messagingClient = ITMClient::createClient(tokenStr,
			                                               messagingListener,
			                                               configurationProvider,
														   notificationClientPtr,
			                                               ([](TMResult result) { LOGW( "Java_com_twilio_example_TestRTDJNI_getMessagingClient : Client init.");}));

	return reinterpret_cast<jlong>(messagingClient.get());
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
			                                               ([](TMResult result) { LOGW( "Java_com_twilio_example_TestRTDJNI_getMessagingClient : Client init.");}));


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



