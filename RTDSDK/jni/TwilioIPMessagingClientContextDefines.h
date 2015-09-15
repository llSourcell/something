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
#include "IPMessagingClientListener_Internal.h"

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


typedef struct ChannelContext {
	ITMChannelPtr channel;
} ChannelContext;

typedef struct ChannelsContext {
	ITMChannelsPtr channels;
} ChannelsContext;

typedef struct MessageContext {
	ITMessagePtr message;
} MessageContext;

typedef struct MessagesContext {
	ITMessagesPtr messages;
} MessagesContext;

typedef struct MemberContext {
	ITMMemberPtr member;
} MemberContext;

typedef struct MembersContext {
	ITMMembersPtr members;
} MembersContext;
