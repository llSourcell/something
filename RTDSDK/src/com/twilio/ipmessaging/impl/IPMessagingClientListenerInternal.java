package com.twilio.ipmessaging.impl;

import java.util.Map;

import com.twilio.ipmessaging.Channel;
import com.twilio.ipmessaging.ChannelListener;
import com.twilio.ipmessaging.IPMessagingClientListener;
import com.twilio.ipmessaging.Member;
import com.twilio.ipmessaging.Message;

import android.app.PendingIntent;
import android.app.PendingIntent.CanceledException;
import android.content.Intent;

public class IPMessagingClientListenerInternal {
	
	private static final Logger logger = Logger.getLogger(IPMessagingClientListenerInternal.class);
	private IPMessagingClientListener listener;
	

	public IPMessagingClientListenerInternal(IPMessagingClientListener listener) {
		this.listener = listener;
	}

	public void onMessageAdd(final Message message) {
		logger.d("Entered onMessageAdd");
		String cSid = message.getChannelSid();
		ChannelImpl channel = (ChannelImpl) TwilioIPMessagingClientImpl.publicChannelMap.get(cSid);
		channel.handleIncomingMessage((MessageImpl) message);
	}

	
	public void onMessageChange(Message message) {
		
	}

	
	public void onMessageDelete(Message message) {
		// TODO Auto-generated method stub
		
	}

	
	public void onMemberJoin(Member member) {
		// TODO Auto-generated method stub
		
	}


	public void onMemberChange(Member member) {
		// TODO Auto-generated method stub
		
	}


	public void onMemberDelete(Member member) {
		// TODO Auto-generated method stub
		
	}

	
	public void onAttributesChange(Map<String, String> updatedAttributes) {
		// TODO Auto-generated method stub
		
	}


	public void onChannelAdd(Channel channel) {
		TwilioIPMessagingClientImpl.getInstance().handleChannelAddEvent(channel);
	}
	
	public void onChannelInvite(Channel channel) {
		logger.d("Entered onChannelInvite");
		TwilioIPMessagingClientImpl.getInstance().handleIncomingInvite(channel);		
	}

	
	public void onChannelChange(Channel channel) {
		logger.d("Entered onChannelChange");
		TwilioIPMessagingClientImpl.getInstance().handleChannelChanged(channel);	
	}


	public void onChannelDelete(Channel channel) {
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
	

}
