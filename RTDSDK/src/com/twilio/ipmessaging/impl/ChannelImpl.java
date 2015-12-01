package com.twilio.ipmessaging.impl;

import java.util.HashMap;
import java.util.Map;

import org.json.JSONException;
import org.json.JSONObject;

import com.twilio.ipmessaging.Channel;
import com.twilio.ipmessaging.ChannelListener;
import com.twilio.ipmessaging.Constants.StatusListener;
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
	protected String friendlyName;

	/** The messages list object for this channel. */
	Messages messages;

	/** The members list object for this channel. */
	Members members;

	/** The current user's status on this channel. */
	protected ChannelStatus status;
	
	/** The unique name for this channel. */
	protected String uniqueName;

	/** The channel's visibility type. */
	protected ChannelType type;
	protected long nativeChannelContextHandle;
	
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
	public Channel.ChannelStatus getStatus() {
		logger.d("getStatus called");
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
		
		logger.e("Setting listener for: " + this.hashCode()+"|"+this.sid);
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
	public Map<String, String> getAttributes() {
		Map<String,String> attrMap = new HashMap<String,String>();
		String attrString = getChannelAttributesNative(this.nativeChannelContextHandle);
		try {
			JSONObject jsonObj = new JSONObject(attrString);
			attrMap = Utils.toMap(jsonObj);
			return attrMap;
		} catch (JSONException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return null;
		}				
	}

	@Override
	public void setAttributes(Map<String, String> updatedAttributes, StatusListener listener) {
		if(updatedAttributes != null) {
			if (listener != null) {
				JSONObject jsonObj = new JSONObject(updatedAttributes);
				if(jsonObj != null) { 
					this.updateChannelAttributes(getNativeClientContextHandle(), this.getSid(), jsonObj.toString(), listener);
				}
			} else {
				logger.e("StatusListener is null.");
			}
		}
	}

	@Override
	public String getFriendlyName() {
		return this.friendlyName;
	}
	
	@Override
	public void setFriendlyName(String friendlyName, StatusListener listener) {
		if (friendlyName != null && this.getSid() != null) {
			synchronized(this) {
				updateChannelName(this.nativeChannelContextHandle, this.getSid(), friendlyName, listener);
			}
		}
	}
	
	@Override
	public ChannelType getType() {
		return this.type;
	}
	
	@Override
	public void setType(ChannelType type, StatusListener listener) {
		if (listener != null) {
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
				updateChannelType(this.nativeChannelContextHandle, this.getSid(), channelType, listener);
			}
		} else {
			logger.e("StatusListener is null.");
		}
	}
	
	@Override
	public String getUniqueName() {
		return getUniqueName(this.nativeChannelContextHandle);
	}
	
	@Override
	public void setUniqueName(String uniqueName, StatusListener listener) {
		this.updateUniqueName(this.nativeChannelContextHandle,this.getSid(), uniqueName, listener);
	}

	@Override
	public void join(StatusListener statusListener) {
		logger.d("channelimpl join called");
		if (this.getSid() != null) {
			synchronized (this) {
				this.joinChannel(this.nativeChannelContextHandle, this.getSid(), statusListener);
			}
		}
	}

	@Override
	public void leave(StatusListener statusListener) {
		logger.d("channelimpl leave called");
		if (this.getSid() != null) {
			synchronized (this) {
				this.leaveChannel(this.nativeChannelContextHandle, this.getSid(), statusListener);
			}
		}
	}

	@Override
	public void destroy(StatusListener statusListener) {
		logger.d("channelimpl destroy called");
		if (this.getSid() != null) {
			synchronized (this) {
				destroyChannel(this.nativeChannelContextHandle, this.getSid(), statusListener);
			}
		}
	}
	
	@Override
	public void declineInvitation(StatusListener statusListener) {
		logger.d("channelimpl decline called");
		if (this.getSid() != null) {
			this.declineChannelInvite(this.nativeChannelContextHandle, this.getSid(), statusListener);
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
		 dest.writeLong(this.nativeChannelContextHandle);
	}


	/* Parcelable */
    public static final Parcelable.Creator<ChannelImpl> CREATOR = new Parcelable.Creator<ChannelImpl>()
    {
    	@Override
        public ChannelImpl createFromParcel(Parcel in)
        {
            String sid = in.readString();
            String friendlyName = in.readString();
            long nativeHandleContext = in.readLong();
            ChannelImpl chImpl = new ChannelImpl(friendlyName, sid, nativeHandleContext);
            return chImpl;
        }

		@Override
		public ChannelImpl[] newArray(int size) {
			// TODO Auto-generated method stub
			return null;
		}
    };
    
	private long getNativeClientContextHandle() {
		return this.nativeChannelContextHandle;
	}
    
   
	public void handleIncomingMessage(final MessageImpl message) {
		logger.d("setupListenerHandler for channel: " + this.getFriendlyName() + ", handler is " + handler + " hashCode:" + this.hashCode());
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
	
	public void handleEditMessage(final MessageImpl message) {
		logger.d("setupListenerHandler for channel: " + this.getFriendlyName() + ", handler is " + handler + " hashCode:" + this.hashCode());
		if (handler != null) {
			handler.post(new Runnable() {
				@Override
				public void run() {
					if(listener!= null) {
					 listener.onMessageChange(message);
					}
				}
			});
		}
	}

	public void handleDeleteMessage(final MessageImpl message) {
		logger.d("setupListenerHandler for channel: " + this.getFriendlyName() + ", handler is " + handler + " hashCode:" + this.hashCode());
		if (handler != null) {
			handler.post(new Runnable() {
				@Override
				public void run() {
					if(listener!= null) {
					 listener.onMessageDelete(message);
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
			throw new IllegalArgumentException("Channel Listener must have a Looper.");
		}
		logger.d("*****setupListenerHandler for channel: " + this.getSid() + ", handler is " + handler);
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
	
	public void handleOnChannelSync(final ChannelImpl channel) {
		logger.e("handleOnChannelSync 1 " + this.hashCode());
		if (handler != null) {
			logger.e("handleOnChannelSync 2");
			handler.post(new Runnable() {
				@Override
				public void run() {
					logger.e("handleOnChannelSync 3");
					if(listener!= null) {
						logger.e("handleOnChannelSync 4");
						listener.onChannelHistoryLoaded(channel);
					}
				}
			});
		}		
	}
	
	private native void joinChannel(long nativeChannelContextHandle, String channel_sid, StatusListener listener);
	private native void leaveChannel(long nativeChannelContextHandle, String channel_sid, StatusListener listener);
	private native void destroyChannel(long nativeChannelContextHandle, String channel_sid, StatusListener listener);
	private native void declineChannelInvite(long nativeChannelContextHandle, String channel_sid, StatusListener listener);
	
	private native void updateChannelName(long nativeChannelContextHandle, String channel_sid, String name, StatusListener listener);
	private native void updateChannelType(long nativeChannelContextHandle, String channel_sid, int channelType, StatusListener listener);
	private native void updateChannelAttributes(long nativeChannelContextHandle, String channel_sid, String attrMap, StatusListener listener);
	private native void updateUniqueName(long nativeChannelContextHandle, String channel_sid, String name, StatusListener listener);
	
	private native Messages getMessagesObject(long nativeChannelContextHandle, String channel_sid);
	private native int getStatus(long nativeChannelContextHandle, String channel_sid);   
	private native Members getMembers(long nativeChannelContextHandle, String channel_sid);
	private native String getChannelSidNative(long nativeChannelContextHandle);
	private native void typingStartNative(long nativeChannelContextHandle);
	private native String getChannelAttributesNative(long nativeChannelContextHandle);
	private native String getUniqueName(long nativeChannelContextHandle);

	

}
