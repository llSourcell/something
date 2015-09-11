#include "ITMClient.h"
#include "ITMChannels.h"
#include "ITMChannel.h"
#include "ITMessages.h"
#include "ITMessage.h"
#include "ITMMembers.h"
#include "ITMMember.h"

using namespace rtd;


typedef struct MessagesContext {
	ITMessagesPtr messages;
} MessagesContext;


typedef struct ChannelContext {
	ITMessagesPtr messages;
} ChannelContext;


typedef struct MessageContext {
	ITMessagePtr message;
} MessageContext;
