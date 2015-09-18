package com.twilio.ipmessaging.impl;

import java.util.Map;

import com.twilio.ipmessaging.Channel;
import com.twilio.ipmessaging.ChannelListener;
import com.twilio.ipmessaging.IPMessagingClientListener;
import com.twilio.ipmessaging.Member;
import com.twilio.ipmessaging.Message;

public class IPMessagingClientListenerInternal implements IPMessagingClientListener, ChannelListener{
	
	private static final Logger logger = Logger.getLogger(IPMessagingClientListenerInternal.class);
	
	private long nativeIPMessagingClientListener;
	private IPMessagingClientListener listener;
	

	public IPMessagingClientListenerInternal(IPMessagingClientListener listener) {
		this.listener = listener;
	}


	@Override
	public void onMessageAdd(Message message) {
		logger.d("Entered onMessageAdd");
		String body = message.getMessageBody();
		String author = message.getAuthor();
		String cSid = message.getChannelSid();
		Channel channel = TwilioIPMessagingClientImpl.publicChannelMap.get(cSid);
		ChannelListener listener = channel.getListener();
		if(listener != null ) {
			listener.onMessageAdd(message);
		}
		logger.d("Leaving onMessageAdd");
		
	}

	@Override
	public void onMessageChange(Message message) {
		
	}

	@Override
	public void onMessageDelete(Message message) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onMemberJoin(Member member) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onMemberChange(Member member) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onMemberDelete(Member member) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onAttributesChange(Map<String, String> updatedAttributes) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onChannelAdd(Channel channel) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onChannelChange(Channel channel) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onChannelDelete(Channel channel) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onError(int errorCode, String errorText) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onAttributesChange(String attributes) {
		// TODO Auto-generated method stub
		
	}
	

}
