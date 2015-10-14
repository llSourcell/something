package com.twilio.ipmessaging.impl;

import com.twilio.ipmessaging.Channel;
import com.twilio.ipmessaging.Channel.ChannelType;
import com.twilio.ipmessaging.ChannelListener;
import com.twilio.ipmessaging.Channels;
import com.twilio.ipmessaging.TwilioIPMessagingClient.CreateChannelListener;


public class ChannelsImpl implements Channels {

	private static final Logger logger = Logger.getLogger(ChannelsImpl.class);
	private long nativeChannelsHandler;

	public ChannelsImpl(long handler) {
		super();
		this.nativeChannelsHandler = handler;
	}
	
	@Override
	public void createChannel(String friendlyName, ChannelType type, CreateChannelListener listener) {
		int nativeType = 0;
		switch (type) {
		case CHANNEL_TYPE_PUBLIC:
			nativeType = 0;
			break;
		case CHANNEL_TYPE_PRIVATE:
			nativeType = 1;
			break;
		}
	
		if(listener != null) {
			this.createChannelNativeWithListener(friendlyName, nativeType, this.nativeChannelsHandler, listener);
		} else {
			this.createChannelNative(friendlyName, nativeType, this.nativeChannelsHandler);
		}
	}


	@Override
	public Channel getChannel(String channelId) {
		
		return TwilioIPMessagingClientImpl.publicChannelMap.get(channelId);//this.getChannelNative(channelId, nativeChannelsHandler);
	}

	@Override
	public Channel[] getChannels() {	
		
		Channel[] channelArray =  getChannelsNative(this.nativeChannelsHandler);

		for(int i=0; i<channelArray.length; i++) {
			logger.e("getChannels(): Channel Sid " + channelArray[i].getSid() + "HashCode is : " + channelArray[i].hashCode());
			TwilioIPMessagingClientImpl.publicChannelMap.put(channelArray[i].getSid(), (ChannelImpl) channelArray[i]);
		}
		return channelArray;
	}
	
	private native void createChannelNative(String friendlyName, int type, long nativeChannelsContext);
	private native void createChannelNativeWithListener(String friendlyName, int type, long nativeChannelsContext, CreateChannelListener listener);
	private native ChannelImpl getChannelNative(String channelId, long handle);
	private native ChannelImpl[] getChannelsNative(long handle);
	private native ChannelImpl[] getMyChannelsNative(long handle);
}
