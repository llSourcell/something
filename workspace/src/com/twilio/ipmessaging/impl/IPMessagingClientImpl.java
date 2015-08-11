package com.twilio.ipmessaging.impl;

import java.util.Map;

import android.content.Context;

import com.twilio.common.IdentityManager;
import com.twilio.ipmessaging.Channel;
import com.twilio.ipmessaging.Channels;
import com.twilio.ipmessaging.IPMessagingClient;
import com.twilio.ipmessaging.IPMessagingClientListener;

public class IPMessagingClientImpl extends IPMessagingClient {
	
	private static IPMessagingClientImpl instance = null;
	
	public static IPMessagingClientImpl getInstance(){
		if(instance == null) {
			instance = new IPMessagingClientImpl();
		}		
		return instance;
	}

	@Override
	public void initializeSDK(Context inContext, InitListener inListener) {
		// TODO Auto-generated method stub
		
	}

	
	public IPMessagingClient initMessagingClientWithToken(String accessToken,
			Map<String, String> attributes, IPMessagingClientListener listener) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public void setListener(IPMessagingClientListener listener) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public IPMessagingClientListener getListener() {
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
	public Channels getChannels() {
		// TODO Auto-generated method stub
		return null;
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

	@Override
	public IPMessagingClient init(
			IdentityManager tokenManager, IPMessagingClientListener listener) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public String getIdentity() {
		// TODO Auto-generated method stub
		return null;
	}
	
}
