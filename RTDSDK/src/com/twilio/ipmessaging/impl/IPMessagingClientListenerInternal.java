package com.twilio.ipmessaging.impl;

import java.util.Map;

import com.twilio.ipmessaging.Channel;
import com.twilio.ipmessaging.IPMessagingClientListener;
import com.twilio.ipmessaging.Member;
import com.twilio.ipmessaging.Message;

public class IPMessagingClientListenerInternal {
	
	private static final Logger logger = Logger.getLogger(IPMessagingClientListenerInternal.class);
	private IPMessagingClientListener listener;
	

	public IPMessagingClientListenerInternal(IPMessagingClientListener listener) {
		this.listener = listener;
	}

	public void onMessageAdd(final Message message) {
		logger.d("Entered onMessageAdd");
		if(message != null) {
			String cSid = message.getChannelSid();
			ChannelImpl channelImpl = (ChannelImpl) TwilioIPMessagingClientImpl.publicChannelMap.get(cSid);
			if(channelImpl != null) {
				channelImpl.handleIncomingMessage((MessageImpl) message);
			}
		}
	}

	
	public void onMessageChange(Message message) {
		
	}

	
	public void onMessageDelete(Message message) {
		// TODO Auto-generated method stub
		
	}

	
	public void onMemberJoin(Member member, Channel channel) {
		logger.d("Entered onMemberJoin");
		if(channel != null) {
			String cSid = channel.getSid();
			ChannelImpl channelImpl = (ChannelImpl) TwilioIPMessagingClientImpl.publicChannelMap.get(cSid);
			if(channelImpl != null) {
				channelImpl.handleOnMemberJoin(member);
			}
		}
	}

	public void onMemberChange(Member member, Channel channel) {
		logger.d("Entered onMemberChange");
		if(channel != null) {
			String cSid = channel.getSid();
			ChannelImpl channelImpl = (ChannelImpl) TwilioIPMessagingClientImpl.publicChannelMap.get(cSid);
			if(channelImpl != null) {
				channelImpl.handleOnMemberChange(member);
			}
		}
	}


	public void onMemberDelete(Member member, Channel channel) {
		logger.d("Entered onMemberDelete");
		if(channel != null) {
			String cSid = channel.getSid();
			ChannelImpl channelImpl = (ChannelImpl) TwilioIPMessagingClientImpl.publicChannelMap.get(cSid);
			if(channelImpl != null) {
				channelImpl.handleOnMemberDelete(member);
			}
		}
	}

	public void onAttributesChange(Map<String, String> updatedAttributes) {
		// TODO Auto-generated method stub
		
	}

	public void onChannelAdd(ChannelImpl channel) {
		TwilioIPMessagingClientImpl.getInstance().handleChannelAddEvent(channel);
	}
	
	public void onChannelInvite(Channel channel) {
		logger.d("Entered onChannelInvite");
		TwilioIPMessagingClientImpl.getInstance().handleIncomingInvite(channel);		
	}

	
	public void onChannelChange(ChannelImpl channel) {
		logger.d("Entered onChannelChange");
		TwilioIPMessagingClientImpl.getInstance().handleChannelChanged(channel);	
	}


	public void onChannelDelete(ChannelImpl  channel) {
		logger.d("Entered onChannelChange");
		TwilioIPMessagingClientImpl.getInstance().handleChannelDeleted(channel);
	}


	public void onError(int errorCode, String errorText) {
		// TODO Auto-generated method stub
		
	}

	
	public void onAttributesChange(String attribute) {
		logger.d("Entered onChannelChange");
		TwilioIPMessagingClientImpl.getInstance().handleChannelAttributeChange(attribute);
	}
	
	
	public void onTypingStarted(Channel channel, Member member) {
		logger.d("Entered onTypingStarted");
		if(channel != null) {
			String cSid = channel.getSid();
			ChannelImpl channelImpl = (ChannelImpl) TwilioIPMessagingClientImpl.publicChannelMap.get(cSid);
			if(channelImpl != null) {
				channelImpl.handleOnTypingStarted(member);
			}
		}
	}
	
	public void onTypingEnded(Channel channel, Member member) {
		logger.d("Entered onTypingEnded");
		if(channel != null) {
			String cSid = channel.getSid();
			ChannelImpl channelImpl = (ChannelImpl) TwilioIPMessagingClientImpl.publicChannelMap.get(cSid);
			if(channelImpl != null) {
				channelImpl.handleOnTypingEnded(member);
			}
		}
	}

}