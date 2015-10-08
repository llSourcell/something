TWSDK_JNI_PATH := $(call my-dir)

include $(TWSDK_JNI_PATH)/../external/twilio-jni/Android.mk

LOCAL_PATH := $(TWSDK_JNI_PATH)
LOCAL_PREBUILT_ROOT = $(TWSDK_JNI_PATH)/../target/android
TOOLCHAIN_PLAT = darwin-x86_64

# OpenSSL
include $(CLEAR_VARS)
LOCAL_MODULE := OpenSSL
LOCAL_SRC_FILES := $(LOCAL_PREBUILT_ROOT)/openssl/lib/release/armv7/libssl.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PREBUILT_ROOT)/openssl/include
include $(PREBUILT_STATIC_LIBRARY)

# OpenSSLCrypto
include $(CLEAR_VARS)
LOCAL_MODULE := OpenSSLCrypto
LOCAL_SRC_FILES := $(LOCAL_PREBUILT_ROOT)/openssl/lib/release/armv7/libcrypto.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PREBUILT_ROOT)/openssl/include
include $(PREBUILT_STATIC_LIBRARY)

# POCO Foundation
include $(CLEAR_VARS)
LOCAL_MODULE := PocoFoundation 
LOCAL_SRC_FILES := $(LOCAL_PREBUILT_ROOT)/poco/lib/release/armv7/libPocoFoundation.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PREBUILT_ROOT)/poco/include
include $(PREBUILT_STATIC_LIBRARY)

# POCO Net
include $(CLEAR_VARS)
LOCAL_MODULE := PocoNet
LOCAL_SRC_FILES := $(LOCAL_PREBUILT_ROOT)/poco/lib/release/armv7/libPocoNet.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PREBUILT_ROOT)/poco/include
include $(PREBUILT_STATIC_LIBRARY)

#POCO Crypto
include $(CLEAR_VARS)
LOCAL_MODULE := PocoCrypto
LOCAL_SRC_FILES := $(LOCAL_PREBUILT_ROOT)/poco/lib/release/armv7/libPocoCrypto.a
LOCAL_EXPORT_C_INCLUDES := $(LOCALOCAL_PREBUILT_ROOTL_PATH)/poco/include
include $(PREBUILT_STATIC_LIBRARY)

# POCO Net SSL
LOCAL_MODULE := PocoNetSSL
LOCAL_SRC_FILES := $(LOCAL_PREBUILT_ROOT)/poco/lib/release/armv7/libPocoNetSSL.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PREBUILT_ROOT)/poco/include
include $(PREBUILT_STATIC_LIBRARY)

# POCO Util
include $(CLEAR_VARS)
LOCAL_MODULE := PocoUtil
LOCAL_SRC_FILES := $(LOCAL_PREBUILT_ROOT)/poco/lib/release/armv7/libPocoUtil.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PREBUILT_ROOT)/poco/include
include $(PREBUILT_STATIC_LIBRARY)

# POCO XML
include $(CLEAR_VARS)
LOCAL_MODULE := PocoXML
LOCAL_SRC_FILES := $(LOCAL_PREBUILT_ROOT)/poco/lib/release/armv7/libPocoXML.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PREBUILT_ROOT)/poco/include
include $(PREBUILT_STATIC_LIBRARY)

# POCO JSON
include $(CLEAR_VARS)
LOCAL_MODULE := PocoJSON
LOCAL_SRC_FILES := $(LOCAL_PREBUILT_ROOT)/poco/lib/release/armv7/libPocoJSON.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PREBUILT_ROOT)/poco/include
include $(PREBUILT_STATIC_LIBRARY)

# RTDMessaging

include $(CLEAR_VARS)

LOCAL_MODULE := RTDMessaging
LOCAL_SRC_FILES := $(LOCAL_PREBUILT_ROOT)/ip-messaging-lib/lib/release/armv7/libRTDIPMessagingLib.a
	
LOCAL_EXPORT_C_INCLUDES := \
		$(LOCAL_PREBUILT_ROOT)/datasync-client-core/include \
		$(LOCAL_PREBUILT_ROOT)/ip-messaging-lib/include

include $(PREBUILT_STATIC_LIBRARY)


# RTDDataSync

include $(CLEAR_VARS)

LOCAL_MODULE := RTDDataSync
LOCAL_SRC_FILES := $(LOCAL_PREBUILT_ROOT)/datasync-client-core/lib/release/armv7/libRTDDataSyncLib.a
	
LOCAL_EXPORT_C_INCLUDES := \
		$(LOCAL_PREBUILT_ROOT)/datasync-client-core/include/Common \
		$(LOCAL_PREBUILT_ROOT)/datasync-client-core/include/DataSync 

include $(PREBUILT_STATIC_LIBRARY)

# RTDNotification

include $(CLEAR_VARS)

LOCAL_MODULE := RTDNotification
LOCAL_SRC_FILES := $(LOCAL_PREBUILT_ROOT)/datasync-client-core/lib/release/armv7/libRTDNotificationLib.a
	
LOCAL_EXPORT_C_INCLUDES := \
		$(LOCAL_PREBUILT_ROOT)/datasync-client-core/include/Common \
		$(LOCAL_PREBUILT_ROOT)/datasync-client-core/include/Notification 

include $(PREBUILT_STATIC_LIBRARY)


# twilio-rtd-native

include $(CLEAR_VARS)


LOCAL_MODULE := twilio-rtd-native	
LOCAL_CPPFLAGS := -std=gnu++11 -frtti
	
LOCAL_LDLIBS    := -llog 
				
LOCAL_SRC_FILES := \
	TwilioIPMessagingConfigurationProvider.cpp \
	TwilioIPMessagingNotificationClientListener.cpp \
	TwilioIPMessagingClientListener.cpp \
	IPMessagingClientImpl.cpp \
	MessagesImpl.cpp \
	ChannelsImpl.cpp \
	ChannelImpl.cpp \
	MembersImpl.cpp \
	MessageImpl.cpp 
	
LOCAL_STATIC_LIBRARIES := \
    RTDMessaging \
	RTDNotification \
	RTDDataSync\
	PocoNet \
	PocoNetSSL \
	PocoCrypto \
	PocoUtil \
	PocoXML \
	PocoJSON \
	PocoFoundation \
	OpenSSL \
	OpenSSLCrypto \
	twilio-jni 
	
	
	
include $(BUILD_SHARED_LIBRARY)
