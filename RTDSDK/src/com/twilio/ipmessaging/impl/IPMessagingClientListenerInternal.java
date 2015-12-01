package com.twilio.ipmessaging.impl;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import com.twilio.ipmessaging.Channel;
import com.twilio.ipmessaging.ChannelListener;
import com.twilio.ipmessaging.IPMessagingClientListener;
import com.twilio.ipmessaging.Member;
import com.twilio.ipmessaging.Message;

import android.os.Handler;

public class IPMessagingClientListenerInternal {
	
	private static final Logger logger = Logger.getLogger(IPMessagingClientListenerInternal.class);
	private IPMessagingClientListener listener;
	private TwilioIPMessagingClientImpl ipmClient;
	

	public IPMessagingClientListenerInternal(TwilioIPMessagingClientImpl client, IPMessagingClientListener listener) {
		this.listener = listener;
		this.ipmClient = client;
	}

	public void onMessageAdd(final Message message) {
		logger.d("Entered onMessageAdd");
		if(message != null) {
			String cSid = message.getChannelSid();
			ChannelImpl channelImpl = (ChannelImpl) this.ipmClient.publicChannelMap.get(cSid);
			if(channelImpl != null) {
				channelImpl.handleIncomingMessage((MessageImpl) message);
			}
		}
	}

	
	public void onMessageChange(Message message) {
		logger.d("Entered onMessageChange");
		if(message != null) {
			String cSid = message.getChannelSid();
			ChannelImpl channelImpl = (ChannelImpl) this.ipmClient.publicChannelMap.get(cSid);
			if(channelImpl != null) {
				channelImpl.handleEditMessage((MessageImpl) message);
			}
		}
	}

	
	public void onMessageDelete(Message message) {
		logger.d("Entered onMessageChange");
		if(message != null) {
			String cSid = message.getChannelSid();
			ChannelImpl channelImpl = (ChannelImpl) this.ipmClient.publicChannelMap.get(cSid);
			if(channelImpl != null) {
				channelImpl.handleDeleteMessage((MessageImpl) message);
			}
		}
	}

	
	public void onMemberJoin(Member member, Channel channel) {
		logger.d("Entered onMemberJoin");
		if(channel != null) {
			String cSid = channel.getSid();
			ChannelImpl channelImpl = (ChannelImpl) this.ipmClient.publicChannelMap.get(cSid);
			if(channelImpl != null) {
				channelImpl.handleOnMemberJoin(member);
			}
		}
	}

	public void onMemberChange(Member member, Channel channel) {
		logger.d("Entered onMemberChange");
		if(channel != null) {
			String cSid = channel.getSid();
			ChannelImpl channelImpl = (ChannelImpl) this.ipmClient.publicChannelMap.get(cSid);
			if(channelImpl != null) {
				channelImpl.handleOnMemberChange(member);
			}
		}
	}

	public void onMemberDelete(Member member, Channel channel) {
		logger.d("Entered onMemberDelete");
		if(channel != null) {
			String cSid = channel.getSid();
			ChannelImpl channelImpl = (ChannelImpl) this.ipmClient.publicChannelMap.get(cSid);
			if(channelImpl != null) {
				channelImpl.handleOnMemberDelete(member);
			}
		}
	}

	public void onAttributesChange(Map<String, String> updatedAttributes) {
		// TODO Auto-generated method stub
		
	}

	public void onError(int errorCode, String errorText) {
		// TODO Auto-generated method stub
		
	}

	
	public void onAttributesChange(String attribute) {
		logger.d("Entered onChannelChange");
		this.ipmClient.handleChannelAttributeChange(attribute);
	}
	
	
	public void onTypingStarted(Channel channel, Member member) {
		logger.d("Entered onTypingStarted");
		if(channel != null) {
			String cSid = channel.getSid();
			ChannelImpl channelImpl = (ChannelImpl) this.ipmClient.publicChannelMap.get(cSid);
			if(channelImpl != null) {
				channelImpl.handleOnTypingStarted(member);
			}
		}
	}
	
	public void onTypingEnded(Channel channel, Member member) {
		logger.d("Entered onTypingEnded");
		if(channel != null) {
			String cSid = channel.getSid();
			ChannelImpl channelImpl = (ChannelImpl) this.ipmClient.publicChannelMap.get(cSid);
			if(channelImpl != null) {
				channelImpl.handleOnTypingEnded(member);
			}
		}
	}
	
	public void onChannelAdd(ChannelImpl channel) {
		logger.d("this.ipmClient" + this.ipmClient.hashCode());
		synchronized(this.ipmClient) {
			this.ipmClient.handleChannelAddEvent(channel);
		}
	}
	
	public void onChannelInvite(Channel channel) {
		logger.d("Entered onChannelInvite");
		this.ipmClient.handleIncomingInvite(channel);		
	}

	
	public void onChannelChange(ChannelImpl channel) {
		logger.d("Entered onChannelChange");
		this.ipmClient.handleChannelChanged(channel);	
	}


	public void onChannelDelete(ChannelImpl  channel) {
		logger.d("Entered onChannelDelete");
		this.ipmClient.handleChannelDeleted(channel);
	}
	
	public void onChannelSync(ChannelImpl channel) {
		logger.d("Entered onChannelSync");
		if(channel != null) {
			String cSid = channel.getSid();
			final ChannelImpl channelImpl = (ChannelImpl) this.ipmClient.publicChannelMap.get(cSid);
			if(channelImpl != null) {
				logger.d("onChannelSync channelImpl " + channelImpl.getSid() + "|"+channelImpl.hashCode());
			}
			
			Map<ChannelListener, Handler> listenerMap = this.ipmClient.channelListenerMap.get(cSid);
			if(listenerMap != null) {
				logger.d("onChannelSync channelImpl listenerMap " + listenerMap.toString()); 
 				for (Map.Entry<ChannelListener, Handler> entry : listenerMap.entrySet()) {
 					final ChannelListener listener = entry.getKey();
 					logger.d("onChannelSync channelImpl listener " + listener.hashCode());
 				    Handler handler = entry.getValue();
 				    if (handler != null) {
 						logger.d("handleOnChannelSync handler not null.");
 						handler.post(new Runnable() {
 							@Override
 							public void run() {
 								if(listener!= null) {
 									logger.d("handleOnChannelSync calling listener");
 									listener.onChannelHistoryLoaded(channelImpl);
 								}
 							}
 						});
 					}	
 				}
			} else {
				if(channelImpl != null) {
					channelImpl.handleOnChannelSync(channel);
				}
			}
			
			if(this.ipmClient != null) {
				logger.d("Calling ipmClient:handleOnChannelSync");
				this.ipmClient.handleOnChannelSync(channel);
			}
		}
	}
	
	public void onChannelCreated(ChannelImpl channel) {
		logger.d("ipmClient " + this.ipmClient.hashCode());
		synchronized(this.ipmClient) {
			this.ipmClient.handleChannelCreate(channel);
		}
	}

}
