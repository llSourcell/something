package com.twilio.ipmessaging.impl;


import java.util.Map;
import java.util.Set;

import com.twilio.example.Logger;
import com.twilio.ipmessaging.Channel;
import com.twilio.ipmessaging.ChannelListener;
import com.twilio.ipmessaging.Member;
import com.twilio.ipmessaging.Members;
import com.twilio.ipmessaging.Message;
import com.twilio.ipmessaging.Messages;

import android.os.Parcel;
import android.os.Parcelable;

public class ChannelImpl implements Channel, Parcelable{
	
	private static final Logger logger = Logger.getLogger(ChannelImpl.class);
	
	/** ChannelListener */
	private ChannelListener listener;

	/** The unique identifier for this channel. */
	private String sid;

	/** The friendly name for this channel. */
	private String friendlyName;

	/** The messages list object for this channel. */
	Messages messages;

	/** The members list object for this channel. */
	Members members;

	/** The current user's status on this channel. */
	ChannelStatus status;

	/** The channel's visibility type. */
	private ChannelType type;
	private long nativeChannelHandle;


	public ChannelImpl(ChannelListener listener, String friendlyName, String sid) {
		super();
		this.listener = listener;
		this.friendlyName = friendlyName;
		this.sid = sid;
	}
	
	public ChannelImpl(String friendlyName, String sid/*, long nativeHandle*/) {
		super();
		this.friendlyName = friendlyName;
		this.sid = sid;
		//this.nativeChannelHandle = nativeHandle;
	}
	
	public ChannelImpl(String friendlyName, String sid, long nativeHandle) {
		super();
		this.friendlyName = friendlyName;
		this.sid = sid;
		this.nativeChannelHandle = nativeHandle;
	}

	@Override
	public String getSid() {
		return this.sid;
	}

	@Override
	public String getFriendlyName() {
		return this.friendlyName;
	}

	@Override
	public Map<String, String> getAttributes() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public Channel.ChannelStatus getStatus() {
		logger.d("channelimpl leave called");
		int status = getStatus(this.getSid());
		switch (status) {
		case 0:
			return Channel.ChannelStatus.INVITED;

		case 1:
			return Channel.ChannelStatus.JOINED;

		case 2:
			return Channel.ChannelStatus.NOT_PARTICIATING;
		}
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
	public Members getMembers() {
		
		return null;
	}
	
	@Override
	public Member[] getMemberArray() {
		
		return getMembers(this.sid);
	}

	@Override
	public void updateAttributes(Map<String, String> updatedAttributes) {
		
	}

	@Override
	public void updateFriendlyName(String friendlyName) {
		updateChannelName(this.getSid(), friendlyName);
	}

	@Override
	public void join() {
		logger.d("channelimpl join called");
		joinChannel(this.getSid());
	}

	@Override
	public void leave() {
		logger.d("channelimpl leave called");
		leaveChannel(this.getSid());
	}

	@Override
	public void destroy() {
		logger.d("channelimpl destroy called");
		destroyChannel(this.getSid());
	}

	@Override
	public void removeMember(Member member) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public Messages getMessages(int count) {
		
		return null;
	}
	
	public Message[] getMessages() {
		
		return getMessages(this.sid);
	}

	@Override
	public void addByIdentity(String identity) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void inviteByIdentity(String identity) {
		// TODO Auto-generated method stub
		
	}
	
	public long getNativeHandle() {
		return this.nativeChannelHandle;
	}
	
	public native void joinChannel(String channel_sid);
	public native void leaveChannel(String channel_sid);
	public native void destroyChannel(String channel_sid);
	public native int getStatus(String channel_sid);
	public native Message[] getMessages(String channel_sid);
	public native Messages getMessagesObject(String channel_sid);
	public native void updateChannelName(String channel_sid, String name);
	
	public native Member[] getMembers(String channel_sid);
	public native void updateChannelAttributes(String channel_sid, Map<String, String> attrMap);

	@Override
	public int describeContents() {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public void writeToParcel(Parcel dest, int flags) {
		 dest.writeString(this.sid);
		 dest.writeString(this.friendlyName);		
	}


	/* Parcelable */
    public static final Parcelable.Creator<ChannelImpl> CREATOR = new Parcelable.Creator<ChannelImpl>()
    {
    	@Override
        public ChannelImpl createFromParcel(Parcel in)
        {
            String sid = in.readString();
            String friendlyName = in.readString();
            
            ChannelImpl chImpl = new ChannelImpl(friendlyName, sid);
            return chImpl;
        }

		@Override
		public ChannelImpl[] newArray(int size) {
			// TODO Auto-generated method stub
			return null;
		}
    };

}
