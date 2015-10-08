#include "ITMClient.h"
#include "ITMChannels.h"
#include "ITMChannel.h"
#include "ITMessages.h"
#include "ITMessage.h"
#include "ITMMembers.h"
#include "ITMMember.h"

#include "TwilioIPMessagingConfigurationProvider.h"
#include "TwilioIPMessagingNotificationClientListener.h"
#include "TwilioIPMessagingClientListener.h"

#define LOGD(tag, fmt, ...)  _tw_jni_log_printer(__func__, __LINE__, TW_LOG_LEVEL_DEBUG, tag, fmt, ##__VA_ARGS__)
//#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__))
#define LOGW(tag, fmt, ...)  _tw_jni_log_printer(__func__, __LINE__, TW_LOG_LEVEL_WARN, tag, fmt, ##__VA_ARGS__)
//(...) ((void)__android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__))

using namespace rtd;

// structure with all the shared pointer
typedef struct IPMessagingClientContext {
	std::shared_ptr<TwilioIPMessagingNotificationClientListener>  notificationClientObserver;
	std::shared_ptr<TwilioIPMessagingConfigurationProvider> configurationProvider;
	std::shared_ptr<TwilioIPMessagingClientListener> messagingListener;
	ITNNotificationClientPtr notificationClient;
	ITMClientPtr messagingClient;
	ITMChannelsPtr channels;
	std::map<const char*,ITMChannelPtr>   channelMap;

} IPMessagingClientContext;

//Channel
typedef struct ChannelContext {
	ITMChannelPtr channel;
} ChannelContext;

//Channels
typedef struct ChannelsContext {
	ITMChannelsPtr channels;
} ChannelsContext;

//Message
typedef struct MessageContext {
	ITMessagePtr message;
} MessageContext;

//Members
typedef struct MessagesContext {
	ITMessagesPtr messages;
} MessagesContext;

//Member
typedef struct MemberContext {
	ITMMemberPtr member;
} MemberContext;

//Members
typedef struct MembersContext {
	ITMMembersPtr members;
} MembersContext;
