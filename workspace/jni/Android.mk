TWRTDSDK_JNI_PATH := $(call my-dir)

LOCAL_PATH := $(TWRTDSDK_JNI_PATH)

# RTDMessaging

include $(CLEAR_VARS)

LOCAL_MODULE := RTDMessaging
LOCAL_SRC_FILES := libRTDIPMessagingLib.a
	
LOCAL_EXPORT_C_INCLUDES := \
		$(LOCAL_PATH)/datasync-client-core/include \
		$(LOCAL_PATH)/ip-messaging-lib/include

include $(PREBUILT_STATIC_LIBRARY)


# RTDDataSync

include $(CLEAR_VARS)

LOCAL_MODULE := RTDDataSync
LOCAL_SRC_FILES := libRTDDataSyncLib.a
	
LOCAL_EXPORT_C_INCLUDES := \
		$(LOCAL_PATH)/datasync-client-core/include/Common \
		$(LOCAL_PATH)/datasync-client-core/include/DataSync \

include $(PREBUILT_STATIC_LIBRARY)

# RTDNotification

include $(CLEAR_VARS)

LOCAL_MODULE := RTDNotification
LOCAL_SRC_FILES := libRTDNotificationLib.a
	
LOCAL_EXPORT_C_INCLUDES := \
		$(LOCAL_PATH)/datasync-client-core/include/Common \
		$(LOCAL_PATH)/datasync-client-core/include/Notification \

include $(PREBUILT_STATIC_LIBRARY)

# twilio-rtd-native

include $(CLEAR_VARS)

LOCAL_MODULE := twilio-rtd-native	
LOCAL_CPPFLAGS := -std=gnu++11 -fno-rtti
LOCAL_LDLIBS    := -llog 

LOCAL_SRC_FILES := \
	TestRTDJNI.cpp


LOCAL_STATIC_LIBRARIES := \
	RTDMessaging \
	RTDNotification \
	RTDDataSyncLib

	
include $(BUILD_SHARED_LIBRARY)
