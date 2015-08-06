package com.twilio.ipmessaging;

import java.util.Map;

public interface Channels {
		
	/**
	 * Method to create channel with attributes.
	 * 
	 * @param friendlyName	Friendly name of the new channel.
	 * @param listener 		listener that receives this channel related events.
	 */
	public Channel createChannel(String friendlyName, ChannelListener listener);
	

}
