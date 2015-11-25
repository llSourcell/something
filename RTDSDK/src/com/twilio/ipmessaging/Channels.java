package com.twilio.ipmessaging;

import java.util.Map;

import com.twilio.ipmessaging.Channel.ChannelType;
import com.twilio.ipmessaging.Constants.CreateChannelListener;
import com.twilio.ipmessaging.Constants.StatusListener;



public interface Channels {
	
	
	/**
	 * Method to create channel with attributes.
	 * 
	 * @param friendlyName	Friendly name of the new channel.
	 * @param channelType   Channel type
	 * @param listener 		Listener that receives the status of create channel action.
	 */
	public void createChannel(String friendlyName, ChannelType type, CreateChannelListener listener);
	
	/**
	 * Method to create channel with options.
	 * 
	 * @param options		These options can be passed to createChannel.
	 * @param listener 		Listener that receives the status of create channel action.
	 */
	public void createChannel(Map<String, Object> options, CreateChannelListener listener);
	
	
	/** Request loading of all channels from server to be delivered via callbacks.
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
	
	/** Retrieves a channel with the unique name. 
	 * 
	 * @param uniqueName Unique name for the channel.
	 * @return The channel.
	 */
	public Channel getChannelByUniqueName(String uniqueName);

	
	/** 
	 * Request a list of all channels loaded so far.  Will be an incomplete list until loading completes.
	 * 
	 * @return The channels loaded so far.
	 */
	public Channel[] getChannels();
}
