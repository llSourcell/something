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
	public void addByIdentity(String member) {
		this.add(member);
	}

	@Override
	public void inviteByIdentity(String identity) {
		this.invite(identity);
	}

	@Override
	public void removeMember(Member member) {
		//long memberContextHandle = ((MemberImpl)member).getNativeMemberHandler();
		if(member != null) {
			remove(member);
		}
		
	}
	
	private native Member[] getMembersNative();
	private native void invite(String member);
	private native void add(String member);
	private native void remove(Member handle);

}
