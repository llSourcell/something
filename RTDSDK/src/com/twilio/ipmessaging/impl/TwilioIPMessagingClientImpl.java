package com.twilio.ipmessaging.impl;


import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import com.twilio.ipmessaging.Channel;
import com.twilio.ipmessaging.Channel.ChannelType;
import com.twilio.ipmessaging.Channels;
import com.twilio.ipmessaging.IPMessagingClientListener;
import com.twilio.ipmessaging.TwilioIPMessagingClient;
import com.twilio.ipmessaging.TwilioIpMessagingClientService;
import com.twilio.ipmessaging.TwilioIpMessagingClientService.TwilioBinder;

import android.app.PendingIntent;
import android.app.PendingIntent.CanceledException;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.ServiceInfo;
import android.os.IBinder;

public class TwilioIPMessagingClientImpl extends TwilioIPMessagingClient {
	
	static {
		System.loadLibrary("twilio-rtd-native"); 
	}
	
	private static final Logger logger = Logger.getLogger(TwilioIPMessagingClientImpl.class);
	private static final String TWILIO_IPMESSAGING_SERVICE_NAME = "com.twilio.ipmessaging.TwilioIpMessagingClientService";
	
	private static final String[] requiredPermissions = {
			"android.permission.INTERNET",
			"android.permission.ACCESS_NETWORK_STATE",
			"android.permission.ACCESS_WIFI_STATE",
		};
	
	private static TwilioIPMessagingClientImpl instance = null;
	private static Context context;	
	private IPMessagingClientListener ipMessagingListener;
	private IPMessagingClientListenerInternal ipMessagingClientListenerInternal;
	private long nativeClientParamContextHandle;
	private long nativeObserverHandle;
	private InitListener listener;	
	final static Map<String, ChannelImpl> publicChannelMap = new HashMap<String, ChannelImpl>();
	final static Map<String, ChannelImpl> privateChannelList = new HashMap<String,ChannelImpl>();
	private IPMessagingClientListenerInternal internalListener;
	private PendingIntent incomingIntent;
	private boolean sdkIniting;
	private ServiceConnection serviceConn;
	protected TwilioBinder twBinder;

	
	private TwilioIPMessagingClientImpl(Context context2,
			IPMessagingClientListener inListener) {
		TwilioIPMessagingClientImpl.context = context2;
		this.ipMessagingListener = inListener;
		
		this.ipMessagingClientListenerInternal = new IPMessagingClientListenerInternal(inListener);
	}

	
	public TwilioIPMessagingClientImpl() {
		create();
	}


	public static TwilioIPMessagingClientImpl getInstance()
	{
		if (instance == null)
		{
			synchronized (TwilioIPMessagingClientImpl.class)
			{
				if (instance == null)
					instance = new TwilioIPMessagingClientImpl();
			}
		}
		
		return instance;
	}

	public void initialize(Context inContext, final TwilioIPMessagingClient.InitListener inListener) {
		TwilioIPMessagingClientImpl.context = inContext;
		this.listener = inListener;

		if (isInitialized())
		{
			inListener.onError(new RuntimeException("Twilio.initialize() already called"));
			return;
		}

		sdkIniting = true;

		try {
			PackageManager pm = inContext.getPackageManager();
			PackageInfo pinfo = pm.getPackageInfo(inContext.getPackageName(),
					PackageManager.GET_PERMISSIONS
							| PackageManager.GET_SERVICES);

			// check application permissions
			Map<String, Boolean> appPermissions = new HashMap<String, Boolean>(
					pinfo.requestedPermissions != null ? pinfo.requestedPermissions.length
							: 0);
			if (pinfo.requestedPermissions != null) {
				for (String permission : pinfo.requestedPermissions) {
					appPermissions.put(permission, true);
				}
			}

			List<String> missingPermissions = new LinkedList<String>();
			for (String permission : requiredPermissions) {
				if (!appPermissions.containsKey(permission)) {
					missingPermissions.add(permission);
				}
			}

			if (!missingPermissions.isEmpty()) {
				StringBuilder builder = new StringBuilder(
						"Your app is missing the following required permissions:");
				for (String permission : missingPermissions) {
					builder.append(' ').append(permission);
				}

				throw new RuntimeException(builder.toString());
			}

			// check that the service is declared properly
			boolean serviceFound = false;
			if (pinfo.services != null)
			{
				for (ServiceInfo service : pinfo.services)
				{
					if (service.name.equals(TWILIO_IPMESSAGING_SERVICE_NAME))
					{
						serviceFound = true;
						if (service.exported)
							throw new RuntimeException(TWILIO_IPMESSAGING_SERVICE_NAME+" is exported.  You must add android:exported=\"false\" to the <service> declaration in AndroidManifest.xml");
					}
				}
			}

			if (!serviceFound)
				throw new RuntimeException(TWILIO_IPMESSAGING_SERVICE_NAME + " is not declared in AndroidManifest.xml");
		} catch (Exception e) {
			inListener.onError(e);
			sdkIniting = false;
			return;
		}


		context = inContext;
		final Intent service = new Intent(context, TwilioIpMessagingClientService.class);

		serviceConn = new ServiceConnection()
		{
			private boolean sdkInited;

			@Override
			public void onServiceConnected(ComponentName name, IBinder binder)
			{
				sdkIniting = false;
				sdkInited = true;

				// we must never die!
				context.startService(service);
				twBinder = (TwilioBinder)binder;
				TwilioIPMessagingClientImpl twilioIpMessagingClient = twBinder.getTwiloIPMessagingClient();
				if (twilioIpMessagingClient != null &&inListener!= null)
				{
					inListener.onInitialized();
				} else {
					onServiceDisconnected(name);
					inListener.onError(null);
				}
			}

			@Override
			public void onServiceDisconnected(ComponentName name)
			{
				sdkIniting = sdkInited = false;
				twBinder = null;
				context = null;
			}
		};

		// We need to both startService() and bindService() here.  The startService() call
		// will ensure that the Service keeps running even if the calling Activity gets
		// destroyed and recreated.  The bindService() gives us the IBinder instance.

		if (!context.bindService(service, serviceConn, Context.BIND_AUTO_CREATE))
		{
			context = null;
			inListener.onError(new RuntimeException("Failed to start TwiloRTCService.  Please ensure it is declared in AndroidManifest.xml"));
		}

		
		/*if(inListener != null) {
			inListener.onInitialized();
		}*/
	}


	private boolean isInitializing() {
		// TODO Auto-generated method stub
		return false;
	}


	private boolean isInitialized() {
		// TODO Auto-generated method stub
		return false;
	}


	public TwilioIPMessagingClient initMessagingClientWithToken(String accessToken,
			Map<String, String> attributes, IPMessagingClientListener listener) {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public void setListener(IPMessagingClientListener listener) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public IPMessagingClientListener getListener() {
		return this.ipMessagingListener;
	}

	@Override
	public String getAttributes() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public void updateAttributes(Map<String, String> attributes) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void updateToken(String accessToken) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public Channels getChannels() {		
		return getChannelsNative(this.nativeClientParamContextHandle);
	}

	public TwilioIPMessagingClient initClientWithToken(String token, IPMessagingClientListener listener) {

		if (getInstance() == null) {
			TwilioIPMessagingClientImpl.instance = new TwilioIPMessagingClientImpl(context, listener);	
		}
		this.ipMessagingListener = listener;
		this.internalListener = new IPMessagingClientListenerInternal(listener);
		nativeClientParamContextHandle = initNative(token, internalListener);
		long status = createMessagingClient(token, this.nativeClientParamContextHandle);

		return instance;
	}



	public long getNativeClientParam() {
		return nativeClientParamContextHandle;
	}

	@Override
	public String getIdentity() {
		// TODO Auto-generated method stub
		return null;
	}
	
	public static Context getContext() {
		return context;
	}
	
	public void setIncomingIntent(PendingIntent intent) {
		this.incomingIntent = intent;
	}
	
	public void handleIncomingInvite(Channel channel) {
		if (this.incomingIntent != null) {
			Intent intent = new Intent();
			intent.putExtra(Channel.EXTRA_CHANNEL, (ChannelImpl)channel);
			intent.putExtra(Channel.EXTRA_ACTION, Channel.EXTRA_ACTION_INVITE);
			try {
				this.incomingIntent.send(TwilioIPMessagingClientImpl.getContext(), 0, intent);
			} catch (final CanceledException e) {
				logger.e(
						"Unable to send PendingIntent for incoming connection", e);
			}
		}
	}
	
	public void handleChannelAddEvent(ChannelImpl channel) {
		if(this.ipMessagingListener != null) {
			if(channel != null) {
				if(channel.getType() == ChannelType.CHANNEL_TYPE_PRIVATE) {
					privateChannelList.put(channel.getSid(),channel);
				} 
				publicChannelMap.put(channel.getSid(), channel);
			}
			this.ipMessagingListener.onChannelAdd(channel);
		}
	}
	
	public void handleChannelChanged(ChannelImpl channel) {
		if(this.ipMessagingListener != null) {
			if(channel != null) {
				if(channel.getType() == ChannelType.CHANNEL_TYPE_PRIVATE) {
					privateChannelList.put(channel.getSid(),channel);
				} 
				publicChannelMap.put(channel.getSid(), channel);
			}
			this.ipMessagingListener.onChannelChange(channel);
		}
	}
	
	public void handleChannelDeleted(ChannelImpl channel) {
		if(this.ipMessagingListener != null) {
			privateChannelList.remove(channel.getSid());
			publicChannelMap.remove(channel.getSid());
			this.ipMessagingListener.onChannelDelete(channel);
		}
	}
	
	public void handleChannelAttributeChange(String attribute) {
		if(this.ipMessagingListener != null) {
			this.ipMessagingListener.onAttributesChange(attribute);
		}
	}
	
	private native void create();
	public native long initNative(String token, IPMessagingClientListenerInternal listener);
	public native long createMessagingClient(String token, long nativeClientParamContextHandle);
	private native ChannelsImpl getChannelsNative(long nativeClientParam);
}
