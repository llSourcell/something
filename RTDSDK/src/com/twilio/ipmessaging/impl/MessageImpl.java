package com.twilio.ipmessaging.impl;

import java.util.Date;

import com.twilio.ipmessaging.Message;

public class MessageImpl implements Message {
	private String author;
	private Date date;
	private String body;

	public MessageImpl(String author, String body) {
		this.author = author;
		this.body = body;
	}

	@Override
	public String getSid() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public String getAuthor() {
		// TODO Auto-generated method stub
		return this.author;
	}

	@Override
	public Date getDateUpdated() {
		// TODO Auto-generated method stub
		return this.date;
	}

	@Override
	public String getMessageBody() {
		// TODO Auto-generated method stub
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

}
