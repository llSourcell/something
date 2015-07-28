package com.twilio.rtd.shared;

public interface AccessTokenManagerListener {
	
	/**
	 * Called when token expires.
	 * 
	 * @param manager
	 */
	public void onTokenExpire(AccessTokenManager manager);

}
