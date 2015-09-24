package com.twilio.ipmessaging.impl;

import com.twilio.ipmessaging.Channel;
import com.twilio.ipmessaging.ChannelListener;
import com.twilio.ipmessaging.Channels;


public class ChannelsImpl implements Channels {
	
	private long nativeChannelsHandler;

	public ChannelsImpl(long handler) {
		super();
		this.nativeChannelsHandler = handler;
	}

	@Override
	public Channel createChannel(String friendlyName, ChannelListener listener) {	
		
		ChannelImpl channel = this.createChannelNative(friendlyName, listener);
		String cSid = channel.getSid();
		TwilioIPMessagingClientImpl.publicChannelMap.put(cSid, channel);
		return channel;
	}

	@Override
	public Channel getChannel(String channelId) {
		
		return this.getChannelNative(channelId, nativeChannelsHandler);
	}

	@Override
	public Channel[] getChannels() {	
		
		Channel[] channelArray =  getChannelsNative(this.nativeChannelsHandler);

		for(int i=0; i<channelArray.length; i++) {
			TwilioIPMessagingClientImpl.publicChannelMap.put(channelArray[i].getSid(), (ChannelImpl) channelArray[i]);
		}
		return channelArray;
	}
	
	private native ChannelImpl createChannelNative(String friendlyName, ChannelListener listener);
	private native ChannelImpl getChannelNative(String channelId, long handle);
	private native ChannelImpl[] getChannelsNative(long handle);

}
