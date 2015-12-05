package com.twilio.ipmessaging.impl;

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
			String mSid = message.getChannelSid();
			ChannelImpl channel = (ChannelImpl) this.ipmClient.publicChannelMap.get(mSid);
			if(channel != null) {
				String cSid = channel.getSid();
				final ChannelImpl channelImpl = (ChannelImpl) this.ipmClient.publicChannelMap.get(cSid);
				if(channelImpl != null) {
					logger.d("onMessageAdd channelImpl " + channelImpl.getSid() + "|"+channelImpl.hashCode());
				}
				
				Map<ChannelListener, Handler> listenerMap = this.ipmClient.channelListenerMap.get(cSid);
				if(listenerMap != null) {
					logger.d("onMessageAdd channelImpl listenerMap " + listenerMap.toString()); 
	 				for (Map.Entry<ChannelListener, Handler> entry : listenerMap.entrySet()) {
	 					final ChannelListener listener = entry.getKey();
	 					logger.d("onMessageAdd channelImpl listener " + listener.hashCode());
	 				    Handler handler = entry.getValue();
	 				    if (handler != null) {
	 						logger.d("onMessageAdd handler not null.");
	 						handler.post(new Runnable() {
	 							@Override
	 							public void run() {
	 								if(listener!= null) {
	 									logger.d("onMessageAdd calling listener");
	 									listener.onMessageAdd((MessageImpl) message);
	 								}
	 							}
	 						});
	 					}	
	 				}
				} else {
					if(channelImpl != null) {
						channelImpl.handleIncomingMessage((MessageImpl) message);
					}
				}
			}
		}
	}

	
	public void onMessageChange(final Message message) {
		logger.d("Entered onMessageChange");
		if(message != null) {
			String mSid = message.getChannelSid();
			ChannelImpl channel = (ChannelImpl) this.ipmClient.publicChannelMap.get(mSid);
			if(channel != null) {
				String cSid = channel.getSid();
				final ChannelImpl channelImpl = (ChannelImpl) this.ipmClient.publicChannelMap.get(cSid);
				if(channelImpl != null) {
					logger.d("onMessageChange channelImpl " + channelImpl.getSid() + "|"+channelImpl.hashCode());
				}
				
				Map<ChannelListener, Handler> listenerMap = this.ipmClient.channelListenerMap.get(cSid);
				if(listenerMap != null) {
					logger.d("onMessageChange channelImpl listenerMap " + listenerMap.toString()); 
	 				for (Map.Entry<ChannelListener, Handler> entry : listenerMap.entrySet()) {
	 					final ChannelListener listener = entry.getKey();
	 					logger.d("onMessageChange channelImpl listener " + listener.hashCode());
	 				    Handler handler = entry.getValue();
	 				    if (handler != null) {
	 						logger.d("onMessageChange handler not null.");
	 						handler.post(new Runnable() {
	 							@Override
	 							public void run() {
	 								if(listener!= null) {
	 									logger.d("onMessageChange calling listener");
	 									listener.onMessageChange((MessageImpl) message);
	 								}
	 							}
	 						});
	 					}	
	 				}
				} else {
					if(channelImpl != null) {
						channelImpl.handleEditMessage((MessageImpl) message);
					}
				}
			}
		}
	}

	
	public void onMessageDelete(final Message message) {
		logger.d("Entered onMessageDelete");
		if(message != null) {
			String mSid = message.getChannelSid();
			ChannelImpl channel = (ChannelImpl) this.ipmClient.publicChannelMap.get(mSid);
			if(channel != null) {
				String cSid = channel.getSid();
				final ChannelImpl channelImpl = (ChannelImpl) this.ipmClient.publicChannelMap.get(cSid);
				if(channelImpl != null) {
					logger.d("onMessageDelete channelImpl " + channelImpl.getSid() + "|"+channelImpl.hashCode());
				}
				
				Map<ChannelListener, Handler> listenerMap = this.ipmClient.channelListenerMap.get(cSid);
				if(listenerMap != null) {
					logger.d("onMessageDelete channelImpl listenerMap " + listenerMap.toString()); 
	 				for (Map.Entry<ChannelListener, Handler> entry : listenerMap.entrySet()) {
	 					final ChannelListener listener = entry.getKey();
	 					logger.d("onMessageDelete channelImpl listener " + listener.hashCode());
	 				    Handler handler = entry.getValue();
	 				    if (handler != null) {
	 						logger.d("onMessageDelete handler not null.");
	 						handler.post(new Runnable() {
	 							@Override
	 							public void run() {
	 								if(listener!= null) {
	 									logger.d("onMessageDelete calling listener");
	 									listener.onMessageDelete((MessageImpl) message);
	 								}
	 							}
	 						});
	 					}	
	 				}
				} else {
					if(channelImpl != null) {
						channelImpl.handleDeleteMessage((MessageImpl) message);
					}
				}
			}
		}
	}

	
	public void onMemberJoin(final Member member, final Channel channel) {
		logger.d("Entered onMemberJoin");
		if(channel != null) {
			String cSid = channel.getSid();
			final ChannelImpl channelImpl = (ChannelImpl) this.ipmClient.publicChannelMap.get(cSid);
			if(channelImpl != null) {
				logger.d("onMemberJoin channelImpl " + channelImpl.getSid() + "|"+channelImpl.hashCode());
			}
			
			Map<ChannelListener, Handler> listenerMap = this.ipmClient.channelListenerMap.get(cSid);
			if(listenerMap != null) {
				logger.d("onMemberJoin channelImpl listenerMap " + listenerMap.toString()); 
 				for (Map.Entry<ChannelListener, Handler> entry : listenerMap.entrySet()) {
 					final ChannelListener listener = entry.getKey();
 					logger.d("onMemberJoin channelImpl listener " + listener.hashCode());
 				    Handler handler = entry.getValue();
 				    if (handler != null) {
 						logger.d("onMemberJoin handler not null.");
 						handler.post(new Runnable() {
 							@Override
 							public void run() {
 								if(listener!= null) {
 									logger.d("onMemberJoin calling listener");
 									listener.onMemberJoin(member);
 								}
 							}
 						});
 					}	
 				}
			} else {
				if(channelImpl != null) {
					channelImpl.handleOnMemberJoin(member);
				}
			}
		}
	}

	public void onMemberChange(final Member member, final Channel channel) {
		logger.d("Entered onMemberChange");
		if(channel != null) {
			String cSid = channel.getSid();
			final ChannelImpl channelImpl = (ChannelImpl) this.ipmClient.publicChannelMap.get(cSid);
			if(channelImpl != null) {
				logger.d("onMemberChange channelImpl " + channelImpl.getSid() + "|"+channelImpl.hashCode());
			}
			
			Map<ChannelListener, Handler> listenerMap = this.ipmClient.channelListenerMap.get(cSid);
			if(listenerMap != null) {
				logger.d("onMemberChange channelImpl listenerMap " + listenerMap.toString()); 
 				for (Map.Entry<ChannelListener, Handler> entry : listenerMap.entrySet()) {
 					final ChannelListener listener = entry.getKey();
 					logger.d("onMemberChange channelImpl listener " + listener.hashCode());
 				    Handler handler = entry.getValue();
 				    if (handler != null) {
 						logger.d("onMemberChange handler not null.");
 						handler.post(new Runnable() {
 							@Override
 							public void run() {
 								if(listener!= null) {
 									logger.d("onMemberChange calling listener");
 									listener.onMemberChange(member);
 								}
 							}
 						});
 					}	
 				}
			} else {
				if(channelImpl != null) {
					channelImpl.handleOnMemberChange(member);
				}
			}
		}
	}

	public void onMemberDelete(final Member member, final Channel channel) {
		logger.d("Entered onMemberDelete");
		if(channel != null) {
			String cSid = channel.getSid();
			final ChannelImpl channelImpl = (ChannelImpl) this.ipmClient.publicChannelMap.get(cSid);
			if(channelImpl != null) {
				logger.d("onMemberDelete channelImpl " + channelImpl.getSid() + "|"+channelImpl.hashCode());
			}
			
			Map<ChannelListener, Handler> listenerMap = this.ipmClient.channelListenerMap.get(cSid);
			if(listenerMap != null) {
				logger.d("onMemberDelete channelImpl listenerMap " + listenerMap.toString()); 
 				for (Map.Entry<ChannelListener, Handler> entry : listenerMap.entrySet()) {
 					final ChannelListener listener = entry.getKey();
 					logger.d("onMemberDelete channelImpl listener " + listener.hashCode());
 				    Handler handler = entry.getValue();
 				    if (handler != null) {
 						logger.d("onMemberDelete handler not null.");
 						handler.post(new Runnable() {
 							@Override
 							public void run() {
 								if(listener!= null) {
 									logger.d("onMemberDelete calling listener");
 									listener.onMemberDelete(member);
 								}
 							}
 						});
 					}	
 				}
			} else {
				if(channelImpl != null) {
					channelImpl.handleOnMemberDelete(member);
				}
			}
		}
	}

	public void onAttributesChange(final Map<String, String> updatedAttributes) {
		// TODO Auto-generated method stub
		
	}

	public void onError(final int errorCode, final String errorText) {
		// TODO Auto-generated method stub
		
	}

	
	public void onAttributesChange(final String attribute) {
		logger.d("Entered onChannelChange");
		this.ipmClient.handleChannelAttributeChange(attribute);
	}
	
	
	public void onTypingStarted(final Channel channel, final Member member) {
		logger.d("Entered onTypingStarted");
		if(channel != null) {
			String cSid = channel.getSid();
			final ChannelImpl channelImpl = (ChannelImpl) this.ipmClient.publicChannelMap.get(cSid);
			if(channelImpl != null) {
				logger.d("onTypingStarted channelImpl " + channelImpl.getSid() + "|"+channelImpl.hashCode());
			}
			
			Map<ChannelListener, Handler> listenerMap = this.ipmClient.channelListenerMap.get(cSid);
			if(listenerMap != null) {
				logger.d("onTypingStarted channelImpl listenerMap " + listenerMap.toString()); 
 				for (Map.Entry<ChannelListener, Handler> entry : listenerMap.entrySet()) {
 					final ChannelListener listener = entry.getKey();
 					logger.d("onTypingStarted channelImpl listener " + listener.hashCode());
 				    Handler handler = entry.getValue();
 				    if (handler != null) {
 						logger.d("onTypingStarted handler not null.");
 						handler.post(new Runnable() {
 							@Override
 							public void run() {
 								if(listener!= null) {
 									logger.d("onTypingStarted calling listener");
 									listener.onTypingStarted(member);
 								}
 							}
 						});
 					}	
 				}
			} else {
				if(channelImpl != null) {
					channelImpl.handleOnTypingStarted(member);
				}
			}
		}
	}
	
	public void onTypingEnded(final Channel channel, final Member member) {
		logger.d("Entered onTypingEnded");
		if(channel != null) {
			String cSid = channel.getSid();
			final ChannelImpl channelImpl = (ChannelImpl) this.ipmClient.publicChannelMap.get(cSid);
			if(channelImpl != null) {
				logger.d("onTypingEnded channelImpl " + channelImpl.getSid() + "|"+channelImpl.hashCode());
			}
			
			Map<ChannelListener, Handler> listenerMap = this.ipmClient.channelListenerMap.get(cSid);
			if(listenerMap != null) {
				logger.d("onTypingEnded channelImpl listenerMap " + listenerMap.toString()); 
 				for (Map.Entry<ChannelListener, Handler> entry : listenerMap.entrySet()) {
 					final ChannelListener listener = entry.getKey();
 					logger.d("onTypingEnded channelImpl listener " + listener.hashCode());
 				    Handler handler = entry.getValue();
 				    if (handler != null) {
 						logger.d("onTypingEnded handler not null.");
 						handler.post(new Runnable() {
 							@Override
 							public void run() {
 								if(listener!= null) {
 									logger.d("onTypingEnded calling listener");
 									listener.onTypingEnded(member);
 								}
 							}
 						});
 					}	
 				}
			} else {
				if(channelImpl != null) {
					channelImpl.handleOnTypingEnded(member);
				}
			}
		}
	}
	
	public void onChannelAdd(final ChannelImpl channel) {
		logger.d("this.ipmClient" + this.ipmClient.hashCode());
		synchronized(this.ipmClient) {
			this.ipmClient.handleChannelAddEvent(channel);
		}
	}
	
	public void onChannelInvite(final Channel channel) {
		logger.d("Entered onChannelInvite");
		this.ipmClient.handleIncomingInvite(channel);		
	}

	
	public void onChannelChange(final ChannelImpl channel) {
		logger.d("Entered onChannelChange");
		this.ipmClient.handleChannelChanged(channel);	
	}


	public void onChannelDelete(final ChannelImpl  channel) {
		logger.d("Entered onChannelDelete");
		this.ipmClient.handleChannelDeleted(channel);
	}
	
	public void onChannelSync(final ChannelImpl channel) {
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
	
	public void onChannelCreated(final ChannelImpl channel) {
		logger.d("ipmClient " + this.ipmClient.hashCode());
		synchronized(this.ipmClient) {
			this.ipmClient.handleChannelCreate(channel);
		}
	}

}
