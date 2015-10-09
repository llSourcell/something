package com.twilio.ipmessaging.demo;

import java.util.Arrays;
import java.util.Comparator;
import java.util.List;

import com.twilio.ipmessaging.Channel;
import com.twilio.ipmessaging.IPMessagingClientListener;
import com.twilio.ipmessaging.TwilioIPMessagingClient;

import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class BasicIPMessagingClient implements IPMessagingClientListener {


	private static final String TAG = "BasicIPMessagingClient";
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

	public String getCapabilityToken() {
		return capabilityToken;
	}

	public void setCapabilityToken(String capabilityToken) {
		this.capabilityToken = capabilityToken;
	}

	
	public void doLogin(final String capabilityToken, final LoginListener listener) {
		
		TwilioIPMessagingClient.initializeSDK(context, new TwilioIPMessagingClient.InitListener()
        {
            @Override
            public void onInitialized()
            {
            	ipMessagingClient = TwilioIPMessagingClient.initIPMessagingClientWithToken(capabilityToken, BasicIPMessagingClient.this);
            	Intent intent = new Intent(context,ChannelActivity.class);
            	PendingIntent pendingIntent = PendingIntent.getActivity(context, 0, intent, PendingIntent.FLAG_UPDATE_CURRENT);
            	ipMessagingClient.setIncomingIntent(pendingIntent);
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
		Log.i(TAG, "A Channel :"+ channel.getFriendlyName() + " got added");		
	}

	@Override
	public void onChannelChange(Channel channel) {
		//Log.i(TAG, "Channel Name : "+ channel.getFriendlyName() + " got Changed");	
	}

	@Override
	public void onChannelDelete(Channel channel) {
		//Log.i(TAG, "A Channel :"+ channel.getFriendlyName() + " got deleted");		
	}

	@Override
	public void onError(int errorCode, String errorText) {
		Log.i(TAG, "onError called");	
	}

	@Override
	public void onAttributesChange(String attributes) {
		Log.i(TAG, "Channel's attribute changed");	
	}
	
	public TwilioIPMessagingClient getIpMessagingClient() {
		return ipMessagingClient;
	}

}