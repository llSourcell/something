#include "MembersImpl.h"
#include "TwilioIPMessagingClientContextDefines.h"

#include <twilio-jni/twilio-jni.h>
#include <android/log.h>

#define TAG  "MembersImpl(native)"

JNIEXPORT jobjectArray JNICALL Java_com_twilio_ipmessaging_impl_MembersImpl_getMembersNative
  (JNIEnv *env, jobject obj, jlong nativeMembersContext) {

	jobject member;

	//jlong nativeMembersContext = tw_jni_fetch_long(env, obj, "nativeMembersHandler");

	LOGD(TAG,"Java_com_twilio_ipmessaging_impl_MembersImpl_getMembersNative : Checking nativeMessagesContext.");

	if (nativeMembersContext == 0) {
			LOGW(TAG,"nativeMembersContext is null");
			return nullptr;
	} else {

		MembersContext *membersContextRecreate = reinterpret_cast<MembersContext *>(nativeMembersContext);

		LOGD(TAG, "client context is recreated.");

		if(membersContextRecreate == nullptr) {
			LOGW(TAG,"Java_com_twilio_ipmessaging_impl_MembersImpl_getMembersNative : membersContextRecreate is NULL.");
			return nullptr;
		}

		if(membersContextRecreate->members == nullptr) {
			LOGD(TAG,"Java_com_twilio_ipmessaging_impl_MembersImpl_getMembersNative : ITMMembers is NULL.");
			return 0;
		}

		ITMMembersPtr members = membersContextRecreate->members;

		std::vector<ITMMemberPtr> memberList;
		members->getMembersList(memberList);

		jclass java_member_impl_cls = tw_jni_find_class(env, "com/twilio/ipmessaging/impl/MemberImpl");

		if(java_member_impl_cls != NULL) {
			LOGD(TAG,"Found java_member_impl_cls class" );
		}

		jmethodID construct = tw_jni_get_method_by_class(env, java_member_impl_cls, "<init>", "(Ljava/lang/String;Ljava/lang/String;J)V");
		jobjectArray memberArray = (jobjectArray) env->NewObjectArray(memberList.size(),java_member_impl_cls, 0);

		for (int i= 0; i< memberList.size() ; i++ ) {
			ITMMemberPtr memberPtr = memberList[i];

			MemberContext* memberContext_ = new MemberContext();
			memberContext_->member = memberPtr;
			jlong messageContextHandle = reinterpret_cast<jlong>(memberContext_);
			const char* sid = memberPtr->getSid().c_str();
			const char* name = memberPtr->getUsername().c_str();
			LOGD(TAG,"Name  : %s.", name );
			LOGD(TAG,"sid is %s", sid);
			jstring nameString = env->NewStringUTF(name);
			jstring sidString = env->NewStringUTF(sid);
			member = tw_jni_new_object(env, java_member_impl_cls, construct, sidString, nameString, messageContextHandle);
			LOGD(TAG,"Created Member Object.");
			env->SetObjectArrayElement(memberArray, i, member);
			LOGD(TAG,"Added member object to array");
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
  (JNIEnv *env, jobject obj, jstring memberIdentity, jlong nativeMembersContext) {

	LOGD(TAG, "invite : Entered.");

	const char *nativeString = env->GetStringUTFChars(memberIdentity, JNI_FALSE);
	//jlong nativeMembersContext = tw_jni_fetch_long(env, obj, "nativeMembersHandler");

	if (nativeMembersContext == 0) {
		LOGD(TAG,"nativeMembersContext is null");
		return;
	} else {

		MembersContext *membersContextRecreate = reinterpret_cast<MembersContext *>(nativeMembersContext);
		LOGD(TAG, "client context is recreated.");

		if(membersContextRecreate == nullptr) {
			LOGW(TAG,"invite : membersContextRecreate is NULL.");
			return;
		}

		if(membersContextRecreate->members == nullptr) {
			LOGW(TAG,"invite : ITMMembers is NULL.");
			return;
		}

		ITMMembersPtr members = membersContextRecreate->members;
		ITMMemberPtr memberPtr = members->createMember(nativeString);
		members->invite(memberPtr, [](TMResult result) {LOGD(TAG,"Members invite command processed");});

		LOGD(TAG,"Invite sent.");
	}

	env->ReleaseStringUTFChars(memberIdentity, nativeString);
}

/*
 * Class:     com_twilio_ipmessaging_impl_MembersImpl
 * Method:    add
 * Signature: (Lcom/twilio/ipmessaging/Member;)V
 */
JNIEXPORT void JNICALL Java_com_twilio_ipmessaging_impl_MembersImpl_add
  (JNIEnv *env, jobject obj, jstring memberIdentity, jlong nativeMembersContext) {

	LOGD(TAG,"add member : Entered.");
	//jlong nativeMembersContext = tw_jni_fetch_long(env, obj, "nativeMembersHandler");
	const char *nativeString = env->GetStringUTFChars(memberIdentity, JNI_FALSE);

	if (nativeMembersContext == 0) {
		LOGW(TAG,"nativeMembersContext is null");
		return;
	} else {

		MembersContext *membersContextRecreate = reinterpret_cast<MembersContext *>(nativeMembersContext);

		if(membersContextRecreate != nullptr) {
			if(membersContextRecreate->members == nullptr) {
				LOGW(TAG, "Member add : ITMMembers is NULL.");
				return;
			} else {
				ITMMembersPtr members = membersContextRecreate->members;
				ITMMemberPtr memberPtr = members->createMember(nativeString);
				members->add(memberPtr, [](TMResult result) {LOGD(TAG,"Members add command processed");});
				LOGD(TAG,"add : Member added.");
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
  (JNIEnv *env, jobject obj, jlong nativeMemberContext, jlong nativeMembersContext) {

	LOGD(TAG,"Remove member : Entered.");
	//jlong nativeMembersContext = tw_jni_fetch_long(env, obj, "nativeMembersHandler");

	if (nativeMembersContext == 0) {
		LOGW(TAG,"nativeMembersContext is null");
		return;
	} else {

		MembersContext *membersContextRecreate = reinterpret_cast<MembersContext *>(nativeMembersContext);

		if(membersContextRecreate != nullptr) {

			//jlong nativeMemberContext = tw_jni_fetch_long(env, memberObj, "nativeMemberHandler");

			if (nativeMemberContext == 0) {
				LOGW(TAG, "nativeMemberContext is null");
				return;
			} else {

				MemberContext *memberContextRecreate = reinterpret_cast<MemberContext *>(nativeMemberContext);
				LOGD(TAG,"memberContext is recreated.");

				if(memberContextRecreate == nullptr) {
					LOGW(TAG,"membersContextRecreate is NULL.");
					return;
				}

				if(memberContextRecreate->member == nullptr) {
					LOGW(TAG,"ITMMembers is NULL.");
					return;
				}

				ITMMembersPtr members = membersContextRecreate->members;
				ITMMemberPtr memberPtr = memberContextRecreate->member;
				members->remove(memberPtr, [](TMResult result) {LOGD(TAG,"Members remove command processed");});
			}
		}
	}
}


