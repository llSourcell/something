package com.twilio.ipmessaging.impl;

import java.util.ArrayList;
import java.util.List;

import com.twilio.ipmessaging.Channel;
import com.twilio.ipmessaging.Channel.ChannelType;
import com.twilio.ipmessaging.Channels;
import com.twilio.ipmessaging.Constants.CreateChannelListener;
import com.twilio.ipmessaging.Constants.StatusListener;
import android.os.AsyncTask;
import android.os.Handler;
import android.os.Looper;


public class ChannelsImpl implements Channels {

	private static final Logger logger = Logger.getLogger(ChannelsImpl.class);
	private long nativeChannelsHandler;
	private Channel[] alllObjects;
	private Handler handler;

	public ChannelsImpl(long handler) {
		super();
		this.nativeChannelsHandler = handler;
	}
	
	@Override
	public void createChannel(String friendlyName, ChannelType type, CreateChannelListener listener) {
		int nativeType = 0;
		switch (type) {
			case CHANNEL_TYPE_PUBLIC:
				nativeType = 0;
				break;
			case CHANNEL_TYPE_PRIVATE:
				nativeType = 1;
				break;
			default:
				break;
		}
	
		if(listener != null) {
			this.createChannelNativeWithListener(friendlyName, nativeType, this.nativeChannelsHandler, listener);
		} else {
			this.createChannelNative(friendlyName, nativeType, this.nativeChannelsHandler);
		}
	}


	@Override
	public Channel getChannel(String channelId) {
		
		return TwilioIPMessagingClientImpl.publicChannelMap.get(channelId);//this.getChannelNative(channelId, nativeChannelsHandler);
	}
	
	@Override
	public Channel[] getChannels() {	
		return this.alllObjects;
	}

	public Channel[] getChannelsArraysAndCombine() {	
		
		alllObjects =  getChannelsNative(this.nativeChannelsHandler);

		for(int i=0; i<alllObjects.length; i++) {
			if(alllObjects[i] != null) {
				TwilioIPMessagingClientImpl.publicChannelMap.put(alllObjects[i].getSid(), (ChannelImpl) alllObjects[i]);
			}
		}
		List<ChannelImpl> list = new ArrayList<ChannelImpl>(TwilioIPMessagingClientImpl.privateChannelList.values());
		if(list != null && list.size() > 0) {
			ChannelImpl[] privateChannel = list.toArray(new ChannelImpl[list.size()]);
			int publicChannelArrayLength = alllObjects.length;
		    int privateChannelLength = list.toArray().length;
		    ChannelImpl[] combineChannelArray= new ChannelImpl[publicChannelArrayLength+privateChannelLength];
		    System.arraycopy(alllObjects, 0, combineChannelArray, 0, publicChannelArrayLength);
		    System.arraycopy(privateChannel, 0, combineChannelArray, publicChannelArrayLength, privateChannelLength);
		    return combineChannelArray;
		}
		return alllObjects;
	}
	
	@Override
	public void loadChannelsWithListener(StatusListener listener) {
		//getChannels1();
		setupListenerHandler();
		 new GetChannelsTask(listener).executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
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

	
	private native void createChannelNative(String friendlyName, int type, long nativeChannelsContext);
	private native void createChannelNativeWithListener(String friendlyName, int type, long nativeChannelsContext, CreateChannelListener listener);
	private native ChannelImpl getChannelNative(String channelId, long handle);
	private native ChannelImpl[] getChannelsNative(long handle);
	
	private class GetChannelsTask extends AsyncTask<Void, Void, Void> {
		StatusListener listener;
		
		 public GetChannelsTask(StatusListener statusListener) {
		        super();
		        this.listener = statusListener;
		    }
		
		@Override
		protected void onPostExecute(Void result) {
			super.onPostExecute(result);
			if (handler != null) {
				handler.post(new Runnable() {
					@Override
					public void run() {
						if(listener!= null) {
							listener.onSuccess();
						}
					}
				});
			}
			
		}

		@Override
		protected Void doInBackground(Void... params) {
			getChannelsArraysAndCombine();
			return null;
		}

	}

}