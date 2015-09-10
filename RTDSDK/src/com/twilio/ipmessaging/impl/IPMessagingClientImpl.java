package com.twilio.ipmessaging.impl;


import java.util.Map;

import android.content.Context;

import com.twilio.common.IdentityManager;
import com.twilio.ipmessaging.Channel;
import com.twilio.ipmessaging.Channels;
import com.twilio.ipmessaging.IPMessagingClient;
import com.twilio.ipmessaging.IPMessagingClientListener;

public class IPMessagingClientImpl extends IPMessagingClient {
	
	static
	{
		System.loadLibrary("twilio-rtd-native");
	}
	
	private static IPMessagingClientImpl instance = null;
	protected Context context;
	private IPMessagingClientListener ipMessagingListener;
	private IPMessagingClientListenerInternal ipMessagingClientListenerInternal;
	
	
	class IPMessagingClientListenerInternal implements NativeHandleInterface {

		private long nativeIPMessagingClientListener;

		public IPMessagingClientListenerInternal(IPMessagingClientListener listener) {
			// this.listener = listener;
			this.nativeIPMessagingClientListener = wrapNativeObserver(listener, IPMessagingClientImpl.this);
		}

		private native long wrapNativeObserver(IPMessagingClientListener listener,
				IPMessagingClient client);

		// ::TODO figure out when to call this - may be Endpoint.release() ??
		private native void freeNativeObserver(long nativeEndpointObserver);

		@Override
		public long getNativeHandle() {
			return nativeIPMessagingClientListener;
		}

	}

	
	public IPMessagingClientImpl(Context context2,
			IPMessagingClientListener inListener) {
		this.context = context2;
		this.ipMessagingListener = inListener;
		
		this.ipMessagingClientListenerInternal = new IPMessagingClientListenerInternal(inListener);
	}

	public static IPMessagingClientImpl getInstance(){
		/*if(instance == null) {
			instance = new IPMessagingClientImpl();
		}*/		
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
	public IPMessagingClient init(String tokenr,
			IPMessagingClientListener listener) {
			String token = "";
			final IPMessagingClientImpl msgClient = new IPMessagingClientImpl(context, listener);
			long nativeObserverHandle = msgClient.getIPMessagingClientListenerHandle();
			if (nativeObserverHandle == 0) {
				return null;
			}
			final long nativeEndpointHandle = createEndpoint(token,
					nativeObserverHandle);
			if (nativeEndpointHandle == 0) {
				return null;
			}
			msgClient.setNativeHandle(nativeEndpointHandle);
			
			return msgClient;
	}



	private void setNativeHandle(long nativeEndpointHandle) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public String getIdentity() {
		// TODO Auto-generated method stub
		return null;
	}
	
	long getIPMessagingClientListenerHandle() {
		return this.ipMessagingClientListenerInternal.getNativeHandle();
	}
	
	private native long createEndpoint(String token, long nativeEndpointObserver);

	
}
