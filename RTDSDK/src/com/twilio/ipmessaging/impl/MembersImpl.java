package com.twilio.ipmessaging.impl;

import com.twilio.ipmessaging.Constants.StatusListener;
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
	public void addByIdentity(String member, StatusListener listener) {
		this.add(member, this.nativeMembersContextHandle, listener);
	}

	@Override
	public void inviteByIdentity(String identity, StatusListener listener) {
		this.invite(identity, this.nativeMembersContextHandle, listener);
	}

	@Override
	public void removeMember(Member member, StatusListener listener) {
		long nativeMemberContextHandle = ((MemberImpl)member).getNativeMemberHandler();
		if(member != null) {
			remove(nativeMemberContextHandle, this.nativeMembersContextHandle, listener);
		}
		
	}
	
	private native Member[] getMembersNative(long nativeMembersHandle);
	private native void invite(String member, long nativeMembersHandle, StatusListener listener);
	private native void add(String member, long nativeMembersHandle, StatusListener listener);
	private native void remove(long nativeMemberHandle, long nativeMembersHandle, StatusListener listener);

}
