package com.twilio.ipmessaging.impl;

import com.twilio.ipmessaging.Channel;
import com.twilio.ipmessaging.ChannelListener;
import com.twilio.ipmessaging.Channels;
import com.twilio.ipmessaging.Message;

public class ChannelsImpl implements Channels {
	
	private long nativeChannelsHandler;

	public ChannelsImpl(long handler) {
		super();
		this.nativeChannelsHandler = handler;
	}

	@Override
	public Channel createChannel(String friendlyName, ChannelListener listener) {	
		return this.createChannelNative(friendlyName, listener);
	}

	@Override
	public Channel getChannel(String channelId) {
		
		return this.getChannelNative(channelId, nativeChannelsHandler);
	}

	@Override
	public Channel[] getChannels() {		
		return getChannelsNative(this.nativeChannelsHandler);
	}
	
	private native Channel createChannelNative(String friendlyName, ChannelListener listener);
	private native Channel getChannelNative(String channelId, long handle);
	private native Channel[] getChannelsNative(long handle);

}
