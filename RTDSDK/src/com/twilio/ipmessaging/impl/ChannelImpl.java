package com.twilio.ipmessaging.impl;


import java.util.Map;

import com.twilio.ipmessaging.Channel;
import com.twilio.ipmessaging.ChannelListener;
import com.twilio.ipmessaging.Members;
import com.twilio.ipmessaging.Messages;

import android.app.PendingIntent;
import android.app.PendingIntent.CanceledException;
import android.content.Intent;
import android.os.Handler;
import android.os.Looper;
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
	
	private Handler handler;
	
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
	
	public ChannelImpl(String friendlyName, String sid, long nativeHandle, int status) {
		super();
		this.friendlyName = friendlyName;
		this.sid = sid;
		this.nativeChannelHandle = nativeHandle;
		switch (status) {
			case 0:
				this.status = Channel.ChannelStatus.INVITED;
				break;
			case 1:
				this.status =Channel.ChannelStatus.JOINED;
				break;
			case 2:
				this.status = Channel.ChannelStatus.NOT_PARTICIATING;
				return;
			default:
				break;
		}	
		
		setupListenerHandler();
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
		
		return getMembers(this.getNativeClientContextHandle(), this.sid);
	}

	@Override
	public void setAttributes(Map<String, String> updatedAttributes) {
		this.updateChannelAttributes(getNativeClientContextHandle(), this.getSid(), updatedAttributes);
	}

	@Override
	public void setFriendlyName(String friendlyName) {
		long nativeClientHandle = TwilioIPMessagingClientImpl.getInstance().getNativeClientParam();
		updateChannelName(nativeClientHandle, this.getSid(), friendlyName);
	}
	
	@Override
	public void setType(ChannelType type) {
		long nativeClientHandle = TwilioIPMessagingClientImpl.getInstance().getNativeClientParam();
		int channelType = 0;
		switch (type) {
		case CHANNEL_TYPE_PUBLIC:
			channelType = 0;
			break;
		case CHANNEL_TYPE_PRIVATE:
			channelType = 1;
			break;
		}
		updateChannelType(nativeClientHandle, this.getSid(), channelType);
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
	public Messages getMessages(int count) {
		long nativeClientHandle = TwilioIPMessagingClientImpl.getInstance().getNativeClientParam();
		return getMessagesObject(nativeClientHandle, this.sid);
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
	
	/*
	 * Use the thread looper or the main thread looper if the thread does not
	 * provide one to callback on the thread that provided the event listener.
	 */
	private void setupListenerHandler() {
		Looper looper;
		if((looper = Looper.myLooper()) != null) {
			handler = new Handler(looper);
		} else if((looper = Looper.getMainLooper()) != null) {
			handler = new Handler(looper);
		} else {
			handler = null;
		}
	}

	
	
	public native Messages getMessagesObject(long nativeClientParamHandle, String channel_sid);
    public native int getStatus(long nativeClientParamHandle, String channel_sid);   
    public native void joinChannel(long nativeClientParamHandle, String channel_sid);
	public native void leaveChannel(long nativeClientParamHandle, String channel_sid);
	public native void destroyChannel(long nativeClientParamHandle, String channel_sid);
	public native void updateChannelName(long nativeClientParamHandle, String channel_sid, String name);
	private native void updateChannelType(long nativeClientHandle, String channel_sid, int channelType);
	public native Members getMembers(long nativeClientParamHandle, String channel_sid);
	public native void updateChannelAttributes(long nativeClientParamHandle, String channel_sid, Map<String, String> attrMap);
	public native void declineChannelInvite(long nativeClientParamHandle, String channel_sid);
}
