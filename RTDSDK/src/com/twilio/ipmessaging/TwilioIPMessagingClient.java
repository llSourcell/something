package com.twilio.ipmessaging;

import java.util.Map;
import java.util.Set;

import com.twilio.common.IdentityManager;
import com.twilio.ipmessaging.impl.TwilioIPMessagingClientImpl;

import android.content.Context;

/**
 * Represents an IP Messaging client connection to Twilio. It holds the product-level client context.
 * 
 */

public class TwilioIPMessagingClient {
	
	/**
	 * Interface for the listener object to pass to
	 * {@link TwilioIPMessagingClient#initialize(Context, InitListener)}.
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
	 *            A {@link TwilioIPMessagingClient.InitListener} that will notify you when the
	 *            service is ready. Cannot be null.
	 * 
	 * @throws IllegalArgumentException
	 */
	public static void initializeSDK(Context inContext,TwilioIPMessagingClient.InitListener inListener) {
		TwilioIPMessagingClientImpl.getInstance().initialize(inContext, inListener);;
	}

	/**
	 * Initialize a new IP Messaging client instance with a token manager.
	 * 
	 * @param tokenManager	an AccessTokenManager instance to be used for authentication.
	 * @param listener		the listener for thisIP MessagingClient.
	 * @return The newly initialized IPMessagingClient.
	 */
	public static TwilioIPMessagingClient init(String token, IPMessagingClientListener listener) {
		return TwilioIPMessagingClientImpl.getInstance().initClient(token, listener);
	}
	
	/**
	 * Gets the unique identity identifier of this user in the IP Messaging system.
	 * 
	 * @return the unique id for this client.
	 */
	public String getIdentity() {
		return TwilioIPMessagingClientImpl.getInstance().getIdentity();
	}
	
	/**
	 * Method to set listener for this IPMessagingClient.
	 * 
	 * @param listener	the listener for thisIP MessagingClient.
	 */
	public void setListener(IPMessagingClientListener listener){
		TwilioIPMessagingClientImpl.getInstance().setListener(listener);
	}
	
	/**
	 * Method to retrieve listener for this IPMessagingClient.
	 * 
	 * @return MessagingClientListener	the listener for thisIP MessagingClient.
	 */
	public IPMessagingClientListener getListener() {
		return TwilioIPMessagingClientImpl.getInstance().getListener();
	}
	
	
	/**
	 * Method to get attributes from the server side.
	 * 
	 * @return a map of custom attributes associated with the local User.
	 */
	public String getAttributes() {
		return TwilioIPMessagingClientImpl.getInstance().getAttributes();
	}
	
	/**
	 * Updates attributes on the server side.
	 * 
	 * @param attributes a map of custom attributes associated with the local User.
	 */
	public void updateAttributes(Map<String, String> attributes) {
		TwilioIPMessagingClientImpl.getInstance().updateAttributes(attributes);
	}
	
	/**
	 * Method to update the authentication token for this client.
	 * 
	 * @param accessToken	an AccessToken for this Client.
	 */
	public void updateToken(String accessToken) {
		TwilioIPMessagingClientImpl.getInstance().updateToken(accessToken);
	}
	
	/**
	 * List of channels available to the MessageClient.
	 * 
	 * @return The list of channels for this IPMessagingClient.
	 */
	public Channels getChannels() {
		return TwilioIPMessagingClientImpl.getInstance().getChannels();
	}
	
	
	/**
	 * Method to join a Channel. ChannelStatus should transition from INVITED->JOINED or IDLE->JOINED
	 * when this method is called.
	 * 
	 * @param channelSid	The identifier of the channel to join.
	 */
	public void joinChannel(String channelSid) {
		TwilioIPMessagingClientImpl.getInstance().joinChannel(channelSid);
	}
	
	/**
	 * Retrieve a channel by identifier. 
	 * 
	 * @param channelSid  The identifier of the channel to retrieve.
	 * @return The channel with channelSid.
	 */
	public Channel getChannel(String channelSid) {
		return TwilioIPMessagingClientImpl.getInstance().getChannel(channelSid);
	}
	
	
	
	
}
