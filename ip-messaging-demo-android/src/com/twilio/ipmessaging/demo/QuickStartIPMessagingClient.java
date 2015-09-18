package com.twilio.ipmessaging.demo;
import java.util.Map;

import android.content.Context;
import android.os.AsyncTask;

import com.twilio.common.IdentityManager;
import com.twilio.common.IdentityManagerListener;
import com.twilio.ipmessaging.Channel;
import com.twilio.ipmessaging.ChannelListener;
import com.twilio.ipmessaging.Channels;
import com.twilio.ipmessaging.TwilioIPMessagingClient;
import com.twilio.ipmessaging.Member;
import com.twilio.ipmessaging.Message;
import com.twilio.ipmessaging.IPMessagingClientListener;


public class QuickStartIPMessagingClient implements IdentityManagerListener, IPMessagingClientListener, ChannelListener {

	private String token;
	private Context context;

	   

	public QuickStartIPMessagingClient(String token) {
		super();
		this.token = token;
	}
	
	
	private void doSample() {
		
		String jwt = this.fetchToken();
		IdentityManager idManager = IdentityManager.init(jwt, this);
		
		TwilioIPMessagingClient messagingClient = new TwilioIPMessagingClient();
		messagingClient.init(jwt, this);
		
		//Get the list of channels for this IPMessaging client
		Channels myChannels = messagingClient.getChannels();
		
		//Create a new Channel and invite bob
		Channel myNewChannel = myChannels.createChannel("RTDTest", this);
		myNewChannel.inviteByIdentity("Bob@RdtTest.com");
		
	}

	@Override
	public void onTokenExpire(IdentityManager manager) {
		// TODO Auto-generated method stub
		
	}
	
	
	private String fetchToken() {
		return null;
	}

	//MessagingClientListener
	@Override
	public void onChannelAdd(Channel channel) {
		// TODO Auto-generated method stub
		
	}


	@Override
	public void onChannelChange(Channel channel) {
		// TODO Auto-generated method stub
		
	}


	@Override
	public void onChannelDelete(Channel channel) {
		// TODO Auto-generated method stub
		
	}


	@Override
	public void onError(int errorCode, String errorText) {
		// TODO Auto-generated method stub
		
	}

	//ChannelListener
	@Override
	public void onMessageAdd(Message message) {
		// TODO Auto-generated method stub
		
	}


	@Override
	public void onMessageChange(Message message) {
		// TODO Auto-generated method stub
		
	}


	@Override
	public void onMessageDelete(Message message) {
		// TODO Auto-generated method stub
		
	}


	@Override
	public void onMemberJoin(Member member) {
		// TODO Auto-generated method stub
		
	}


	@Override
	public void onMemberChange(Member member) {
		// TODO Auto-generated method stub
		
	}


	@Override
	public void onMemberDelete(Member member) {
		// TODO Auto-generated method stub
		
	}


	@Override
	public void onAttributesChange(Map<String, String> updatedAttributes) {
		// TODO Auto-generated method stub
		
	}
	
	
	private class GetAuthTokenAsyncTask extends AsyncTask<String, Void, String> {

		@Override
		protected void onPostExecute(String result) {
			super.onPostExecute(result);
		}

		@Override
		protected String doInBackground(String... params) {
			String capabilityToken = null;
			try {
				StringBuffer URL = new StringBuffer();
				URL.append("https://dev-us1.twilio.com/v1/channel/authtoken?systemId=UUID");
				capabilityToken = HttpHelper.httpGet(URL.toString());;
			} catch (Exception e) {
				e.printStackTrace();
			}
			return capabilityToken;
		}
    }


	@Override
	public void onAttributesChange(String attributes) {
		// TODO Auto-generated method stub
		
	}
	

	
}


