package com.twilio.common;

public interface IdentityManagerListener {
	
	/**
	 * Called when token expires.
	 * 
	 * @param manager
	 */
	public void onTokenExpire(IdentityManager manager);

}
