/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
#include "ChannelsImpl.h"
#include "TwilioIPMessagingClientContextDefines.h"
#include "ITMChannel.h"

#include "TwilioIPMessagingLogger.h"
#include <twilio-jni/JNIEnvAttacher.h>
#include <twilio-jni/tw-jni.h>
#include <twilio-jni/tw-jni-compat.h>
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
	const char *nativeNameString = NULL;
	LOG_DEBUG(TAG,"createChannelNative : Checking nativeChannelsContext.");
	if (nativeChannelsContext == 0) {
		LOG_DEBUG(TAG,"nativeChannelsContext is null");
	} else {
		ChannelsContext *channelsContextRecreate = reinterpret_cast<ChannelsContext *>(nativeChannelsContext);
		LOG_DEBUG(TAG,"client context is recreated.");

		if(channelsContextRecreate == nullptr) {
			LOG_DEBUG(TAG,"channelsContextRecreate is NULL.");
		}

		if(channelsContextRecreate->channels == nullptr) {
			LOG_DEBUG(TAG, "createChannelNative : ITChannelsPtr is NULL.");
		}

		ITMChannelsPtr channelsPtr = channelsContextRecreate->channels;

		if (channelsPtr != nullptr) {
			jobject j_createChanneListener_ = env->NewGlobalRef(listener);
			jclass cls = (env)->GetObjectClass(j_createChanneListener_);
			jmethodID j_onCreated_ = (env)->GetMethodID(cls, "onCreated", "(Lcom/twilio/ipmessaging/Channel;)V");
			jmethodID j_onError_ = (env)->GetMethodID(cls, "onError", "()V");

			ITMChannelPtr channelPtr = channelsPtr->createChannel();
			if(type == 0) {
				LOG_DEBUG(TAG, "Creating public channel");
				channelPtr->setType(rtd::TMChannelType::kTMChannelTypePublic, [](TMResult result) {
					LOG_DEBUG(TAG,"Channel setType to kTMChannelTypePublic command processed");
				});

			} else {
				LOG_DEBUG(TAG, "Creating private channel");
				channelPtr->setType(rtd::TMChannelType::kTMChannelTypePrivate, [](TMResult result) {
					LOG_DEBUG(TAG,"Channel setType to kTMChannelTypePrivate command processed");
				});
			}

			LOG_DEBUG(TAG,"createChannelNative: release native string.");
			if(friendlyName != NULL) {
				nativeNameString = env->GetStringUTFChars(friendlyName, JNI_FALSE);
				channelPtr->setFriendlyName(nativeNameString, NULL);
				env->ReleaseStringUTFChars(friendlyName, nativeNameString);
			}
			channelsPtr->add(channelPtr, [channelPtr,j_createChanneListener_,j_onCreated_, j_onError_](TMResult result) {
				LOG_DEBUG(TAG,"Channel add to kTMChannelTypePrivate command processed");
				JNIEnvAttacher jniAttacher;
				if (result == rtd::TMResult::kTMResultSuccess) {
					//Create channel context
					ChannelContext* channelContext_ = new ChannelContext();
					channelContext_->channel = channelPtr;
					jlong channelContextHandle = reinterpret_cast<jlong>(channelContext_);
					const char* sid = channelPtr->getSid().c_str();
					const char* name = channelPtr->getFriendlyName().c_str();
					LOG_DEBUG(TAG, "Channel Sid 1 %s", sid);
					jstring nameString = jniAttacher.get()->NewStringUTF(name);
					jstring sidString = jniAttacher.get()->NewStringUTF(sid);
					//create channel object
					jclass java_channel_impl_cls = tw_jni_find_class(jniAttacher.get(), "com/twilio/ipmessaging/impl/ChannelImpl");
					if(java_channel_impl_cls != nullptr) {
						LOG_DEBUG(TAG, "Found java_channel_impl_cls class" );
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
							//LOG_DEBUG(TAG, "Setting type to kTMChannelTypePublic");
							type = 0;
							break;
						case rtd::TMChannelType::kTMChannelTypePrivate:
							//LOG_DEBUG(TAG, "Setting type to kTMChannelTypePrivate");
							type = 1;
							break;
						default:
							break;
					}

					//LOG_DEBUG(TAG, "Channel type %d: ", type);
					jmethodID construct = tw_jni_get_method_by_class(jniAttacher.get(), java_channel_impl_cls, "<init>", "(Ljava/lang/String;Ljava/lang/String;JII)V");
					jobject channel = tw_jni_new_object(jniAttacher.get(), java_channel_impl_cls, construct, nameString, sidString, channelContextHandle, status, type);
					LOG_DEBUG(TAG, "Created Channel Object with type %d", type);

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
			LOG_ERROR(TAG,"channels is null");
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
	const char *nativeNameString = NULL;
	LOG_DEBUG(TAG,"createChannelNative : Checking nativeChannelsContext.");
	if (nativeChannelsContext == 0) {
		LOG_WARN(TAG,"nativeChannelsContext is null");
	} else {
		ChannelsContext *channelsContextRecreate = reinterpret_cast<ChannelsContext *>(nativeChannelsContext);
		LOG_DEBUG(TAG,"client context is recreated.");

		if(channelsContextRecreate == nullptr) {
			LOG_WARN(TAG,"channelsContextRecreate is NULL.");
		}

		if(channelsContextRecreate->channels == nullptr) {
			LOG_WARN(TAG, "createChannelNative : ITChannelsPtr is NULL.");
		}

		ITMChannelsPtr channelsPtr = channelsContextRecreate->channels;

		if (channelsPtr != nullptr) {

			ITMChannelPtr channelPtr = channelsPtr->createChannel();
			if(type == rtd::kTMChannelTypePublic) {
				channelPtr->setType(rtd::kTMChannelTypePublic, [](TMResult result) {LOG_DEBUG(TAG,"Channel setType to kTMChannelTypePublic command processed");});
			} else {
				channelPtr->setType(rtd::kTMChannelTypePrivate, [](TMResult result) {LOG_DEBUG(TAG,"Channel setType to kTMChannelTypePrivate command processed");});
			}

			if(friendlyName != NULL) {
				nativeNameString = env->GetStringUTFChars(friendlyName, JNI_FALSE);
				channelPtr->setFriendlyName(nativeNameString, NULL);
				env->ReleaseStringUTFChars(friendlyName, nativeNameString);
			}
			channelsPtr->add(channelPtr, [channelPtr](TMResult result) {
				LOG_DEBUG(TAG,"Channel setType to kTMChannelTypePrivate command processed");
				JNIEnvAttacher jniAttacher;
				if (result == rtd::TMResult::kTMResultSuccess) {
					LOG_DEBUG(TAG, "Successfully created channel");
				} else {
					LOG_DEBUG(TAG, "Channel creation failed");
					LOG_DEBUG(TAG, "Error creating Channel Object.");
				}
			});

		} else {
			LOG_ERROR(TAG,"channels is null");
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

		LOG_DEBUG( TAG, "createChannelNative : Checking nativeChannelsContext.");

		if (nativeChannelsContext == 0) {
			LOG_WARN(TAG, "nativeChannelsContext is null");
			return nullptr;
		} else {

			ChannelsContext *channelsContextRecreate = reinterpret_cast<ChannelsContext *>(nativeChannelsContext);
			LOG_DEBUG(TAG, "client context is recreated.");

			if(channelsContextRecreate == nullptr) {
				LOG_WARN(TAG,"createChannelNative : channelsContextRecreate is NULL.");
				return 0;
			}

			if(channelsContextRecreate->channels == nullptr) {
				LOG_WARN(TAG, "createChannelNative : ITChannelsPtr is NULL.");
				return 0;
			}

			ITMChannelsPtr channelsPtr = channelsContextRecreate->channels;
			if(channelsPtr != nullptr) {
				ITMChannelPtr channelPtr = channelsPtr->getChannel(nativeString);
				if(channelPtr != nullptr) {
					LOG_DEBUG(TAG, "Creating channel with sid : %s ", nativeString);

					//Create channel context
					ChannelContext* channelContext_ = new ChannelContext();
					channelContext_->channel = channelPtr;
					jlong channelContextHandle = reinterpret_cast<jlong>(channelContext_);

					const char* sid = channelPtr->getSid().c_str();
					const char* name = channelPtr->getFriendlyName().c_str();

					LOG_DEBUG(TAG, "Channel Name  : %s.", name );
					LOG_DEBUG(TAG, "Channel Sid %s", sid);

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
					LOG_DEBUG(TAG, "Created Channel Object.");

				} else {
					LOG_WARN(TAG, "Java_com_twilio_ipmessaging_impl_ChannelImpl_joinChannel channels is null");
					return nullptr;
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

	LOG_DEBUG(TAG, "Checking nativeChannelsContext.");
	if (nativeChannelsContext == 0) {
		LOG_WARN(TAG, "nativeChannelsContext is null");
			return nullptr;
	} else {

		ChannelsContext *channelsContextRecreate = reinterpret_cast<ChannelsContext *>(nativeChannelsContext);
		if(channelsContextRecreate == nullptr) {
			LOG_WARN(TAG, "channelsContextRecreate is null.");
			return 0;
		}

		if(channelsContextRecreate->channels == nullptr) {
			LOG_WARN(TAG, " ITChannelsPtr is null.");
			return 0;
		}

		ITMChannelsPtr channels = channelsContextRecreate->channels;

		std::vector<ITMChannelPtr> publicChannels;
		channels->getPublicChannelsList(publicChannels);
		LOG_DEBUG(TAG,"public channels count : %d",publicChannels.size());

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
					type = 0;
					break;
				case rtd::TMChannelType::kTMChannelTypePrivate:
					type = 1;
					break;
				default:
					break;
			}

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

/*
 * Class:     com_twilio_ipmessaging_impl_ChannelsImpl
 * Method:    getChannelNativeWithUniqueName
 * Signature: (Ljava/lang/String;J)V
 */
JNIEXPORT jobject JNICALL Java_com_twilio_ipmessaging_impl_ChannelsImpl_getChannelNativeWithUniqueName
  (JNIEnv *env, jobject obj, jstring channel_sid, jlong nativeChannelsContext) {

	jobject channel;

	if(channel_sid != nullptr) {
		const char *nativeString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
		LOG_DEBUG( TAG, "getChannelNativeWithUniqueName : Checking nativeChannelsContext.");

		if (nativeChannelsContext == 0) {
			LOG_WARN(TAG, "nativeChannelsContext is null");
			return nullptr;
		} else {
			ChannelsContext *channelsContextRecreate = reinterpret_cast<ChannelsContext *>(nativeChannelsContext);
			LOG_DEBUG(TAG, "client context is recreated.");

			if(channelsContextRecreate == nullptr) {
				LOG_WARN(TAG,"createChannelNative : channelsContextRecreate is NULL.");
				return 0;
			}

			if(channelsContextRecreate->channels == nullptr) {
				LOG_WARN(TAG, "createChannelNative : ITChannelsPtr is NULL.");
				return 0;
			}

			ITMChannelsPtr channelsPtr = channelsContextRecreate->channels;
			if(channelsPtr != nullptr) {
				ITMChannelPtr channelPtr = channelsPtr->getChannelByUniqueName(nativeString);
				if(channelPtr != nullptr) {
					LOG_DEBUG(TAG, "Creating channel with sid : %s ", nativeString);
					//Create channel context
					ChannelContext* channelContext_ = new ChannelContext();
					channelContext_->channel = channelPtr;
					jlong channelContextHandle = reinterpret_cast<jlong>(channelContext_);

					const char* sid = channelPtr->getSid().c_str();
					const char* name = channelPtr->getFriendlyName().c_str();

					LOG_DEBUG(TAG, "Channel Name  : %s.", name );
					LOG_DEBUG(TAG, "Channel Sid %s", sid);

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
					LOG_DEBUG(TAG, "Created Channel Object.");

				} else {
					LOG_WARN(TAG, "Channel by unique name %s not found", nativeString);
					return nullptr;
				}
			}
		}
	}
	return channel;
}


