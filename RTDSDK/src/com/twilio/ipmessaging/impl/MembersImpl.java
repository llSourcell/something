package com.twilio.ipmessaging.impl;

import com.twilio.ipmessaging.Member;
import com.twilio.ipmessaging.Members;

public class MembersImpl implements Members{
	
	private long nativeMembersHandler;

	public MembersImpl(long handler) {
		super();
		this.nativeMembersHandler = handler;
	}

	@Override
	public Member[] getMembers() {		
		return getMembersNative();
	}

	@Override
	public void addByIdentity(String identity) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void inviteByIdentity(String identity) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void removeMember(Member member) {
		// TODO Auto-generated method stub
		
	}
	
	private native Member[] getMembersNative();

}
