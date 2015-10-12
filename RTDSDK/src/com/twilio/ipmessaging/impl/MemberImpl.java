package com.twilio.ipmessaging.impl;

import java.util.Map;

import com.twilio.ipmessaging.Member;

public class MemberImpl implements Member {
	
	private String sid;
	private String identity;
	private long nativeMemberHandler;
	

	public MemberImpl(String sid, String identity) {
		super();
		this.sid = sid;
		this.identity = identity;
	}
	
	public MemberImpl(String sid, String identity, long handle) {
		super();
		this.sid = sid;
		this.identity = identity;
		this.nativeMemberHandler = handle;
	}

	@Override
	public String getSid() {
		return sid;
	}

	public long getNativeMemberHandler() {
		return nativeMemberHandler;
	}

	@Override
	public Map<String, String> getAttributes() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public String getIdentity() {
		return identity;
	}
}
