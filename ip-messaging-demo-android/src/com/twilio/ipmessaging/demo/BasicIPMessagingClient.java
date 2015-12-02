package com.twilio.ipmessaging.demo;

import java.util.Arrays;
import java.util.List;

import com.twilio.common.TwilioAccessManager;
import com.twilio.common.TwilioAccessManagerFactory;
import com.twilio.common.TwilioAccessManagerListener;
import com.twilio.ipmessaging.Channel;
import com.twilio.ipmessaging.IPMessagingClientListener;
import com.twilio.ipmessaging.TwilioIPMessagingClient;
import com.twilio.ipmessaging.TwilioIPMessagingSDK;
import com.twilio.ipmessaging.Constants.InitListener;

import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class BasicIPMessagingClient implements IPMessagingClientListener, TwilioAccessManagerListener {

	private static final Logger logger = Logger.getLogger(BasicIPMessagingClient.class);
	private String capabilityToken;
	private String gcmToken;
	private long nativeClientParam;
	private TwilioIPMessagingClient ipMessagingClient;
	private TwilioIPMessagingClient ipMessagingClientWithAccessManager;
	private Channel[] channels;
	private Context context;
	private TwilioAccessManager acessMgr;
	
	public BasicIPMessagingClient(Context context) {
		super();
		this.context = context;
	}

	public interface LoginListener {
		public void onLoginStarted();

		public void onLoginFinished();

		public void onLoginError(String errorMessage);

		public void onLogoutFinished();
	}

	public String getCapabilityToken() {
		return capabilityToken;
	}

	public void setCapabilityToken(String capabilityToken) {
		this.capabilityToken = capabilityToken;
	}

	public String getGCMToken() {
		return gcmToken;
	}

	public void setGCMToken(String gcmToken) {
		this.gcmToken = gcmToken;
	}
	
	public void doLogin(final String capabilityToken, final LoginListener listener) {
		TwilioIPMessagingSDK.setLogLevel(android.util.Log.ERROR	);
		if(!TwilioIPMessagingSDK.isInitialized()) {
			TwilioIPMessagingSDK.initializeSDK(context, new InitListener()
	        {
	            @Override
	            public void onInitialized()
	            {
	            	createClientWithToken(listener);
	            	//createClientWithAccessManager(listener);
	            }
	
	            @Override
	            public void onError(Exception error)
	            {
	               logger.e("Error initializing the SDK :" + error.getMessage());
	            }
	        });
		} else {
			createClientWithToken(listener);
		}

	}

	public BasicIPMessagingClient() {
		super();
	}

	public void cleanupTest() {
		// TODO
	}
	
	public List<Channel> getChannelList() {
		List<Channel> list = Arrays.asList(this.channels);
		return list;
	}
	
	public long getNativeClientParam() {
		return nativeClientParam;
	}

	public void setNativeClientParam(long nativeClientParam) {
		this.nativeClientParam = nativeClientParam;
	}

	@Override
	public void onChannelAdd(Channel channel) {
		if(channel != null) {
			logger.d("A Channel :"+ channel.getFriendlyName() + " got added");	
		} else {
			logger.d("Received onChannelAdd event.");
		}
	}

	@Override
	public void onChannelChange(Channel channel) {
		if(channel != null) {
			logger.d("Channel Name : "+ channel.getFriendlyName() + " got Changed");	
		} else {
			logger.d("received onChannelChange event.");
		}
	}

	@Override
	public void onChannelDelete(Channel channel) {
		if(channel != null) {
			logger.d("A Channel :"+ channel.getFriendlyName() + " got deleted");	
		} else {
			logger.d("received onChannelDelete event.");
		}
	}

	@Override
	public void onError(int errorCode, String errorText) {
		logger.d("Received onError event.");	
	}

	@Override
	public void onAttributesChange(String attributes) {
		logger.d("Received onAttributesChange event.");	
	}
	
	public TwilioIPMessagingClient getIpMessagingClient() {
		return ipMessagingClient;
	}

	private void createClientWithToken(LoginListener listener) {
		ipMessagingClient = TwilioIPMessagingSDK.createIPMessagingClientWithToken(capabilityToken, BasicIPMessagingClient.this);
    	if(ipMessagingClient != null) {
    		ipMessagingClient.setListener(this);
        	Intent intent = new Intent(context,ChannelActivity.class);
        	PendingIntent pendingIntent = PendingIntent.getActivity(context, 0, intent, PendingIntent.FLAG_UPDATE_CURRENT);
        	ipMessagingClient.setIncomingIntent(pendingIntent);
        	if(listener != null) {
				listener.onLoginFinished();
        	}
    	} else {
    		listener.onLoginError("ipMessagingClient is null");
    	}
	}
	
	
	private void createClientWithAccessManager(LoginListener listener) {
		String token = "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiIsImN0eSI6InR3aWxpby1mcGE7dj0xIn0.eyJqdGkiOiJTS2E3MDA0MjRhNDExMjY3ODZhZTBiNTk5YjA3NGM3NWNmLTE0NDg2MjczNTQiLCJpc3MiOiJTS2E3MDA0MjRhNDExMjY3ODZhZTBiNTk5YjA3NGM3NWNmIiwic3ViIjoiQUNmNzVmNDRiZjUzZDQ2ZmJjYjNlNDI0MDY1N2FlMjY4ZiIsIm5iZiI6MTQ0ODYyNzM1NCwiZXhwIjoxNDQ4NjMyMzU0LCJncmFudHMiOnsiaWRlbnRpdHkiOiJBbGV4IiwiaXBfbWVzc2FnaW5nIjp7InNlcnZpY2Vfc2lkIjoiSVMzZjA2Y2NhNGIzOTk0NjlhYThhMDM5OGVkMTc4MDY0ZCIsImVuZHBvaW50X2lkIjoiMTUyNTIzLTIxMzUyMzUyMTM1LTMyNTEyMzUtMjM1In19fQ.33VHyDboW4CeIq3j8SbPdxaN8Wxcl078PPp2k3U5JfM";
        TwilioAccessManager manager = TwilioAccessManagerFactory.createAccessManager(token, new TwilioAccessManagerListener() {
            @Override
            public void onAccessManagerTokenExpire(TwilioAccessManager twilioAccessManager) {
                Log.d("Test", "token expire");
            }

            @Override
            public void onTokenUpdated(TwilioAccessManager twilioAccessManager) {
                Log.d("Test", "token updated");
            }

            @Override
            public void onError(TwilioAccessManager twilioAccessManager, String s) {
                Log.d("Test", "token error: " + s);
            }
        });
    	acessMgr = TwilioAccessManagerFactory.createAccessManager(capabilityToken, null);
		
    	
		ipMessagingClientWithAccessManager = TwilioIPMessagingSDK.createIPMessagingClientWithAccessManager(this.acessMgr, BasicIPMessagingClient.this);
    	if(ipMessagingClientWithAccessManager != null) {
    		ipMessagingClientWithAccessManager.setListener(this);
        	Intent intent = new Intent(context,ChannelActivity.class);
        	PendingIntent pendingIntent = PendingIntent.getActivity(context, 0, intent, PendingIntent.FLAG_UPDATE_CURRENT);
        	ipMessagingClientWithAccessManager.setIncomingIntent(pendingIntent);
        	if(listener != null) {
				listener.onLoginFinished();
        	}
    	} else {
    		listener.onLoginError("ipMessagingClientWithAccessManager is null");
    	}
	}

	@Override
	public void onChannelHistoryLoaded(Channel channel) {
		logger.e("Received onChannelHistoryLoaded callback " + channel.getFriendlyName());
	}

	@Override
	public void onAccessManagerTokenExpire(TwilioAccessManager arg0) {
		logger.d("Received AccessManager:onAccessManagerTokenExpire.");
	}

	@Override
	public void onError(TwilioAccessManager arg0, String arg1) {
		logger.d("Received AccessManager:onError.");
	}

	@Override
	public void onTokenUpdated(TwilioAccessManager arg0) {
		logger.d("Received AccessManager:onTokenUpdated.");
	}
}