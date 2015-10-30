package com.twilio.ipmessaging.impl;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

import com.twilio.ipmessaging.Channel;
import com.twilio.ipmessaging.Channel.ChannelType;
import com.twilio.ipmessaging.Channels;
import com.twilio.ipmessaging.IPMessagingClientListener;
import com.twilio.ipmessaging.TwilioIPMessagingSDK;
import com.twilio.ipmessaging.TwilioIPMessagingClientService;
import com.twilio.ipmessaging.TwilioIPMessagingClientService.TwilioBinder;
import com.twilio.ipmessaging.Constants.InitListener;
import com.twilio.ipmessaging.Constants;

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

public class TwilioIPMessagingSDKImpl extends TwilioIPMessagingSDK {
	
	static {
		System.loadLibrary("twilio-rtd-native"); 
	}
	
	private static final Logger logger = Logger.getLogger(TwilioIPMessagingSDKImpl.class);
	private static final String TWILIO_IPMESSAGING_SERVICE_NAME = "com.twilio.ipmessaging.TwilioIPMessagingClientService";
	
	private static final String[] requiredPermissions = {
			"android.permission.INTERNET",
			"android.permission.ACCESS_NETWORK_STATE",
			"android.permission.ACCESS_WIFI_STATE",
		};
	
	private static TwilioIPMessagingSDKImpl instance = null;
	private static Context context;	
	
	private long nativeObserverHandle;
	private InitListener listener;	
	
	
	private boolean sdkIniting;
	private ServiceConnection serviceConn;
	protected TwilioBinder twBinder;
	private IPMessagingClientListener ipMessagingListener;
	private IPMessagingClientListenerInternal ipMessagingClientListenerInternal;

		
	private TwilioIPMessagingSDKImpl() {
		create();
	}


	public static TwilioIPMessagingSDKImpl getInstance()
	{
		if (instance == null)
		{
			synchronized (TwilioIPMessagingSDKImpl.class)
			{
				if (instance == null)
					instance = new TwilioIPMessagingSDKImpl();
			}
		}
		
		return instance;
	}

	public void initialize(Context inContext, final InitListener inListener) {
		TwilioIPMessagingSDKImpl.context = inContext;
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
		final Intent service = new Intent(context, TwilioIPMessagingClientService.class);

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
				TwilioIPMessagingSDKImpl twilioIpMessagingClient = twBinder.getTwiloIPMessagingSDKImpl();
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
		return false;
	}

	public TwilioIPMessagingClientImpl createClientWithToken(String token, IPMessagingClientListener listener) {
		return  new TwilioIPMessagingClientImpl(context, token, listener);	
	}	
	
	private native void create();	
}
