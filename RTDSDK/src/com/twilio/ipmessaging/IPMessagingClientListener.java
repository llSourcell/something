package com.twilio.ipmessaging;

/**
 * This interface defines IPMessagingClient callback methods.
 * 
 */
public interface IPMessagingClientListener {
	
	/**
	 * Called when the current user adds a channel to their list of channels.
	 * 
	 * @param channel The channel that got added.
	 */
	public void onChannelAdd(Channel channel);
	
	/**
	 * Called when one the channel of the current user is changed.
	 * 
	 * @param channel The channel that changed.
	 */
	public void onChannelChange(Channel channel);
	
	/**
	 * Called when one of the channel of the current user is deleted.
	 * 
	 * @param channel The channel that got deleted.
	 */
	public void onChannelDelete(Channel channel);
	
	/**
	 * Called when an error condition occurs.
	 * 
	 * @param errorCode	The error code specifying the error.
	 * @param errorText	The error text describing the error condition.
	 */
	public void onError(int errorCode, String errorText);
	
	/**
	 * Called when an attribute has changed.
	 * 
	 * @param attributes The updated attributes string.
	 */
	public void onAttributesChange(String attributes);
	
	/**
	 * Called when Messages synchronization for this Channel completes.
	 * 
	 */
	public void onChannelHistoryLoaded(Channel channel);
	
}
