LOCAL_PATH := $(call my-dir)


# OpenSSL
include $(CLEAR_VARS)
LOCAL_MODULE := OpenSSL
LOCAL_SRC_FILES := $(LOCAL_PATH)/openssl/lib/libssl.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/openssl/include
include $(PREBUILT_STATIC_LIBRARY)

# OpenSSLCrypto
include $(CLEAR_VARS)
LOCAL_MODULE := OpenSSLCrypto
LOCAL_SRC_FILES := $(LOCAL_PATH)/openssl/lib/libcrypto.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/openssl/include
include $(PREBUILT_STATIC_LIBRARY)

# POCO Foundation
include $(CLEAR_VARS)
LOCAL_MODULE := PocoFoundation
LOCAL_SRC_FILES := libPocoFoundation.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/poco/include
include $(PREBUILT_STATIC_LIBRARY)

# POCO Net
include $(CLEAR_VARS)
LOCAL_MODULE := PocoNet
LOCAL_SRC_FILES := libPocoNet.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/poco/include
include $(PREBUILT_STATIC_LIBRARY)

#POCO Crypto
include $(CLEAR_VARS)
LOCAL_MODULE := PocoCrypto
LOCAL_SRC_FILES := libPocoCrypto.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/poco/include
include $(PREBUILT_STATIC_LIBRARY)

# POCO Net SSL
LOCAL_MODULE := PocoNetSSL
LOCAL_SRC_FILES := libPocoNetSSL.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/poco/include
include $(PREBUILT_STATIC_LIBRARY)

# POCO Util
include $(CLEAR_VARS)
LOCAL_MODULE := PocoUtil
LOCAL_SRC_FILES := libPocoUtil.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/poco/include
include $(PREBUILT_STATIC_LIBRARY)

# POCO XML
include $(CLEAR_VARS)
LOCAL_MODULE := PocoXML
LOCAL_SRC_FILES := libPocoXML.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/poco/include
include $(PREBUILT_STATIC_LIBRARY)

# POCO JSON
include $(CLEAR_VARS)
LOCAL_MODULE := PocoJSON
LOCAL_SRC_FILES := libPocoJSON.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/poco/include
include $(PREBUILT_STATIC_LIBRARY)

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
		$(LOCAL_PATH)/datasync-client-core/include/DataSync 

include $(PREBUILT_STATIC_LIBRARY)

# RTDNotification

include $(CLEAR_VARS)

LOCAL_MODULE := RTDNotification
LOCAL_SRC_FILES := libRTDNotificationLib.a
	
LOCAL_EXPORT_C_INCLUDES := \
		$(LOCAL_PATH)/datasync-client-core/include/Common \
		$(LOCAL_PATH)/datasync-client-core/include/Notification 

include $(PREBUILT_STATIC_LIBRARY)

# twilio-rtd-native

include $(CLEAR_VARS)

LOCAL_MODULE := twilio-rtd-native	
LOCAL_CPPFLAGS := -std=gnu++11 -frtti
	
LOCAL_LDLIBS    := -llog 
#-L$(LOCAL_PATH)/poco/include/Poco/
#-ld -l$(MY_STATIC_LIBS)
				
LOCAL_SRC_FILES := \
	TestRTDJNI.cpp \
	TwilioIPMessagingConfigurationProvider.cpp \
	TwilioIPMessagingNotificationClientListener.cpp \
	TwilioIPMessagingClientListener.cpp

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
