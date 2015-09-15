#include "com_twilio_ipmessaging_impl_MembersImpl.h"
#include "TwilioIPMessagingClientContextDefines.h"

#include <twilio-jni/twilio-jni.h>
#include <android/log.h>


JNIEXPORT jobjectArray JNICALL Java_com_twilio_ipmessaging_impl_MembersImpl_getMembersNative
  (JNIEnv *env, jobject obj) {

	jobject member;

	jlong nativeMembersContext = tw_jni_fetch_long(env, obj, "nativeMembersHandler");

	LOGW( "Java_com_twilio_ipmessaging_impl_MembersImpl_getMembersNative : Checking nativeMessagesContext.");

	if (nativeMembersContext == 0) {
			LOGW("nativeMembersContext is null");
			return nullptr;
	} else {

		MembersContext *membersContextRecreate = reinterpret_cast<MembersContext *>(nativeMembersContext);

		__android_log_print(ANDROID_LOG_ERROR, TAG, "client context is recreated.");

		if(membersContextRecreate == nullptr) {
			LOGW( "Java_com_twilio_ipmessaging_impl_MembersImpl_getMembersNative : membersContextRecreate is NULL.");
			return nullptr;
		}

		if(membersContextRecreate->members == nullptr) {
			LOGW( "Java_com_twilio_ipmessaging_impl_MembersImpl_getMembersNative : ITMMembers is NULL.");
			return 0;
		}

		ITMMembersPtr members = membersContextRecreate->members;

		std::vector<ITMMemberPtr> memberList;
		members->getMembersList(memberList);

		jclass java_member_impl_cls = tw_jni_find_class(env, "com/twilio/ipmessaging/impl/MemberImpl");

		if(java_member_impl_cls != NULL) {
			LOGW("Found java_member_impl_cls class" );
		}

		jmethodID construct = tw_jni_get_method_by_class(env, java_member_impl_cls, "<init>", "(Ljava/lang/String;Ljava/lang/String;)V");
		jobjectArray memberArray = (jobjectArray) env->NewObjectArray(memberList.size(),java_member_impl_cls, 0);

		for (int i= 0; i< memberList.size() ; i++ ) {
			ITMMemberPtr memberPtr = memberList[i];

			const char* sid = memberPtr->getSid().c_str();
			const char* name = memberPtr->getUsername().c_str();

			LOGW("Name  : %s.", name );
			LOGW("sid is %s", sid);

			jstring nameString = env->NewStringUTF(name);
			jstring sidString = env->NewStringUTF(sid);

			member = tw_jni_new_object(env, java_member_impl_cls, construct, sidString, nameString);
			LOGW("Created Member Object.");
			env->SetObjectArrayElement(memberArray, i, member);
			LOGW("Added member object to array");
		}

		if(memberArray != NULL) {
			LOGW("messagesArray is NOT NULL ");
		} else {
			LOGW("messagesArray is  NULL *********");
		}

		return memberArray;
	}

	return nullptr;
}
