package com.twilio.ipmessaging;

import java.util.Map;

import android.app.PendingIntent;

/**
 *  Container for channel object.
 * 
 * @author kbagchi
 *
 */


public interface Channel {
	
	/**
	 * Key into an Intent's extras data that points to a {@link Channel} object.
	 */
	public static final String EXTRA_CHANNEL = "com.twilio.ipmessaging.Channel";
	
	/**
	 * Key into an Intent's extras data that points to a {@link Message} object.
	 */
	public static final String EXTRA_MESSAGE = "com.twilio.ipmessaging.Message";
	
	/**
	 * Key into an Intent's extras data that points to a {@link Message} object.
	 */
	public static final String EXTRA_ACTION = "channel.action";
	
	/**
	 * Key into an Intent's extras data that points to a {@link Message} object.
	 */
	public static final String EXTRA_ACTION_INVITE = "channel.action.invite";
	
	
	/** 
	 * The Channel.ChannelStatus enum represents the various states of the member with respect to this channel 
	 * 
	 */ 
	public enum ChannelStatus
	{
		/** User has been invited to this channel */
		INVITED,
		/** User has been joined to this channel. */
		JOINED,
		/** User has NOT been invited or joined to this channel. */
		NOT_PARTICIATING
	};
	
	/** 
	 * Enumeration indicating the channel's visibility. 
	 * 
	 */
	public enum ChannelType {
	    CHANNEL_TYPE_PUBLIC,      ///< Channel is publicly visible
	    CHANNEL_TYPE_PRIVATE      ///< Channel is private and only visible to invited members.
	};

	
	/**
	 * Method that returns the unique identifier for this channel.
	 * 
	 * @return the sid.
	 */
	public String getSid();
	
	/**
	 * Method that returns the friendlyName of the channel.
	 * 
	 * @return the firendlyName.
	 */
	public String getFriendlyName();
	
	/**
	 * Custom attributes associated with the Channel.
	 * 
	 * @return the map of attributes.
	 */
	public Map<String, String> getAttributes();
	
	/**
	 * Method to get messages.
	 * 
	 * @param count number of messages to return.
	 * @return the count number of messages.
	 */
	public Messages getMessages(int count);
	
	/**
	 * Returns the current user's status on this channel.
	 * 
	 * @return the channel status.
	 */
	public Channel.ChannelStatus getStatus();
	
	/**
	 * Method to set ChannelListener for this Channel.
	 * 
	 * @param listener The channel listener.
	 */
	public void setListener(ChannelListener listener);
	
	/**
	 * Method to retrieve ChannelListener for this Channel.
	 * 
	 * @return listener for this channel.
	 */
	public ChannelListener getListener();
		
	/**
	 * Method to get the channel members.
	 * 
	 * @return Set of Member objects.
	 */
	public Members getMembers();
	
	/**
	 * Updates the attribute data for this channel.
	 * 
	 * @param updatedAttributes
	 */
	public void setAttributes(Map<String, String> updatedAttributes);
	
	/**
	 * Updates the friendly name for this channel.
	 * 
	 * @param friendlyName updated friendly name.
	 */
	public void setFriendlyName(String friendlyName);
	
	/**
	 * Updates the channel type for this channel.
	 * 
	 * @param type updated channel type.
	 */
	public void setType(ChannelType type);
	
	/**
	 * Joins the current user to the channel.
	 * 
	 */
	public void join();
	
	/**
	 * Leaves the current channel.
	 */
	public void leave();
	
	/**
	 * Destroys the current channel.
	 * 
	 */
	public void destroy();	

	/**
	 * Destroys the current channel.
	 * 
	 */
	public void declineInvitation();
	
	/**
	 * Sets a {@link PendingIntent} that will be sent when an incoming message is received.
	 * 
	*/
	public void setIncomingIntent(PendingIntent inIntent);

}


