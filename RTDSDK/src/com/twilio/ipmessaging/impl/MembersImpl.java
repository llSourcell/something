package com.twilio.ipmessaging.impl;

import java.util.Arrays;

import com.twilio.ipmessaging.Constants.StatusListener;
import com.twilio.ipmessaging.Member;
import com.twilio.ipmessaging.Members;
import com.twilio.ipmessaging.TwilioIPMessagingClientService;

import android.util.Log;

public class MembersImpl implements Members{
	
	private static final Logger logger = Logger.getLogger(MembersImpl.class);
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
	public void addByIdentity(String identity, StatusListener listener) {
		Member[] memeberList = getMembers();
		for (Member member : memeberList) {
			if(member != null) {
				if (member.getIdentity().compareTo(identity) == 0) {
					if (listener != null) {
						listener.onError();
						return;
					}
				}
			}
		}
		if(listener != null) {
			this.add(identity, this.nativeMembersContextHandle, listener);
		} else {
			logger.e("Listener is null.");
		}
	}

	@Override
	public void inviteByIdentity(String identity, StatusListener listener) {
		Member[] memeberList = getMembers();
		for (Member member : memeberList) {
			if(member != null) {
				if (member.getIdentity().compareTo(identity) == 0) {
					if (listener != null) {
						listener.onError();
						return;
					}
				}
			}
		}
		if(listener != null) {
			this.invite(identity, this.nativeMembersContextHandle, listener);
		} else {
			logger.e("Listener is null.");
		}
	}

	@Override
	public void removeMember(Member member, StatusListener listener) {
		long nativeMemberContextHandle = ((MemberImpl)member).getNativeMemberHandler();
		if(member != null) {
			if(listener != null) {
				remove(nativeMemberContextHandle, this.nativeMembersContextHandle, listener);
			} else {
				logger.e("Listener is null.");
			}
		}
		
	}
	
	private native Member[] getMembersNative(long nativeMembersHandle);
	private native void invite(String member, long nativeMembersHandle, StatusListener listener);
	private native void add(String member, long nativeMembersHandle, StatusListener listener);
	private native void remove(long nativeMemberHandle, long nativeMembersHandle, StatusListener listener);

}
