#include "MembersImpl.h"
#include "TwilioIPMessagingClientContextDefines.h"
#include "TwilioIPMessagingLogger.h"

#include <twilio-jni/JNIEnvAttacher.h>
#include <twilio-jni/tw-jni.h>
#include <twilio-jni/tw-jni-compat.h>
#include <android/log.h>
#include "talk/app/webrtc/java/jni/jni_helpers.h"

#define TAG  "MembersImpl(native)"

using namespace webrtc_jni;

JNIEXPORT jobjectArray JNICALL Java_com_twilio_ipmessaging_impl_MembersImpl_getMembersNative
  (JNIEnv *env, jobject obj, jlong nativeMembersContext) {

	jobject member;

	LOG_DEBUG(TAG,"Java_com_twilio_ipmessaging_impl_MembersImpl_getMembersNative : Checking nativeMessagesContext.");

	if (nativeMembersContext == 0) {
		LOG_WARN(TAG,"nativeMembersContext is null");
		return nullptr;
	} else {

		MembersContext *membersContextRecreate = reinterpret_cast<MembersContext *>(nativeMembersContext);

		LOG_DEBUG(TAG, "client context is recreated.");

		if (membersContextRecreate == nullptr) {
			LOG_WARN(TAG,"Java_com_twilio_ipmessaging_impl_MembersImpl_getMembersNative : membersContextRecreate is NULL.");
			return nullptr;
		}

		if(membersContextRecreate->members == nullptr) {
			LOG_DEBUG(TAG,"Java_com_twilio_ipmessaging_impl_MembersImpl_getMembersNative : ITMMembers is NULL.");
			return 0;
		}

		ITMMembersPtr members = membersContextRecreate->members;

		std::vector<ITMMemberPtr> memberList;
		members->getMembersList(memberList);

		jclass java_member_impl_cls = tw_jni_find_class(env, "com/twilio/ipmessaging/impl/MemberImpl");

		if(java_member_impl_cls != NULL) {
			LOG_DEBUG(TAG,"Found java_member_impl_cls class" );
		}

		jmethodID construct = tw_jni_get_method_by_class(env, java_member_impl_cls, "<init>", "(Ljava/lang/String;Ljava/lang/String;J)V");
		jobjectArray memberArray = (jobjectArray) env->NewObjectArray(memberList.size(),java_member_impl_cls, 0);

		for (int i= 0; i< memberList.size() ; i++ ) {
			ITMMemberPtr memberPtr = memberList[i];

			MemberContext* memberContext_ = new MemberContext();
			memberContext_->member = memberPtr;
			jlong messageContextHandle = reinterpret_cast<jlong>(memberContext_);
			std::string sid = memberPtr->getSid();
			std::string name = memberPtr->getUsername();

			LOG_DEBUG(TAG,"Name  : %s.", name.c_str() );
			LOG_DEBUG(TAG,"sid is %s", sid.c_str());
			jstring nameString = JavaStringFromStdString(env, name);
			jstring sidString = JavaStringFromStdString(env, sid);
			member = tw_jni_new_object(env, java_member_impl_cls, construct, sidString, nameString, messageContextHandle);
			LOG_DEBUG(TAG,"Created Member Object.");
			env->SetObjectArrayElement(memberArray, i, member);
			LOG_DEBUG(TAG,"Added member object to array");
		}
		return memberArray;
	}

	return nullptr;
}


/*
 * Class:     com_twilio_ipmessaging_impl_MembersImpl
 * Method:    invite
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_MembersImpl_invite
  (JNIEnv *env, jobject obj, jstring memberIdentity, jlong nativeMembersContext, jobject listener) {

	LOG_DEBUG(TAG, "invite : Entered.");

	const char *nativeString = env->GetStringUTFChars(memberIdentity, JNI_FALSE);

	if (nativeMembersContext == 0) {
		LOG_DEBUG(TAG, "nativeMembersContext is null.");
		return;
	} else {
		MembersContext *membersContextRecreate = reinterpret_cast<MembersContext *>(nativeMembersContext);
		if(listener != nullptr) {
			LOG_DEBUG(TAG, "client context is recreated.");
			jobject j_statusListener_ = env->NewGlobalRef(listener);
			jclass cls = (env)->GetObjectClass(j_statusListener_);
			jmethodID j_onSuccess_ = (env)->GetMethodID(cls, "onSuccess", "()V");
			jmethodID j_onError_ = (env)->GetMethodID(cls, "onError", "()V");

			if(membersContextRecreate != nullptr && membersContextRecreate->members != nullptr) {

				ITMMembersPtr members = membersContextRecreate->members;
				ITMMemberPtr memberPtr = members->createMember(nativeString);
				members->invite(memberPtr, [j_statusListener_,j_onSuccess_, j_onError_](TMResult result){
					JNIEnvAttacher jniAttacher;
					if (result == rtd::TMResult::kTMResultSuccess) {
						LOG_DEBUG(TAG, "Invite sent successfully. Calling java listener.");
						//Call Java
						if(j_statusListener_) {
							jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onSuccess_);
							jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
						}
					} else {
						LOG_DEBUG(TAG, "Invite resulted in error.");
						//Call Java
						if(j_statusListener_) {
							jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onError_);
							jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
						}
					}
				});
			} else {
				if(j_statusListener_) {
					env->DeleteGlobalRef(j_statusListener_);
					j_statusListener_ = NULL;
				}
			}
		} else {
			LOG_DEBUG(TAG, "Sending Invite. StatusListener is set to null.");
			ITMMembersPtr members = membersContextRecreate->members;
			ITMMemberPtr memberPtr = members->createMember(nativeString);
			members->invite(memberPtr, nullptr);
		}
	}

	env->ReleaseStringUTFChars(memberIdentity, nativeString);
}

/*
 * Class:     com_twilio_ipmessaging_impl_MembersImpl
 * Method:    add
 * Signature: (Lcom/twilio/ipmessaging/Member;)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_MembersImpl_add
  (JNIEnv *env, jobject obj, jstring memberIdentity, jlong nativeMembersContext, jobject listener) {

	LOG_DEBUG(TAG,"add member : Entered.");
	const char *nativeString = env->GetStringUTFChars(memberIdentity, JNI_FALSE);

	if (nativeMembersContext == 0) {
		LOG_WARN(TAG,"nativeMembersContext is null");
		return;
	} else {

		MembersContext *membersContextRecreate = reinterpret_cast<MembersContext *>(nativeMembersContext);

		if(membersContextRecreate != nullptr) {
			if(listener != nullptr) {
				jobject j_statusListener_ = env->NewGlobalRef(listener);
				jclass cls = (env)->GetObjectClass(j_statusListener_);
				jmethodID j_onSuccess_ = (env)->GetMethodID(cls, "onSuccess", "()V");
				jmethodID j_onError_ = (env)->GetMethodID(cls, "onError", "()V");

				if(membersContextRecreate->members != nullptr) {
					ITMMembersPtr members = membersContextRecreate->members;
					ITMMemberPtr memberPtr = members->createMember(nativeString);
					members->add(memberPtr, [j_statusListener_,j_onSuccess_, j_onError_](TMResult result){
						JNIEnvAttacher jniAttacher;
						if (result == rtd::TMResult::kTMResultSuccess) {
							LOG_DEBUG(TAG, "member add was successful. Calling java listener.");
							//Call Java
							if(j_statusListener_ != nullptr) {
								jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onSuccess_);
								jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
							}
						} else {
							LOG_WARN(TAG, "member add failed. Calling java listener.");
							//Call Java
							if(j_statusListener_ != nullptr) {
								jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onError_);
								jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
							}
						}
					});
				} else {
					if(j_statusListener_ != nullptr) {
						env->DeleteGlobalRef(j_statusListener_);
						j_statusListener_ = nullptr;
					}
				}
			} else {
				LOG_DEBUG(TAG, "Adding member. StatusListener is set to null.");
				if(membersContextRecreate->members != nullptr) {
					ITMMembersPtr members = membersContextRecreate->members;
					ITMMemberPtr memberPtr = members->createMember(nativeString);
					members->add(memberPtr, nullptr);
				}
			}
		}
	}
}

/*
 * Class:     com_twilio_ipmessaging_impl_MembersImpl
 * Method:    remove
 * Signature: (Lcom/twilio/ipmessaging/Member;)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_MembersImpl_remove
  (JNIEnv *env, jobject obj, jlong nativeMemberContext, jlong nativeMembersContext, jobject listener) {

	LOG_DEBUG(TAG,"Remove member : Entered.");

	if (nativeMembersContext == 0) {
		LOG_WARN(TAG,"nativeMembersContext is null");
		return;
	} else {
		MembersContext *membersContextRecreate = reinterpret_cast<MembersContext *>(nativeMembersContext);
		if(membersContextRecreate != nullptr) {
			MemberContext *memberContextRecreate = reinterpret_cast<MemberContext *>(nativeMemberContext);
			if(listener != nullptr) {
				jobject j_statusListener_ = env->NewGlobalRef(listener);
				jclass cls = (env)->GetObjectClass(j_statusListener_);
				jmethodID j_onSuccess_ = (env)->GetMethodID(cls, "onSuccess", "()V");
				jmethodID j_onError_ = (env)->GetMethodID(cls, "onError", "()V");

				LOG_DEBUG(TAG,"memberContext is recreated.");
				if(memberContextRecreate->member != nullptr) {
					ITMMembersPtr members = membersContextRecreate->members;
					ITMMemberPtr memberPtr = memberContextRecreate->member;
					members->remove(memberPtr, [j_statusListener_,j_onSuccess_, j_onError_](TMResult result){
						JNIEnvAttacher jniAttacher;
						if (result == rtd::TMResult::kTMResultSuccess) {
							LOG_DEBUG(TAG, "member remove was successful. Calling java listener.");
							//Call Java
							if(j_statusListener_) {
								jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onSuccess_);
								jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
							}
						} else {
							LOG_DEBUG(TAG, "member remove failed. Calling java listener.");
							//Call Java
							if(j_statusListener_) {
								jniAttacher.get()->CallVoidMethod(j_statusListener_,j_onError_);
								jniAttacher.get()->DeleteGlobalRef(j_statusListener_);
							}
						}
					});
				} else {
					if(j_statusListener_) {
						env->DeleteGlobalRef(j_statusListener_);
						j_statusListener_ = NULL;
					}
				}
			} else {
				LOG_WARN(TAG, "Removing member. StatusListener is set to null.");
				if(membersContextRecreate->members != nullptr) {
					ITMMembersPtr members = membersContextRecreate->members;
					ITMMemberPtr memberPtr = memberContextRecreate->member;
					members->remove(memberPtr, nullptr);
				}
			}
		}
	}
}


