package com.twilio.ipmessaging.impl;


import java.util.HashMap;
import java.util.Map;

import com.twilio.ipmessaging.Channel;
import com.twilio.ipmessaging.Channels;
import com.twilio.ipmessaging.IPMessagingClientListener;
import com.twilio.ipmessaging.TwilioIPMessagingClient;

import android.accounts.Account;
import android.content.Context;

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
	
	final static Map<String, Channel> publicChannelMap = new HashMap<String, Channel>();
	
	private IPMessagingClientListenerInternal internalListener;
	
	/*class IPMessagingClientListenerInternal implements NativeHandleInterface {

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

	} */

	
	private TwilioIPMessagingClientImpl(Context context2,
			IPMessagingClientListener inListener) {
		this.context = context2;
		this.ipMessagingListener = inListener;
		
		this.ipMessagingClientListenerInternal = new IPMessagingClientListenerInternal(inListener);
	}

	
	public TwilioIPMessagingClientImpl() {
		create();
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

	
	private void initChannelMap() {
		// TODO Auto-generated method stub
		Channels channels = this.getChannels();
		Channel[] channelArray = channels.getChannels();
		
		for(int i=0; i<channelArray.length; i++) {
			this.publicChannelMap.put(channelArray[i].getSid(), channelArray[i]);
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
			//nativeObserverHandle = instance.getIPMessagingClientListenerHandle();
			//if (nativeObserverHandle == 0) {
			//	return null;
			//}			
		}
		this.ipMessagingListener = listener;
		this.internalListener = new IPMessagingClientListenerInternal(listener);
		nativeClientParam = initNative(token, internalListener);
		long status = createMessagingClient(token);
		
		initChannelMap();

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
	
	/*long getIPMessagingClientListenerHandle() {
		return this.ipMessagingClientListenerInternal.getNativeHandle();
	} */
	
	private native void create();
	public native long initNative(String token, IPMessagingClientListenerInternal listener);
	public native long createMessagingClient(String token);
	private native Channels getChannelsNative();
	
}
