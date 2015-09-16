package com.twilio.ipmessaging.demo;

import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

import com.twilio.ipmessaging.Channel;
import com.twilio.ipmessaging.Channels;
import com.twilio.ipmessaging.IPMessagingClientListener;
import com.twilio.ipmessaging.TwilioIPMessagingClient;

import android.content.Context;

public class BasicIPMessagingClient implements IPMessagingClientListener {


	private String capabilityToken;
	private long nativeClientParam;
	private TwilioIPMessagingClient ipMessagingClient;
	private Channel[] channels;
	private Context context;
	
	public BasicIPMessagingClient(Context context) {
		super();
		this.context = context;
	}

	public interface LoginListener {
		public void onLoginStarted();

		public void onLoginFinished();

		public void onLoginError(String errorMessage);

		public void onLogoutFinished();
	}
	
	private class CustomComparator implements Comparator<Channel> {
		@Override
		public int compare(Channel lhs, Channel rhs) {
			return lhs.getFriendlyName().compareTo(rhs.getFriendlyName());		
		}
	}

	public String getCapabilityToken() {
		return capabilityToken;
	}

	public void setCapabilityToken(String capabilityToken) {
		this.capabilityToken = capabilityToken;
	}

	
	
	public void doTest(final String capabilityToken, final LoginListener listener) {
		
		TwilioIPMessagingClient.initializeSDK(context, new TwilioIPMessagingClient.InitListener()
        {
            @Override
            public void onInitialized()
            {
            	ipMessagingClient = TwilioIPMessagingClient.init(capabilityToken, BasicIPMessagingClient.this);
            	if(listener != null) {
					listener.onLoginFinished();
            	}
            }

            @Override
            public void onError(Exception error)
            {
               
            }
        });

	}

	public BasicIPMessagingClient() {
		super();
		// TODO Auto-generated constructor stub
	}

	public void cleanupTest() {
		// TODO
	}
	
	public List<Channel> getChannelList() {
		List<Channel> list = Arrays.asList(this.channels);
		return list;
	}
	
	public long getNativeClientParam() {
		return nativeClientParam;
	}

	public void setNativeClientParam(long nativeClientParam) {
		this.nativeClientParam = nativeClientParam;
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
	
	public TwilioIPMessagingClient getIpMessagingClient() {
		return ipMessagingClient;
	}

}