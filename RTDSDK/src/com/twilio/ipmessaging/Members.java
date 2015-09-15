package com.twilio.ipmessaging;

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
	 * @param identity
	 *            The username to add to this channel.
	 */
	public void addByIdentity(String identity);

	/**
	 * Invite specified username to this channel.
	 * 
	 * @param identity
	 *            The username to invite to this channel.
	 */
	public void inviteByIdentity(String identity);

	/**
	 * Remove specified username from this channel.
	 * 
	 * @param member
	 *            The member to remove from this channel.
	 */
	public void removeMember(Member member);

}
