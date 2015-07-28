package com.twilio.rtd;

import java.util.Map;

/**
 * Interface to report channel operations such as member join/leave and add/delete/update messages. 
 * 
 * @author kbagchi
 *
 */

public interface ChannelListener {
	
	/**
	 * Called when a Message is added to the channel the current user is subscribed to.
	 * 
	 * @param message
	 */
	void onMessageAdded(Message message);
	
	/**
	 * Called when a Message is changed for the channel the current user is subscribed to.
	 * 
	 * @param msg
	 */
	void onMessageChanged(Message message);   
	
	/**
	 * Called when a Message is deleted from the channel the current user is subscribed to.
	 * 
	 * @param msg
	 */
	void onMessageDeleted(Message message);
	
	/**
	 * Called when a Member is added to the channel the current user is subscribed to.
	 * 
	 * @param member
	 */
	void onMemberJoined(Member member);
	
	/**
	 * Called when a Member is changed in the channel the current user is subscribed to.
	 * @param member
	 */
	void onMemberChanged(Member member);
	
	/**
	 * Called when a Member is deleted from the channel the current user is subscribed to.
	 * @param member
	 */
	void onMemberDeleted(Member member);
	
	/**
	 * Called when channel attributes have changed.
	 * 
	 * @param updatedAttributes updated channel attribute. 
	 */
	void onAttributeChanged(Map<String, String> updatedAttributes);
}
