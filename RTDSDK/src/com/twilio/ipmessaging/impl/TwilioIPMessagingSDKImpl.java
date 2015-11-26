package com.twilio.ipmessaging.impl;

import java.lang.ref.WeakReference;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.UUID;
import java.util.concurrent.atomic.AtomicReference;

import com.twilio.common.TwilioAccessManager;
import com.twilio.common.TwilioAccessManagerFactory;
import com.twilio.common.TwilioAccessManagerListener;
import com.twilio.ipmessaging.Constants.InitListener;
import com.twilio.ipmessaging.IPMessagingClientListener;
import com.twilio.ipmessaging.TwilioIPMessagingClient;
import com.twilio.ipmessaging.TwilioIPMessagingClientService;
import com.twilio.ipmessaging.TwilioIPMessagingClientService.TwilioBinder;
import com.twilio.ipmessaging.Version;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.ServiceInfo;
import android.os.IBinder;

public class TwilioIPMessagingSDKImpl implements TwilioAccessManagerListener {
	
	static {
		System.loadLibrary("twilio-rtd-native"); 
	}
	
	private static final Logger logger = Logger.getLogger(TwilioIPMessagingSDKImpl.class);
	private static final String TWILIO_IPMESSAGING_SERVICE_NAME = "com.twilio.ipmessaging.TwilioIPMessagingClientService";
	private enum State {
		UNINITIALIZED, INITIALIZING, INITIALIZED, SHUTDOWN
	}
	
	private static final String[] requiredPermissions = {
			"android.permission.INTERNET",
			"android.permission.ACCESS_NETWORK_STATE",
			"android.permission.ACCESS_WIFI_STATE",
		};
	
	private static TwilioIPMessagingSDKImpl instance = null;
	private static Context context;	
	private final AtomicReference<State> state = new AtomicReference<State>(State.UNINITIALIZED);
	private long nativeObserverHandle;
	private InitListener listener;	
	private boolean sdkIniting;
	private ServiceConnection serviceConn;
	protected TwilioBinder twBinder;
	private IPMessagingClientListener ipMessagingListener;
	private IPMessagingClientListenerInternal ipMessagingClientListenerInternal;
	
	protected final Map<UUID, WeakReference<TwilioIPMessagingClientImpl>> clients = new HashMap<UUID, WeakReference<TwilioIPMessagingClientImpl>>();

		
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
	
		if (!state.compareAndSet(State.UNINITIALIZED, State.INITIALIZING))
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
						if (service.exported) {
							throw new RuntimeException(TWILIO_IPMESSAGING_SERVICE_NAME+" is exported.  You must add android:exported=\"false\" to the <service> declaration in AndroidManifest.xml");
						}
					}
				}
			}

			if (!serviceFound) {
				throw new RuntimeException(TWILIO_IPMESSAGING_SERVICE_NAME + " is not declared in AndroidManifest.xml");
			}
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
					state.set(State.INITIALIZED);
					inListener.onInitialized();
				} else {
					state.set(State.UNINITIALIZED);
					onServiceDisconnected(name);
					inListener.onError(null);
				}
			}

			@Override
			public void onServiceDisconnected(ComponentName name)
			{
				state.set(State.UNINITIALIZED);
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
			state.set(State.UNINITIALIZED);
			inListener.onError(new RuntimeException("Failed to start TwiloRTCService.  Please ensure it is declared in AndroidManifest.xml"));
		}

	}


	protected boolean isInitializing()
	{
		return state.get() == State.INITIALIZING;
	}


	public boolean isInitialized()
	{
		return state.get() == State.INITIALIZED;
	}


	public TwilioIPMessagingClientImpl createClientWithToken(String token, IPMessagingClientListener listener) {
		if(token != null) {

			final TwilioIPMessagingClientImpl client = new TwilioIPMessagingClientImpl(context, token, listener);	
			synchronized (clients)
			{
				//TODO:: Change the key to Identity after Access Manager integration.
				clients.put(client.getUUID(), new WeakReference<TwilioIPMessagingClientImpl>(client));
			}
			return client;
		} else {
			return null;
		}
	}
	
	
	public void shutdown()
	{
		if (!state.compareAndSet(State.INITIALIZED, State.SHUTDOWN))
		{
			if (isInitializing())
				logger.w("Twilio.shutdown() called before Twilio.initialize() has finished");
			else
				logger.e("Twilio.shutdown() called before Twilio.initialize()");
			return;
		}
		
		synchronized (clients)
		{
			for (Map.Entry<UUID, WeakReference<TwilioIPMessagingClientImpl>> entry : clients.entrySet())
			{
				WeakReference<TwilioIPMessagingClientImpl> clientRef = entry.getValue();
				TwilioIPMessagingClientImpl client = clientRef.get();
				if (client != null)
					client.shutdown(); // calls disconnectAll() internally.
			}
			
		}
		
		twBinder = null;
		context.unbindService(serviceConn);
		context.stopService(new Intent(context, TwilioIPMessagingClientService.class));
		serviceConn = null;
		context = null;
		
		instance = null;
		state.set(State.UNINITIALIZED);

	}
	
	private native void create();	

	public String getVersion() {
		return Version.SDK_VERSION;
	}

	public void setLogLevel(int level) {
		Logger.setLogLevel(level);
		setLogLevelNative(level);
	}
	
	private native void setLogLevelNative(int level);
	
	public TwilioIPMessagingClient createClientWithAccessManager(TwilioAccessManager accessManager,
			IPMessagingClientListener listener) {
		if(accessManager == null) {
			return null;
		}
		//TODO :: revisit this code
		String newCapabilityToken = accessManager.getToken();		
		return createClientWithToken(newCapabilityToken, listener);
	}


	@Override
	public void onAccessManagerTokenExpire(TwilioAccessManager arg0) {
		// TODO Auto-generated method stub
		
	}


	@Override
	public void onError(TwilioAccessManager arg0, String arg1) {
		// TODO Auto-generated method stub
		
	}


	@Override
	public void onTokenUpdated(TwilioAccessManager arg0) {
		// TODO Auto-generated method stub
		
	} 
}
