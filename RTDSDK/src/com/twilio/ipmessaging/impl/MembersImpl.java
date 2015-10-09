package com.twilio.ipmessaging.impl;

import com.twilio.ipmessaging.Member;
import com.twilio.ipmessaging.Members;

public class MembersImpl implements Members{
	
	private long nativeMembersContextHandle;

	public MembersImpl(long handler) {
		super();
		this.nativeMembersContextHandle = handler;
	}

	@Override
	public Member[] getMembers() {		
		return getMembersNative(this.nativeMembersContextHandle);
	}

	@Override
	public void addByIdentity(String member) {
		this.add(member, this.nativeMembersContextHandle);
	}

	@Override
	public void inviteByIdentity(String identity) {
		this.invite(identity, this.nativeMembersContextHandle);
	}

	@Override
	public void removeMember(Member member) {
		long nativeMemberContextHandle = ((MemberImpl)member).getNativeMemberHandler();
		if(member != null) {
			remove(nativeMemberContextHandle, this.nativeMembersContextHandle);
		}
		
	}
	
	private native Member[] getMembersNative(long nativeMembersHandle);
	private native void invite(String member, long nativeMembersHandle);
	private native void add(String member, long nativeMembersHandle);
	private native void remove(long nativeMemberHandle, long nativeMembersHandle);

}
