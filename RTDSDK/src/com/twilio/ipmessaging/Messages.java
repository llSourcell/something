package com.twilio.ipmessaging;

/**
 * Representation of an IP Messaging channel's message list.
 * 
 * @author kbagchi
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
	 */
	public void sendMessage(Message message);
	
	/**
	 * Returns messages received so far from backend, messages are loaded asynchronously so this may 
	 * not fully represent all history available for channel.
	 * 
	 */
	public Message[] getMessages();
}
