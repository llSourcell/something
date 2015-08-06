package com.twilio.ipmessaging.impl;

import java.util.Map;
import java.util.Set;

import com.twilio.ipmessaging.Channel;
import com.twilio.ipmessaging.ChannelListener;
import com.twilio.ipmessaging.Member;
import com.twilio.ipmessaging.Message;
import com.twilio.ipmessaging.Messages;

public class ChannelImpl implements Channel {

	@Override
	public String getSid() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public String getFriendlyName() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Map<String, String> getAttributes() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Channel.ChannelStatus getStatus() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public void setListener(ChannelListener listener) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public ChannelListener getListener() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Set<Member> getMembers() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public void updateAttributes(Map<String, String> updatedAttributes) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void updateFriendlyName(String friendlyName) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void join() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void leave() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void destroy() {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void removeMember(Member member) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public Messages getMessages(int count) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public void addByIdentity(String identity) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void inviteByIdentity(String identity) {
		// TODO Auto-generated method stub
		
	}



}
