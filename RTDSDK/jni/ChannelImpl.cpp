#include <twilio-jni/JNIEnvAttacher.h>
#include <twilio-jni/tw-jni.h>
#include <twilio-jni/tw-jni-compat.h>

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
#include "TwilioIPMessagingLogger.h"

#define TAG  "ChannelImpl(native)"

/*
 * Class:     com_twilio_ipmessaging_impl_ChannelImpl
 * Method:    joinChannel
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_ChannelImpl_joinChannel
(JNIEnv *env, jobject obj, jlong nativeChannelContext, jstring channel_sid, jobject listener) {

	LOG_DEBUG(TAG,"joinChannel Entered ");
	LOG_DEBUG(TAG, "joinChannel Entered ");
	const char *nativeString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
	ChannelContext *channelContext = reinterpret_cast<ChannelContext *>(nativeChannelContext);

	if(channelContext != nullptr) {
		if(listener!= nullptr) {
			LOG_DEBUG(TAG, "Join channel listener is not null.");
			jobject j_statusListener_ = env->NewGlobalRef(listener);
			jclass cls = (env)->GetObjectClass(j_statusListener_);
			jmethodID j_onSuccess_ = (env)->GetMethodID(cls, "onSuccess", "()V");
			jmethodID j_onError_ = (env)->GetMethodID(cls, "onError", "()V");

			ITMChannelPtr channel = channelContext->channel;
			if(channel != nullptr) {
				LOG_DEBUG(TAG, "Joining channel with sid : %s ", nativeString);
				channel->join([j_statusListener_,j_onSuccess_, j_onError_](TMResult result){
					JNIEnvAttacher jniAttacher;
					if (result == rtd::TMResult::kTMResultSuccess) {
						LOG_DEBUG(TAG, "Join channel is successful. Calling java listener.");
						//Call Java
						jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onSuccess_);
						jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
					} else {
						LOG_WARN(TAG, "Join channel failed.");
						//Call Java
						jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onError_);
						jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
					}
				});
			} else {
				env->DeleteGlobalRef(j_statusListener_);
				LOG_WARN(TAG, "Channel is null");
			}
		} else {
			LOG_WARN(TAG, "Joining Channel. StausListener is null.");
			ITMChannelPtr channel = channelContext->channel;
			if(channel != nullptr) {
				LOG_DEBUG(TAG, "Joining channel with sid : %s ", nativeString);
				channel->join(nullptr);
			} else {
				LOG_DEBUG(TAG, "Channel is null.");
			}
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

	LOG_DEBUG(TAG,"leaveChannel Entered ");
	const char *nativeString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
	ChannelContext *channelContext = reinterpret_cast<ChannelContext *>(nativeChannelContext);

	if(channelContext != nullptr) {
		ITMChannelPtr channel = channelContext->channel;
		if(listener!= nullptr) {

			jobject j_statusListener_ = env->NewGlobalRef(listener);
			jclass cls = (env)->GetObjectClass(j_statusListener_);
			jmethodID j_onSuccess_ = (env)->GetMethodID(cls, "onSuccess", "()V");
			jmethodID j_onError_ = (env)->GetMethodID(cls, "onError", "()V");

			if(channel != nullptr) {
				LOG_DEBUG(TAG,"Leaving channel with sid : %s ", nativeString);
				channel->leave([j_statusListener_,j_onSuccess_, j_onError_](TMResult result){
					JNIEnvAttacher jniAttacher;
					if (result == rtd::TMResult::kTMResultSuccess) {
						LOG_DEBUG(TAG, "leave channel is successful. Calling java listener.");
						//Call Java
						jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onSuccess_);
						jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
					} else {
						LOG_DEBUG(TAG, "leave channel failed");
						//Call Java
						jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onError_);
						jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
					}
				});
			} else {
				env->DeleteGlobalRef(j_statusListener_);
				LOG_WARN(TAG,"channel is null");
			}
		} else {
			LOG_WARN(TAG, "Leaving Channel. StausListener is null.");
			if(channel != nullptr) {
				LOG_DEBUG(TAG,"Leaving channel with sid : %s ", nativeString);
				channel->leave(nullptr);
			} else {
				LOG_WARN(TAG,"channel is null");
			}
		}
	} else {
		LOG_WARN(TAG,"channels is null");
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
	LOG_DEBUG(TAG,"destroyChannel: Entered ");
	const char *nativeString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
	ChannelContext *channelContext = reinterpret_cast<ChannelContext *>(nativeChannelContext);

	if(channelContext != nullptr) {
		ITMChannelPtr channel = channelContext->channel;
		if(listener != nullptr) {
			jobject j_statusListener_ = env->NewGlobalRef(listener);
			jclass cls = (env)->GetObjectClass(j_statusListener_);
			jmethodID j_onSuccess_ = (env)->GetMethodID(cls, "onSuccess", "()V");
			jmethodID j_onError_ = (env)->GetMethodID(cls, "onError", "()V");


			if(channel != nullptr) {
				LOG_WARN(TAG, "Destroying channel with sid : %s ", nativeString);
				channel->destroy([j_statusListener_,j_onSuccess_, j_onError_](TMResult result){
					JNIEnvAttacher jniAttacher;
					if (result == rtd::TMResult::kTMResultSuccess) {
						LOG_DEBUG(TAG, "Destroy channel is successful. Calling java listener.");
						//Call Java
						jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onSuccess_);
						jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
					} else {
						LOG_DEBUG(TAG, "Destroy channel failed");
						if(j_onError_ == NULL) {
							LOG_DEBUG(TAG, "Destroy channel failed j_onError_ is null.");
						}
						//Call Java
						jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onError_);
						jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
					}
				});
			} else {
				env->DeleteGlobalRef(j_statusListener_);
				LOG_WARN(TAG, "channel is null");
			}
		} else {
			LOG_WARN(TAG, "Destroying Channel. StausListener is null.");
			if(channel != nullptr) {
				LOG_WARN(TAG, "Destroying channel with sid : %s ", nativeString);
				channel->destroy(nullptr);
			} else {
				LOG_WARN(TAG, "channel is null");
			}
		}
	} else {
		LOG_WARN(TAG, "channels is null");
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
		ITMChannelPtr channel = channelContext->channel;

		if(listener != nullptr) {
			jobject j_statusListener_ = env->NewGlobalRef(listener);
			jclass cls = (env)->GetObjectClass(j_statusListener_);
			jmethodID j_onSuccess_ = (env)->GetMethodID(cls, "onSuccess", "()V");
			jmethodID j_onError_ = (env)->GetMethodID(cls, "onError", "()V");

			if(channel != nullptr) {
				LOG_WARN("Update Name for channel with sid : %s ", nativeSidString);
				channel->setFriendlyName(nativeNameString, [j_statusListener_,j_onSuccess_, j_onError_](TMResult result){
					JNIEnvAttacher jniAttacher;
					if (result == rtd::TMResult::kTMResultSuccess) {
						LOG_DEBUG(TAG, "Update Name for channel is successful. Calling java listener.");
						//Call Java
						jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onSuccess_);
						jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
					} else {
						LOG_DEBUG(TAG, "Update Name for channel failed");

						//Call Java
						jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onError_);
						jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
					}
				});
			} else {
				env->DeleteGlobalRef(j_statusListener_);
			}
		} else {
			LOG_WARN(TAG, "Setting FriendlyName. StausListener is null.");
			if(channel != nullptr) {
				LOG_DEBUG(TAG, "Update Name for channel with sid : %s ", nativeSidString);
				channel->setFriendlyName(nativeNameString, nullptr);
			} else {
				LOG_DEBUG(TAG, "Channel is null.");
			}
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
		ITMChannelPtr channel = channelContext->channel;

		if(listener != nullptr) {
			jobject j_statusListener_ = env->NewGlobalRef(listener);
			jclass cls = (env)->GetObjectClass(j_statusListener_);
			jmethodID j_onSuccess_ = (env)->GetMethodID(cls, "onSuccess", "()V");
			jmethodID j_onError_ = (env)->GetMethodID(cls, "onError", "()V");

			if(channel != nullptr) {
				LOG_WARN(TAG,"Update channel Type for channel with sid : %s ", nativeSidString);
				if(type == rtd::kTMChannelTypePublic) {
					LOG_DEBUG(TAG, "Setting Channel Type to public");
					channel->setType(rtd::kTMChannelTypePublic, [j_statusListener_,j_onSuccess_, j_onError_](TMResult result){
						JNIEnvAttacher jniAttacher;
						if (result == rtd::TMResult::kTMResultSuccess) {
							LOG_DEBUG(TAG, "SetType channel is successful. Calling java listener.");
							//Call Java
							jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onSuccess_);
							jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
						} else {
							LOG_DEBUG(TAG, "SetType channel failed");

							//Call Java
							jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onError_);
							jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
						}
					});
				} else {
					LOG_DEBUG(TAG, "Setting Channel Type to private");
					channel->setType(rtd::kTMChannelTypePrivate, [j_statusListener_,j_onSuccess_, j_onError_](TMResult result){
						JNIEnvAttacher jniAttacher;
						if (result == rtd::TMResult::kTMResultSuccess) {
							LOG_DEBUG(TAG, "Destroy channel is successful. Calling java listener.");
							//Call Java
							jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onSuccess_);
							jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
						} else {
							LOG_DEBUG(TAG, "Destroy channel failed");

							//Call Java
							jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onError_);
							jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
						}
					});
				}
			} else {
				LOG_DEBUG(TAG, "Channel is null.");
				env->DeleteGlobalRef(j_statusListener_);
			}
		} else {
			LOG_WARN(TAG, "Setting Type. StausListener is null.");
			if(channel != nullptr) {
				LOG_WARN("Update channel Type for channel with sid : %s ", nativeSidString);
				if(type == rtd::kTMChannelTypePublic) {
					LOG_DEBUG(TAG, "Setting Channel Type to public");
					channel->setType(rtd::kTMChannelTypePublic, nullptr);
				} else {
					LOG_DEBUG(TAG, "Setting Channel Type to private");
					channel->setType(rtd::kTMChannelTypePrivate, nullptr);
				}
			} else {
				LOG_DEBUG(TAG, "Channel is null.");
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
  (JNIEnv *env, jobject obj, jlong nativeChannelContext, jstring channel_sid, jstring attrString, jobject listener) {

	const char *nativeSidString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
	const char *nativeAttrString = env->GetStringUTFChars(attrString, JNI_FALSE);
	ChannelContext *channelContext = reinterpret_cast<ChannelContext *>(nativeChannelContext);

	if(channelContext != nullptr) {
		ITMChannelPtr channel = channelContext->channel;
		if(listener != nullptr) {
			jobject j_statusListener_ = env->NewGlobalRef(listener);
			jclass cls = (env)->GetObjectClass(j_statusListener_);
			jmethodID j_onSuccess_ = (env)->GetMethodID(cls, "onSuccess", "()V");
			jmethodID j_onError_ = (env)->GetMethodID(cls, "onError", "()V");

			if(channel != nullptr) {
				LOG_WARN("Update Attribute for channel with sid : %s ", nativeSidString);
				channel->setAttributes(nativeAttrString, [j_statusListener_,j_onSuccess_, j_onError_](TMResult result){
					JNIEnvAttacher jniAttacher;
					if (result == rtd::TMResult::kTMResultSuccess) {
						LOG_DEBUG(TAG, "Update Attribute for channel is successful. Calling java listener.");
						//Call Java
						jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onSuccess_);
						jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
					} else {
						LOG_DEBUG(TAG, "Update Attribute for channel failed");

						//Call Java
						jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onError_);
						jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
					}
				});
			} else {
				env->DeleteGlobalRef(j_statusListener_);
			}
		} else {
			LOG_WARN(TAG, "Setting Attributes.StatusListener is null.");
			if(channel != nullptr) {
				LOG_DEBUG(TAG,"Update Attribute for channel with sid : %s ", nativeSidString);
				channel->setAttributes(nativeAttrString, nullptr);
			} else {
				LOG_ERROR(TAG, "Channel is null.");
			}
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

	LOG_WARN(TAG,"declineChannelInvite app: Entered ");

	const char *nativeSidString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
	ChannelContext *channelContext = reinterpret_cast<ChannelContext *>(nativeChannelContext);

	if(channelContext != nullptr) {
		ITMChannelPtr channel = channelContext->channel;
		if (listener != nullptr) {
			jobject j_statusListener_ = env->NewGlobalRef(listener);
			jclass cls = (env)->GetObjectClass(j_statusListener_);
			jmethodID j_onSuccess_ = (env)->GetMethodID(cls, "onSuccess", "()V");
			jmethodID j_onError_ = (env)->GetMethodID(cls, "onError", "()V");

			if(channel != nullptr) {
				LOG_DEBUG(TAG,"Decline invite for channel with sid : %s ", nativeSidString);
				channel->declineInvitation([j_statusListener_,j_onSuccess_, j_onError_](TMResult result){
					JNIEnvAttacher jniAttacher;
					if (result == rtd::TMResult::kTMResultSuccess) {
						LOG_DEBUG(TAG, "Decline invite for channel is successful. Calling java listener.");
						//Call Java
						jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onSuccess_);
						jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
					} else {
						LOG_DEBUG(TAG, "Decline invite for channel failed");

						//Call Java
						jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onError_);
						jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
					}
				});
			} else {
				env->DeleteGlobalRef(j_statusListener_);
				LOG_WARN(TAG,"declineChannelInvite ELEMENT NOT found");
			}
		} else {
			LOG_WARN(TAG, "declineInvitation : StatusListener passed is null.");
			if(channel != nullptr) {
				LOG_DEBUG(TAG,"Decline invite for channel with sid : %s ", nativeSidString);
				channel->declineInvitation(nullptr);
			} else {
				LOG_WARN(TAG,"declineChannelInvite channel is null.");
			}
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
	LOG_DEBUG(TAG, "Entered getChannelAttributesNative");
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
			LOG_DEBUG(TAG, "Channel getMessagesObject channel with sid %s ", nativeSidString );
			LOG_DEBUG(TAG,"getMessages for channel with sid : %s ", nativeSidString);
			ITMessagesPtr messagesLocal = channel->getMessages();

			while (messagesLocal == nullptr)
			{
			   //LOG_DEBUG(TAG,"messages not available...");
			   Poco::Thread::sleep(1000);
			   messagesLocal = channel->getMessages();
			}

			MessagesContext* messagesContext_ = new MessagesContext();
			messagesContext_->messages = messagesLocal;

			jlong messagesContextHandle = reinterpret_cast<jlong>(messagesContext_);

			jclass java_messages_impl_cls = tw_jni_find_class(env, "com/twilio/ipmessaging/impl/MessagesImpl");
			if(java_messages_impl_cls != NULL) {
				LOG_DEBUG(TAG,"Found java_messages_impl_cls class" );
			}

			jmethodID construct = tw_jni_get_method_by_class(env, java_messages_impl_cls, "<init>", "(J)V");

			messagesObj = tw_jni_new_object(env, java_messages_impl_cls, construct, messagesContextHandle);
			LOG_DEBUG(TAG,"Created messagesObj Object.");


			return messagesObj;

		} else {
			LOG_WARN(TAG,"clientParams_ ELEMENT NOT found");
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
			LOG_WARN(TAG, "Get Members for channel with sid : %s ", nativeSidString);
			ITMMembersPtr membersLocal = channel->getMembers();
			while (membersLocal == nullptr)
			{
				LOG_WARN(TAG,"app: members not available...");
				Poco::Thread::sleep(1000);
				membersLocal = channel->getMembers();
			}

			MembersContext* membersContext_ = new MembersContext();
			membersContext_->members = membersLocal;

			jlong membersContextHandle = reinterpret_cast<jlong>(membersContext_);

			jclass java_members_impl_cls = tw_jni_find_class(env, "com/twilio/ipmessaging/impl/MembersImpl");
			if(java_members_impl_cls != NULL) {
				LOG_WARN(TAG,"Found java_members_impl_cls class" );
			}

			jmethodID construct = tw_jni_get_method_by_class(env, java_members_impl_cls, "<init>", "(J)V");

			membersObj = tw_jni_new_object(env, java_members_impl_cls, construct, membersContextHandle);
			LOG_WARN(TAG,"Created messagesObj Object.");

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
	LOG_DEBUG(TAG,"getStatus Entered ");
	const char *nativeString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
	ChannelContext *channelContext = reinterpret_cast<ChannelContext *>(nativeChannelContext);

	if(channelContext != nullptr) {
		ITMChannelPtr channel = channelContext->channel;
		if(channel != nullptr) {
			LOG_DEBUG(TAG,"GetStatus channel with sid : %s ", nativeString);
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
			LOG_WARN(TAG,"channel is null");
		}
	}
	env->ReleaseStringUTFChars(channel_sid, nativeString);
}


/*
 * Class:     com_twilio_ipmessaging_impl_ChannelImpl
 * Method:    updateChannelName
 * Signature: (JLjava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_ChannelImpl_updateUniqueName
  (JNIEnv *env, jobject obj, jlong nativeChannelContext, jstring channel_sid, jstring modifiedChannelName, jobject listener) {

	const char *nativeNameString =  env->GetStringUTFChars(modifiedChannelName, JNI_FALSE);
	const char *nativeSidString = env->GetStringUTFChars(channel_sid, JNI_FALSE);
	ChannelContext *channelContext = reinterpret_cast<ChannelContext *>(nativeChannelContext);

	if(channelContext != nullptr) {
		ITMChannelPtr channel = channelContext->channel;
		if(listener != nullptr) {
			jobject j_statusListener_ = env->NewGlobalRef(listener);
			jmethodID j_onSuccess_ = tw_jni_get_method(env, j_statusListener_, "onSuccess", "()V");
			jmethodID j_onError_ = tw_jni_get_method(env, j_statusListener_, "onError", "()V");

			if(channel != nullptr) {
				LOG_DEBUG("Update Unique Name for channel with sid : %s ", nativeSidString);
				channel->setUniqueName(nativeNameString, [j_statusListener_,j_onSuccess_, j_onError_](TMResult result){
					JNIEnvAttacher jniAttacher;
					if (result == rtd::TMResult::kTMResultSuccess) {
						LOG_DEBUG(TAG, "Update unique name for channel is successful. Calling java listener.");
						//Call Java
						jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onSuccess_);
						jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
					} else {
						LOG_DEBUG(TAG, "Update Unique Name for channel failed");
						//Call Java
						jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onError_);
						jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
					}
				});
			} else {
				env->DeleteGlobalRef(j_statusListener_);
			}
		} else {
			LOG_WARN(TAG, "Setting UniqueName. StatusListener is null.");
			if(channel != nullptr) {
				LOG_DEBUG("Update Unique Name for channel with sid : %s ", nativeSidString);
				channel->setUniqueName(nativeNameString, nullptr);
			}
		}
	}
	env->ReleaseStringUTFChars(channel_sid, nativeSidString);
	env->ReleaseStringUTFChars(channel_sid, nativeNameString);
}


/*
 * Class:     com_twilio_ipmessaging_impl_ChannelImpl
 * Method:    getUniqueName
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_twilio_ipmessaging_impl_ChannelImpl_getUniqueName
(JNIEnv *env, jobject obj, jlong nativeChannelContext) {
	LOG_DEBUG(TAG, "Entered getUniqueName");
	ChannelContext *clientChannelContext = reinterpret_cast<ChannelContext *>(nativeChannelContext);
	jstring nameString;
	if(clientChannelContext != nullptr) {
		ITMChannelPtr channel = clientChannelContext->channel;
		if(channel != nullptr) {
			const char* name = channel->getUniqueName().c_str();
			LOG_DEBUG(TAG, "Retrieved unique name|%s|", name);
			nameString = env->NewStringUTF(name);
		}
	}
	return nameString;
}

