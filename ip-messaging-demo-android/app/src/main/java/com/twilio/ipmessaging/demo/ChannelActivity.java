package com.twilio.ipmessaging.demo;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Random;

import com.twilio.ipmessaging.demo.R;
import com.twilio.ipmessaging.Channel;
import com.twilio.ipmessaging.Channel.ChannelType;
import com.twilio.ipmessaging.ChannelListener;
import com.twilio.ipmessaging.Channels;
import com.twilio.ipmessaging.Constants;
import com.twilio.ipmessaging.Constants.CreateChannelListener;
import com.twilio.ipmessaging.Constants.StatusListener;
import com.twilio.ipmessaging.IPMessagingClientListener;
import com.twilio.ipmessaging.Member;
import com.twilio.ipmessaging.Message;
import com.twilio.ipmessaging.TwilioIPMessagingSDK;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Parcelable;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;
import uk.co.ribot.easyadapter.EasyAdapter;
import android.util.Log;
import com.twilio.ipmessaging.demo.BasicIPMessagingClient.LoginListener;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.IntentFilter;


@SuppressLint("InflateParams")
public class ChannelActivity extends Activity implements ChannelListener, IPMessagingClientListener, LoginListener{

	private static final String[] CHANNEL_OPTIONS = { "Join" };
	private static final int JOIN = 0;
	//private static final int DESTROY = 1;

	private ListView listView;
	private BasicIPMessagingClient basicClient;
	private List<Channel> channels = new ArrayList<Channel>();
	private EasyAdapter<Channel> adapter;
	private AlertDialog createChannelDialog;
	private Channels channelsObject;
	private Channel[] channelArray;
	
	private static final Handler handler = new Handler();
	private AlertDialog incomingChannelInvite;
	private ProgressDialog progressDialog;
	private StatusListener joinListener;
	private StatusListener declineInvitationListener;

	private String accessToken = null;

	private String urlString;
	private Context context = this;


	private static final String ACCESS_TOKEN_SERVICE_URL = BuildConfig.ACCESS_TOKEN_SERVICE_URL;

	@Override
	public void onLoginStarted() {
		Log.v("Log", "Log in started");
	}

	@Override
	public void onLoginFinished() {
		//Intent intent = new Intent(this, ChannelActivity.class);
		//this.startActivity(intent);
	}

	@Override
	public void onLoginError(String errorMessage) {
	}

	@Override
	public void onLogoutFinished() {
		// TODO Auto-generated method stub
	}



	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_channel);
		//init the client
		basicClient = TwilioApplication.get().getBasicClient();

		//init the IPM client
		accessToken = "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiIsImN0eSI6InR3aWxpby1mcGE7dj0xIn0.eyJqdGkiOiJTSzZjMzY4M2VmZTAyNmFmNjA4MDNiY2ZkZjg5ODYwOTkxLTE0NTM2MTUyMDEiLCJpc3MiOiJTSzZjMzY4M2VmZTAyNmFmNjA4MDNiY2ZkZjg5ODYwOTkxIiwic3ViIjoiQUNlM2MxZDU0ODgwMTMxODEwOWRlNTA5MzY1NDRiZWU4NiIsIm5iZiI6MTQ1MzYxNTIwMSwiZXhwIjoxNDUzNjE4ODAxLCJncmFudHMiOnsiaWRlbnRpdHkiOiJKb2x0aW5Bbm5hQW5jaG9yYWdlIiwiaXBfbWVzc2FnaW5nIjp7InNlcnZpY2Vfc2lkIjoiSVM0NTRhOGY3ZWFmNWE0ZjRmOTU4ZjAxZDAxMWMwNDhmMyIsImVuZHBvaW50X2lkIjoiVHdpbGlvQ2hhdERlbW86Sm9sdGluQW5uYUFuY2hvcmFnZToifX19.AZweCewTeyD7tzVCZ72VDpyUGmRGvpT1gGXf95m3Lwk";
		basicClient.setAccessToken(accessToken);
		StringBuilder url = new StringBuilder();
		url.append(ACCESS_TOKEN_SERVICE_URL);
		urlString = url.toString();
		ChannelActivity.this.basicClient.doLogin(accessToken, ChannelActivity.this, urlString);
		Log.v("YO", "The current token is" + accessToken + urlString);

	}

	BroadcastReceiver mReceiver = new BroadcastReceiver() {
		@Override
		public void onReceive(Context context, Intent intent) {
			Log.v("YO", "DID THIS WORK?");
			if(basicClient != null && basicClient.getIpMessagingClient() != null) {
				basicClient.getIpMessagingClient().setListener(ChannelActivity.this);
				setupListView();
				final Channel channel = channelsObject.getChannelByUniqueName("general");
				Log.v("YO", "This is the channel" + channel.getFriendlyName());
				if (channel.getStatus() == Channel.ChannelStatus.JOINED) {
					Log.v("YO", "You've joined the channel");
					Intent i = new Intent(ChannelActivity.this, MessageActivity.class);
					i.putExtra(Constants.EXTRA_CHANNEL, (Parcelable) channel);
					i.putExtra("C_SID", channel.getSid());
					startActivity(i);
				} else {
					Log.v("YO", "You haven't joined the channel");
					joinListener = new StatusListener() {

						@Override
						public void onError() {
							Log.v("Log", "failed to join channel");
						}

						@Override
						public void onSuccess() {
							runOnUiThread(new Runnable() {
								@Override
								public void run() {
									adapter.notifyDataSetChanged();
								}
							});
							Log.v("Log", "Successfully joined channel");
							Intent i = new Intent(ChannelActivity.this, MessageActivity.class);
							i.putExtra(Constants.EXTRA_CHANNEL, (Parcelable) channel);
							i.putExtra("C_SID", channel.getSid());
							startActivity(i);
						}

					};
					channel.join(joinListener);

				}

			} else {
				Log.v("OH", "OH SHIT the client isn't initialized");
			}
		}
	};

	@Override
	protected void onResume() {
		super.onResume();
		registerReceiver(mReceiver, new IntentFilter(basicClient.BROADCAST_FILTER));
		Log.v("YO", "DID THIS WORK??");

	}

	@Override
	protected void onDestroy() {
		unregisterReceiver(mReceiver);
		super.onDestroy();
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		getMenuInflater().inflate(R.menu.channel, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case R.id.action_create_public:
			showCreateChannelDialog(ChannelType.CHANNEL_TYPE_PUBLIC);
			break;
		case R.id.action_create_private:
			showCreateChannelDialog(ChannelType.CHANNEL_TYPE_PRIVATE);
			break;
		case R.id.action_create_public_withoptions:
		{
			Random rand = new Random(); 
			int value = rand.nextInt(50); 
			
			Channels channelsLocal= basicClient.getIpMessagingClient().getChannels();
			final Map<String, String> attrs = new HashMap<String, String>();
			attrs.put("topic", "testing channel creation with options");
			Map<String, Object> options = new HashMap<String, Object>();
			options.put(Constants.CHANNEL_FRIENDLY_NAME, "Pub_TestChannelF_"+value);
			options.put(Constants.CHANNEL_UNIQUE_NAME, "Pub_TestChannelU_"+value);
			options.put(Constants.CHANNEL_TYPE, ChannelType.CHANNEL_TYPE_PUBLIC);
			options.put("attributes", attrs);
			channelsLocal.createChannel(options, new CreateChannelListener()
	        {
	            @Override
	            public void onCreated(final Channel newChannel){
	            	Log.v("Log", "Successfully created a channel with options.");
	            }

				@Override
				public void onError() {
					Log.v("Log","Error creating a channel");
				}
	        });
			break;
		}
		case R.id.action_create_private_withoptions:
		{
			Random rand = new Random(); 
			int value = rand.nextInt(50); 
			
			Channels channelsLocal= basicClient.getIpMessagingClient().getChannels();
			Map<String, Object> options = new HashMap<String, Object>();
			options.put(Constants.CHANNEL_FRIENDLY_NAME, "Priv_TestChannelF_"+value);
			options.put(Constants.CHANNEL_UNIQUE_NAME, "Priv_TestChannelU_"+value);
			options.put(Constants.CHANNEL_TYPE, ChannelType.CHANNEL_TYPE_PUBLIC);
			channelsLocal.createChannel(null, null);
			break;
		}
		case R.id.action_search_by_unique_name:
			showSearchChannelDialog();
			break;
		case R.id.action_logout:
			basicClient.getIpMessagingClient().shutdown();
			finish();
			break;
		case R.id.action_shutdown:
			TwilioIPMessagingSDK.shutdown();
			finish();
			break;
		}
		return super.onOptionsItemSelected(item);
	}

//	@Override
//	protected void onResume() {
//		super.onResume();
//		handleIncomingIntent(getIntent());
//		getChannels(null);
//	}

	private boolean handleIncomingIntent(Intent intent) {
		if(intent != null) {
			Channel channel = intent.getParcelableExtra(Constants.EXTRA_CHANNEL);
			String action = intent.getStringExtra(Constants.EXTRA_ACTION);
			intent.removeExtra(Constants.EXTRA_CHANNEL);
		    intent.removeExtra(Constants.EXTRA_ACTION);
			if(action != null) {
				if(action.compareTo(Constants.EXTRA_ACTION_INVITE) == 0 ) {
					this.showIncomingInvite(channel);
				}
			}
		}
		return false;
	}

	private void showCreateChannelDialog(final ChannelType type) {
		AlertDialog.Builder builder = new AlertDialog.Builder(ChannelActivity.this);
		// Get the layout inflater
		LayoutInflater inflater = getLayoutInflater();
		String title = "Enter " + type.toString() + " name";

		// Inflate and set the layout for the dialog
		// Pass null as the parent view because its going in the dialog layout
		builder.setView(inflater.inflate(R.layout.dialog_add_channel, null)).setTitle(title)
				.setPositiveButton("Create", new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog, int id) {
						
						String channelName = ((EditText) createChannelDialog.findViewById(R.id.channel_name)).getText()
								.toString();
						Log.v("Log", "Creating channel with friendly Name|" + channelName +"|");
						Channels channelsLocal= basicClient.getIpMessagingClient().getChannels();
						channelsLocal.createChannel(channelName,type, new CreateChannelListener()
				        {
				            @Override
				            public void onCreated(final Channel newChannel)
				            {
				            	Log.v("Log", "Successfully created a channel");
				            	if(newChannel != null) {
				            		final String sid = newChannel.getSid();
				            		ChannelType type = newChannel.getType();
				            	 	newChannel.setListener(ChannelActivity.this);
				            		Log.v("Log", "Channel created|SID|"+sid+"|TYPE|" + type.toString());
								} 
				            }

				            @Override
				            public void onError()
				            {
				               
				            }
				        }); 

					}
				}).setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int id) {
						dialog.cancel();
					}
				});
		createChannelDialog = builder.create();
		createChannelDialog.show();
	}
	
	
	private void showSearchChannelDialog() {
		AlertDialog.Builder builder = new AlertDialog.Builder(ChannelActivity.this);
		// Get the layout inflater
		LayoutInflater inflater = getLayoutInflater();
		String title = "Enter unique channel name";

		// Inflate and set the layout for the dialog
		// Pass null as the parent view because its going in the dialog layout
		builder.setView(inflater.inflate(R.layout.dialog_search_channel, null)).setTitle(title)
				.setPositiveButton("Search", new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog, int id) {
						
						String channelName = ((EditText) createChannelDialog.findViewById(R.id.channel_name)).getText()
								.toString();
						Log.v("Log", channelName);
						Channels channelsLocal= basicClient.getIpMessagingClient().getChannels();
						final Channel channel = channelsLocal.getChannelByUniqueName(channelName);
				            		
	            		runOnUiThread(new Runnable() {
	            	        @Override
	            	        public void run() {
	            	        	if(channel !=null ) {
	            	        		showToast(channel.getSid()+":"+channel.getFriendlyName());
	            	        	} else {
	            	        		showToast("Channel not found.");
	            	        	}
	            	        }
	            	    });
					}
				});
		createChannelDialog = builder.create();
		createChannelDialog.show();
	}


	private void setupListView() {
		listView = (ListView) findViewById(R.id.channel_list);
		adapter = new EasyAdapter<Channel>(this, ChannelViewHolder.class, channels,
				new ChannelViewHolder.OnChannelClickListener() {
					@Override
					public void onChannelClicked(final Channel channel) {
						if (channel.getStatus() == Channel.ChannelStatus.JOINED) {
							final Channel channelSelected = channelsObject.getChannel(channel.getSid());
							if(channelSelected!= null) {
								new Handler().postDelayed(new Runnable() {
						            @Override
						            public void run() {
						            	Intent i = new Intent(ChannelActivity.this, MessageActivity.class);
										i.putExtra(Constants.EXTRA_CHANNEL, (Parcelable) channelSelected);
										i.putExtra("C_SID", channelSelected.getSid());
										startActivity(i);
						            }
						        }, 0);
								
							}
							return;
						} 
						AlertDialog.Builder builder = new AlertDialog.Builder(ChannelActivity.this);
						builder.setTitle("Select an option").setItems(CHANNEL_OPTIONS,
								new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog, int which) {
								if (which == JOIN) {
									dialog.cancel();
									joinListener = new StatusListener() {
				            			
				    					@Override
				    					public void onError() {
				    						Log.v("Log", "failed to join channel");
				    					}
				    	
				    					@Override
				    					public void onSuccess() {
				    						runOnUiThread(new Runnable() {
						            	        @Override
						            	        public void run() {
						            	        	adapter.notifyDataSetChanged();
						            	        }
						            	    });
				    						Log.v("Log", "Successfully joined channel");
				    					}
				    	      			
				    	      		};
									channel.join(joinListener);	     	
								} 
							}
						});
						builder.show();
					}
				});
		listView.setAdapter(adapter);
		getChannels(null);
	}
	
	private void getChannels(String channelId) {
		if (this.channels != null) {
			if(basicClient != null && basicClient.getIpMessagingClient() != null) {
				channelsObject= basicClient.getIpMessagingClient().getChannels();
					if(channelsObject != null) {
						channelsObject.loadChannelsWithListener(new StatusListener() {
							@Override
							public void onError() {
								Log.v("Log", "Failed to loadChannelsWithListener");
							}
			
							@Override
							public void onSuccess() {
								Log.v("Log", "Successfully loadChannelsWithListener.");
								if(channels != null) {
									channels.clear();
								}
								if (channelsObject != null) {
									channelArray = channelsObject.getChannels();
									setupListenersForChannel(channelArray);
									if(ChannelActivity.this.channels != null && channelArray != null ) {
										ChannelActivity.this.channels.addAll(new ArrayList<Channel>(Arrays.asList(channelArray)));
										Collections.sort(ChannelActivity.this.channels, new CustomChannelComparator());
										adapter.notifyDataSetChanged();
									}
								}
							}
			      		});	     	
					}
			}
		}
	}

	@Override
	public void onMessageAdd(Message message) {
		if(message != null) {
			Log.v("Log", "Received onMessageAdd event");
		}
	}

	@Override
	public void onAttributesChange(Map<String, String> updatedAttributes) {
		if(updatedAttributes != null) {
			Log.v("Log", "updatedAttributes event received");
		}
	}

	public void onMessageChange(Message message) {
		if(message != null) {
			Log.v("Log", "Received onMessageChange event");
		}		
	}
	
	private void showIncomingInvite(final Channel channel)
    {
        handler.post(new Runnable()
        {
            @Override
            public void run()
            {
                if (incomingChannelInvite == null) {
                    incomingChannelInvite = new AlertDialog.Builder(ChannelActivity.this)
                        .setTitle(R.string.incoming_call)
                        .setMessage(R.string.incoming_call_message)
                        .setPositiveButton(R.string.join, new DialogInterface.OnClickListener()
                        {
                            @Override
                            public void onClick(DialogInterface dialog, int which)
                            {
                            	channel.join(new StatusListener() {
			            			
			    					@Override
			    					public void onError() {
			    						Log.v("Log", "Failed to join channel");
			    					}
			    					
			    					@Override
			    					public void onSuccess() {
			    						runOnUiThread(new Runnable() {
					            	        @Override
					            	        public void run() {
					            	        	adapter.notifyDataSetChanged();
					            	        }
					            	    });
			    						Log.v("Log", "Successfully joined channel");
			    					}
			    	      		});	     	
                                incomingChannelInvite = null;
                            }
                        })
                        .setNegativeButton(R.string.decline, new DialogInterface.OnClickListener()
                        {

							@Override
                            public void onClick(DialogInterface dialog, int which)
                            {
							declineInvitationListener = new StatusListener() {

								@Override
								public void onError() {
									Log.v("Log", "Failed to decline channel invite");
								}

								@Override
								public void onSuccess() {
									Log.v("Log", "Successfully to declined channel invite");
								}

							};
                               channel.declineInvitation(declineInvitationListener);	     	
                               incomingChannelInvite = null;
                            }
                        })
                        .create();
                    incomingChannelInvite.show();
                }
            }
        });
    }
	
	private class CustomChannelComparator implements Comparator<Channel> {
		@Override
		public int compare(Channel lhs, Channel rhs) {
			return lhs.getFriendlyName().compareTo(rhs.getFriendlyName());		
		}
	}

	@Override
	public void onMessageDelete(Message message) {
		Log.v("Log", "Member deleted");
	}

	@Override
	public void onMemberJoin(Member member) {
		if(member != null) {
			Log.v("Log", member.getIdentity() + " joined");
		}
	}

	@Override
	public void onMemberChange(Member member) {
		if(member != null) {
			Log.v("Log", member.getIdentity() + " changed");
		}
	}
	
	@Override
	public void onMemberDelete(Member member) {
		if(member != null) {
			Log.v("Log", member.getIdentity() + " deleted");
		}
		
	}
	
	private void setupListenersForChannel(Channel[] channelArray){
		if(channelArray != null) {
			for(int i=0; i<channelArray.length; i++) {
				if(channelArray[i] != null) {
					channelArray[i].setListener(ChannelActivity.this);
				}
			}
		}
	}
	
	private void showToast(String text) {
		Toast toast= Toast.makeText(getApplicationContext(),text, Toast.LENGTH_LONG);
		toast.setGravity(Gravity.CENTER_HORIZONTAL, 0, 0);
		LinearLayout toastLayout = (LinearLayout) toast.getView();
		TextView toastTV = (TextView) toastLayout.getChildAt(0);
		toastTV.setTextSize(30);
		toast.show(); 
	}
	
	@Override
	public void onTypingStarted(Member member){
		if(member != null) {
		}
	}
	
	@Override
	public void onTypingEnded(Member member) {
		if(member != null) {
		}
	}
	
	@Override
	public void onChannelHistoryLoaded(Channel channel) {
	}

	@Override
	public void onChannelAdd(Channel channel) {
		runOnUiThread(new Runnable() {
	        @Override
	        public void run() {
	        	getChannels(null);
	        }
	    });
	}

	@Override
	public void onChannelChange(Channel channel) {
	}

	@Override
	public void onChannelDelete(Channel channel) {
	}

	@Override
	public void onError(int errorCode, String errorText) {
	}

	@Override
	public void onAttributesChange(String attributes) {
	}
}
