package com.twilio.ipmessaging.demo;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import com.twilio.example.R;
import com.twilio.ipmessaging.Channel;
import com.twilio.ipmessaging.ChannelListener;
import com.twilio.ipmessaging.Channels;
import com.twilio.ipmessaging.Member;
import com.twilio.ipmessaging.Message;
import com.twilio.ipmessaging.Messages;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.PendingIntent;
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

@SuppressLint("InflateParams")
public class ChannelActivity extends Activity implements ChannelListener {

	private static final String[] CHANNEL_OPTIONS = { "Join", "Destroy" };
	private static final Logger logger = Logger.getLogger(ChannelActivity.class);
	private static final int JOIN = 0;
	private static final int DESTROY = 1;

	private ListView listView;
	private BasicIPMessagingClient rtdJni;
	private List<Channel> channels = new ArrayList<Channel>();
	private EasyAdapter<Channel> adapter;
	private AlertDialog createChannelDialog;
	private Channels channelsLocal;
	private Channel[] channelArray;
	
	private static final Handler handler = new Handler();
	private AlertDialog incoingChannelInvite;

	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_channel);
		rtdJni = TwilioApplication.get().getRtdJni();
		setupListView();
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		getMenuInflater().inflate(R.menu.channel, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case R.id.action_create:
			showCreateChannelDialog();
			break;
		}
		return super.onOptionsItemSelected(item);
	}

	@Override
	protected void onResume() {
		super.onResume();		
		handleIncomingIntent(getIntent());
		getChannels(null);
	}

	private boolean handleIncomingIntent(Intent intent) {
		if(intent != null) {
			Channel channel = intent.getParcelableExtra(Channel.EXTRA_CHANNEL);
			String action = intent.getStringExtra(Channel.EXTRA_ACTION);
			intent.removeExtra(Channel.EXTRA_CHANNEL);
		    intent.removeExtra(Channel.EXTRA_ACTION);
			if(action != null) {
				if(action.compareTo(Channel.EXTRA_ACTION_INVITE) == 0 ) {
					this.showIncomingInvite(channel);
				}
			}
		}
		return false;
	}

	private void showCreateChannelDialog() {
		AlertDialog.Builder builder = new AlertDialog.Builder(ChannelActivity.this);
		// Get the layout inflater
		LayoutInflater inflater = getLayoutInflater();

		// Inflate and set the layout for the dialog
		// Pass null as the parent view because its going in the dialog layout
		builder.setView(inflater.inflate(R.layout.dialog_add_channel, null)).setTitle("Enter channel name")
				.setPositiveButton("Create", new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog, int id) {
						String channelName = ((EditText) createChannelDialog.findViewById(R.id.channel_name)).getText()
								.toString();
						logger.e(channelName);
						Channels channelsLocal= rtdJni.getIpMessagingClient().getChannels();
						Channel newChannel = channelsLocal.createChannel(channelName, ChannelActivity.this);
						getChannels(newChannel.getSid());
					}
				}).setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int id) {
						dialog.cancel();
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
							Intent i = new Intent(ChannelActivity.this, MessageActivity.class);
							i.putExtra(Channel.EXTRA_CHANNEL, (Parcelable) channel);
							startActivity(i);
							return;
						} 
						AlertDialog.Builder builder = new AlertDialog.Builder(ChannelActivity.this);
						builder.setTitle("Select an option").setItems(CHANNEL_OPTIONS,
								new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog, int which) {
								if (which == JOIN) {
									channel.join();
								} else if (which == DESTROY) {
									channel.destroy();
									getChannels(null);
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
			this.channels.clear();
			channelsLocal= rtdJni.getIpMessagingClient().getChannels();
			channelArray = channelsLocal.getChannels();	
			this.channels.addAll(new ArrayList<Channel>(Arrays.asList(channelArray)));
			Collections.sort(this.channels, new CustomChannelComparator());
			adapter.notifyDataSetChanged();
		}
	}

	@Override
	public void onMessageAdd(Message message) {
		logger.d("Message received");
		StringBuffer text = new StringBuffer();
		text.append("From: " + message.getAuthor());
		text.append("Body:" + message.getMessageBody());
		text.append("cSid: " + message.getChannelSid());
		
		Toast toast= Toast.makeText(getApplicationContext(),text.toString(), Toast.LENGTH_LONG);
		toast.setGravity(Gravity.CENTER_HORIZONTAL, 0, 0);
		LinearLayout toastLayout = (LinearLayout) toast.getView();
		TextView toastTV = (TextView) toastLayout.getChildAt(0);
		toastTV.setTextSize(30);
		toast.show(); 
		
	}



	@Override
	public void onMessageDelete(Message message) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onMemberJoin(Member member) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onMemberChange(Member member) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onMemberDelete(Member member) {
		// TODO Auto-generated method stub
		
	}

	@Override
	public void onAttributesChange(Map<String, String> updatedAttributes) {
		// TODO Auto-generated method stub
		
	}

	public void onMessageChange(Message arg0) {
		// TODO Auto-generated method stub
		
	}
	
	private void showIncomingInvite(final Channel channel)
    {
        handler.post(new Runnable()
        {
            @Override
            public void run()
            {
                if (incoingChannelInvite == null) {
                    incoingChannelInvite = new AlertDialog.Builder(ChannelActivity.this)
                        .setTitle(R.string.incoming_call)
                        .setMessage(R.string.incoming_call_message)
                        .setPositiveButton(R.string.join, new DialogInterface.OnClickListener()
                        {
                            @Override
                            public void onClick(DialogInterface dialog, int which)
                            {
                            	channel.join();
                                incoingChannelInvite = null;
                            }
                        })
                        .setNegativeButton(R.string.decline, new DialogInterface.OnClickListener()
                        {
                            @Override
                            public void onClick(DialogInterface dialog, int which)
                            {
                               channel.declineInvitation();
                               incoingChannelInvite = null;
                            }
                        })
                        .create();
                    incoingChannelInvite.show();
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
}
