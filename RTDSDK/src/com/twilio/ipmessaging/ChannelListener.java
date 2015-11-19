package com.twilio.ipmessaging;

import java.util.Map;

/**
 * Interface to report channel operations such as member join/leave and add/delete/update messages. 
 * 
 *
 */

public interface ChannelListener {
	
	/**
	 * Called when a Message is added to the channel the current user is subscribed to.
	 * 
	 * @param message
	 */
	void onMessageAdd(Message message);
	
	/**
	 * Called when a Message is changed for the channel the current user is subscribed to.
	 * 
	 * @param message The message that changed.
	 */
	void onMessageChange(Message message);   
	
	/**
	 * Called when a Message is deleted from the channel the current user is subscribed to.
	 * 
	 * @param message The message that got deleted.
	 */
	void onMessageDelete(Message message);
	
	/**
	 * Called when a Member is added to the channel the current user is subscribed to.
	 * 
	 * @param member The member that joined.
	 */
	void onMemberJoin(Member member);
	
	/**
	 * Called when a Member is changed in the channel the current user is subscribed to.
	 * 
	 * @param member The member that changed.
	 */
	void onMemberChange(Member member);
	
	/**
	 * Called when a Member is deleted from the channel the current user is subscribed to.
	 * 
	 * @param member The member that was deleted.
	 */
	void onMemberDelete(Member member);
	
	/**
	 * Called when channel attributes have changed.
	 * 
	 * @param updatedAttributes updated channel attribute. 
	 */
	void onAttributesChange(Map<String, String> updatedAttributes);
	
	/**
	 * Called when an Member starts typing on a Channel.
	 * 
	 * @param member The member that started typing.
	 */
	public void onTypingStarted(Member member);
	
	/**
	 * Called when an Member stops typing on a Channel.
	 * 
	 * @param member The member that stopped typing.
	 */
	public void onTypingEnded(Member member);
	
	/**
	 * Called when Messages synchronization for this Channel completes.
	 * 
	 */
	public void onChannelHistoryLoaded();
	
}
