package com.twilio.ipmessaging;

import com.twilio.ipmessaging.Channel.ChannelType;
import com.twilio.ipmessaging.Constants.CreateChannelListener;
import com.twilio.ipmessaging.Constants.StatusListener;



public interface Channels {
	
	/**
	 * Method to create channel with attributes.
	 * 
	 * @param friendlyName	Friendly name of the new channel.
	 * @param channelType  Channel type
	 * @param listener 		listener that receives this channel related events.
	 */
	public void createChannel(String friendlyName, ChannelType type, CreateChannelListener listener);
	
	
	/** Requesting loading of all channels from server to be delivered via callbacks.
	 * 
	 *  @param listener Listener that will receive callback of the result.
	 */
	public void loadChannelsWithListener(StatusListener listener);
	
	/**
	 * Retrieves a channel with the specified id.
	 * 
	 *  @param channelId Identifier for the channel.
	 *  @return The channel.
	 */
	public Channel getChannel(String channelId);
	
	/** 
	 * Request a list of all channels loaded so far.  Will be an incomplete list until loading completes.
	 * 
	 * @return The channels loaded so far.
	 */
	public Channel[] getChannels();
}
