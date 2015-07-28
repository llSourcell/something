package com.twilio.rtd.impl;

import java.util.Map;
import java.util.Set;

import android.content.Context;

import com.twilio.rtd.Channel;
import com.twilio.rtd.ChannelListener;
import com.twilio.rtd.IPMessagingClient;
import com.twilio.rtd.MessagingClientListener;

public class IPMessagingClientImpl implements IPMessagingClient{

	@Override
	public void initialize(Context inContext, InitListener inListener) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public IPMessagingClient initMessagingClientWithToken(String accessToken,
			Map<String, String> attributes, MessagingClientListener listener) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public void setListener(MessagingClientListener listener) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public MessagingClientListener getListener() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public String getAttributes() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public void updateAttributes(Map<String, String> attributes) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void updateToken(String accessToken) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public Set<Channel> getChannels() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public void createChannel(Map<String, String> attributes,
			String friendlyName, ChannelListener listener) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void joinChannel(String channelSid) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public Channel getChannel(String channelSid) {
		// TODO Auto-generated method stub
		return null;
	}
	

	

}
