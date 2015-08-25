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

#define WITH_SSL 1

void resultHandler(TMResult result) {

}

std::string capabilityToken = "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJzY29wZSI6InNjb3BlOmNsaWVudDpvdXRnb2luZz9hcHBTaWQ9QVA3MDBkNzNmZTE1Y2FkYzM1NzFlOGE2MmU0ZDgxYzg1NyZhcHBQYXJhbXM9ZW5kcG9pbnRfaWQlM0RDNzYwRjM1Mi1EQTE1LTQwN0UtODEzNi1DQTY5MTJFOTlENDAlMjZjcmVkZW50aWFsX3NpZCUzRENSNDJjZDcyMjViYjIxZTkzYTE1NThjYTZjMGVhMDI4Y2QlMjZpZGVudGl0eSUzRGt1bWt1bSUyNnNlcnZpY2Vfc2lkJTNESVM2YjBmYTYwODM5YzUxMWU1YTE1MWZlZmY4MTljZGM5ZiIsImlzcyI6IkFDMGY5MjU4MTAyYjExMzg5MTY1ODZmYmQxYzcyMWRjZWMiLCJleHAiOjE0Mzk4ODIxNjR9.fuXwj-D-LY-7JGTkDl-MYS0GudC-TM1w3__xPgpP0do";
// Implementation of native method testRTD() of TestRTDJNI class


JNIEXPORT void JNICALL Java_com_twilio_example_TestRTDJNI_testRTD(JNIEnv *env, jobject thisObj) {

	std::array<const char*, sizeof("TMessaging")> filters = { "TMessaging" };
	rtd::ITDLogger::initialize(true, filters.data(), filters.size());

#if WITH_SSL
	Poco::Net::initializeSSL();
	auto context = new Poco::Net::Context(Poco::Net::Context::CLIENT_USE, "", "", "", Poco::Net::Context::VERIFY_NONE, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
	Poco::Net::SSLManager::instance().initializeClient(nullptr, nullptr, context);
#endif



	std::string userName = "kumkum";

	__android_log_print(ANDROID_LOG_VERBOSE, TAG, "Capability token");
	configMap.insert(std::make_pair("RTDIPMessagingServiceAddr", TEST_IPMESSAGING_SERVICE));
	configMap.insert(std::make_pair("RTDRegistrationServiceAddr", TEST_REGISTRATION_SERVICE));
	configMap.insert(std::make_pair("RTDTwilsockServiceAddr", TEST_TWILSOCK_SERVICE));
	configMap.insert(std::make_pair("RTDDataServiceAddr", TEST_DATA_SERVICE));
	configMap.insert(std::make_pair("RTDSubscriptionServiceAddr", TEST_SUBSCRIPTIONS_SERVICE));

	auto configurationProvider = std::make_shared<TwilioIPMessagingConfigurationProvider>(configMap);

	__android_log_print(ANDROID_LOG_VERBOSE, TAG, "ITNNotificationClientPtr");
	ITNNotificationClientPtr notificationClient;
	notificationClient = TNNotificationClientFactory::CreateNotificationClient(capabilityToken,configurationProvider);

	__android_log_print(ANDROID_LOG_VERBOSE, TAG, "notificationClientObserver");
	std::shared_ptr<TwilioIPMessagingNotificationClientListener>  notificationClientObserver = std::make_shared<TwilioIPMessagingNotificationClientListener>();
	notificationClient->Init(notificationClientObserver);

	__android_log_print(ANDROID_LOG_VERBOSE, TAG, "TwilioIPMessagingClientListener");
	std::shared_ptr<TwilioIPMessagingClientListener> messagingListener = std::make_shared<TwilioIPMessagingClientListener>(thisObj);
	ITMClientPtr messagingClient = ITMClient::createClient(capabilityToken,
		                                                   messagingListener,
		                                                   configurationProvider,
		                                                   notificationClient,
														   [](TMResult result){ std::cout << "client initialized" << std::endl; });


	{
	        //get channels object//////////////////////////////////////
	        ITMChannelsPtr channels = messagingClient->getChannels();
	        while (channels == nullptr)
	        {
	            std::cout << "app: messaging lib not ready, retrying..." << std::endl;
	            Poco::Thread::sleep(1000);
	            channels = messagingClient->getChannels();
	        }
	}



   return;
}






