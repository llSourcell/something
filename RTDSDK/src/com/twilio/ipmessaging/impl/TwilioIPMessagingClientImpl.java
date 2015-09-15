package com.twilio.ipmessaging.impl;


import java.util.Map;

import android.content.Context;

import com.twilio.common.IdentityManager;
import com.twilio.ipmessaging.Channel;
import com.twilio.ipmessaging.Channels;
import com.twilio.ipmessaging.TwilioIPMessagingClient;
import com.twilio.ipmessaging.IPMessagingClientListener;

public class TwilioIPMessagingClientImpl extends TwilioIPMessagingClient {
	
	static {
		System.loadLibrary("twilio-rtd-native"); 
	}
	
	private static TwilioIPMessagingClientImpl instance = null;
	protected Context context;
	private IPMessagingClientListener ipMessagingListener;
	private IPMessagingClientListenerInternal ipMessagingClientListenerInternal;
	private long nativeClientParam;
	private long nativeObserverHandle;
	private InitListener listener;
	
	class IPMessagingClientListenerInternal implements NativeHandleInterface {

		private long nativeIPMessagingClientListener;

		public IPMessagingClientListenerInternal(IPMessagingClientListener listener) {
			// this.listener = listener;
			this.nativeIPMessagingClientListener = wrapNativeObserver(listener, TwilioIPMessagingClientImpl.this);
		}

		private native long wrapNativeObserver(IPMessagingClientListener listener,
				TwilioIPMessagingClient client);

		// ::TODO figure out when to call this - may be Endpoint.release() ??
		private native void freeNativeObserver(long nativeEndpointObserver);

		@Override
		public long getNativeHandle() {
			return nativeIPMessagingClientListener;
		}

	}

	
	private TwilioIPMessagingClientImpl(Context context2,
			IPMessagingClientListener inListener) {
		this.context = context2;
		this.ipMessagingListener = inListener;
		
		this.ipMessagingClientListenerInternal = new IPMessagingClientListenerInternal(inListener);
	}

	
	public TwilioIPMessagingClientImpl() {
		
	}


	public static TwilioIPMessagingClientImpl getInstance()
	{
		if (instance == null)
		{
			synchronized (TwilioIPMessagingClientImpl.class)
			{
				if (instance == null)
					instance = new TwilioIPMessagingClientImpl();
			}
		}
		
		return instance;
	}

	public void initialize(Context inContext, TwilioIPMessagingClient.InitListener inListener) {
		this.context = inContext;
		this.listener = inListener;
		//TODO:: init service 
		if(inListener != null) {
			inListener.onInitialized();
		}
	}

	
	public TwilioIPMessagingClient initMessagingClientWithToken(String accessToken,
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
		
		return getChannelsNative();
		
		//return null;
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

	public TwilioIPMessagingClient initClient(String token, IPMessagingClientListener listener) {

		if (getInstance() == null) {
			TwilioIPMessagingClientImpl.instance = new TwilioIPMessagingClientImpl(context, listener);
			nativeObserverHandle = instance.getIPMessagingClientListenerHandle();
			if (nativeObserverHandle == 0) {
				return null;
			}			
		}
		nativeClientParam = initNative(token, listener);
		long status = createMessagingClient(token);

		return instance;
	}



	public long getNativeClientParam() {
		return nativeClientParam;
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
	
	public native long initNative(String token, IPMessagingClientListener listener);
	public native long createMessagingClient(String token);
	private native Channels getChannelsNative();
	
}
