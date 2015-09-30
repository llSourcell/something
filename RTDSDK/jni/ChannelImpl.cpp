#include <twilio-jni/twilio-jni.h>
#include <android/log.h>

#include <Poco/Net/SSLManager.h>
#include <Poco/Net/Context.h>
#include <Poco/Net/PrivateKeyPassphraseHandler.h>
#include <Poco/UUIDGenerator.h>

#include "ITMClient.h"
#include "ITMChannels.h"
#include "ITMChannel.h"
#include "ITMessages.h"
#include "ITMessage.h"
#include "ITMMembers.h"
#include "ITMMember.h"
#include "ChannelImpl.h"
#include "TwilioIPMessagingClientContextDefines.h"

#define TAG  "ChannelImpl(native)"

/*
 * Class:     com_twilio_ipmessaging_impl_ChannelImpl
 * Method:    destroyChannel
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_ChannelImpl_destroyChannel
(JNIEnv *env, jobject obj, jlong nativeClientContext, jstring channel_sid) {

	LOGW(TAG,"Java_com_twilio_ipmessaging_impl_ChannelImpl_leaveChannel app: Entered ");

	const char *nativeString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
	IPMessagingClientContext *clientParamsRecreate = reinterpret_cast<IPMessagingClientContext *>(nativeClientContext);

	if(clientParamsRecreate != nullptr) {
		ITMChannelsPtr channels = clientParamsRecreate->channels;
		if(channels != NULL) {
			ITMChannelPtr channel = clientParamsRecreate->channels->getChannel(nativeString);

			if(channel != nullptr) {

				LOGW("Joining channel with sid : %s ", nativeString);
				channel->destroy(nullptr);
			} else {
				LOGW("Java_com_twilio_ipmessaging_impl_ChannelImpl_joinChannel ELEMENT NOT found");
				}
		} else {
			LOGW("Java_com_twilio_ipmessaging_impl_ChannelImpl_joinChannel channels is null");
		}
	}

	env->ReleaseStringUTFChars(channel_sid, nativeString);
}

/*
 * Class:     com_twilio_ipmessaging_impl_ChannelImpl
 * Method:    leaveChannel
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_ChannelImpl_leaveChannel
(JNIEnv *env, jobject obj, jlong nativeClientContext, jstring channel_sid) {

	LOGW("Java_com_twilio_ipmessaging_impl_ChannelImpl_leaveChannel app: Entered ");

	const char *nativeString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
	IPMessagingClientContext *clientParamsRecreate = reinterpret_cast<IPMessagingClientContext *>(nativeClientContext);

	if(clientParamsRecreate != nullptr) {
		ITMChannelsPtr channels = clientParamsRecreate->channels;
		if(channels != NULL) {
			ITMChannelPtr channel = clientParamsRecreate->channels->getChannel(nativeString);

			if(channel != nullptr) {
				LOGW("Joining channel with sid : %s ", nativeString);
				channel->leave(nullptr);
			} else {
				LOGW("Java_com_twilio_ipmessaging_impl_ChannelImpl_joinChannel ELEMENT NOT found");
				}
		} else {
			LOGW("Java_com_twilio_ipmessaging_impl_ChannelImpl_joinChannel channels is null");
		}
	}

	env->ReleaseStringUTFChars(channel_sid, nativeString);
}

/*
 * Class:     com_twilio_ipmessaging_impl_ChannelImpl
 * Method:    joinChannel
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_ChannelImpl_joinChannel
(JNIEnv *env, jobject obj, jlong nativeClientContext, jstring channel_sid) {

	LOGW("Java_com_twilio_ipmessaging_impl_ChannelImpl_joinChannel app: Entered ");

	const char *nativeString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
	IPMessagingClientContext *clientParamsRecreate = reinterpret_cast<IPMessagingClientContext *>(nativeClientContext);

	if(clientParamsRecreate != nullptr) {
		ITMChannelsPtr channels = clientParamsRecreate->channels;
		if(channels != NULL) {
			ITMChannelPtr channel = clientParamsRecreate->channels->getChannel(nativeString);

			if(channel != nullptr) {
				LOGW("Joining channel with sid : %s ", nativeString);
				channel->join(nullptr);
			} else {
				LOGW("Java_com_twilio_ipmessaging_impl_ChannelImpl_joinChannel ELEMENT NOT found");
				}
		} else {
			LOGW("Java_com_twilio_ipmessaging_impl_ChannelImpl_joinChannel channels is null");
		}
	}

	env->ReleaseStringUTFChars(channel_sid, nativeString);
}

/*
 * Class:     com_twilio_ipmessaging_impl_ChannelImpl
 * Method:    getStatus
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_twilio_ipmessaging_impl_ChannelImpl_getStatus
  (JNIEnv *env, jobject obj, jlong nativeClientContext, jstring channel_sid) {

	LOGW("Java_com_twilio_ipmessaging_impl_ChannelImpl_joinChannel app: Entered ");

	const char *nativeString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
	IPMessagingClientContext *clientParamsRecreate = reinterpret_cast<IPMessagingClientContext *>(nativeClientContext);

	if(clientParamsRecreate != nullptr) {
		ITMChannelsPtr channels = clientParamsRecreate->channels;
		if(channels != NULL) {
			ITMChannelPtr channel = clientParamsRecreate->channels->getChannel(nativeString);
			if(channel != nullptr) {
				LOGW("GetStatus channel with sid : %s ", nativeString);
				switch (channel->getStatus()) {
					case TMChannelStatus::kTMChannelStatusInvited:
						return 0;
					case TMChannelStatus::kTMChannelStatusJoined:
						return 1;
					case TMChannelStatus::kTMChannelStatusNotParticipating:
						return 2;
					default:
						break;
				}
			} else {
				LOGW("Java_com_twilio_ipmessaging_impl_ChannelImpl_getStatus ELEMENT NOT found");
			}
		} else {
			LOGW("Java_com_twilio_ipmessaging_impl_ChannelImpl_getStatus channels is null");
		}
	}

	env->ReleaseStringUTFChars(channel_sid, nativeString);

}


JNIEXPORT jobject JNICALL Java_com_twilio_ipmessaging_impl_ChannelImpl_getMessagesObject
(JNIEnv *env, jobject obj, jlong nativeClientContext, jstring channel_sid) {

	jobject messagesObj = NULL;
	const char *nativeString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
	IPMessagingClientContext *clientParams_ = reinterpret_cast<IPMessagingClientContext *>(nativeClientContext);

	if(clientParams_ != nullptr) {

		LOGW("Java_com_twilio_ipmessaging_impl_ChannelImpl_getStatus: Entered ");
		const char *nativeString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
		ITMChannelsPtr channels = clientParams_->channels;
		if(channels != NULL) {
			ITMChannelPtr channel = channels->getChannel(nativeString);
			env->ReleaseStringUTFChars(channel_sid, nativeString);
			if(channel != nullptr) {
				LOGW("getMessages for channel with sid : %s ", nativeString);
				ITMessagesPtr messagesLocal = channel->getMessages();

				while (messagesLocal == nullptr)
				{
				   LOGW("app: messages not available...");
				   Poco::Thread::sleep(1000);
				   messagesLocal = channel->getMessages();
				}

				MessagesContext* messagesContext_ = new MessagesContext();
				messagesContext_->messages = messagesLocal;

				jlong messagesContextHandle = reinterpret_cast<jlong>(messagesContext_);

				jclass java_messages_impl_cls = tw_jni_find_class(env, "com/twilio/ipmessaging/impl/MessagesImpl");
				if(java_messages_impl_cls != NULL) {
					LOGW("Found java_messages_impl_cls class" );
				}

				jmethodID construct = tw_jni_get_method_by_class(env, java_messages_impl_cls, "<init>", "(J)V");

				messagesObj = tw_jni_new_object(env, java_messages_impl_cls, construct, messagesContextHandle);
				LOGW("Created messagesObj Object.");


				return messagesObj;

			} else {
				LOGW("Java_com_twilio_ipmessaging_impl_ChannelImpl_getStatus ELEMENT NOT found");
				return nullptr;
			}
		} else {
			LOGW("Java_com_twilio_ipmessaging_impl_ChannelImpl_getStatus channels is null");
			return nullptr;
		}
	}
	return nullptr;
}


/*
 * Class:     com_twilio_ipmessaging_impl_ChannelImpl
 * Method:    updateChannelName
 * Signature: (JLjava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_ChannelImpl_updateChannelName
  (JNIEnv *env, jobject obj, jlong nativeClientContext, jstring channel_sid, jstring modifiedChannelName) {

	const char *nativeString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
	IPMessagingClientContext *clientParams_ = reinterpret_cast<IPMessagingClientContext *>(nativeClientContext);

	if(clientParams_ != nullptr) {

		LOGW("Java_com_twilio_ipmessaging_impl_ChannelImpl_updateChannelName: Entered ");
		const char *nativeSidString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
		const char *nativeNameString = env->GetStringUTFChars(modifiedChannelName, JNI_FALSE);

		ITMChannelsPtr channels = clientParams_->channels;
		if(channels != nullptr) {
			ITMChannelPtr channel = channels->getChannel(nativeSidString);

			if(channel != nullptr) {
				LOGW("Update Name for channel with sid : %s ", nativeSidString);
				channel->setName(nativeNameString, NULL);
			}
		}
		env->ReleaseStringUTFChars(channel_sid, nativeSidString);
		env->ReleaseStringUTFChars(channel_sid, nativeNameString);
	}

}

/*
 * Class:     com_twilio_ipmessaging_impl_ChannelImpl
 * Method:    getMembers
 * Signature: (JLjava/lang/String;)Lcom/twilio/ipmessaging/Members;
 */
JNIEXPORT jobject JNICALL Java_com_twilio_ipmessaging_impl_ChannelImpl_getMembers
  (JNIEnv *env, jobject obj, jlong nativeClientContext, jstring channel_sid) {

	jobject membersObj;

	const char *nativeString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
	IPMessagingClientContext *clientParams_ = reinterpret_cast<IPMessagingClientContext *>(nativeClientContext);

	if(clientParams_ != nullptr) {

		LOGW("Java_com_twilio_ipmessaging_impl_ChannelImpl_getMembers: Entered ");
		const char *nativeSidString = env->GetStringUTFChars(channel_sid, JNI_FALSE);

		ITMChannelsPtr channels = clientParams_->channels;
		if(channels != nullptr) {
			ITMChannelPtr channel = channels->getChannel(nativeSidString);

			if(channel != nullptr) {
				LOGW("Get Members for channel with sid : %s ", nativeSidString);
				ITMMembersPtr membersLocal = channel->getMembers();
				while (membersLocal == nullptr)
				{
					LOGW("app: members not available...");
					Poco::Thread::sleep(1000);
					membersLocal = channel->getMembers();
				}

				MembersContext* membersContext_ = new MembersContext();
				membersContext_->members = membersLocal;

				jlong membersContextHandle = reinterpret_cast<jlong>(membersContext_);

				jclass java_members_impl_cls = tw_jni_find_class(env, "com/twilio/ipmessaging/impl/MembersImpl");
				if(java_members_impl_cls != NULL) {
					LOGW("Found java_members_impl_cls class" );
				}

				jmethodID construct = tw_jni_get_method_by_class(env, java_members_impl_cls, "<init>", "(J)V");

				membersObj = tw_jni_new_object(env, java_members_impl_cls, construct, membersContextHandle);
				LOGW("Created messagesObj Object.");

				env->ReleaseStringUTFChars(channel_sid, nativeSidString);
			}
		}
	}

	return membersObj;
}

/*
 * Class:     com_twilio_ipmessaging_impl_ChannelImpl
 * Method:    updateChannelAttributes
 * Signature: (JLjava/lang/String;Ljava/util/Map;)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_ChannelImpl_updateChannelAttributes
(JNIEnv *env, jobject obj, jlong nativeClientContext, jstring channel_sid, jobject attributes) {

}



/*
 * Class:     com_twilio_ipmessaging_impl_ChannelImpl
 * Method:    declineChannelInvite
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_ChannelImpl_declineChannelInvite
(JNIEnv *env, jobject obj, jlong nativeClientContext, jstring channel_sid) {

	LOGW("declineChannelInvite app: Entered ");

	const char *nativeString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
	IPMessagingClientContext *clientParamsRecreate = reinterpret_cast<IPMessagingClientContext *>(nativeClientContext);

	if(clientParamsRecreate != nullptr) {
		ITMChannelsPtr channels = clientParamsRecreate->channels;
		if(channels != NULL) {
			ITMChannelPtr channel = clientParamsRecreate->channels->getChannel(nativeString);

			if(channel != nullptr) {
				LOGW("Joining channel with sid : %s ", nativeString);
				channel->declineInvitation(nullptr);
			} else {
				LOGW("declineChannelInvite ELEMENT NOT found");
				}
		} else {
			LOGW("declineChannelInvite: channels is null");
		}
	}

	env->ReleaseStringUTFChars(channel_sid, nativeString);
}
