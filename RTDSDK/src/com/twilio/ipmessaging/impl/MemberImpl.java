package com.twilio.ipmessaging.impl;

import java.util.Map;

import com.twilio.ipmessaging.Member;

public class MemberImpl implements Member {
	
	private String sid;
	private String identity;
	

	public MemberImpl(String sid, String identity) {
		super();
		this.sid = sid;
		this.identity = identity;
	}

	@Override
	public String getSid() {
		// TODO Auto-generated method stub
		return sid;
	}

	@Override
	public Map<String, String> getAttributes() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public String getIdentity() {
		// TODO Auto-generated method stub
		return identity;
	}


}
