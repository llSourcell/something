package com.twilio.ipmessaging;

import java.util.Map;

import com.twilio.ipmessaging.Constants.StatusListener;


/**
 *  Container for channel object.
 */

public interface Channel {
	
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
		NOT_PARTICIPATING
	};
	
	/** 
	 * Enumeration indicating the channel's visibility. 
	 * 
	 */
	public enum ChannelType {
	    CHANNEL_TYPE_PUBLIC,      //Channel is publicly visible
	    CHANNEL_TYPE_PRIVATE      //Channel is private and only visible to invited members.
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
	 * Updates the friendly name for this channel.
	 * 
	 * @param friendlyName updated friendly name.
	 */
	public void setFriendlyName(String friendlyName, StatusListener listener);
	
	/**
	 * Custom attributes associated with the Channel.
	 * 
	 * @return the map of attributes.
	 */
	public Map<String, String> getAttributes();
	
	/**
	 * Updates the attribute data for this channel.
	 * 
	 * @param updatedAttributes
	 */
	public void setAttributes(Map<String, String> updatedAttributes, StatusListener listener);

	/**
	 * Method to get messages.
	 * 
	 * @return Messages object for this channel.
	 */
	public Messages getMessages();
	
	/**
	 * Returns the current user's status on this channel.
	 * 
	 * @return the channel status.
	 */
	public Channel.ChannelStatus getStatus();
	
	/**
	 * Method to retrieve ChannelListener for this Channel.
	 * 
	 * @return listener for this channel.
	 */
	public ChannelListener getListener();
	
	/**
	 * Method to set ChannelListener for this Channel.
	 * 
	 * @param listener A channel listener.
	 */
	public void setListener(ChannelListener listener);
			
	/**
	 * Method to get the channel members.
	 * 
	 * @return Set of Member objects.
	 */
	public Members getMembers();
	
	/**
	 * Method to get the channel type.
	 * 
	 * @return ChannelType of the channel.
	 */
	public ChannelType getType();
	
	/**
	 * Method that returns the unique Name of the channel.
	 * 
	 * @return the unique Name.
	 */
	public String getUniqueName();
	
	/**
	 * Updates the unique name for this channel.
	 * 
	 * @param uniqueName	The new unique name for this channel.
	 * @param listener		Listener that will receive callback with the result.
	 * 
	 */
	public void setUniqueName(String uniqueName, StatusListener listener);
	
	/**
	 * Joins the current user to the channel.
	 * @param listener
	 * 
	 * @param listener Listener that will receive callback with the result.
	 */
	public void join(StatusListener listener);
	
	/**
	 * Leaves the current channel.
	 * 
	 * @param listener Listener that will receive callback with the result.
	 */
	public void leave(StatusListener listener);
	
	/**
	 * Destroys the current channel.
	 * 
	 * @param listener Listener that will receive callback with the result.
	 */
	public void destroy(StatusListener listener);	

	/**
	 * Declines an invite on this channel.
	 * @param listener StatusListener to report status of the async operation via callback.
	 * 
	 * @param listener Listener that will receive callback with the result.
	 */
	public void declineInvitation(StatusListener listener);
	
	/**
	 * Called when Member starts typing on this channel.
	 * 
	 */
	public void typing();
}


