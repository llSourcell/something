package com.twilio.rtd;

import java.util.Map;

public interface Member {
	
	/**
	 * Returns unique identifier of a member on the server side.
	 * 
	 * @return The unique id of a member.
	 */
	public String getSid();
	
	/**
	 * The attributes for this user on this channel. 
	 * 
	 * @return Attributes for this channel.
	 */
	public Map<String, String> getAttributes();
	
}
