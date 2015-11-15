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
	 * Method to update the authentication token for this client.
	 * 
	 * @param accessToken	an AccessToken for this Client.
	 */
	public void updateToken(String accessToken);
	
	
	/**
	 * Method to set listener for this TwilioIPMessagingClient.
	 * 
	 * @param listener	the listener for this TwilioIPMessagingClient.
	 */
	public void setListener(IPMessagingClientListener listener);
	
	/**
	 * Method to retrieve listener for this TwilioIPMessagingClient.
	 * 
	 * @return IPMessagingClientListener the listener for this TwilioIPMessagingClient.
	 */
	public IPMessagingClientListener getListener();	
	
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
	
	/** Register GCM token for push notification updates.
	 * 
	 * @param token The registration token an Android application needs to register with GCM connection servers before 
	 * 				it can receive messages.
	 * @param listener Listener that will receive callback with the result.
	 */
	 public void registerWithToken(String token, StatusListener listener);

	/** Unregister push notification updates.
	 * 
	 * @param token The registration token provided for GCM push notification registration.
	 * @param listener Listener that will receive callback with the result.
	 * 
	 */
	public void unregisterWithToken(String token, StatusListener listener);
	
	
	/**
	 * Cleanly shut down the messaging subsystem when you are done with it. 
	 * 
	 */
	public void shutdown();
}
