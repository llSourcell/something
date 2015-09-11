package com.twilio.ipmessaging.impl;

import com.twilio.ipmessaging.Message;
import com.twilio.ipmessaging.Messages;

public class MessagesImpl implements Messages{
	
	private long nativeMessagesHandler;

	public MessagesImpl(long handler) {
		super();
		this.nativeMessagesHandler = handler;
	}

	@Override
	public Message createMessage(String message) {
		
		return createMessageNative(message);
	}

	@Override
	public void sendMessage(Message message) {
		sendMessageNative(message);
	}

	@Override
	public Message[] getMessages() {
		return this.getMessagesNative(this.nativeMessagesHandler);
	}
	
	private native Message createMessageNative(String message);
	private native void sendMessageNative(Message message);
	private native Message[] getMessagesNative(long handle);

}
