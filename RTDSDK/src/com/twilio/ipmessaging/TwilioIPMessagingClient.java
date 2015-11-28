package com.twilio.ipmessaging;

import com.twilio.ipmessaging.Constants.StatusListener;

import android.app.PendingIntent;

public interface TwilioIPMessagingClient {
	
	/**
	 * Gets the unique identity identifier of this user in the Twilio IP Messaging system.
	 * 
	 * @return the unique id for this TwilioIPMessagingClient.
	 */
	public String getIdentity();
		
	/**
	 * Method to retrieve listener for this TwilioIPMessagingClient.
	 * 
	 * @return IPMessagingClientListener the listener for this TwilioIPMessagingClient.
	 */
	public IPMessagingClientListener getListener();
	
	/**
	 * Method to set listener for this TwilioIPMessagingClient.
	 * 
	 * @param listener	the listener for this TwilioIPMessagingClient.
	 */
	public void setListener(IPMessagingClientListener listener);
	
	/**
	 * Method to update the authentication token for this client.
	 * 
	 * @param accessToken	an AccessToken for this Client.
	 */
	public void updateToken(String accessToken, StatusListener listener);
	
	/**
	 * List of channels available to the TwilioIPMessagingClient.
	 * 
	 * @return The list of channels for this TwilioIPMessagingClient.
	 */
	public Channels getChannels();
		
	/**
	 * Sets a {@link PendingIntent} that will be sent when an incoming channel invite is received.
	 * 
	*/
	public void setIncomingIntent(PendingIntent inIntent);
	
	/**
	 * Cleanly shuts down the messaging subsystem when you are done with it. 
	 * 
	 */
	public void shutdown();
}
