package com.twilio.ipmessaging.impl;

import java.util.Date;

import com.twilio.ipmessaging.Channel;
import com.twilio.ipmessaging.Message;

import android.os.Parcel;
import android.os.Parcelable;

public class MessageImpl implements Message, Parcelable {
	private String author;
	private String date;
	private String body;
	private String sid;
	private long nativeMessageHandle; 
	private Channel channel;

	public MessageImpl(String author, String body, String timeStamp, long handle) {
		this.author = author;
		this.body = body;
		this.date = timeStamp;
		this.nativeMessageHandle = handle;
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
	public String getDateUpdated() {
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

	@Override
	public Date getDateCreated() {
		// TODO Auto-generated method stub
		return null;
	}

	public long getNativeHandle() {
		return nativeMessageHandle;
	}
	
	@Override
	public String getChannelSid() {
		String sid = getChannelSidNative();
		return sid;
	}
	
	public native String getChannelSidNative();

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

}
