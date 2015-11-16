package com.twilio.ipmessaging.impl;

import com.twilio.ipmessaging.Channel;
import com.twilio.ipmessaging.Constants.StatusListener;
import com.twilio.ipmessaging.Message;

import android.os.Parcel;
import android.os.Parcelable;

public class MessageImpl implements Message, Parcelable {
	private String author;
	private String date;
	private String body;
	private String sid;
	private long nativeMessageContextHandle; 
	private Channel channel;

	public MessageImpl(String author, String body, String timeStamp, long handle) {
		this.author = author;
		this.body = body;
		this.date = timeStamp;
		this.nativeMessageContextHandle = handle;
	}
	
	public MessageImpl(String author, String body, String timeStamp, String sid, long handle) {
		this.author = author;
		this.body = body;
		this.date = timeStamp;
		this.sid = sid;
		this.nativeMessageContextHandle = handle;
	}
	
	public MessageImpl(String author, String body) {
		this.author = author;
		this.body = body;
	}
	
	public MessageImpl(String sid, String author, String body) {
		this.author = author;
		this.body = body;
	}

	@Override
	public String getSid() {		
		return this.sid;
	}

	@Override
	public String getAuthor() {
		return this.author;
	}

	@Override
	public String getTimeStamp() {
		return this.date;
	}

	@Override
	public String getMessageBody() {
		return body;
	}

	@Override
	public void updateMessageBody(String body) {
		// TODO Auto-generated method stub

	}
	
	public void setSid(String sid) {
		this.sid = sid;
	}

	public long getNativeHandle() {
		return nativeMessageContextHandle;
	}
	
	@Override
	public String getChannelSid() {
		String sid = getChannelSidNative(this.nativeMessageContextHandle);
		return sid;
	}
	
	@Override
	public int describeContents() {
			return 0;
	}

	@Override
	public void writeToParcel(Parcel dest, int flags) {
		 dest.writeString(this.sid);
		 dest.writeString(this.author);
		 dest.writeString(this.body);		
	}
	
	/* Parcelable */
    public static final Parcelable.Creator<MessageImpl> CREATOR = new Parcelable.Creator<MessageImpl>()
    
    {
    	@Override
        public MessageImpl createFromParcel(Parcel in)
        {
            String sid = in.readString();
            String author = in.readString();
            String body = in.readString();
            
            MessageImpl msgImpl = new MessageImpl(sid, author, body);
            return msgImpl;
        }

		@Override
		public MessageImpl[] newArray(int size) {
			// TODO Auto-generated method stub
			return null;
		}
    };
    
    private native String getChannelSidNative(long nativeMessageContextHandle);
    private native void setBodyNative(long nativeMessageContextHandle, StatusListener listener);

}
