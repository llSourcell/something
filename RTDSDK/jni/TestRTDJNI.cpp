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

//using namespace rtd;

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

void resultHandler(TMResult result) {

}

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__))

std::string capabilityToken = "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJzY29wZSI6InNjb3BlOmNsaWVudDpvdXRnb2luZz9hcHBTaWQ9QVA3MDBkNzNmZTE1Y2FkYzM1NzFlOGE2MmU0ZDgxYzg1NyZhcHBQYXJhbXM9ZW5kcG9pbnRfaWQlM0RDNzYwRjM1Mi1EQTE1LTQwN0UtODEzNi1DQTY5MTJFOTlENDAlMjZjcmVkZW50aWFsX3NpZCUzRENSNDJjZDcyMjViYjIxZTkzYTE1NThjYTZjMGVhMDI4Y2QlMjZpZGVudGl0eSUzRGt1bWt1bSUyNnNlcnZpY2Vfc2lkJTNESVM2YjBmYTYwODM5YzUxMWU1YTE1MWZlZmY4MTljZGM5ZiIsImlzcyI6IkFDMGY5MjU4MTAyYjExMzg5MTY1ODZmYmQxYzcyMWRjZWMiLCJleHAiOjE0NDA2MTMxMzB9.EVkU9nLmdwGeIqwp2LfTBpWEtViIJ2OQcSExKPH8KXY";
// Implementation of native method testRTD() of TestRTDJNI class


JNIEXPORT void JNICALL Java_com_twilio_example_TestRTDJNI_testRTD(JNIEnv *env, jobject thisObj) {

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

	LOGW( "Java_com_twilio_example_TestRTDJNI_testRTD 7 : ITNNotificationClientPtr");
	ITNNotificationClientPtr notificationClient;
	notificationClient = TNNotificationClientFactory::CreateNotificationClient(capabilityToken,configurationProvider);

	LOGW( "Java_com_twilio_example_TestRTDJNI_testRTD 8: notificationClientObserver");
	std::shared_ptr<TwilioIPMessagingNotificationClientListener>  notificationClientObserver = std::make_shared<TwilioIPMessagingNotificationClientListener>();

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


JNIEXPORT jlong JNICALL Java_com_twilio_example_TestRTDJNI_getMessagingListener(JNIEnv *env, jobject obj) {

	LOGW( "creating MessagingClientListener: TwilioIPMessagingClientListener");
	//std::shared_ptr<TwilioIPMessagingClientListener> messagingListener;
	auto messagingListener = std::make_shared<TwilioIPMessagingClientListener>();
	return 0;//reinterpret_cast<jlong>(messagingListener);
}



JNIEXPORT jlong JNICALL Java_com_twilio_example_TestRTDJNI_getConfigurationProvider(JNIEnv *env, jobject obj) {

	/*LOGW( "Java_com_twilio_example_TestRTDJNI_testRTD 4");
	configMap.insert(std::make_pair("RTDIPMessagingServiceAddr", TEST_IPMESSAGING_SERVICE));
	configMap.insert(std::make_pair("RTDRegistrationServiceAddr", TEST_REGISTRATION_SERVICE));
	configMap.insert(std::make_pair("RTDTwilsockServiceAddr", TEST_TWILSOCK_SERVICE));
	configMap.insert(std::make_pair("RTDDataServiceAddr", TEST_DATA_SERVICE));
	configMap.insert(std::make_pair("RTDSubscriptionServiceAddr", TEST_SUBSCRIPTIONS_SERVICE));

	LOGW( "Java_com_twilio_example_TestRTDJNI_testRTD 5: capability Token");
	auto configurationProvider = std::make_shared<TwilioIPMessagingConfigurationProvider>(configMap);
	LOGW( "Java_com_twilio_example_TestRTDJNI_testRTD 6"); */

	return 0;//(jlong)configurationProvider;
}


JNIEXPORT jlong JNICALL Java_com_twilio_example_TestRTDJNI_getNotificationClient(JNIEnv *env, jobject obj, jlong nativeConfigurationProvider) {
/*
	TwilioIPMessagingConfigurationProvider configurationProvider= TwilioIPMessagingConfigurationProvider(reinterpret_cast<TwilioIPMessagingConfigurationProvider*>(nativeConfigurationProvider));
);

	LOGW( "Java_com_twilio_example_TestRTDJNI_getNotificationClient : ITNNotificationClientPtr");
	ITNNotificationClientPtr notificationClient;
	notificationClient = TNNotificationClientFactory::CreateNotificationClient(capabilityToken,configurationProvider);

	LOGW( "Java_com_twilio_example_TestRTDJNI_getNotificationClient : notificationClientObserver");
	std::shared_ptr<TwilioIPMessagingNotificationClientListener>  notificationClientObserver = std::make_shared<TwilioIPMessagingNotificationClientListener>();

	notificationClient->Init(notificationClientObserver);

*/
	return 0; //(jlong)notificationClient;
}




