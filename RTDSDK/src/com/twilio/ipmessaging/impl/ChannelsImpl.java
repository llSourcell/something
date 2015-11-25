package com.twilio.ipmessaging.impl;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.json.JSONObject;

import com.twilio.ipmessaging.Channel;
import com.twilio.ipmessaging.Channel.ChannelType;
import com.twilio.ipmessaging.Channels;
import com.twilio.ipmessaging.Constants;
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
	private TwilioIPMessagingClientImpl ipmClient;

	public ChannelsImpl(TwilioIPMessagingClientImpl client, long handler) {
		super();
		this.ipmClient = client;
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

	//::TODO Need to implement setting attributes in JNI - need discussion.
	@Override
	public void createChannel(Map<String, Object> options, CreateChannelListener listener) {
		Map<String,String> attrMap = new HashMap<String,String>();
		int nativeType = 0;
		String friendlyName = null;
		String uniqueName = null;
		ChannelType type;
		String jsonObjStr = null;
		
		if(options != null) {
			for (Map.Entry<String, Object> entry : options.entrySet())
			{
			    String entry_key = entry.getKey();
			    if(entry_key.compareTo(Constants.CHANNEL_FRIENDLY_NAME)== 0) {
			    	friendlyName = (String)entry.getValue();
			    }
			    
			    if(entry_key.compareTo(Constants.CHANNEL_UNIQUE_NAME) == 0) {
			    	uniqueName = (String) entry.getValue();
			    }
			    
				if (entry_key.compareTo(Constants.CHANNEL_TYPE) == 0) {
					type = (ChannelType) entry.getValue();
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
				}
			    
			    if(entry_key.compareTo(Constants.CHANNE_ATTRIBUTES) == 0) {
			    	attrMap = (Map<String,String>) entry.getValue();
			    	JSONObject jsonObj = new JSONObject(attrMap);
			    	if(jsonObj != null) {
			    		jsonObjStr = jsonObj.toString();
			    	}
			    }	    
			}
		}
		
		if(listener != null) {
			this.createChannelNativeWithOptionsWithListener(friendlyName, uniqueName, jsonObjStr, nativeType, this.nativeChannelsHandler, listener);
		} else {
			this.createChannelWithOptionsNative(friendlyName,uniqueName, jsonObjStr, nativeType, this.nativeChannelsHandler);
		}
		
			
	}

	@Override
	public Channel getChannel(String channelId) {
		return this.ipmClient.publicChannelMap.get(channelId);
	}
	
	@Override
	public Channel getChannelByUniqueName(String uniqueName) {
		return this.getChannelNativeWithUniqueName(uniqueName, this.nativeChannelsHandler);
	}
	
	@Override
	public Channel[] getChannels() {	
		return this.alllObjects;
	}

	public Channel[] getChannelsArraysAndCombine() {	
		Channel[] localCopyChannelArray;
		localCopyChannelArray =  getChannelsNative(this.nativeChannelsHandler);
	
		if (this.ipmClient.publicChannelMap != null) {
			logger.d("ChannelList Size : " + this.ipmClient.publicChannelMap.size());
		}

		if(localCopyChannelArray != null ) {
			for(int i=0; i<localCopyChannelArray.length; i++) {
				if(localCopyChannelArray[i] != null) {
					this.ipmClient.publicChannelMap.put(localCopyChannelArray[i].getSid(), (ChannelImpl) localCopyChannelArray[i]);
				}
			}
		}
				
		List<ChannelImpl> list = new ArrayList<ChannelImpl>(this.ipmClient.privateChannelList.values());
		if(list != null && list.size() > 0) {
			ChannelImpl[] privateChannel = list.toArray(new ChannelImpl[list.size()]);
			int publicChannelArrayLength = localCopyChannelArray.length;
		    int privateChannelLength = list.toArray().length;
		    ChannelImpl[] combineChannelArray= new ChannelImpl[publicChannelArrayLength+privateChannelLength];
		    System.arraycopy(localCopyChannelArray, 0, combineChannelArray, 0, publicChannelArrayLength);
		    System.arraycopy(privateChannel, 0, combineChannelArray, publicChannelArrayLength, privateChannelLength);
		    alllObjects = combineChannelArray;
		} else {
			alllObjects = localCopyChannelArray;
		}
		return alllObjects;
	}
	
	@Override
	public void loadChannelsWithListener(StatusListener listener) {
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
			//TODO :: throw exception
			handler = null;
		}
	}
		
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
	
	private native void createChannelNative(String friendlyName, int type, long nativeChannelsContext);
	private native void createChannelNativeWithListener(String friendlyName, int type, long nativeChannelsContext, CreateChannelListener listener);
	private native void createChannelWithOptionsNative(String friendlyName, String uniqueName, String jsonAttr, int type, long nativeChannelsContext);
	private native void createChannelNativeWithOptionsWithListener(String friendlyName, String uniqueName, String jsonAttr, int type, long nativeChannelsContext, CreateChannelListener listener);
	private native ChannelImpl getChannelNative(String channelId, long handle);
	private native ChannelImpl[] getChannelsNative(long handle);
	private native ChannelImpl getChannelNativeWithUniqueName(String uniqueChannelName, long handle);

}