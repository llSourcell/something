package com.twilio.ipmessaging.demo;

import java.util.Arrays;
import java.util.List;

import com.twilio.common.TwilioAccessManager;
import com.twilio.common.TwilioAccessManagerFactory;
import com.twilio.common.TwilioAccessManagerListener;
import com.twilio.ipmessaging.Channel;
import com.twilio.ipmessaging.Constants.InitListener;
import com.twilio.ipmessaging.Constants.StatusListener;
import com.twilio.ipmessaging.IPMessagingClientListener;
import com.twilio.ipmessaging.TwilioIPMessagingClient;
import com.twilio.ipmessaging.TwilioIPMessagingSDK;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.content.Context;


public class BasicIPMessagingClient implements IPMessagingClientListener, TwilioAccessManagerListener {

	private static final String TAG = "BasicIPMessagingClient";
	private String accessToken;
	private long nativeClientParam;
	private TwilioIPMessagingClient ipMessagingClient;
	private Channel[] channels;
	private Context context;
	private TwilioAccessManager acessMgr;
	private Handler loginListenerHandler;
	private String urlString;
	public static final String BROADCAST_FILTER = "ManageConection_broadcast_receiver_intent_filter";


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

	public String getAccessToken() {
		return accessToken;
	}

	public void setAccessToken(String capabilityToken) {
		this.accessToken = capabilityToken;
	}



	public void doLogin(final String capabilityToken, final LoginListener listener, String url) {
		Log.v("YO", "The current token in doLogin is" + capabilityToken);

		this.urlString = url;
		this.loginListenerHandler = setupListenerHandler();
		if(!TwilioIPMessagingSDK.isInitialized()) {
			TwilioIPMessagingSDK.initializeSDK(context, new InitListener()
	        {
	            @Override
	            public void onInitialized()
	            {
	            	//createClientWithToken(listener);
	            	createClientWithAccessManager(listener);
	            }
	
	            @Override
	            public void onError(Exception error)
	            {
	               Log.v("Log", "Error initializing the SDK :" + error.getMessage());
	            }
	        });
		} else {
			createClientWithToken(listener);
		}

	}

	public BasicIPMessagingClient() {
		super();
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
			Log.v("Log", "A Channel :"+ channel.getFriendlyName() + " got added");
		} else {
			Log.v("Log", "Received onChannelAdd event.");
		}
	}

	@Override
	public void onChannelChange(Channel channel) {
		if(channel != null) {
			Log.v("Log", "Channel Name : "+ channel.getFriendlyName() + " got Changed");
		} else {
			Log.v("Log","received onChannelChange event.");
		}
	}

	@Override
	public void onChannelDelete(Channel channel) {
		if(channel != null) {
			Log.v("Log", "A Channel :"+ channel.getFriendlyName() + " got deleted");
		} else {
			Log.v("Log", "received onChannelDelete event.");
		}
	}

	@Override
	public void onError(int errorCode, String errorText) {
		Log.v("Log", "Received onError event.");
	}

	@Override
	public void onAttributesChange(String attributes) {
		Log.v("Log","Received onAttributesChange event.");
	}
	
	public TwilioIPMessagingClient getIpMessagingClient() {
		return ipMessagingClient;
	}

	private void createClientWithToken(LoginListener listener) {
		ipMessagingClient = TwilioIPMessagingSDK.createIPMessagingClientWithToken(accessToken, BasicIPMessagingClient.this);
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
	
	
	private void createClientWithAccessManager(final LoginListener listener) {
		Log.v("YO", "About to create IPM client " + accessToken);
		this.acessMgr = TwilioAccessManagerFactory.createAccessManager(this.accessToken, new TwilioAccessManagerListener() {
            @Override
            public void onAccessManagerTokenExpire(TwilioAccessManager twilioAccessManager) {
                Log.d(TAG, "token expired.");
                new GetCapabilityTokenAsyncTask().execute(BasicIPMessagingClient.this.urlString);
            }

            @Override
            public void onTokenUpdated(TwilioAccessManager twilioAccessManager) {
                Log.d(TAG, "token updated. Creating client with valid token.");
                ipMessagingClient = TwilioIPMessagingSDK.createIPMessagingClientWithAccessManager(BasicIPMessagingClient.this.acessMgr, BasicIPMessagingClient.this);
             	if(ipMessagingClient != null) {
					Log.d(TAG, "init'd the ipm client");

					ipMessagingClient.setListener(BasicIPMessagingClient.this);
                 	Intent intent = new Intent(context,ChannelActivity.class);
                 	PendingIntent pendingIntent = PendingIntent.getActivity(context, 0, intent, PendingIntent.FLAG_UPDATE_CURRENT);
                 	ipMessagingClient.setIncomingIntent(pendingIntent);
					Intent i = new Intent(BROADCAST_FILTER);
					i.putExtra("connection_established", true);
					context.sendBroadcast(i);
                 	BasicIPMessagingClient.this.loginListenerHandler.post(new Runnable() {
        					@Override
        					public void run() {
        						if(listener != null) {
        							listener.onLoginFinished();
        			        	}
        					}
        				});
             	} else {
             		listener.onLoginError("ipMessagingClientWithAccessManager is null");
             	}
            }

            @Override
            public void onError(TwilioAccessManager twilioAccessManager, String s) {
                Log.d(TAG, "token error: " + s);
            }
        });
	}

	@Override
	public void onChannelHistoryLoaded(Channel channel) {
		Log.v("Log", "Received onChannelHistoryLoaded callback " + channel.getFriendlyName());
	}

	@Override
	public void onAccessManagerTokenExpire(TwilioAccessManager arg0) {
		Log.v("Log","Received AccessManager:onAccessManagerTokenExpire.");
	}

	@Override
	public void onError(TwilioAccessManager arg0, String arg1) {
		Log.v("Log", "Received AccessManager:onError.");
	}

	@Override
	public void onTokenUpdated(TwilioAccessManager arg0) {
		Log.v("Log", "Received AccessManager:onTokenUpdated.");
	}
	
	private Handler setupListenerHandler() {
		Looper looper;
		Handler handler;
		if((looper = Looper.myLooper()) != null) {
			handler = new Handler(looper);
		} else if((looper = Looper.getMainLooper()) != null) {
			handler = new Handler(looper);
		} else {
			handler = null;
			throw new IllegalArgumentException("Channel Listener must have a Looper.");
		}
		return handler;
	}

private class GetCapabilityTokenAsyncTask extends AsyncTask<String, Void, String> {

	@Override
	protected void onPostExecute(String result) {
		super.onPostExecute(result);
		ipMessagingClient.updateToken(accessToken, new StatusListener() {

		@Override
		public void onSuccess() {
			Log.v("Log", "Updated Token was successfull");
		}

		@Override
		public void onError() {
			Log.v("Log", "Updated Token failed");
		}}); 
		acessMgr.updateToken(null);
	}

	@Override
	protected void onPreExecute() {
		super.onPreExecute();
	}

	@Override
	protected String doInBackground(String... params) {
		try {
			accessToken = HttpHelper.httpGet(params[0]);
		} catch (Exception e) {
			e.printStackTrace();
		}
		return accessToken;
	}
}
}