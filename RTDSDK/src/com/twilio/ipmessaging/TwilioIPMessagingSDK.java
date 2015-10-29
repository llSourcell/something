package com.twilio.ipmessaging;

import com.twilio.ipmessaging.impl.TwilioIPMessagingSDKImpl;
import com.twilio.ipmessaging.Constants.InitListener;
import android.content.Context;

/**
 * Represents an IP Messaging client connection to Twilio. It holds the product-level client context.
 * 
 */

public class TwilioIPMessagingSDK {
	
	/**
	 * Initialize the TwilioIPMessagingSDK.
	 * 
	 * @param inContext
	 *            The Application Context from your Android application. Make
	 *            sure you don't pass an Activity Context. You can retrieve the
	 *            Application Context by calling getApplicationContext() on your
	 *            Activity. Cannot be null.
	 * 
	 * @param inListener
	 *            A {@link TwilioIPMessagingSDK.InitListener} that will notify you when the
	 *            service is ready. Cannot be null.
	 * 
	 */
	public static void initializeSDK(Context inContext, InitListener inListener) {
		TwilioIPMessagingSDKImpl.getInstance().initialize(inContext, inListener);;
	}

	/**
	 * Creates a new IP Messaging client with a token.
	 * 
	 * @param token			The authentication token for the user. 
	 * @param listener		IPMessagingClientListener for this IPMessagingClient.
	 * @return 				The newly initialized TwilioIPMessagingClient.
	 */
	public static TwilioIPMessagingClient createIPMessagingClientWithToken(String token, IPMessagingClientListener listener) {
		return TwilioIPMessagingSDKImpl.getInstance().createClientWithToken(token, listener);
	}
}
