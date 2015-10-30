package com.twilio.ipmessaging;

public interface Constants {
	
	/**
	 * Key into an Intent's extras data that points to a {@link Channel} object.
	 */
	public static final String EXTRA_CHANNEL = "com.twilio.ipmessaging.Channel";
	
	/**
	 * Key into an Intent's extras data that points to a {@link Message} object.
	 */
	public static final String EXTRA_ACTION = "channel.action";
	
	/**
	 * Key into an Intent's extras data that points to a {@link Message} object.
	 */
	public static final String EXTRA_ACTION_INVITE = "channel.action.invite";
	

	/**
	 * Interface for the listener object to pass to
	 * {@link TwilioIPMessagingSDK#initialize(Context, InitListener)}.
	 */
	public interface InitListener {
		/**
		 * Callback to report when TwilioRTD Client SDK has been successfully
		 * initialized.
		 */
		public void onInitialized();

		/**
		 * Called if there is an error initializing the TwilioRTD Client SDK.
		 * 
		 * @param error
		 *            An exception describing the error that occurred
		 */
		public void onError(Exception error);
	}

	/**
	 * Interface for the listener object to pass to while creatting Channel
	 * 
	 */
	public interface CreateChannelListener {
		/**
		 * Callback to report when Channel is successfully created.
		 */
		public void onCreated(Channel newChannel);

		/**
		 * Called if there is an error creating a channel.
		 * 
		 * @param error
		 *            An exception describing the error that occurred
		 */
		public void onError(Exception error);
	}

	
	/**
	 * Interface for a generic listener object.
	 */
	 
	public interface StatusListener {
		/**
		 * Callback to report success status of an asynchronous call to the backend.
		 * 
		 */
		public void onSuccess();

		/**
		 * Callback to report error status of an asynchronous call to the backend.
		 * 
		 * @param error
		 *            An exception describing the error that occurred
		 */
		public void onError(Exception error);
	}
}
