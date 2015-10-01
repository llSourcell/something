package com.twilio.ipmessaging.impl;


import java.util.Map;
import java.util.Set;

import com.twilio.ipmessaging.Channel;
import com.twilio.ipmessaging.ChannelListener;
import com.twilio.ipmessaging.Member;
import com.twilio.ipmessaging.Members;
import com.twilio.ipmessaging.Message;
import com.twilio.ipmessaging.Messages;

import android.app.PendingIntent;
import android.app.PendingIntent.CanceledException;
import android.content.Intent;
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
	
	private PendingIntent incomingIntent;
	
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
		logger.d("getStatus called");
		long nativeClientHandle = TwilioIPMessagingClientImpl.getInstance().getNativeClientParam();
		int status = getStatus(nativeClientHandle, this.getSid());
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
		this.listener = listener;
	}

	@Override
	public ChannelListener getListener() {		
		return this.listener;
	}

	@Override
	public Members getMembers() {
		
		return null;
	}
	
	@Override
	public Members getMemberArray() {
		
		return getMembers(this.getNativeClientContextHandle(), this.sid);
	}

	@Override
	public void updateAttributes(Map<String, String> updatedAttributes) {
		this.updateChannelAttributes(getNativeClientContextHandle(), this.getSid(), updatedAttributes);
	}

	@Override
	public void updateFriendlyName(String friendlyName) {
		long nativeClientHandle = TwilioIPMessagingClientImpl.getInstance().getNativeClientParam();
		updateChannelName(nativeClientHandle, this.getSid(), friendlyName);
	}

	@Override
	public void join() {
		logger.d("channelimpl join called");
		long nativeClientHandle = TwilioIPMessagingClientImpl.getInstance().getNativeClientParam();
		this.joinChannel(nativeClientHandle, this.getSid());
	}

	@Override
	public void leave() {
		logger.d("channelimpl leave called");
		long nativeClientHandle = TwilioIPMessagingClientImpl.getInstance().getNativeClientParam();
		this.leaveChannel(nativeClientHandle, this.getSid());
	}

	@Override
	public void destroy() {
		logger.d("channelimpl destroy called");
		long nativeClientHandle = TwilioIPMessagingClientImpl.getInstance().getNativeClientParam();
		destroyChannel(nativeClientHandle, this.getSid());
	}
	
	@Override
	public void declineInvitation() {
		logger.d("channelimpl decline called");
		long nativeClientHandle = TwilioIPMessagingClientImpl.getInstance().getNativeClientParam();
		this.declineChannelInvite(nativeClientHandle, this.getSid());
		
	}


	@Override
	public void removeMember(Member member) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public Messages getMessages(int count) {
		long nativeClientHandle = TwilioIPMessagingClientImpl.getInstance().getNativeClientParam();
		return getMessagesObject(nativeClientHandle, this.sid);
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
	
	
	@Override
	public int describeContents() {
		// TODO Auto-generated method stub
		return 0;
	}
	
	@Override
	public void setIncomingIntent(PendingIntent inIntent) {
		this.incomingIntent = inIntent;
		ChannelImpl channelCopy = TwilioIPMessagingClientImpl.publicChannelMap.get(this.sid);
		channelCopy.incomingIntent = inIntent;
	}
	

	public PendingIntent getIncomingIntent() {
		return incomingIntent;
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
    
	private long getNativeClientContextHandle() {
		return TwilioIPMessagingClientImpl.getInstance().getNativeClientParam();
	}
    
   
	public void handleIncomingMessage(MessageImpl message) {
		if (this.incomingIntent != null) {
			Intent intent = new Intent();
			intent.putExtra(Channel.EXTRA_CHANNEL, this);
			intent.putExtra(Channel.EXTRA_MESSAGE, message);
			try {
				this.incomingIntent.send(TwilioIPMessagingClientImpl.getContext(), 0, intent);
			} catch (final CanceledException e) {
				logger.e(
						"Unable to send PendingIntent for incoming connection", e);
			}
		}
	}
	
	
	public native Messages getMessagesObject(long nativeClientParamHandle, String channel_sid);
    public native int getStatus(long nativeClientParamHandle, String channel_sid);   
    public native void joinChannel(long nativeClientParamHandle, String channel_sid);
	public native void leaveChannel(long nativeClientParamHandle, String channel_sid);
	public native void destroyChannel(long nativeClientParamHandle, String channel_sid);
	public native void updateChannelName(long nativeClientParamHandle, String channel_sid, String name);	
	public native Members getMembers(long nativeClientParamHandle, String channel_sid);
	public native void updateChannelAttributes(long nativeClientParamHandle, String channel_sid, Map<String, String> attrMap);
	public native void declineChannelInvite(long nativeClientParamHandle, String channel_sid);
}
