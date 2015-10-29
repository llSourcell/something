package com.twilio.ipmessaging.impl;

import java.util.HashMap;
import java.util.Map;

import org.json.JSONException;
import org.json.JSONObject;

import com.twilio.ipmessaging.Channel;
import com.twilio.ipmessaging.ChannelListener;
import com.twilio.ipmessaging.Member;
import com.twilio.ipmessaging.Members;
import com.twilio.ipmessaging.Messages;

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
	private long nativeChannelContextHandle;
	
	private Handler handler;
	
	public ChannelImpl(String friendlyName, String sid) {
		super();
		this.friendlyName = friendlyName;
		this.sid = sid;
	}
	
	public ChannelImpl(String friendlyName, String sid, long nativeHandle) {
		super();
		this.friendlyName = friendlyName;
		this.sid = sid;
		this.nativeChannelContextHandle = nativeHandle;
		logger.d("created channel");
	} 
	
	public ChannelImpl(String friendlyName, String sid, long nativeHandle, int status, int type) {
		super();
		this.friendlyName = friendlyName;
		this.sid = sid;
		this.nativeChannelContextHandle = nativeHandle;
		switch (status) {
			case 0:
				this.status = Channel.ChannelStatus.INVITED;
				break;
			case 1:
				this.status =Channel.ChannelStatus.JOINED;
				break;
			case 2:
				this.status = Channel.ChannelStatus.NOT_PARTICIPATING;
				break;
			default:
				break;
		}	
		
		switch (type) {
			case 0:
				this.type = Channel.ChannelType.CHANNEL_TYPE_PUBLIC;
				break;
			case 1:
				this.type = Channel.ChannelType.CHANNEL_TYPE_PRIVATE;
				break;
			default:
				break;
		}	
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
		Map<String,String> attrMap = new HashMap<String,String>();
		String attrString = getChannelAttributesNative(this.nativeChannelContextHandle);
		try {
			JSONObject jsonObj = new JSONObject(attrString);
			attrMap = Utils.toMap(jsonObj);
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}		
		return attrMap;
	}

	@Override
	public Channel.ChannelStatus getStatus() {
		logger.d("getStatus called");
		//long nativeClientHandle = TwilioIPMessagingSDKImpl.getInstance().getNativeClientParam();
		int status = getStatus(this.nativeChannelContextHandle, this.getSid());
		switch (status) {
			case 0:
				return Channel.ChannelStatus.INVITED;
			case 1:
				return Channel.ChannelStatus.JOINED;
			case 2:
				return Channel.ChannelStatus.NOT_PARTICIPATING;
		}
		return null;
	}

	@Override
	public void setListener(ChannelListener listener) {
		this.listener = listener;
		setupListenerHandler();
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
		if(updatedAttributes != null) {
			JSONObject jsonObj = new JSONObject(updatedAttributes);
			if(jsonObj != null) { 
				this.updateChannelAttributes(getNativeClientContextHandle(), this.getSid(), jsonObj.toString());
			}
		}
	}

	@Override
	public void setFriendlyName(String friendlyName) {
		if (friendlyName != null && this.getSid() != null) {
			synchronized(this) {
				updateChannelName(this.nativeChannelContextHandle, this.getSid(), friendlyName);
			}
		}
	}
	
	@Override
	public void setType(ChannelType type) {
		int channelType = 0;
		switch (type) {
		case CHANNEL_TYPE_PUBLIC:
			channelType = 0;
			break;
		case CHANNEL_TYPE_PRIVATE:
			channelType = 1;
			break;
		}
		synchronized(this) {
			updateChannelType(this.nativeChannelContextHandle, this.getSid(), channelType);
		}
	}

	@Override
	public void join() {
		logger.d("channelimpl join called");
		if (this.getSid() != null) {
			synchronized (this) {
				this.joinChannel(this.nativeChannelContextHandle, this.getSid());
			}
		}
	}

	@Override
	public void leave() {
		logger.d("channelimpl leave called");
	//	long nativeClientHandle = TwilioIPMessagingSDKImpl.getInstance().getNativeClientParam();
		if (this.getSid() != null) {
			synchronized (this) {
				this.leaveChannel(this.nativeChannelContextHandle, this.getSid());
			}
		}
	}

	@Override
	public void destroy() {
		logger.d("channelimpl destroy called");
		if(this.getSid() != null) {
			synchronized(this) {
				destroyChannel(this.nativeChannelContextHandle, this.getSid());
			}
		}
	}
	
	@Override
	public void declineInvitation() {
		logger.d("channelimpl decline called");
		if(this.getSid() != null) {
			this.declineChannelInvite(this.nativeChannelContextHandle, this.getSid());
		}
	}

	@Override
	public Messages getMessages() {
		if(this.getSid() != null) {
			return getMessagesObject(this.nativeChannelContextHandle, this.sid);
		} else {
			return null;
		}
	}
	
	@Override
	public ChannelType getType() {
		return this.type;
	}
	
	@Override
	public void typing() {
		typingStartNative(this.nativeChannelContextHandle);
	}
	
	public long getNativeHandle() {
		return this.nativeChannelContextHandle;
	}
	
	@Override
	public int describeContents() {
		// TODO Auto-generated method stub : Parcelable
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
    
	private long getNativeClientContextHandle() {
		return this.nativeChannelContextHandle;//TwilioIPMessagingSDKImpl.getInstance().getNativeClientParam();
	}
    
   
	public void handleIncomingMessage(final MessageImpl message) {
		if (handler != null) {
			handler.post(new Runnable() {
				@Override
				public void run() {
					if(listener!= null) {
					 listener.onMessageAdd(message);
					}
				}
			});
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

	public void handleOnMemberJoin(final Member member) {
		if (handler != null) {
			handler.post(new Runnable() {
				@Override
				public void run() {
					if(listener!= null) {
						listener.onMemberJoin(member);
					}
				}
			});
		}

	}

	public void handleOnMemberChange(final Member member) {
		if (handler != null) {
			handler.post(new Runnable() {
				@Override
				public void run() {
					if(listener!= null) {
						listener.onMemberChange(member);
					}
				}
			});
		}	
	}

	public void handleOnMemberDelete(final Member member) {
		if (handler != null) {
			handler.post(new Runnable() {
				@Override
				public void run() {
					if(listener!= null) {
						listener.onMemberDelete(member);
					}
				}
			});
		}	
	}
	
	public void handleOnTypingStarted(final Member member) {
		if (handler != null) {
			handler.post(new Runnable() {
				@Override
				public void run() {
					if(listener!= null) {
						listener.onTypingStarted(member);
					}
				}
			});
		}	
	}

	public void handleOnTypingEnded(final Member member) {
		if (handler != null) {
			handler.post(new Runnable() {
				@Override
				public void run() {
					if(listener!= null) {
						listener.onTypingEnded(member);
					}
				}
			});
		}	
	}

	
	private native Messages getMessagesObject(long nativeChannelContextHandle, String channel_sid);
	private native int getStatus(long nativeChannelContextHandle, String channel_sid);   
	private native void joinChannel(long nativeChannelContextHandle, String channel_sid);
	private native void leaveChannel(long nativeChannelContextHandle, String channel_sid);
	private native void destroyChannel(long nativeChannelContextHandle, String channel_sid);
	private native void updateChannelName(long nativeChannelContextHandle, String channel_sid, String name);
	private native void updateChannelType(long nativeChannelContextHandle, String channel_sid, int channelType);
	private native Members getMembers(long nativeChannelContextHandle, String channel_sid);
	private native void updateChannelAttributes(long nativeChannelContextHandle, String channel_sid, String attrMap);
	private native void declineChannelInvite(long nativeChannelContextHandle, String channel_sid);
	private native String getChannelSidNative(long nativeChannelContextHandle);
	private native void typingStartNative(long nativeChannelContextHandle);
	private native String getChannelAttributesNative(long nativeChannelContextHandle);

}
