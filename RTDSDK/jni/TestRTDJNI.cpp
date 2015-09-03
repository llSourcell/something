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
#include <Poco/UUIDGenerator.h>

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
#define CHANNEL_PREFIX "RTD Android Test / kbagchi@twilio.com / "


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
	ITMClientPtr messagingClient;
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

	clientParams_->messagingClient->shutdown();
	clientParams_->notificationClient->Shutdown();
	clientParams_->notificationClientObserver->waitShutdown();

	clientParams_->notificationClientObserver.reset();
	clientParams_->notificationClient.reset();
	clientParams_->messagingClient.reset();

	if(j_rtd_test_obj_ != NULL) {
		env->DeleteGlobalRef(j_rtd_test_obj_);
		j_rtd_test_obj_ = NULL;
	}
}

std::string generateRandomName()
{
    auto guid = Poco::UUIDGenerator::defaultGenerator().createRandom();
    return std::string(CHANNEL_PREFIX + guid.toString().substr(0, 8));
}



JNIEXPORT jlong JNICALL Java_com_twilio_example_TestRTDJNI_createMessagingClientTest(JNIEnv *env, jobject obj, jstring token) {

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

		clientParams_->messagingClient = messagingClient;


		//get channels object//////////////////////////////////////
		ITMChannelsPtr channels = messagingClient->getChannels();
		while (channels == nullptr)
		{
			LOGW("app: messaging lib not ready, retrying...");
			Poco::Thread::sleep(1000);
			channels = messagingClient->getChannels();
		}

		 //get channel lists//////////////////////////////////////
		std::vector<ITMChannelPtr> channelsList;
		channels->getMyChannelsList(channelsList);

		std::vector<ITMChannelPtr> publicChannels;
		channels->getPublicChannelsList(publicChannels);

        LOGW("app: public channels count : %d",publicChannels.size());
        LOGW("app: my channels count : %d.", channelsList.size() );

        //create new channel//////////////////////////////////////
	   ITMChannelPtr channel = channels->createChannel();
	   channel->setType(rtd::kTMChannelTypePublic, [](TMResult result) {});
	   channel->setName(generateRandomName(), NULL);

	   channel->setAttributes("{\"name\":\"sample name\"}", NULL);

	   channels->add(channel, NULL);

	   channels->getMyChannelsList(channelsList);
	   LOGW("app: my channels count : %d.", channelsList.size() );

	   ITMessagesPtr messages = channel->getMessages();
	   while (messages == nullptr)
	   {
		   LOGW("app: messages not available...");
		   Poco::Thread::sleep(1000);
		   messages = channel->getMessages();
	   }

	   ITMMembersPtr members = channel->getMembers();
	   while (members == nullptr)
	   {
		   LOGW("app: members not available...");
		   Poco::Thread::sleep(1000);
		   members = channel->getMembers();
	   }

	   //join created channel//////////////////////////////////////
	   channel->join(NULL);

	   channel->setName(generateRandomName(), NULL);

	   channel->setAttributes("{\"name\":\"second channel name\"}", NULL);


	   //add member//////////////////////////////////////
	   ITMMemberPtr member = members->createMember("kbagchi@twilio.com");
	   members->add(member, NULL);


	   //post messages//////////////////////////////////////
	   ITMessagePtr message = messages->createMessage();
	   message->setBody("there is a sausage in the kettle", [](TMResult result) {});
	   messages->send(message, NULL);

	   LOGW("message 1 was added");


	   ITMessagePtr message2 = messages->createMessage();
	   message2->setBody("there are 2 sausages in the kettle", [](TMResult result) {});
	   messages->send(message2, NULL);

	   LOGW("message 2 was added");

	   ITMessagePtr message3 = messages->createMessage();
	   message3->setBody("there are 3 sausages in the kettle", [](TMResult result) {});
	   messages->send(message3, NULL);

	   LOGW("message 3 was added");

	   ITMessagePtr message4 = messages->createMessage();
	   message4->setBody("there are 4 sausages in the kettle", [](TMResult result) {});
	   messages->send(message4, NULL);

	   LOGW("message 4 was added");

	   //leave channel//////////////////////////////////////
	   channel->leave(NULL);

	}

	return 0;//reinterpret_cast<jlong>(messagingClient.get());
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

		clientParams_->messagingClient = messagingClient;



	}

	return 0;//reinterpret_cast<jlong>(messagingClient.get());
}



JNIEXPORT jobjectArray JNICALL Java_com_twilio_example_TestRTDJNI_getChannels(JNIEnv *env, jobject obj) {

	jobject channel = NULL;
	if( clientParams_ != NULL) {
		if(clientParams_->messagingClient != NULL) {
			//get channels object//////////////////////////////////////
			ITMChannelsPtr channels = clientParams_->messagingClient->getChannels();
			while (channels == nullptr)
			{
				LOGW("app: messaging lib not ready, retrying...");
				Poco::Thread::sleep(1000);
				channels = clientParams_->messagingClient->getChannels();
			}

			 //get channel lists//////////////////////////////////////
			std::vector<ITMChannelPtr> channelsList;
			channels->getMyChannelsList(channelsList);

			std::vector<ITMChannelPtr> publicChannels;
			channels->getPublicChannelsList(publicChannels);

			LOGW("app: public channels count : %d",publicChannels.size());
			LOGW("app: my channels count : %d.", channelsList.size() );

			jclass java_channel_impl_cls = tw_jni_find_class(env, "com/twilio/ipmessaging/impl/ChannelImpl");
			jmethodID construct = tw_jni_get_method_by_class(env, java_channel_impl_cls, "<init>", "(Lcom/twilio/ipmessaging/ChannelListener;Ljava/lang/String;)V");
			jobjectArray channelsArray = (jobjectArray) env->NewObjectArray(publicChannels.size(),java_channel_impl_cls, 0);

			/*for (int i= 0; i<publicChannels.size() ; i++ ) {

				const char* name = publicChannels[i].get()->getName();
				const char* sid = publicChannels[i].get()->getSid();
				LOGW("Channel Name  : %s.", name );
				channel = tw_jni_new_object(env, java_channel_impl_cls, construct, name, sid );
				env->SetObjectArrayElement(channelsArray, i, channel);
			} */


		}
	}

	return NULL;

}
