package com.twilio.ipmessaging.impl;


import java.util.HashMap;
import java.util.Map;

import com.twilio.ipmessaging.Channel;
import com.twilio.ipmessaging.Channels;
import com.twilio.ipmessaging.IPMessagingClientListener;
import com.twilio.ipmessaging.TwilioIPMessagingClient;

import android.app.PendingIntent;
import android.app.PendingIntent.CanceledException;
import android.content.Context;
import android.content.Intent;

public class TwilioIPMessagingClientImpl extends TwilioIPMessagingClient {
	
	static {
		System.loadLibrary("twilio-rtd-native"); 
	}
	
	private static final Logger logger = Logger.getLogger(TwilioIPMessagingClientImpl.class);
	
	private static TwilioIPMessagingClientImpl instance = null;
	private static Context context;
	
	private IPMessagingClientListener ipMessagingListener;
	private IPMessagingClientListenerInternal ipMessagingClientListenerInternal;
	private long nativeClientParam;
	private long nativeObserverHandle;
	private InitListener listener;
	
	final static Map<String, ChannelImpl> publicChannelMap = new HashMap<String, ChannelImpl>();
	
	private IPMessagingClientListenerInternal internalListener;
	private PendingIntent incomingIntent;

	
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
		}
		this.ipMessagingListener = listener;
		this.internalListener = new IPMessagingClientListenerInternal(listener);
		nativeClientParam = initNative(token, internalListener);
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
	
	public static Context getContext() {
		return context;
	}
	
	public void setIncomingIntent(PendingIntent intent) {
		this.incomingIntent = intent;
	}
	
	public void handleIncomingInvite(Channel channel) {
		if (this.incomingIntent != null) {
			Intent intent = new Intent();
			intent.putExtra(Channel.EXTRA_CHANNEL, (ChannelImpl)channel);
			intent.putExtra(Channel.EXTRA_ACTION, Channel.EXTRA_ACTION_INVITE);
			try {
				this.incomingIntent.send(TwilioIPMessagingClientImpl.getContext(), 0, intent);
			} catch (final CanceledException e) {
				logger.e(
						"Unable to send PendingIntent for incoming connection", e);
			}
		}
	}
	
	private native void create();
	public native long initNative(String token, IPMessagingClientListenerInternal listener);
	public native long createMessagingClient(String token);
	private native ChannelsImpl getChannelsNative();
	
}
