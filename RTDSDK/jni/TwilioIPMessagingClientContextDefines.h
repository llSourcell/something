#include "ITMClient.h"
#include "ITMChannels.h"
#include "ITMChannel.h"
#include "ITMessages.h"
#include "ITMessage.h"
#include "ITMMembers.h"
#include "ITMMember.h"
#include <Twilsock/ITNTwilsockClient.h>

#include "TwilioIPMessagingConfigurationProvider.h"
#include "TwilioIPMessagingNotificationClientListener.h"
#include "TwilioIPMessagingClientListener.h"
#include "RegistrationObserverImpl.h"

using namespace rtd;

// structure with all the shared pointer
typedef struct IPMessagingClientContext {
	std::shared_ptr<TwilioIPMessagingNotificationClientListener>  notificationClientObserver;
	std::shared_ptr<TwilioIPMessagingConfigurationProvider> configurationProvider;
	std::shared_ptr<TwilioIPMessagingClientListener> messagingListener;
	std::shared_ptr<RegistrationObserverImpl> regObservationListener;
	ITNTwilsockClientPtr twilsock;
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
