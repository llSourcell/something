/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
#include "ChannelsImpl.h"
#include "TwilioIPMessagingClientContextDefines.h"
#include "ITMChannel.h"

#include <twilio-jni/twilio-jni.h>
#include <android/log.h>

using namespace rtd;

#define TAG  "ChannelsImpl(native)"

jobject createChannelOnject() {

}

/*
 * Class:     com_twilio_ipmessaging_impl_ChannelsImpl
 * Method:    createChannelNativeWithListener
 * Signature: (Ljava/lang/String;IJLcom/twilio/ipmessaging/TwilioIPMessagingClient/CreateChannelListener;)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_ChannelsImpl_createChannelNativeWithListener
(JNIEnv *env, jobject obj, jstring friendlyName, jint type, jlong nativeChannelsContext, jobject listener) {
	jobject channel;
	LOGD(TAG,"createChannelNative : Checking nativeChannelsContext.");
	if (nativeChannelsContext == 0) {
		LOG_W(TAG,"nativeChannelsContext is null");
	} else {
		ChannelsContext *channelsContextRecreate = reinterpret_cast<ChannelsContext *>(nativeChannelsContext);
		LOGD(TAG,"client context is recreated.");

		if(channelsContextRecreate == nullptr) {
			LOGW(TAG,"channelsContextRecreate is NULL.");
		}

		if(channelsContextRecreate->channels == nullptr) {
			LOGW(TAG, "createChannelNative : ITChannelsPtr is NULL.");
		}

		ITMChannelsPtr channelsPtr = channelsContextRecreate->channels;

		if (channelsPtr != nullptr) {
			jobject j_createChanneListener_ = env->NewGlobalRef(listener);
			jmethodID j_onCreated_ = tw_jni_get_method(env, j_createChanneListener_, "onCreated", "(Lcom/twilio/ipmessaging/Channel;)V");
			jmethodID j_onError_ = tw_jni_get_method(env, j_createChanneListener_, "onError", "()V");

			const char *nativeNameString = env->GetStringUTFChars(friendlyName, JNI_FALSE);
			ITMChannelPtr channelPtr = channelsPtr->createChannel();
			if(type == 0) {
				__android_log_print(ANDROID_LOG_INFO, TAG, "Creating public channel");
				channelPtr->setType(rtd::TMChannelType::kTMChannelTypePublic, [](TMResult result) {
					LOGD(TAG,"Channel setType to kTMChannelTypePublic command processed");
				});

			} else {
				__android_log_print(ANDROID_LOG_INFO, TAG, "Creating private channel");
				channelPtr->setType(rtd::TMChannelType::kTMChannelTypePrivate, [](TMResult result) {
					LOGD(TAG,"Channel setType to kTMChannelTypePrivate command processed");
				});
			}
			channelPtr->setFriendlyName(nativeNameString, NULL);
			LOGD(TAG,"createChannelNative: release native string.");
			env->ReleaseStringUTFChars(friendlyName, nativeNameString);
			channelsPtr->add(channelPtr, [channelPtr,j_createChanneListener_,j_onCreated_, j_onError_](TMResult result) {
				LOGD(TAG,"Channel setType to kTMChannelTypePrivate command processed");
				JNIEnvAttacher jniAttacher;
				if (result == rtd::TMResult::kTMResultSuccess) {
					//Create channel context
					ChannelContext* channelContext_ = new ChannelContext();
					channelContext_->channel = channelPtr;
					jlong channelContextHandle = reinterpret_cast<jlong>(channelContext_);
					const char* sid = channelPtr->getSid().c_str();
					const char* name = channelPtr->getFriendlyName().c_str();
					__android_log_print(ANDROID_LOG_INFO, TAG, "Channel Sid 1 %s", sid);
					jstring nameString = jniAttacher.get()->NewStringUTF(name);
					jstring sidString = jniAttacher.get()->NewStringUTF(sid);
					//create channel object
					jclass java_channel_impl_cls = tw_jni_find_class(jniAttacher.get(), "com/twilio/ipmessaging/impl/ChannelImpl");
					if(java_channel_impl_cls != nullptr) {
						LOGD(TAG, "Found java_channel_impl_cls class" );
					}

					int status = 0;
					switch (channelPtr->getStatus()) {
						case TMChannelStatus::kTMChannelStatusInvited:
							status = 0;
						case TMChannelStatus::kTMChannelStatusJoined:
							status = 1;
						case TMChannelStatus::kTMChannelStatusNotParticipating:
							status = 2;
						default:
							break;
					}

					int type = 0;
					switch (channelPtr->getType()) {
						case rtd::TMChannelType::kTMChannelTypePublic:
							//__android_log_print(ANDROID_LOG_INFO, TAG, "Setting type to kTMChannelTypePublic");
							type = 0;
							break;
						case rtd::TMChannelType::kTMChannelTypePrivate:
							//__android_log_print(ANDROID_LOG_INFO, TAG, "Setting type to kTMChannelTypePrivate");
							type = 1;
							break;
						default:
							break;
					}

					//__android_log_print(ANDROID_LOG_INFO, TAG, "Channel type %d: ", type);
					jmethodID construct = tw_jni_get_method_by_class(jniAttacher.get(), java_channel_impl_cls, "<init>", "(Ljava/lang/String;Ljava/lang/String;JII)V");
					jobject channel = tw_jni_new_object(jniAttacher.get(), java_channel_impl_cls, construct, nameString, sidString, channelContextHandle, status, type);
					__android_log_print(ANDROID_LOG_INFO, TAG, "Created Channel Object with type %d", type);

					//Call Java
					jniAttacher.get()->CallVoidMethod(j_createChanneListener_,j_onCreated_, channel);
					jniAttacher.get()->DeleteGlobalRef(j_createChanneListener_);
				 } else {
					//Call Java
					jniAttacher.get()->CallVoidMethod(j_createChanneListener_,j_onError_);
					jniAttacher.get()->DeleteGlobalRef(j_createChanneListener_);
				 }
			});


		} else {
			LOG_E(TAG,"channels is null");
		}
	}
}


/*
 * Class:     com_twilio_ipmessaging_impl_ChannelsImpl
 * Method:    createChannelNative
 * Signature: (Ljava/lang/String;IJ)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_ChannelsImpl_createChannelNative
(JNIEnv *env, jobject obj, jstring friendlyName, jint type, jlong nativeChannelsContext) {
	LOGD(TAG,"createChannelNative : Checking nativeChannelsContext.");
	if (nativeChannelsContext == 0) {
		LOG_W(TAG,"nativeChannelsContext is null");
	} else {
		ChannelsContext *channelsContextRecreate = reinterpret_cast<ChannelsContext *>(nativeChannelsContext);
		LOGD(TAG,"client context is recreated.");

		if(channelsContextRecreate == nullptr) {
			LOGW(TAG,"channelsContextRecreate is NULL.");
		}

		if(channelsContextRecreate->channels == nullptr) {
			LOGW(TAG, "createChannelNative : ITChannelsPtr is NULL.");
		}

		ITMChannelsPtr channelsPtr = channelsContextRecreate->channels;

		if (channelsPtr != nullptr) {
			const char *nativeNameString = env->GetStringUTFChars(friendlyName, JNI_FALSE);
			ITMChannelPtr channelPtr = channelsPtr->createChannel();
			if(type == rtd::kTMChannelTypePublic) {
				channelPtr->setType(rtd::kTMChannelTypePublic, [](TMResult result) {LOGD(TAG,"Channel setType to kTMChannelTypePublic command processed");});
			} else {
				channelPtr->setType(rtd::kTMChannelTypePrivate, [](TMResult result) {LOGD(TAG,"Channel setType to kTMChannelTypePrivate command processed");});
			}
			channelPtr->setFriendlyName(nativeNameString, NULL);
			LOGD(TAG,"createChannelNative: release native string.");
			env->ReleaseStringUTFChars(friendlyName, nativeNameString);
			channelsPtr->add(channelPtr, [channelPtr](TMResult result) {
				LOGD(TAG,"Channel setType to kTMChannelTypePrivate command processed");
				JNIEnvAttacher jniAttacher;
				if (result == rtd::TMResult::kTMResultSuccess) {
					__android_log_print(ANDROID_LOG_INFO, TAG, "Successfully created channel");
				} else {
					__android_log_print(ANDROID_LOG_INFO, TAG, "Channel creation failed");
					LOG_D(TAG, "Error creating Channel Object.");
				}
			});

		} else {
			LOG_E(TAG,"channels is null");
		}
	}
}


/*
 * Class:     com_twilio_ipmessaging_impl_ChannelsImpl
 * Method:    getChannelNative
 * Signature: (Ljava/lang/String;J)V
 */
JNIEXPORT jobject JNICALL Java_com_twilio_ipmessaging_impl_ChannelsImpl_getChannelNative
  (JNIEnv *env, jobject obj, jstring channel_sid, jlong nativeChannelsContext) {

	jobject channel;

	if(channel_sid != nullptr) {
		const char *nativeString = env->GetStringUTFChars(channel_sid, JNI_FALSE);

		LOGD( TAG, "createChannelNative : Checking nativeChannelsContext.");

		if (nativeChannelsContext == 0) {
			LOG_W(TAG, "nativeChannelsContext is null");
			return nullptr;
		} else {

			ChannelsContext *channelsContextRecreate = reinterpret_cast<ChannelsContext *>(nativeChannelsContext);
			LOGD(TAG, "client context is recreated.");

			if(channelsContextRecreate == nullptr) {
				LOG_W(TAG,"createChannelNative : channelsContextRecreate is NULL.");
				return 0;
			}

			if(channelsContextRecreate->channels == nullptr) {
				LOG_W(TAG, "createChannelNative : ITChannelsPtr is NULL.");
				return 0;
			}

			ITMChannelsPtr channelsPtr = channelsContextRecreate->channels;
			if(channelsPtr != nullptr) {
				ITMChannelPtr channelPtr = channelsPtr->getChannel(nativeString);
				if(channelPtr != nullptr) {
					LOGD(TAG, "Creating channel with sid : %s ", nativeString);

					//Create channel context
					ChannelContext* channelContext_ = new ChannelContext();
					channelContext_->channel = channelPtr;
					jlong channelContextHandle = reinterpret_cast<jlong>(channelContext_);

					const char* sid = channelPtr->getSid().c_str();
					const char* name = channelPtr->getFriendlyName().c_str();

					LOGD(TAG, "Channel Name  : %s.", name );
					LOGD(TAG, "Channel Sid %s", sid);

					jstring nameString = env->NewStringUTF(name);
					jstring sidString = env->NewStringUTF(sid);
					int status = 0;
					switch (channelPtr->getStatus()) {
						case rtd::TMChannelStatus::kTMChannelStatusInvited:
							status = 0;
							break;
						case rtd::TMChannelStatus::kTMChannelStatusJoined:
							status = 1;
							break;
						case rtd::TMChannelStatus::kTMChannelStatusNotParticipating:
							status = 2;
							break;
						default:
							break;
					}

					int type = 0;
					switch (channelPtr->getType()) {
						case rtd::TMChannelType::kTMChannelTypePublic:
							type = 0;
							break;
						case rtd::TMChannelType::kTMChannelTypePrivate:
							type = 1;
							break;
						default:
							break;
					}

					//create channel object
					jclass java_channel_impl_cls = tw_jni_find_class(env, "com/twilio/ipmessaging/impl/ChannelImpl");
					jmethodID construct = tw_jni_get_method_by_class(env, java_channel_impl_cls, "<init>", "(Ljava/lang/String;Ljava/lang/String;JII)V");
					channel = tw_jni_new_object(env, java_channel_impl_cls, construct, nameString, sidString, channelContextHandle, status, type);
					LOGD(TAG, "Created Channel Object.");

				} else {
					LOGW(TAG, "Java_com_twilio_ipmessaging_impl_ChannelImpl_joinChannel channels is null");
				}
			}
		}
	}
	return channel;
}

/*
 * Class:     com_twilio_ipmessaging_impl_ChannelsImpl
 * Method:    getChannelsNative
 * Signature: (J)[Lcom/twilio/ipmessaging/Channel;
 */
JNIEXPORT jobjectArray JNICALL Java_com_twilio_ipmessaging_impl_ChannelsImpl_getChannelsNative
  (JNIEnv *env, jobject obj, jlong nativeChannelsContext) {

	jobject channel;
	jobjectArray channelsArray ;

	LOGD(TAG, "Checking nativeChannelsContext.");
	if (nativeChannelsContext == 0) {
		LOGW(TAG, "nativeChannelsContext is null");
			return nullptr;
	} else {

		ChannelsContext *channelsContextRecreate = reinterpret_cast<ChannelsContext *>(nativeChannelsContext);
		if(channelsContextRecreate == nullptr) {
			LOGW(TAG, "channelsContextRecreate is null.");
			return 0;
		}

		if(channelsContextRecreate->channels == nullptr) {
			LOGW(TAG, " ITChannelsPtr is null.");
			return 0;
		}

		ITMChannelsPtr channels = channelsContextRecreate->channels;

		std::vector<ITMChannelPtr> publicChannels;
		channels->getPublicChannelsList(publicChannels);
		LOGD(TAG,"public channels count : %d",publicChannels.size());

		jclass java_channel_impl_cls = tw_jni_find_class(env, "com/twilio/ipmessaging/impl/ChannelImpl");

		jmethodID construct = tw_jni_get_method_by_class(env, java_channel_impl_cls, "<init>", "(Ljava/lang/String;Ljava/lang/String;JII)V");
		channelsArray = (jobjectArray) env->NewObjectArray(publicChannels.size(),java_channel_impl_cls, 0);

		for (int i= 0; i<publicChannels.size() ; i++ ) {
			ITMChannelPtr channelPtr = publicChannels[i];
			const char* sid = channelPtr->getSid().c_str();
			const char* name = channelPtr->getFriendlyName().c_str();
			channelPtr = channels->getChannel(sid);
			int status = 0;
			switch (channelPtr->getStatus()) {
				case rtd::TMChannelStatus::kTMChannelStatusInvited:
					status = 0;
					break;
				case rtd::TMChannelStatus::kTMChannelStatusJoined:
					status = 1;
					break;
				case rtd::TMChannelStatus::kTMChannelStatusNotParticipating:
					status = 2;
					break;
				default:
					break;
			}

			int type = 0;
			switch (channelPtr->getType()) {
				case rtd::TMChannelType::kTMChannelTypePublic:
					//__android_log_print(ANDROID_LOG_INFO, TAG, "getChannelsNative:Setting type to kTMChannelTypePublic");
					type = 0;
					break;
				case rtd::TMChannelType::kTMChannelTypePrivate:
					//__android_log_print(ANDROID_LOG_INFO, TAG, "getChannelsNative:Setting type to kTMChannelTypePrivate");
					type = 1;
					break;
				default:
					break;
			}

			//__android_log_print(ANDROID_LOG_INFO, TAG, "Channel type %d: ", type);

			ChannelContext* channelContext_ = new ChannelContext();
			channelContext_->channel = channelPtr;
			jlong channelContextHandle = reinterpret_cast<jlong>(channelContext_);
			jstring nameString = env->NewStringUTF(name);
			jstring sidString = env->NewStringUTF(sid);
			channel = tw_jni_new_object(env, java_channel_impl_cls, construct, nameString, sidString, channelContextHandle, status, type);
			env->SetObjectArrayElement(channelsArray, i, channel);
		}
	}

	return channelsArray ;

}


