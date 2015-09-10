package com.twilio.ipmessaging.impl;

import com.twilio.ipmessaging.Message;
import com.twilio.ipmessaging.Messages;

public class MessagesImpl implements Messages{
	
	private long nativeMessagesHandler;

	public MessagesImpl() {
		super();
	}

	@Override
	public Message createMessage(String message) {
		
		return createMessageNative(message);
	}

	@Override
	public void sendMessage(Message message) {
		
	}
	
	private native Message createMessageNative(String message);
	private native void sendMessageNative(String message);

}
