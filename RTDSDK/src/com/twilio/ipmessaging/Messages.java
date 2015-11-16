package com.twilio.ipmessaging;

import com.twilio.ipmessaging.Constants.StatusListener;

/**
 * Representation of an IP Messaging channel's message list.
 * 
 */
public interface Messages
{
	/**
	 * Method to createMessage.
	 * 
	 * @param message	Message body
	 * @return the newly created message.
	 */
	public Message createMessage(String message);

	/**
	 * Sends a message to the channel.
	 * 
	 * @param message	The message to send.
	 * @param listener  Status listener to report result of the operation.
	 */
	public void sendMessage(Message message, StatusListener listener);
	
	/**
	 * Removes a message to the channel.
	 * 
	 * @param message	The message to remove.
	 * @param listener  Status listener to report result of the operation.
	 */
	public void removeMessage(Message message, StatusListener listener);
	
	/**
	 * Returns messages received so far from backend, messages are loaded asynchronously so this may 
	 * not fully represent all history available for channel.
	 * 
	 */
	public Message[] getMessages();
}
