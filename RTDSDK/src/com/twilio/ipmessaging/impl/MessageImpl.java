package com.twilio.ipmessaging.impl;

import java.util.Date;

import com.twilio.ipmessaging.Message;

public class MessageImpl implements Message {
	private String author;
	private Date date;
	private String body;
	private String sid;
	private long nativeMessageHandle; 

	public MessageImpl(String author, String body, long handle) {
		this.author = author;
		this.body = body;
		this.nativeMessageHandle = handle;
	}
	
	public MessageImpl(String author, String body) {
		this.author = author;
		this.body = body;
	}

	@Override
	public String getSid() {		
		return this.sid;
	}

	@Override
	public String getAuthor() {
		return this.author;
	}

	@Override
	public Date getDateUpdated() {
		return this.date;
	}

	@Override
	public String getMessageBody() {
		return body;
	}

	@Override
	public void updateMessageBody(String body) {
		// TODO Auto-generated method stub

	}

	@Override
	public Date getDateCreated() {
		// TODO Auto-generated method stub
		return null;
	}

	public long getNativeHandle() {
		return nativeMessageHandle;
	}

}
