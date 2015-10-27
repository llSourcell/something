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
 * Method:    getMembers
 * Signature: (JLjava/lang/String;)Lcom/twilio/ipmessaging/Members;
 */
JNIEXPORT jobject JNICALL Java_com_twilio_ipmessaging_impl_ChannelImpl_getMembers
  (JNIEnv *env, jobject obj, jlong nativeClientContext, jstring channel_sid) {

	jobject membersObj;

	const char *nativeString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
	IPMessagingClientContext *clientParams_ = reinterpret_cast<IPMessagingClientContext *>(nativeClientContext);

	if(clientParams_ != nullptr) {

		LOGW(TAG,"Java_com_twilio_ipmessaging_impl_ChannelImpl_getMembers: Entered ");
		const char *nativeSidString = env->GetStringUTFChars(channel_sid, JNI_FALSE);

		ITMChannelsPtr channels = clientParams_->channels;
		if(channels != nullptr) {
			ITMChannelPtr channel = channels->getChannel(nativeSidString);

			if(channel != nullptr) {
				LOGW(TAG, "Get Members for channel with sid : %s ", nativeSidString);
				ITMMembersPtr membersLocal = channel->getMembers();
				while (membersLocal == nullptr)
				{
					LOGW(TAG,"app: members not available...");
					Poco::Thread::sleep(1000);
					membersLocal = channel->getMembers();
				}

				MembersContext* membersContext_ = new MembersContext();
				membersContext_->members = membersLocal;

				jlong membersContextHandle = reinterpret_cast<jlong>(membersContext_);

				jclass java_members_impl_cls = tw_jni_find_class(env, "com/twilio/ipmessaging/impl/MembersImpl");
				if(java_members_impl_cls != NULL) {
					LOGW(TAG,"Found java_members_impl_cls class" );
				}

				jmethodID construct = tw_jni_get_method_by_class(env, java_members_impl_cls, "<init>", "(J)V");

				membersObj = tw_jni_new_object(env, java_members_impl_cls, construct, membersContextHandle);
				LOGW(TAG,"Created messagesObj Object.");

				env->ReleaseStringUTFChars(channel_sid, nativeSidString);
			}
		}
	}

	return membersObj;
}


JNIEXPORT jobject JNICALL Java_com_twilio_ipmessaging_impl_ChannelImpl_getMessagesObject
(JNIEnv *env, jobject obj, jlong nativeClientContext, jstring channel_sid) {

	jobject messagesObj = NULL;
	const char *nativeString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
	IPMessagingClientContext *clientParams_ = reinterpret_cast<IPMessagingClientContext *>(nativeClientContext);

	if(clientParams_ != nullptr) {
		const char *nativeString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
		ITMChannelsPtr channels = clientParams_->channels;
		if(channels != NULL) {
			ITMChannelPtr channel = channels->getChannel(nativeString);
			env->ReleaseStringUTFChars(channel_sid, nativeString);
			if(channel != nullptr) {
				LOGD(TAG,"getMessages for channel with sid : %s ", nativeString);
				ITMessagesPtr messagesLocal = channel->getMessages();

				while (messagesLocal == nullptr)
				{
				   LOGD(TAG,"messages not available...");
				   Poco::Thread::sleep(1000);
				   messagesLocal = channel->getMessages();
				}

				MessagesContext* messagesContext_ = new MessagesContext();
				messagesContext_->messages = messagesLocal;

				jlong messagesContextHandle = reinterpret_cast<jlong>(messagesContext_);

				jclass java_messages_impl_cls = tw_jni_find_class(env, "com/twilio/ipmessaging/impl/MessagesImpl");
				if(java_messages_impl_cls != NULL) {
					LOGD(TAG,"Found java_messages_impl_cls class" );
				}

				jmethodID construct = tw_jni_get_method_by_class(env, java_messages_impl_cls, "<init>", "(J)V");

				messagesObj = tw_jni_new_object(env, java_messages_impl_cls, construct, messagesContextHandle);
				LOGD(TAG,"Created messagesObj Object.");


				return messagesObj;

			} else {
				LOGW(TAG,"clientParams_ ELEMENT NOT found");
				return nullptr;
			}
		} else {
			LOGW(TAG, "channels is null");
			return nullptr;
		}
	}
	return nullptr;
}

/*
 * Class:     com_twilio_ipmessaging_impl_ChannelImpl
 * Method:    destroyChannel
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_ChannelImpl_destroyChannel
(JNIEnv *env, jobject obj, jlong nativeClientContext, jstring channel_sid) {

	LOGD(TAG,"destroyChannel: Entered ");

	const char *nativeString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
	IPMessagingClientContext *clientParamsRecreate = reinterpret_cast<IPMessagingClientContext *>(nativeClientContext);

	if(clientParamsRecreate != nullptr) {
		ITMChannelsPtr channels = clientParamsRecreate->channels;
		if(channels != NULL) {
			ITMChannelPtr channel = clientParamsRecreate->channels->getChannel(nativeString);
			if(channel != nullptr) {
				LOGW(TAG, "Destroying channel with sid : %s ", nativeString);
				channel->destroy([](TMResult result){
					if (result == rtd::TMResult::kTMResultSuccess) {
						__android_log_print(ANDROID_LOG_INFO, TAG, "Channel Destroy successful");
					} else {
						__android_log_print(ANDROID_LOG_INFO, TAG, "Channel destroy failed");
					}
				});
			} else {
				__android_log_print(ANDROID_LOG_INFO, TAG, "Channel is null");
				LOGW(TAG, "channel is null");
			}
		} else {
			__android_log_print(ANDROID_LOG_INFO, TAG, "Channels is null");
			LOGW(TAG, "channels is null");
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

	LOGD(TAG,"leaveChannel Entered ");

	const char *nativeString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
	IPMessagingClientContext *clientParamsRecreate = reinterpret_cast<IPMessagingClientContext *>(nativeClientContext);

	if(clientParamsRecreate != nullptr) {
		ITMChannelsPtr channels = clientParamsRecreate->channels;
		if(channels != NULL) {
			ITMChannelPtr channel = clientParamsRecreate->channels->getChannel(nativeString);

			if(channel != nullptr) {
				LOGD(TAG,"Leaving channel with sid : %s ", nativeString);
				channel->leave([](TMResult result){LOGW(TAG,"Channel leave command processed"); });
			} else {
				LOGW(TAG,"channel is null");
			}
		} else {
			LOGW(TAG,"channels is null");
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

	LOGW(TAG,"joinChannel Entered ");
	__android_log_print(ANDROID_LOG_INFO, TAG, "joinChannel Entered ");

	const char *nativeString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
	IPMessagingClientContext *clientParamsRecreate = reinterpret_cast<IPMessagingClientContext *>(nativeClientContext);

	if(clientParamsRecreate != nullptr) {
		__android_log_print(ANDROID_LOG_INFO, TAG, "joinChannel clientParamsRecreate is not null.");
		ITMChannelsPtr channels = clientParamsRecreate->channels;
		if(channels != NULL) {
			__android_log_print(ANDROID_LOG_INFO, TAG, "joinChannel channels is not null.");
			ITMChannelPtr channel = clientParamsRecreate->channels->getChannel(nativeString);
			if(channel != nullptr) {
				LOGD(TAG, "Joining channel with sid : %s ", nativeString);
				__android_log_print(ANDROID_LOG_INFO, TAG, "joining channel.");
				channel->join([](TMResult result){
					if (result == rtd::TMResult::kTMResultSuccess) {
						__android_log_print(ANDROID_LOG_INFO, TAG, "Join channel is successful");
					} else {
						__android_log_print(ANDROID_LOG_INFO, TAG, "Join channel failed");
					}
				});
			} else {
				LOGW(TAG, "channel is null");
				}
		} else {
			LOGW(TAG, "channels is null");
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

	LOGD(TAG,"getStatus Entered ");

	const char *nativeString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
	IPMessagingClientContext *clientParamsRecreate = reinterpret_cast<IPMessagingClientContext *>(nativeClientContext);

	if(clientParamsRecreate != nullptr) {
		ITMChannelsPtr channels = clientParamsRecreate->channels;
		if(channels != NULL) {
			ITMChannelPtr channel = clientParamsRecreate->channels->getChannel(nativeString);
			if(channel != nullptr) {
				LOGD(TAG,"GetStatus channel with sid : %s ", nativeString);
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
				LOGW(TAG,"channel is null");
			}
		} else {
			LOGW(TAG,"channels is null");
		}
	}
	env->ReleaseStringUTFChars(channel_sid, nativeString);
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

		LOGW(TAG,"Java_com_twilio_ipmessaging_impl_ChannelImpl_updateChannelName: Entered ");
		const char *nativeSidString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
		const char *nativeNameString = env->GetStringUTFChars(modifiedChannelName, JNI_FALSE);

		ITMChannelsPtr channels = clientParams_->channels;
		if(channels != nullptr) {
			ITMChannelPtr channel = channels->getChannel(nativeSidString);

			if(channel != nullptr) {
				LOGW("Update Name for channel with sid : %s ", nativeSidString);
				channel->setName(nativeNameString, ([](TMResult result){LOGW(TAG,"Channel setName command processed"); }));
			}
		}
		env->ReleaseStringUTFChars(channel_sid, nativeSidString);
		env->ReleaseStringUTFChars(channel_sid, nativeNameString);
	}

}

/*
 * Class:     com_twilio_ipmessaging_impl_ChannelImpl
 * Method:    updateChannelName
 * Signature: (JLjava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_ChannelImpl_updateChannelType
  (JNIEnv *env, jobject obj, jlong nativeClientContext, jstring channel_sid, int type) {

	const char *nativeString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
	IPMessagingClientContext *clientParams_ = reinterpret_cast<IPMessagingClientContext *>(nativeClientContext);

	if(clientParams_ != nullptr) {

		LOGW(TAG,"Java_com_twilio_ipmessaging_impl_ChannelImpl_updateChannelName: Entered ");
		const char *nativeSidString = env->GetStringUTFChars(channel_sid, JNI_FALSE);

		ITMChannelsPtr channelsPtr = clientParams_->channels;
		if(channelsPtr != nullptr) {
			ITMChannelPtr channelPtr = channelsPtr->getChannel(nativeSidString);
			if(channelPtr != nullptr) {
				LOGW("Update Name for channel with sid : %s ", nativeSidString);
				if(type == rtd::kTMChannelTypePublic) {
					__android_log_print(ANDROID_LOG_INFO, TAG, "Setting Channel Type to public");
					channelPtr->setType(rtd::kTMChannelTypePublic, [](TMResult result) {
						if (result == rtd::TMResult::kTMResultSuccess) {
							__android_log_print(ANDROID_LOG_INFO, TAG, "Successfully set channel type to Public");
						} else {
							__android_log_print(ANDROID_LOG_INFO, TAG, "Failed to set channel type to Public");
							LOG_D(TAG, "Error creating Channel Object.");
						}
					});
				} else {
					__android_log_print(ANDROID_LOG_INFO, TAG, "Setting Channel Type to private");
					channelPtr->setType(rtd::kTMChannelTypePrivate, [](TMResult result) {
						if (result == rtd::TMResult::kTMResultSuccess) {
							__android_log_print(ANDROID_LOG_INFO, TAG, "Successfully set channel type to Private");
						} else {
							__android_log_print(ANDROID_LOG_INFO, TAG, "Failed to set channel type to Private");
							LOG_D(TAG, "Error creating Channel Object.");
						}
					});
				}
			}
		}
		env->ReleaseStringUTFChars(channel_sid, nativeSidString);
	}
}


/*
 * Class:     com_twilio_ipmessaging_impl_ChannelImpl
 * Method:    updateChannelAttributes
 * Signature: (JLjava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_ChannelImpl_updateChannelAttributes
  (JNIEnv *env, jobject obj, jlong nativeClientContext, jstring channel_sid, jstring attrString) {
	const char *nativeString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
	IPMessagingClientContext *clientParams_ = reinterpret_cast<IPMessagingClientContext *>(nativeClientContext);

	if(clientParams_ != nullptr) {

		LOGW(TAG,"Java_com_twilio_ipmessaging_impl_ChannelImpl_updateChannelName: Entered ");
		const char *nativeSidString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
		const char *nativeAttrString = env->GetStringUTFChars(attrString, JNI_FALSE);

		ITMChannelsPtr channels = clientParams_->channels;
		if(channels != nullptr) {
			ITMChannelPtr channel = channels->getChannel(nativeSidString);
			if(channel != nullptr) {
				LOGW("Update Name for channel with sid : %s ", nativeSidString);
				channel->setAttributes(nativeAttrString, ([](TMResult result){
					if (result == rtd::TMResult::kTMResultSuccess) {
						__android_log_print(ANDROID_LOG_INFO, TAG, "Channel attribute update successful");
					} else {
						__android_log_print(ANDROID_LOG_INFO, TAG, "Channel attribute update failed");
					}
				}
				));
			}
		}
		env->ReleaseStringUTFChars(channel_sid, nativeSidString);
		env->ReleaseStringUTFChars(channel_sid, nativeAttrString);
	}
}





/*
 * Class:     com_twilio_ipmessaging_impl_ChannelImpl
 * Method:    declineChannelInvite
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_ChannelImpl_declineChannelInvite
(JNIEnv *env, jobject obj, jlong nativeClientContext, jstring channel_sid) {

	LOGW(TAG,"declineChannelInvite app: Entered ");

	const char *nativeString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
	IPMessagingClientContext *clientParamsRecreate = reinterpret_cast<IPMessagingClientContext *>(nativeClientContext);

	if(clientParamsRecreate != nullptr) {
		ITMChannelsPtr channels = clientParamsRecreate->channels;
		if(channels != NULL) {
			ITMChannelPtr channel = clientParamsRecreate->channels->getChannel(nativeString);

			if(channel != nullptr) {
				LOGD(TAG,"Joining channel with sid : %s ", nativeString);
				channel->declineInvitation([](TMResult result){LOGW(TAG,"Channel declineInvitation command processed"); });
			} else {
				LOGW(TAG,"declineChannelInvite ELEMENT NOT found");
				}
		} else {
			LOGW(TAG,"declineChannelInvite: channels is null");
		}
	}
	env->ReleaseStringUTFChars(channel_sid, nativeString);
}


/*
 * Class:     com_twilio_ipmessaging_impl_ChannelImpl
 * Method:    getChannelSidNative
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_twilio_ipmessaging_impl_ChannelImpl_getChannelSidNative
  (JNIEnv *env, jobject obj, jlong nativeChannelContext) {
	ChannelContext *clientChannelContext = reinterpret_cast<ChannelContext *>(nativeChannelContext);
	jstring sidString;
	if(clientChannelContext != nullptr) {
		ITMChannelPtr channelPtr = clientChannelContext->channel;
		if(channelPtr != nullptr) {
			const char* sid = channelPtr->getSid().c_str();
			sidString = env->NewStringUTF(sid);
		}
	}
	return sidString;
}


/*
 * Class:     com_twilio_ipmessaging_impl_ChannelImpl
 * Method:    typingStartNative
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_ChannelImpl_typingStartNative
(JNIEnv *env, jobject obj, jlong nativeChannelContext) {
	ChannelContext *clientChannelContext = reinterpret_cast<ChannelContext *>(nativeChannelContext);
	if(clientChannelContext != nullptr) {
		ITMChannelPtr channelPtr = clientChannelContext->channel;
		if(channelPtr != nullptr) {
			channelPtr->typing();
		}
	}
}


/*
 * Class:     com_twilio_ipmessaging_impl_ChannelImpl
 * Method:    getAttributesNative
 * Signature: (J)Ljava/lang/String;
 */
/*
 * Class:     com_twilio_ipmessaging_impl_ChannelImpl
 * Method:    getChannelAttributesNative
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_twilio_ipmessaging_impl_ChannelImpl_getChannelAttributesNative
(JNIEnv *env, jobject obj, jlong nativeChannelContext) {
	__android_log_print(ANDROID_LOG_INFO, TAG, "Entered getChannelAttributesNative");
	ChannelContext *clientChannelContext = reinterpret_cast<ChannelContext *>(nativeChannelContext);
	jstring attrString;
	if(clientChannelContext != nullptr) {
		ITMChannelPtr channelPtr = clientChannelContext->channel;
		if(channelPtr != nullptr) {
			const char* attr = channelPtr->getAttributes().c_str();
			attrString = env->NewStringUTF(attr);
		}
	}
	return attrString;
}
