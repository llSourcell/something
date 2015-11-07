package com.twilio.ipmessaging.impl;

import java.util.HashMap;
import java.util.Map;

import com.twilio.ipmessaging.Channel;
import com.twilio.ipmessaging.Channels;
import com.twilio.ipmessaging.Constants;
import com.twilio.ipmessaging.IPMessagingClientListener;
import com.twilio.ipmessaging.TwilioIPMessagingClient;
import com.twilio.ipmessaging.Channel.ChannelType;

import android.app.PendingIntent;
import android.app.PendingIntent.CanceledException;
import android.content.Context;
import android.content.Intent;

public class TwilioIPMessagingClientImpl implements TwilioIPMessagingClient {
	
	private static final Logger logger = Logger.getLogger(TwilioIPMessagingClientImpl.class);
	
	private Context context;	
	private String identity;
	private IPMessagingClientListener ipMessagingListener;
	private IPMessagingClientListenerInternal ipMessagingClientListenerInternal;
	private long nativeClientParamContextHandle;
	private PendingIntent incomingIntent;
	protected final Map<String, ChannelImpl> publicChannelMap = new HashMap<String, ChannelImpl>();
	protected final Map<String, ChannelImpl> privateChannelList = new HashMap<String,ChannelImpl>();

		
	public TwilioIPMessagingClientImpl(Context context2, String token, IPMessagingClientListener inListener) {
		this.context = context2;
		this.ipMessagingListener = inListener;		
		this.ipMessagingClientListenerInternal = new IPMessagingClientListenerInternal(this, inListener);
		nativeClientParamContextHandle = initNative(token, ipMessagingClientListenerInternal);
		createMessagingClient(token, this.nativeClientParamContextHandle);
	}


	public long getNativeClientParam() {
		return nativeClientParamContextHandle;
	}
	
	@Override
	public void setListener(IPMessagingClientListener listener) {
		this.ipMessagingListener = listener;
	}

	@Override
	public IPMessagingClientListener getListener() {
		return this.ipMessagingListener;
	}

	@Override
	public void updateToken(String accessToken) {
		if(accessToken != null) {
			synchronized(this) {
				this.updateToken(accessToken, this.nativeClientParamContextHandle);
			}
		}
	}

	@Override
	public Channels getChannels() {		
		return getChannelsNative(this.nativeClientParamContextHandle);
	}

	@Override
	public String getIdentity() {
		// TODO Auto-generated method stub
		return null;
	}
	
	@Override
	public void shutdown() {
		synchronized(this) {
			this.shutDownNative(this.nativeClientParamContextHandle);
		}
		
	}
	
	public Context getContext() {
		return context;
	}
	
	public void setIncomingIntent(PendingIntent intent) {
		this.incomingIntent = intent;
	}
	
	public void handleIncomingInvite(Channel channel) {
		if (this.incomingIntent != null) {
			Intent intent = new Intent();
			intent.putExtra(Constants.EXTRA_CHANNEL, (ChannelImpl)channel);
			intent.putExtra(Constants.EXTRA_ACTION, Constants.EXTRA_ACTION_INVITE);
			try {
				this.incomingIntent.send(this.getContext(), 0, intent);
			} catch (final CanceledException e) {
				logger.e(
						"Unable to send PendingIntent for incoming connection", e);
			}
		}
	}
	
	public void handleChannelAddEvent(ChannelImpl channel) {
		logger.e("handleChannelAddEvent");
		if(this.ipMessagingListener != null) {
			if(channel != null) {
				if(channel.getType() == ChannelType.CHANNEL_TYPE_PRIVATE) {
					privateChannelList.put(channel.getSid(),channel);
				} 
				publicChannelMap.put(channel.getSid(), channel);
			}
			this.ipMessagingListener.onChannelAdd(channel);
		}
	}
	
	public void handleChannelChanged(ChannelImpl channel) {
		if(this.ipMessagingListener != null) {
			if(channel != null) {
				if(channel.getType() == ChannelType.CHANNEL_TYPE_PRIVATE) {
					privateChannelList.put(channel.getSid(),channel);
				} 
				publicChannelMap.put(channel.getSid(), channel);
			}
			this.ipMessagingListener.onChannelChange(channel);
		}
	}
	
	public void handleChannelDeleted(ChannelImpl channel) {
		if(this.ipMessagingListener != null) {
			privateChannelList.remove(channel.getSid());
			publicChannelMap.remove(channel.getSid());
			this.ipMessagingListener.onChannelDelete(channel);
		}
	}
	
	public void handleChannelAttributeChange(String attribute) {
		if(this.ipMessagingListener != null) {
			this.ipMessagingListener.onAttributesChange(attribute);
		}
	}
	
	public native long initNative(String token, IPMessagingClientListenerInternal listener);
	public native long createMessagingClient(String token, long nativeClientParamContextHandle);
	private native ChannelsImpl getChannelsNative(long nativeClientParam);
	private native void updateToken(String token, long nativeClientParam);
	private native void shutDownNative(long nativeClientParam);
}
