package com.twilio.ipmessaging.impl;

import java.util.Map;
import java.util.UUID;
import java.util.concurrent.ConcurrentHashMap;

import org.json.JSONObject;
import com.twilio.common.TwilioAccessManager;
import com.twilio.ipmessaging.Channel;
import com.twilio.ipmessaging.Channel.ChannelType;
import com.twilio.ipmessaging.ChannelListener;
import com.twilio.ipmessaging.Channels;
import com.twilio.ipmessaging.Constants;
import com.twilio.ipmessaging.Constants.StatusListener;
import com.twilio.ipmessaging.IPMessagingClientListener;
import com.twilio.ipmessaging.TwilioIPMessagingClient;
import com.twilio.ipmessaging.Version;

import android.app.PendingIntent;
import android.app.PendingIntent.CanceledException;
import android.content.Context;
import android.content.Intent;
import android.os.Handler;
import android.util.Log;

public class TwilioIPMessagingClientImpl implements TwilioIPMessagingClient {
	
	private static final Logger logger = Logger.getLogger(TwilioIPMessagingClientImpl.class);
	private static final String TAG = "TwilioIPMessagingClientImpl-java";
	private Context context;	
	private String identity;
	private IPMessagingClientListener ipMessagingListener;
	private IPMessagingClientListenerInternal ipMessagingClientListenerInternal;
	private StatusListener gcmRegistrationStatusListener;
	private StatusListener appGCMRegistrationStatusListener;
	private long nativeClientParamContextHandle;
	private PendingIntent incomingIntent;
	private Channels channels;
	protected final Map<String, ChannelImpl> publicChannelMap = new ConcurrentHashMap<String, ChannelImpl>();
	protected String endpointPlatform = "Android|"+Version.SDK_VERSION+"|"+Utils.getDeviceManufacturer()+ "|"+Utils.getDeviceName()+"|"+Utils.getSDKVersion();
	
	protected final Map<String, Map<ChannelListener, Handler>> channelListenerMap = new ConcurrentHashMap<String, Map<ChannelListener, Handler>>();
	
	private final UUID uuid = UUID.randomUUID();
	private TwilioAccessManager accessManager;
		
	public TwilioIPMessagingClientImpl(Context context2, String token, IPMessagingClientListener inListener) {
		this.context = context2;
		this.ipMessagingListener = inListener;
		this.gcmRegistrationStatusListener = new StatusListener() {

			@Override
			public void onError() {
				if (appGCMRegistrationStatusListener != null) {
					appGCMRegistrationStatusListener.onError();
				}
			}

			@Override
			public void onSuccess() {
				if (appGCMRegistrationStatusListener != null) {
					appGCMRegistrationStatusListener.onSuccess();
				}
			}
		};
		this.ipMessagingClientListenerInternal = new IPMessagingClientListenerInternal(this, inListener);
		nativeClientParamContextHandle = initNative(token, ipMessagingClientListenerInternal, gcmRegistrationStatusListener);
		
		createMessagingClient(token, this.nativeClientParamContextHandle, endpointPlatform);
	}


	public TwilioIPMessagingClientImpl(Context context2, String token, TwilioAccessManager accessMgr,
			IPMessagingClientListener inListener) {
		this.context = context2;
		this.ipMessagingListener = inListener;		
		this.accessManager = accessMgr;
		this.gcmRegistrationStatusListener = new StatusListener() {

			@Override
			public void onError() {
				if (appGCMRegistrationStatusListener != null) {
					appGCMRegistrationStatusListener.onError();
				}
			}

			@Override
			public void onSuccess() {
				if (appGCMRegistrationStatusListener != null) {
					appGCMRegistrationStatusListener.onSuccess();
				}
			}
		};
		this.ipMessagingClientListenerInternal = new IPMessagingClientListenerInternal(this, inListener);
		nativeClientParamContextHandle = initNative(token, ipMessagingClientListenerInternal, gcmRegistrationStatusListener);
		createMessagingClient(token, this.nativeClientParamContextHandle, endpointPlatform);
	}


	protected long getNativeClientParam() {
		return nativeClientParamContextHandle;
	}
	
	@Override
	public void setListener(IPMessagingClientListener listener) {
		this.ipMessagingListener = listener;
	}

	@Override
	public IPMessagingClientListener getListener() {
		return this.ipMessagingListener;
	}

	@Override
	public void updateToken(String accessToken, StatusListener listener) {
		if(accessToken != null) {
			synchronized(this) {
				logger.d("Calling update Token");
				this.updateToken(accessToken, this.nativeClientParamContextHandle, listener);
			}
		}
	}

	@Override
	public Channels getChannels() {	
		if(channels == null) {
			channels =  getChannelsNative(this.nativeClientParamContextHandle);
		} 
		return channels;
	}

	@Override
	public String getIdentity() {
		if(this.accessManager != null) {
			return this.accessManager.getIdentity();
		} else {
			return null;
		}
		
	}
	
	@Override
	public void shutdown() {
		synchronized(this) {
			this.shutDownNative(this.nativeClientParamContextHandle);
		}
		
	}
	
	public Context getContext() {
		return context;
	}
	
	public void setIncomingIntent(PendingIntent intent) {
		this.incomingIntent = intent;
	}
	
	public void handleIncomingInvite(Channel channel) {
		if (this.incomingIntent != null) {
			Intent intent = new Intent();
			intent.putExtra(Constants.EXTRA_CHANNEL, (ChannelImpl)channel);
			intent.putExtra(Constants.EXTRA_ACTION, Constants.EXTRA_ACTION_INVITE);
			try {
				this.incomingIntent.send(this.getContext(), 0, intent);
			} catch (final CanceledException e) {
				logger.e(
						"Unable to send PendingIntent for incoming connection", e);
			}
		}
	}
	
	public void handleChannelAddEvent(ChannelImpl channel) {
		logger.d("handleChannelAddEvent |" + channel.getSid());
		if (this.ipMessagingListener != null) {
			if (channel != null) {
				ChannelImpl existingChannel = publicChannelMap.get(channel.getSid());
				if (existingChannel == null) {
					logger.d("handleChannelAddEvent existingChannel is null.");
					synchronized(this) {
						logger.d("Adding channel cid" + channel.getSid() + " hashCode is: " + channel.hashCode());
						publicChannelMap.put(channel.getSid(), channel);
					}
				} else {
					logger.d("handleChannelAddEvent no add|" + existingChannel.getSid() +"|" + existingChannel.hashCode());
				}
			}
			this.ipMessagingListener.onChannelAdd(channel);
		}
	}
	
	public void handleChannelCreate(ChannelImpl channel) {
		logger.d("handleChannelCreate " + channel.hashCode() + "|"+ channel.getSid());
		if(this.ipMessagingListener != null) {
			if(channel != null) {
				logger.d("handleChannelCreate" + channel.hashCode());
				ChannelImpl existingChannel = publicChannelMap.get(channel.getSid());
				if (existingChannel == null) {
					logger.d("*****Adding channel cid" +channel.getSid() + " hashCode is: " + channel.hashCode());
					synchronized(this) {
						publicChannelMap.put(channel.getSid(), channel);
					}
				} 
			}
		}
	}
	
	public void handleChannelChanged(ChannelImpl channel) {
		if(this.ipMessagingListener != null) {
			ChannelImpl existingChannel;
			if(channel != null) {
				if(channel.getType() == ChannelType.CHANNEL_TYPE_PRIVATE) {
					 existingChannel = publicChannelMap.get(channel.getSid());
					if(existingChannel != null) {
						logger.d("*****EXISTING Changed channel cid " +channel.getSid() + " hashCode is: " + existingChannel.hashCode());
						synchronized(this) {
							existingChannel.friendlyName = channel.friendlyName;
							existingChannel.nativeChannelContextHandle = channel.nativeChannelContextHandle;
							existingChannel.status = channel.status;
							existingChannel.type = channel.type;
						}
						this.ipMessagingListener.onChannelChange(existingChannel);
					} else {
						logger.d("Changed channel cid " +channel.getSid() + " hashCode is: " + channel.hashCode());
						synchronized(this) {
							publicChannelMap.put(channel.getSid(), channel);
						}
						this.ipMessagingListener.onChannelChange(channel);
					}
					
				} else if(channel.getType() == ChannelType.CHANNEL_TYPE_PUBLIC) {
					existingChannel = publicChannelMap.get(channel.getSid());
					if(existingChannel != null) {
						logger.d("*****EXISTING Changed channel cid " +channel.getSid() + " hashCode is: " + existingChannel.hashCode());
						synchronized(this) {
							existingChannel.friendlyName = channel.friendlyName;
							existingChannel.nativeChannelContextHandle = channel.nativeChannelContextHandle;
							existingChannel.status = channel.status;
							existingChannel.type = channel.type;
						}
						this.ipMessagingListener.onChannelChange(existingChannel);
					} else {
						logger.d("*****Changed channel cid " +channel.getSid() + " hashCode is: " + channel.hashCode());
						synchronized(this) {
							publicChannelMap.put(channel.getSid(), channel);
						}
						this.ipMessagingListener.onChannelChange(channel);
					}
				}
			}
		}
	}
	
	public void handleChannelDeleted(ChannelImpl channel) {
		if(this.ipMessagingListener != null) {
			synchronized(this) {
				publicChannelMap.remove(channel.getSid());
			}
			this.ipMessagingListener.onChannelDelete(channel);
		}
	}
	
	public void handleChannelAttributeChange(String attribute) {
		if(this.ipMessagingListener != null) {
			this.ipMessagingListener.onAttributesChange(attribute);
		}
	}
	
	public void handleOnChannelSync(ChannelImpl channel) {
		if(this.ipMessagingListener != null) {
			this.ipMessagingListener.onChannelHistoryLoaded(channel);
		}
	}
	
	UUID getUUID()
	{
		return uuid;
	}
	
	protected IPMessagingClientListenerInternal getInternalListener(){
		return this.ipMessagingClientListenerInternal;
	}
	
	@Override
	public void registerGCMToken(String token, StatusListener listener) {
		this.appGCMRegistrationStatusListener = listener;
		registerWithToken(this.nativeClientParamContextHandle, token);
	}

	@Override
	public void unregisterGCMToken(String token, StatusListener listener) {
		this.appGCMRegistrationStatusListener = listener;
		unRegisterWithToken(this.nativeClientParamContextHandle, token);
	}
	
	@Override
	public void handleNotification(Map<String, String> notification) {
		JSONObject jsonObj = new JSONObject(notification);	
		if(jsonObj != null) {
			handleNotificationNative(this.nativeClientParamContextHandle, jsonObj.toString());
		}
	}
	
	
	
	public native long initNative(String token, IPMessagingClientListenerInternal listener, StatusListener reglistener);
	public native long createMessagingClient(String token, long nativeClientParamContextHandle, String endpointPlatform);
	private native ChannelsImpl getChannelsNative(long nativeClientParam);
	private native void updateToken(String token, long nativeClientParam, StatusListener listener);
	private native void shutDownNative(long nativeClientParam);
	private native void registerWithToken(long nativeClientParam, String token);
	private native void unRegisterWithToken(long nativeClientParam, String token);
	private native void handleNotificationNative(long nativeClientParam, String notification);

}
