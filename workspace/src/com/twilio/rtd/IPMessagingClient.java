package com.twilio.rtd;

import java.util.Map;
import java.util.Set;

import android.content.Context;

/**
 * Represents an IP Messaging client connection to Twilio. It holds the product-level client context.
 * 
 */

public interface IPMessagingClient {
	
	/**
	 * Interface for the listener object to pass to
	 * {@link TwilioRTD#initialize(Context, InitListener)}.
	 */
	public interface InitListener {
		/**
		 * Callback to report when TwilioRTC Client SDK has been successfully
		 * initialized.
		 */
		public void onInitialized();

		/**
		 * Called if there is an error initializing the TwilioSignal Client SDK.
		 * 
		 * @param error
		 *            An exception describing the error that occurred
		 */
		public void onError(Exception error);
	}
	
	
	/**
	 * Initialize the TwilioRTD Client SDK.
	 * 
	 * @param inContext
	 *            The Application Context from your Android application. Make
	 *            sure you don't pass an Activity Context. You can retrieve the
	 *            Application Context by calling getApplicationContext() on your
	 *            Activity. Cannot be null.
	 * 
	 * @param inListener
	 *            A {@link TwilioRTD.InitListener} that will notify you when the
	 *            service is ready. Cannot be null.
	 * 
	 * @throws IllegalArgumentException
	 */
	public void initialize(Context inContext,IPMessagingClient.InitListener inListener) ;


	/**
	 * Initialize a MessagingClient.
	 * 
	 * @param accessToken   an AccessToken used to register this Client.
	 * @param attributes	a formless JSON object representing all custom attributes associated with the local User.
	 * @param listener		the listener for thisIP MessagingClient.
	 * @return
	 */
	public IPMessagingClient initMessagingClientWithToken(String accessToken, Map<String, String> attributes, MessagingClientListener listener);

	/**
	 * Method to set listener for this IPMessagingClient.
	 * 
	 * @param listener	the listener for thisIP MessagingClient.
	 */
	public void setListener(MessagingClientListener listener);
	
	/**
	 * Method to retrieve listener for this IPMessagingClient.
	 * 
	 * @return MessagingClientListener	the listener for thisIP MessagingClient.
	 */
	public MessagingClientListener getListener();
	
	/**
	 * Method to get attributes from the server side.
	 * 
	 * @return a map of custom attributes associated with the local User.
	 */
	public String getAttributes();
	
	/**
	 * Updates attributes on the server side.
	 * 
	 * @param attributes a map of custom attributes associated with the local User.
	 */
	public void updateAttributes(Map<String, String> attributes);
	
	/**
	 * Method to update the authentication token for this client.
	 * 
	 * @param accessToken	an AccessToken for this Client.
	 */
	public void updateToken(String accessToken);
	
	/**
	 * List of channels available to the MessageClient.
	 * 
	 * @return
	 */
	public Set<Channel> getChannels();
	
	/**
	 * Method to create channel with attributes.
	 * 
	 * @param attributes	custom attributes associated with the Channel.
	 * @param friendlyName	Friendly name of the new channel.
	 * @param listener 		listener that receives this channel related events.
	 */
	public void createChannel(Map<String, String> attributes, String friendlyName, ChannelListener listener);
	
	
	/**
	 * Method to join a Channel. ChannelStatus should transition from INVITED->JOINED or IDLE->JOINED
	 * when this method is called.
	 * 
	 * @param channelSid	The identifier of the channel to join.
	 */
	public void joinChannel(String channelSid);
	
	/**
	 * Retrieve a channel by identifier. 
	 * 
	 * @param channelSid  The identifier of the channel to retrieve.
	 * @return
	 */
	public Channel getChannel(String channelSid);
	
	
}
