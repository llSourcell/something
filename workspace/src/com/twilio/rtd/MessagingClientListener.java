package com.twilio.rtd;

import java.util.Map;

/**
 * This interface defines IPMessagingClient callback methods.
 * 
 * @author kbagchi
 * 
 * TODO :: Toast Notification related apis
 */
public interface MessagingClientListener {
	
	/**
	 * Called when the specified token is about to expire.
	 * 
	 * @param token the current token.
	 */
	public void onTokenExpire(String token);
	
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
	 * Called when the current user's attributes changes.
	 * 
	 * @param attributes The updated attributes.
	 */
	public void onAttributeChange(Map<String, String> attributes);
	
	/**
	 * Called when an error condition occurs.
	 * 
	 * @param errorCode	The error code specifying the error.
	 * @param errorText	The error text describing the error condition.
	 */
	public void onError(String errorCode, String errorText);
	
	/**
	 * Called when a channel the current user is subscribed to receives a new message.
	 * 
	 * @param channel The channel.
	 * @param message The message.
	 */
	public void onMessageAdded(Channel channel, Message message);
	
	/**
	 * Called when a message on a channel the current user is subscribed to is modified.
	 * 
	 * @param channel The channel.
	 * @param message The message.
	 */
	public void onMessageChanged(Channel channel, Message message);
	
	/**
	 * Called when a message on a channel the current user is subscribed to is deleted.
	 * 
	 * @param channel The channel.
	 * @param message The message.
	 */
	public void onMessageDeleted(Channel channel, Message message);
	
	/**
	 * Called when a channel the current user is subscribed to has a new member join.
	 * 
	 * @param channel The channel.
	 * @param memeber The memeber.
	 */
	public void onMemberJoined(Channel channel, Message member);
	
	/**
	 * Called when a channel the current user is subscribed to has a member modified.
	 * 
	 * @param channel The channel.
	 * @param member  The member.
	 */
	public void onMemberChanged(Channel channel, Member member);
	
	/**
	 * Called when a channel the current user is subscribed to has a member leave.
	 * 
	 * @param channel The channel.
	 * @param member The member.
	 */
	public void onMemberLeft(Channel channel, Member member);
}
