package com.twilio.ipmessaging.impl;

import java.nio.charset.Charset;
import java.util.Formatter;

import com.twilio.ipmessaging.Message;
import com.twilio.ipmessaging.Messages;

import android.os.Parcel;
import android.os.Parcelable;

public class MessagesImpl implements Messages , Parcelable{
	
	private long nativeMessagesContextHandler;

	public MessagesImpl(long handler) {
		super();
		this.nativeMessagesContextHandler = handler;
	}

	@Override
	public Message createMessage(String message) {
		String stringMod = null;
		String unicodedString = escapeUnicode(message);
		stringMod = new String(unicodedString.getBytes(Charset.forName("UTF-8")));
		return createMessageNativeBuffer(stringMod.getBytes());
	}

	@Override
	public void sendMessage(Message message) {
		sendMessageNative(message);
	}

	@Override
	public Message[] getMessages() {
		synchronized(this) {
			return this.getMessagesNative(this.nativeMessagesContextHandler);
		}
	}
	
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
	
	public String escapeUnicode(String input) {
		StringBuilder b = new StringBuilder(input.length());
		Formatter f = new Formatter(b);
		for (char c : input.toCharArray()) {
			if (c < 128) {
				b.append(c);
			} else {
				f.format("\\u%04x", (int) c);
			}
		}
		return b.toString();
	}
	
	private native Message createMessageNative(String message);
	private native Message createMessageNativeBuffer(byte[] message);
	private native void sendMessageNative(Message message);
	private native void removeMessageNative(Message message);
	private native Message[] getMessagesNative(long handle);


}
