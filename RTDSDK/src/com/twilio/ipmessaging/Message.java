package com.twilio.ipmessaging;

import java.util.Date;

public interface Message {
	
	
	/** 
	 * Returns the identifier for this message. 
	 * 
	 * @return The message sid.
	 */
	public String getSid();
	
	/** 
	 * The global identity of the author of this message. 
	 *
	 *@return The global identity.
	 */
	public String getAuthor();
	
	/** 
	 * The last updated timestamp for this message. 
	 * 
	 * @return The time stamp this message was last updated.
	 *
	 */
	public String getTimeStamp();
	
	/** 
	 * The body for this message. 
	 * 
	 * @return The message body.
	 * 
	 */
	public String getMessageBody();
	
	/** Updates the body for a message.
	 * 
	 *  @param body The body for the message.
	 *  
	 */
	public void updateMessageBody(String body);

	/** Returns the Channel Sid of the channel this message belongs to.
	 * 
	 * @return Channel Sid.
	 *  
	 */
	public String getChannelSid();

}
