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
	 * Determines if the Twilio IP Messaging SDK has been initialized or not.
	 * 
	 * If you expect your application to run in the background when the user has
	 * switched to other applications, you will want to check the return value of
	 * this method on startup.  The Android OS may have killed your application
	 * due to memory pressure, but the SDK may still be running in the background.
	 * 
	 * @return true if the SDK is currently initialized, false otherwise
	 *
	 */
	public static boolean isInitialized()
	{
		return TwilioIPMessagingSDKImpl.getInstance().isInitialized();
	}
	
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
	
	/**
	 * Shuts down the Twilio IP Messaging SDK.
	 * 
	 * This will terminate all connections, release all TwilioIPMessagingClient objects, and
	 * release any resources used by the SDK.
	 * 
	 * Note that any attempt to access existing TwilioIPMessagingClient
	 * after calling this method may cause an exception to be thrown, or a crash.
	 */
	public static void shutdown()
	{
		TwilioIPMessagingSDKImpl.getInstance().shutdown();
	}

	/**
	 * Returns the version of the TwilioIPMessagingSDK.
	 *
	 *@return The version of the SDK.
	 */
	public static String getVersion() {
		return TwilioIPMessagingSDKImpl.getInstance().getVersion();
	}

	/**
	 * Sets the logging level for messages logged by the Twilio IP Messaging
	 * SDK.
	 *
	 * Log levels correspond to those specified by Android's
	 * {@link android.util.Log} class.
	 *
	 * To disable all Twilio SDK logging, set this to
	 * {@link android.util.Log#ASSERT}. The default is
	 * {@link android.util.Log#ERROR}.
	 *
	 * @level The logging level
	 */
	public static void setLogLevel(int level) {
		TwilioIPMessagingSDKImpl.getInstance().setLogLevel(level);
	}

}
