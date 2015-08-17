package com.twilio.ipmessaging;

import java.util.Map;

public interface Member {
	
	/**
	 * Returns unique identifier of a member on a channel.
	 * 
	 * @return The unique id of a member/channel.
	 */
	public String getSid();
	
	/**
	 * Returns the global user identity for this memeber.
	 * 
	 * @return The global unique id.
	 */
	public String getIdentity();
	
	/**
	 * The attributes for this user on this channel. 
	 * 
	 * @return Attributes for this channel.
	 */
	public Map<String, String> getAttributes();
	
}
