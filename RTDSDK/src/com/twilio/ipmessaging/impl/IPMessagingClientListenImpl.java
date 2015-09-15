package com.twilio.ipmessaging.impl;

import java.util.Map;

import com.twilio.ipmessaging.Channel;
import com.twilio.ipmessaging.ChannelListener;
import com.twilio.ipmessaging.IPMessagingClientListener;
import com.twilio.ipmessaging.Member;
import com.twilio.ipmessaging.Message;

public class IPMessagingClientListenImpl implements IPMessagingClientListener, ChannelListener{

	@Override
	public void onMessageAdd(Message message) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onMessageChang(Message message) {
		// TODO Auto-generated method stub
		
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
