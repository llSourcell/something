package com.twilio.ipmessaging.impl;

import java.io.UnsupportedEncodingException;

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
		byte[] utf8;
		String stringMod = null;
		try {
			utf8 = message.getBytes("UTF-8");
			stringMod = new String(utf8, "UTF-8");
		} catch (UnsupportedEncodingException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
        
		return createMessageNativeBuffer(message.getBytes());//createMessageNative(stringMod);
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
	private native Message createMessageNativeBuffer(byte[] message);
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
