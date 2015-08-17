package com.twilio.common;

import java.util.Date;
import java.util.Map;
import java.util.Set;

public class IdentityManager {
	
	public enum TwilioTokenState {
	    ACTIVE ,
	    EXPIRED ,
	    INVALID
	} 
	
	private String jwt;
	private Date expiry;
	private Map<String, String> grants;
	private String accountSid;
	private String signingKeySid;
	private IdentityManagerListener listener; 
	private static IdentityManager instance = null;
	
	protected IdentityManager() {
	  // Exists only to defeat instantiation.
	}
	
	/**
	 * Init an instance of IdentityManager with IdentityManagerListener.
	 * 
	 * @param listener an instance of IdentityManagerListener.
	 * @return instance of IdentityManager.
	 */
	public static IdentityManager init(String jwt, IdentityManagerListener listener) {
		if(instance == null) {
			instance = new IdentityManager();
		}
		instance.setAccessTokenManagerListener(listener);
		instance.updateToken(jwt);
		return instance;
	}
	
	/** 
	 * Sets the listener for this AccessTokenManager. 
	 */
	public void setAccessTokenManagerListener(IdentityManagerListener listener) {
		this.listener = listener;
	}
	
	/** 
	 * Gets the listener for this AccessTokenManager. 
	 */
	public IdentityManagerListener getAccessTokenManagerListener() {
		return this.listener;
	}
	
	/** 
	 * Get the raw string based token. 
	 */
	public String getjwt() {
		return this.jwt;
	}

	/** 
	 * The expiration of the access token. 
	 */
	public Date getExpiry() {
		return this.expiry;
	}

	/** 
	 * The parsed claims from the access token. 
	 * 
	 */
	public Map<String, String> getGrants() {
		return this.grants;
	}

	/** 
	 * Gets the parsed account Sid from the access token. 
	 */
	public String getAccountSid() {
		return this.accountSid;
	}

	/** 
	 * The parsed signing key Sid from the access token. 
	 * 
	 */
	public String getSigningKeySid() {
		return this.signingKeySid;
	}
	
	/** Gets the token state */
	public TwilioTokenState getTokenState() {
		//TODO::Implementation
		return TwilioTokenState.ACTIVE;
	}
	
	/**
	 * Updates jwt token.
	 * 
	 * @param jwt The access token.
	 */
	public void updateToken(String jwt) {
		this.jwt = jwt;
		//TODO::Implementation 
	}

}
