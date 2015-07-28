package com.twilio.rtd;

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
		IDLE
	};
	
	/**
	 * Method that returns the unique identifier for this channel.
	 * 
	 * @return
	 */
	public String getSid();
	
	/**
	 * Method that returns the friendlyName of the channel.
	 * 
	 * @return
	 */
	public String getFriendlyName();
	
	/**
	 * Custom attributes associated with the Channel.
	 * 
	 * @return
	 */
	public Map<String, String> getAttributes();
	
	/**
	 * Returns the current user's status on this channel.
	 * 
	 * @return
	 */
	public Channel getStatus();
	
	/**
	 * Method to set ChannelListener for this Channel.
	 * 
	 * @param listener
	 */
	public void setListener(ChannelListener listener);
	
	/**
	 * Method to retrieve ChannelListener for this Channel.
	 * 
	 * @return lister for this channel.
	 */
	public ChannelListener getListener();
		
	/**
	 * Method to fetch the channel members.
	 * 
	 * @return Set of Member objects.
	 */
	public Set<Member> fetchMembers();
	
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
	 * Joins the current user to the channel.
	 */
	public void joinUser();
	
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
	 * Add specified user to this channel without inviting them.
	 * 
	 * @param userName	The userName to add to this channel.
	 */
	public void addUser(String userName);
	
	/**
	 * Invite a user to this Channel.
	 * 
	 * @param userName
	 */
	public void inviteUser(String userName);
	
	/**
	 * Remove the specified member.
	 * 
	 * @param member
	 */
	public void removeMember(Member member);
	
	/**
	 * Method to createMessage.
	 * 
	 * @param message	Message body.
	 */
	public void createMessage(String message);

}


