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

#define WITH_SSL 0


jobject j_rtd_test_obj_;

void resultHandler(TMResult result) {

}

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__))

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


JNIEXPORT void JNICALL Java_com_twilio_example_TestRTDJNI_init(JNIEnv *env, jobject obj) {
	LOGW( "Creating a Global Reference of top Java object");
	j_rtd_test_obj_ = env->NewGlobalRef(obj);
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

	//jlong t = reinterpret_cast<jlong>(configurationProvider.get());

	//__android_log_print(ANDROID_LOG_WARN, "LOG_TAG", " printing the reference of configurationProvider : %d",t);

	return reinterpret_cast<jlong>(configurationProvider.get()) ;
}


JNIEXPORT jlong JNICALL Java_com_twilio_example_TestRTDJNI_getNotificationClientObserver(JNIEnv *env, jobject obj) {

	LOGW( "Java_com_twilio_example_TestRTDJNI_getNotificationClientObserver : Creating notificationClientObserver");

	LOGW( "Java_com_twilio_example_TestRTDJNI_getNotificationClient : notificationClientObserver 2");
	auto  notificationClientObserver = std::make_shared<TwilioIPMessagingNotificationClientListener>();

	jlong t = reinterpret_cast<jlong>(notificationClientObserver.get());

	__android_log_print(ANDROID_LOG_WARN, "LOG_TAG", "Need to print : %d",t);

	return t ;
}


JNIEXPORT jlong JNICALL Java_com_twilio_example_TestRTDJNI_getNotificationClient(JNIEnv *env, jobject obj, jstring token, jlong nativeCfgProvider, jlong nativeNotificationClientObserver) {


	if (token == NULL) {
		LOGW( "Java_com_twilio_example_TestRTDJNI_getNotificationClient : Creating notificationClient, token is null ");
		return 0;
	}
	const char *tokenStr = env->GetStringUTFChars(token, 0);


	LOGW( "Java_com_twilio_example_TestRTDJNI_getNotificationClient : Creating notificationClient ");

	__android_log_print(ANDROID_LOG_WARN, "LOG_TAG", "Printing Native Configuration provider reference : %d",nativeCfgProvider);

	LOGW( "Java_com_twilio_example_TestRTDJNI_getNotificationClient : Creating nativeCfgProvider from the native reference");

	TwilioIPMessagingConfigurationProvider* t = (TwilioIPMessagingConfigurationProvider*) nativeCfgProvider;
	std::shared_ptr<TwilioIPMessagingConfigurationProvider> configurationProvider(t);

	LOGW( "Java_com_twilio_example_TestRTDJNI_getNotificationClient : Creating TwilioIPMessagingNotificationClientListener from the native reference");

	//The following block get compilation error - No idea why ..
	TwilioIPMessagingNotificationClientListener* l = (TwilioIPMessagingNotificationClientListener*) nativeNotificationClientObserver;
	std::shared_ptr<TwilioIPMessagingNotificationClientListener> notificationClientObserver(l);

	LOGW( "Java_com_twilio_example_TestRTDJNI_getNotificationClient : notificationClientObserver 1");
	std::shared_ptr<TwilioIPMessagingNotificationClientListener>  notificationClientObs = std::make_shared<TwilioIPMessagingNotificationClientListener>();

	ITNNotificationClientPtr notificationClient;
	notificationClient = TNNotificationClientFactory::CreateNotificationClient(tokenStr,configurationProvider);
	notificationClient->Init(notificationClientObs);

	LOGW( "Java_com_twilio_example_TestRTDJNI_getNotificationClient : notificationClientObserver 3");

	//jlong ptr = reinterpret_cast<jlong>(notificationClient.get());

	//LOGW( "Java_com_twilio_example_TestRTDJNI_getNotificationClient : notificationClientObserver 4");

	sleep(999);

	return reinterpret_cast<jlong>(&notificationClient);
}



JNIEXPORT jlong JNICALL Java_com_twilio_example_TestRTDJNI_getMessagingClient(JNIEnv *env, jobject obj, jstring token, jlong nativeMsgClientListener, jlong nativeConfigProvider, jlong nativeNotificationClient) {


	if (token == NULL) {
		__android_log_print(ANDROID_LOG_ERROR, TAG, "token is null");
		return 0;
	}
	const char *tokenStr = env->GetStringUTFChars(token, 0);

	TwilioIPMessagingClientListener* mc = (TwilioIPMessagingClientListener*)nativeMsgClientListener;
	std::shared_ptr<TwilioIPMessagingClientListener> messagingListener(mc);

	TwilioIPMessagingConfigurationProvider* cp = (TwilioIPMessagingConfigurationProvider*) nativeConfigProvider;
	std::shared_ptr<TwilioIPMessagingConfigurationProvider> configurationProvider(cp);

	//Commenting out because it does not work :(

	ITNNotificationClientPtr& notificationClient = (ITNNotificationClientPtr&)nativeNotificationClient;
	//std::shared_ptr<ITNNotificationClientPtr> notificationClient(nc);

	///ITNNotificationClientPtr nc = ITNNotificationClientPtr(reinterpret_cast<ITNNotificationClientPtr>(nativeNotificationClient));


	ITMClientPtr messagingClient = ITMClient::createClient(tokenStr,
			                                                  messagingListener,
			                                                  configurationProvider,
															  notificationClient,
			                                                  ([](TMResult result) { }));

	return reinterpret_cast<jlong>(messagingClient.get());
}


