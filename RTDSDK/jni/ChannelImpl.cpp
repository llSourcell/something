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
 * Method:    joinChannel
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_ChannelImpl_joinChannel
(JNIEnv *env, jobject obj, jlong nativeChannelContext, jstring channel_sid, jobject listener) {

	LOGW(TAG,"joinChannel Entered ");
	__android_log_print(ANDROID_LOG_INFO, TAG, "joinChannel Entered ");
	const char *nativeString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
	ChannelContext *channelContext = reinterpret_cast<ChannelContext *>(nativeChannelContext);

	if(channelContext != nullptr) {
		jobject j_statusListener_ = env->NewGlobalRef(listener);
		jmethodID j_onSuccess_ = tw_jni_get_method(env, j_statusListener_, "onSuccess", "()V");
		jmethodID j_onError_ = tw_jni_get_method(env, j_statusListener_, "onError", "()V");

		ITMChannelPtr channel = channelContext->channel;
		if(channel != nullptr) {
			LOGD(TAG, "Joining channel with sid : %s ", nativeString);
			__android_log_print(ANDROID_LOG_INFO, TAG, "joining channel.");
			channel->join([j_statusListener_,j_onSuccess_, j_onError_](TMResult result){
				JNIEnvAttacher jniAttacher;
				if (result == rtd::TMResult::kTMResultSuccess) {
					__android_log_print(ANDROID_LOG_INFO, TAG, "Join channel is successful. Calling java listener.");
					//Call Java
					jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onSuccess_);
					jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
				} else {
					__android_log_print(ANDROID_LOG_INFO, TAG, "Join channel failed");

					//Call Java
					jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onError_);
					jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
				}
			});
		} else {
			env->DeleteGlobalRef(j_statusListener_);
			__android_log_print(ANDROID_LOG_INFO, TAG, "channel is null.");
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
(JNIEnv *env, jobject obj, jlong nativeChannelContext, jstring channel_sid, jobject listener) {

	LOGD(TAG,"leaveChannel Entered ");
	const char *nativeString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
	ChannelContext *channelContext = reinterpret_cast<ChannelContext *>(nativeChannelContext);

	if(channelContext != nullptr) {
		jobject j_statusListener_ = env->NewGlobalRef(listener);
		jmethodID j_onSuccess_ = tw_jni_get_method(env, j_statusListener_, "onSuccess", "()V");
		jmethodID j_onError_ = tw_jni_get_method(env, j_statusListener_, "onError", "()V");

		ITMChannelPtr channel = channelContext->channel;

		if(channel != nullptr) {
			LOGD(TAG,"Leaving channel with sid : %s ", nativeString);
			channel->leave([j_statusListener_,j_onSuccess_, j_onError_](TMResult result){
				JNIEnvAttacher jniAttacher;
				if (result == rtd::TMResult::kTMResultSuccess) {
					__android_log_print(ANDROID_LOG_INFO, TAG, "leave channel is successful. Calling java listener.");
					//Call Java
					jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onSuccess_);
					jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
				} else {
					__android_log_print(ANDROID_LOG_INFO, TAG, "leave channel failed");

					//Call Java
					jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onError_);
					jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
				}
			});
		} else {
			env->DeleteGlobalRef(j_statusListener_);
			LOGW(TAG,"channel is null");
		}
	} else {
		LOGW(TAG,"channels is null");
	}
	env->ReleaseStringUTFChars(channel_sid, nativeString);
}


/*
 * Class:     com_twilio_ipmessaging_impl_ChannelImpl
 * Method:    destroyChannel
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_ChannelImpl_destroyChannel
(JNIEnv *env, jobject obj, jlong nativeChannelContext, jstring channel_sid, jobject listener) {
	LOGD(TAG,"destroyChannel: Entered ");
	const char *nativeString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
	ChannelContext *channelContext = reinterpret_cast<ChannelContext *>(nativeChannelContext);

	if(channelContext != nullptr) {
		jobject j_statusListener_ = env->NewGlobalRef(listener);
		jmethodID j_onSuccess_ = tw_jni_get_method(env, j_statusListener_, "onSuccess", "()V");
		jmethodID j_onError_ = tw_jni_get_method(env, j_statusListener_, "onError", "()V");
		ITMChannelPtr channel = channelContext->channel;
		if(channel != nullptr) {
			LOGW(TAG, "Destroying channel with sid : %s ", nativeString);
			channel->destroy([j_statusListener_,j_onSuccess_, j_onError_](TMResult result){
				JNIEnvAttacher jniAttacher;
				if (result == rtd::TMResult::kTMResultSuccess) {
					__android_log_print(ANDROID_LOG_INFO, TAG, "Destroy channel is successful. Calling java listener.");
					//Call Java
					jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onSuccess_);
					jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
				} else {
					__android_log_print(ANDROID_LOG_INFO, TAG, "Destroy channel failed");

					//Call Java
					jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onError_);
					jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
				}
			});
		} else {
			env->DeleteGlobalRef(j_statusListener_);
			__android_log_print(ANDROID_LOG_INFO, TAG, "Channel is null");
			LOGW(TAG, "channel is null");
		}
	} else {
		__android_log_print(ANDROID_LOG_INFO, TAG, "Channels is null");
		LOGW(TAG, "channels is null");
	}
	env->ReleaseStringUTFChars(channel_sid, nativeString);
}



/*
 * Class:     com_twilio_ipmessaging_impl_ChannelImpl
 * Method:    updateChannelName
 * Signature: (JLjava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_ChannelImpl_updateChannelName
  (JNIEnv *env, jobject obj, jlong nativeChannelContext, jstring channel_sid, jstring modifiedChannelName, jobject listener) {

	const char *nativeSidString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
	const char *nativeNameString = env->GetStringUTFChars(modifiedChannelName, JNI_FALSE);
	ChannelContext *channelContext = reinterpret_cast<ChannelContext *>(nativeChannelContext);

	if(channelContext != nullptr) {
		jobject j_statusListener_ = env->NewGlobalRef(listener);
		jmethodID j_onSuccess_ = tw_jni_get_method(env, j_statusListener_, "onSuccess", "()V");
		jmethodID j_onError_ = tw_jni_get_method(env, j_statusListener_, "onError", "()V");
		ITMChannelPtr channel = channelContext->channel;

		if(channel != nullptr) {
			LOGW("Update Name for channel with sid : %s ", nativeSidString);
			channel->setFriendlyName(nativeNameString, [j_statusListener_,j_onSuccess_, j_onError_](TMResult result){
				JNIEnvAttacher jniAttacher;
				if (result == rtd::TMResult::kTMResultSuccess) {
					__android_log_print(ANDROID_LOG_INFO, TAG, "Update Name for channel is successful. Calling java listener.");
					//Call Java
					jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onSuccess_);
					jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
				} else {
					__android_log_print(ANDROID_LOG_INFO, TAG, "Update Name for channel failed");

					//Call Java
					jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onError_);
					jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
				}
			});
		} else {
			env->DeleteGlobalRef(j_statusListener_);
		}
	}
	env->ReleaseStringUTFChars(channel_sid, nativeSidString);
	env->ReleaseStringUTFChars(channel_sid, nativeNameString);
}

/*
 * Class:     com_twilio_ipmessaging_impl_ChannelImpl
 * Method:    updateChannelName
 * Signature: (JLjava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_ChannelImpl_updateChannelType
  (JNIEnv *env, jobject obj, jlong nativeChannelContext, jstring channel_sid, int type, jobject listener) {

	const char *nativeSidString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
	ChannelContext *channelContext = reinterpret_cast<ChannelContext *>(nativeChannelContext);

	if(channelContext != nullptr) {
		jobject j_statusListener_ = env->NewGlobalRef(listener);
		jmethodID j_onSuccess_ = tw_jni_get_method(env, j_statusListener_, "onSuccess", "()V");
		jmethodID j_onError_ = tw_jni_get_method(env, j_statusListener_, "onError", "()V");

		ITMChannelPtr channel = channelContext->channel;
		if(channel != nullptr) {
			LOGW("Update channel Type for channel with sid : %s ", nativeSidString);
			if(type == rtd::kTMChannelTypePublic) {
				__android_log_print(ANDROID_LOG_INFO, TAG, "Setting Channel Type to public");
				channel->setType(rtd::kTMChannelTypePublic, [j_statusListener_,j_onSuccess_, j_onError_](TMResult result){
					JNIEnvAttacher jniAttacher;
					if (result == rtd::TMResult::kTMResultSuccess) {
						__android_log_print(ANDROID_LOG_INFO, TAG, "Destroy channel is successful. Calling java listener.");
						//Call Java
						jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onSuccess_);
						jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
					} else {
						__android_log_print(ANDROID_LOG_INFO, TAG, "Destroy channel failed");

						//Call Java
						jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onError_);
						jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
					}
				});
			} else {
				__android_log_print(ANDROID_LOG_INFO, TAG, "Setting Channel Type to private");
				channel->setType(rtd::kTMChannelTypePrivate, [j_statusListener_,j_onSuccess_, j_onError_](TMResult result){
					JNIEnvAttacher jniAttacher;
					if (result == rtd::TMResult::kTMResultSuccess) {
						__android_log_print(ANDROID_LOG_INFO, TAG, "Destroy channel is successful. Calling java listener.");
						//Call Java
						jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onSuccess_);
						jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
					} else {
						__android_log_print(ANDROID_LOG_INFO, TAG, "Destroy channel failed");

						//Call Java
						jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onError_);
						jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
					}
				});
			}
		} else {
			env->DeleteGlobalRef(j_statusListener_);
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
  (JNIEnv *env, jobject obj, jlong nativeChannelContext, jstring channel_sid, jstring attrString, jobject listener) {

	const char *nativeSidString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
	const char *nativeAttrString = env->GetStringUTFChars(attrString, JNI_FALSE);
	ChannelContext *channelContext = reinterpret_cast<ChannelContext *>(nativeChannelContext);

	if(channelContext != nullptr) {
		jobject j_statusListener_ = env->NewGlobalRef(listener);
		jmethodID j_onSuccess_ = tw_jni_get_method(env, j_statusListener_, "onSuccess", "()V");
		jmethodID j_onError_ = tw_jni_get_method(env, j_statusListener_, "onError", "()V");

		ITMChannelPtr channel = channelContext->channel;
		if(channel != nullptr) {
			LOGW("Update Attribute for channel with sid : %s ", nativeSidString);
			channel->setAttributes(nativeAttrString, [j_statusListener_,j_onSuccess_, j_onError_](TMResult result){
				JNIEnvAttacher jniAttacher;
				if (result == rtd::TMResult::kTMResultSuccess) {
					__android_log_print(ANDROID_LOG_INFO, TAG, "Update Attribute for channel is successful. Calling java listener.");
					//Call Java
					jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onSuccess_);
					jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
				} else {
					__android_log_print(ANDROID_LOG_INFO, TAG, "Update Attribute for channel failed");

					//Call Java
					jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onError_);
					jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
				}
			});
		}
	}
	env->ReleaseStringUTFChars(channel_sid, nativeSidString);
	env->ReleaseStringUTFChars(channel_sid, nativeAttrString);
}

/*
 * Class:     com_twilio_ipmessaging_impl_ChannelImpl
 * Method:    declineChannelInvite
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_ChannelImpl_declineChannelInvite
(JNIEnv *env, jobject obj, jlong nativeChannelContext, jstring channel_sid, jobject listener) {

	LOGW(TAG,"declineChannelInvite app: Entered ");

	const char *nativeSidString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
	ChannelContext *channelContext = reinterpret_cast<ChannelContext *>(nativeChannelContext);

	if(channelContext != nullptr) {
		jobject j_statusListener_ = env->NewGlobalRef(listener);
		jmethodID j_onSuccess_ = tw_jni_get_method(env, j_statusListener_, "onSuccess", "()V");
		jmethodID j_onError_ = tw_jni_get_method(env, j_statusListener_, "onError", "()V");

		ITMChannelPtr channel = channelContext->channel;

		if(channel != nullptr) {
			LOGD(TAG,"Decline invite for channel with sid : %s ", nativeSidString);
			channel->declineInvitation([j_statusListener_,j_onSuccess_, j_onError_](TMResult result){
				JNIEnvAttacher jniAttacher;
				if (result == rtd::TMResult::kTMResultSuccess) {
					__android_log_print(ANDROID_LOG_INFO, TAG, "Decline invite for channel is successful. Calling java listener.");
					//Call Java
					jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onSuccess_);
					jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
				} else {
					__android_log_print(ANDROID_LOG_INFO, TAG, "Decline invite for channel failed");

					//Call Java
					jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onError_);
					jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
				}
			});
		} else {
			LOGW(TAG,"declineChannelInvite ELEMENT NOT found");
		}
	}
	env->ReleaseStringUTFChars(channel_sid, nativeSidString);
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
		ITMChannelPtr channel = clientChannelContext->channel;
		if(channel != nullptr) {
			const char* sid = channel->getSid().c_str();
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
		ITMChannelPtr channel = clientChannelContext->channel;
		if(channel != nullptr) {
			channel->typing();
		}
	}
}


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
		ITMChannelPtr channel = clientChannelContext->channel;
		if(channel != nullptr) {
			const char* attr = channel->getAttributes().c_str();
			attrString = env->NewStringUTF(attr);
		}
	}
	return attrString;
}



JNIEXPORT jobject JNICALL Java_com_twilio_ipmessaging_impl_ChannelImpl_getMessagesObject
(JNIEnv *env, jobject obj, jlong nativeChannelContext, jstring channel_sid) {

	jobject messagesObj = NULL;
	const char *nativeSidString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
	ChannelContext *channelContext = reinterpret_cast<ChannelContext *>(nativeChannelContext);

	if(channelContext != nullptr) {
		ITMChannelPtr channel = channelContext->channel;
			if(channel != nullptr) {
			__android_log_print(ANDROID_LOG_INFO, TAG, "Channel getMessagesObject channel with sid %s ", nativeSidString );
			LOGD(TAG,"getMessages for channel with sid : %s ", nativeSidString);
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
	}
	return nullptr;
}

/*
 * Class:     com_twilio_ipmessaging_impl_ChannelImpl
 * Method:    getMembers
 * Signature: (JLjava/lang/String;)Lcom/twilio/ipmessaging/Members;
 */
JNIEXPORT jobject JNICALL Java_com_twilio_ipmessaging_impl_ChannelImpl_getMembers
  (JNIEnv *env, jobject obj, jlong nativeChannelContext, jstring channel_sid) {

	jobject membersObj;
	const char *nativeSidString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
	ChannelContext *channelContext = reinterpret_cast<ChannelContext *>(nativeChannelContext);

	if(channelContext != nullptr) {
		ITMChannelPtr channel = channelContext->channel;
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
	return membersObj;
}


/*
 * Class:     com_twilio_ipmessaging_impl_ChannelImpl
 * Method:    getStatus
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_twilio_ipmessaging_impl_ChannelImpl_getStatus
  (JNIEnv *env, jobject obj, jlong nativeChannelContext, jstring channel_sid) {
	LOGD(TAG,"getStatus Entered ");
	const char *nativeString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
	ChannelContext *channelContext = reinterpret_cast<ChannelContext *>(nativeChannelContext);

	if(channelContext != nullptr) {
		ITMChannelPtr channel = channelContext->channel;
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
	}
	env->ReleaseStringUTFChars(channel_sid, nativeString);
}

