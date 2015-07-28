package com.twilio.rtd;

import java.util.Date;

public interface Message {
	
	
	/** 
	 * Returns the identifier for this message. 
	 * 
	 * @return The message sid.
	 */
	public String getSid();
	
	/** 
	 * The author of this message. 
	 *
	 *@return The member.
	 */
	public Member getAuthor();
	
	/** 
	 * The created timestamp for this message.
	 * 
	 * @return The date this message was created.
	 * 
	 * /
	public Date getDateCreated();
	
	/** 
	 * The last updated timestamp for this message. 
	 * 
	 * @return The date this message was last updated.
	 *
	 */
	public Date getDateUPdated();
	
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
	 *  @param completion Completion block that will specify the result of the operation.
	 *  
	 */
	public void updateMessageBody(String body);

}
