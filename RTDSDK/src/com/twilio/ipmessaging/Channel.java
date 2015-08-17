package com.twilio.ipmessaging;

import java.util.Map;
import java.util.Set;

/**
 *  Container for channels.
 * 
 * @author kbagchi
 *
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
		NOT_PARTICIATING
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
	public Set<Member> getMembers();
	
	/**
	 * Updates the attribute data for this channel.
	 * 
	 * @param updatedAttributes
	 */
	public void updateAttributes(Map<String, String> updatedAttributes);
	
	/**
	 * Updates the friendly name for this channel.
	 * 
	 * @param friendlyName updated friendly name.
	 */
	public void updateFriendlyName(String friendlyName);	
	
	/**
	 * Add specified identity to this channel without inviting them.
	 * 
	 * @param identity	The identity to add to this channel.
	 */
	public void addByIdentity(String identity);
		
	/**
	 * Invite a user to this Channel.
	 * 
	 * @param identity The identity to add to this channel. 
	 */
	public void inviteByIdentity(String identity);
	
	/**
	 * Remove the specified member.
	 * 
	 * @param member
	 */
	public void removeMember(Member member);
	
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

}


