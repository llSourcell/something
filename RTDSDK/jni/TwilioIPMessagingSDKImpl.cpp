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
//#include <Common/TLLoggerInitializer.h>
#include <twilio-jni/twilio-jni.h>
//#include <Twilsock/ITNTwilsockClient.h>

#include "ITMClient.h"
#include "ITMChannels.h"
#include "ITMChannel.h"
#include "ITMessages.h"
#include "ITMessage.h"
#include "ITMMembers.h"
#include "ITMMember.h"
#include "TwilioIPMessagingSDKImpl.h"
#include "TwilioIPMessagingClientContextDefines.h"


#define TAG  "TwilioIPMessagingSDK(native)"
/*#define PRODUCTION 0
#define STAGE 1
#define DEV 0
#define WITH_SSL 1 */


using namespace rtd;

/*std::map<std::string, std::string> configMap;

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

static LogListener logger; */

/*
 * Class:     com_twilio_ipmessaging_impl_TwilioIPMessagingSDKImpl
 * Method:    create
 * Signature: ()V
 */

JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_TwilioIPMessagingSDKImpl_create
  (JNIEnv *env, jobject obj) {

	LOGD(TAG, "Entered TwilioIPMessagingClientImpl_create()");

	static const char *class_names[] = {
			"com/twilio/ipmessaging/impl/MessageImpl",
			"com/twilio/ipmessaging/impl/ChannelImpl",
			"com/twilio/ipmessaging/impl/MemberImpl",
			NULL
		};
	static bool classes_precached = false;

	if (!classes_precached) {
		for (int i = 0; class_names[i]; ++i) {
			tw_jni_find_class(env, class_names[i]);
			if (env->ExceptionOccurred()) {
				env->ExceptionClear();
			}
		}
		classes_precached = true;
	}
}



