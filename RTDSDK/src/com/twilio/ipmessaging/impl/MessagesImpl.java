package com.twilio.ipmessaging.impl;

import com.twilio.ipmessaging.Message;
import com.twilio.ipmessaging.Messages;

import android.os.Parcel;
import android.os.Parcelable;

public class MessagesImpl implements Messages , Parcelable{
	
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

	//parcel
	@Override
	public int describeContents() {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public void writeToParcel(Parcel dest, int flags) {
		// TODO Auto-generated method stub
		
	}

}
