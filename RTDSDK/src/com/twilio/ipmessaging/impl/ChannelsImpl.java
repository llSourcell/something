package com.twilio.ipmessaging.impl;

import java.util.ArrayList;
import java.util.List;

import com.twilio.ipmessaging.Channel;
import com.twilio.ipmessaging.Channel.ChannelType;
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
			default:
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
		
		ChannelImpl[] publicChannelArray =  getChannelsNative(this.nativeChannelsHandler);

		for(int i=0; i<publicChannelArray.length; i++) {
			if(publicChannelArray[i] != null) {
				TwilioIPMessagingClientImpl.publicChannelMap.put(publicChannelArray[i].getSid(), (ChannelImpl) publicChannelArray[i]);
			}
		}
		List<ChannelImpl> list = new ArrayList<ChannelImpl>(TwilioIPMessagingClientImpl.privateChannelList.values());
		if(list != null && list.size() > 0) {
			ChannelImpl[] privateChannel = list.toArray(new ChannelImpl[list.size()]);
			int publicChannelArrayLength = publicChannelArray.length;
		    int privateChannelLength = list.toArray().length;
		    ChannelImpl[] combineChannelArray= new ChannelImpl[publicChannelArrayLength+privateChannelLength];
		    System.arraycopy(publicChannelArray, 0, combineChannelArray, 0, publicChannelArrayLength);
		    System.arraycopy(privateChannel, 0, combineChannelArray, publicChannelArrayLength, privateChannelLength);
		    return combineChannelArray;
		}
		return publicChannelArray;
	}
	
	private native void createChannelNative(String friendlyName, int type, long nativeChannelsContext);
	private native void createChannelNativeWithListener(String friendlyName, int type, long nativeChannelsContext, CreateChannelListener listener);
	private native ChannelImpl getChannelNative(String channelId, long handle);
	private native ChannelImpl[] getChannelsNative(long handle);
}
