package com.twilio.ipmessaging;

import com.twilio.ipmessaging.Constants.StatusListener;

public interface Members {

	/**
	 * Obtain an array of members of this channel.
	 * 
	 * @return An array of Member objects representing the membership of the
	 *         channel.
	 */

	public Member[] getMembers();

	/**
	 * Add specified username to this channel without inviting.
	 * 
	 * @param identity The username to add to this channel.
	 * @param listener StatusListener to report status of the action.
	 *            
	 */
	public void addByIdentity(String identity, StatusListener listener);

	/**
	 * Invite specified username to this channel.
	 * 
	 * @param identity The username to invite to this channel.
	 * @param listener StatusListener to report status.
	 */
	public void inviteByIdentity(String identity, StatusListener listener);

	/**
	 * Remove specified username from this channel.
	 * 
	 * @param member The member to remove from this channel.            
	 * @param listener StatusListener to report status.
	 * 
	 */
	public void removeMember(Member member, StatusListener listener);

}
