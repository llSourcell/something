#include <jni.h>
#include <iostream>
#include <string.h>
#include <map>
#include <utility>
#include <android/log.h>
#include <array>
#include <Notification/ITNNotificationClient.h>
#include <Common/TLLoggerInitializer.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/Net/Context.h>
#include <Poco/Net/PrivateKeyPassphraseHandler.h>
#include <Poco/UUIDGenerator.h>
#include <twilio-jni/JNIEnvAttacher.h>
#include <twilio-jni/tw-jni.h>
#include <twilio-jni/tw-jni-compat.h>


#include "ITMClient.h"
#include "ITMChannels.h"
#include "ITMChannel.h"
#include "ITMessages.h"
#include "ITMessage.h"
#include "ITMMembers.h"
#include "ITMMember.h"
#include "TwilioIPMessagingSDKImpl.h"
#include "TwilioIPMessagingClientContextDefines.h"
#include "TwilioIPMessagingLogger.h"



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
#endif*/

class LogListener: public ITLLogListener {
public :
	void onNewEntry(const char * c, const char* prefix, TLLogLevel level) {
		if( level == TLLogLevel::Debug) {
			LOG_DEBUG(TAG, "%s", c);
		} else if( level == TLLogLevel::Info ) {
			LOG_INFO(TAG, "%s", c);
		} else if( level == TLLogLevel::Warning ) {
			LOG_WARN(TAG, "%s", c);
		} else if( level == TLLogLevel::Critical || level == TLLogLevel::Fatal) {
			LOG_ERROR(TAG, "%s", c);
		}
	}

};

static LogListener logger;

/*
 * Class:     com_twilio_ipmessaging_impl_TwilioIPMessagingSDKImpl
 * Method:    create
 * Signature: ()V
 */

JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_TwilioIPMessagingSDKImpl_create
  (JNIEnv *env, jobject obj) {

	LOG_DEBUG(TAG, "Entered TwilioIPMessagingClientImpl_create()");

	rtd::TLLogger::instance().SetLogListener(&logger);
	LOG_DEBUG(TAG, "Set ITDLogger to true.");

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

JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_TwilioIPMessagingSDKImpl_setLogLevelNative
	(JNIEnv *env, jobject obj, jint level) {

	switch (level) {
		case 7:
			tipm_jni_set_log_level(TIPMLogLevel::TIPM_LOG_LEVEL_NONE);
			break;
		case 6:
			tipm_jni_set_log_level(TIPMLogLevel::TIPM_LOG_LEVEL_ERROR);
			break;
		case 5:
			tipm_jni_set_log_level(TIPMLogLevel::TIPM_LOG_LEVEL_WARN);
			break;
		case 4:
			tipm_jni_set_log_level(TIPMLogLevel::TIPM_LOG_LEVEL_INFO);
			break;
		case 3:
			tipm_jni_set_log_level(TIPMLogLevel::TIPM_LOG_LEVEL_DEBUG);
			break;
		default:
			tipm_jni_set_log_level(TIPMLogLevel::TIPM_LOG_LEVEL_ERROR);
	}

}



