package com.twilio.rtd.shared;

import java.util.Date;
import java.util.Map;
import java.util.Set;

public interface AccessTokenManager {
	
	public enum TwilioTokenState {
	    ACTIVE ,
	    EXPIRED ,
	    INVALID
	} 
	
	/** Sets the listener for this AccessTokenManager. */
	public void setAccessTokenManagerListener(AccessTokenManagerListener listener);
	
	/** Gets the list of listeners for this AccessTokenManager. */
	public Set<AccessTokenManagerListener> getAccessTokenManagerListener();
	
	/** Get the raw string based token. */
	public String getjwt();

	/** The expiration of the access token. */
	public Date getExpiry();

	/** The parsed claims from the access token. */
	public Map<String, String> getGrants();

	/** Gets the parsed account Sid from the access token. */
	public String getAccountSid();

	/** The parsed signing key Sid from the access token. */
	public String getSigningKeySid();
	
	/** Gets the token state */
	public TwilioTokenState getTokenState();
	
	/**
	 * Updates token.
	 * 
	 * @param token The access token.
	 */
	public void updateToken(String token);

}
