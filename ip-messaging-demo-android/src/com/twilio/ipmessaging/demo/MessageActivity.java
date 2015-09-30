package com.twilio.ipmessaging.demo;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.twilio.example.R;
import com.twilio.ipmessaging.Channel;
import com.twilio.ipmessaging.Member;
import com.twilio.ipmessaging.Members;
import com.twilio.ipmessaging.Message;
import com.twilio.ipmessaging.Messages;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.PendingIntent;
import android.content.DialogInterface;
import android.content.Intent;
import android.database.DataSetObserver;
import android.os.Bundle;
import android.text.style.SuperscriptSpan;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;
import uk.co.ribot.easyadapter.EasyAdapter;

public class MessageActivity extends Activity{

	private static final Logger logger = Logger.getLogger(MessageActivity.class);
	private ListView messageListView;
	private EditText inputText;
	private EasyAdapter<Message> adapter;
	
	private List<Message> messages =  new ArrayList<Message>();
	private List<Member> members =  new ArrayList<Member>();
	private Channel channel;
	private static final String[] EDIT_OPTIONS = {"Change Friendly Name", "Change Topic", "List Members", "Invite Member", "Add Member", "Remove Member", "Leave" };
	
	private static final int NAME_CHANGE = 0;
	private static final int TOPIC_CHANGE = 1;
	private static final int LIST_MEMBERS = 2;
	private static final int INVITE_MEMBER = 3;
	private static final int ADD_MEMBER = 4;
	private static final int REMOVE_MEMBER = 5;
	private static final int LEAVE = 6;
	
	private AlertDialog editTextDialog;
	private AlertDialog memberListDialog;
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		createUI();
	}
	
	@Override
	protected void onResume() {
		super.onResume();
		Intent intent = getIntent();
		if (intent != null) {
			Channel channel = intent.getParcelableExtra(Channel.EXTRA_CHANNEL);
			if(channel != null) {
				setupListView(channel);
			}
		}
	}
	
	private void createUI() {
		setContentView(R.layout.activity_message);
		messageListView = (ListView) findViewById(R.id.message_list_view);
		if(getIntent() != null) {
			channel = (Channel) getIntent().getParcelableExtra(Channel.EXTRA_CHANNEL);
			if(channel != null) {
				Intent intent = new Intent(this, MessageActivity.class);
				PendingIntent pendingIntent = PendingIntent.getActivity(this, 0, intent, PendingIntent.FLAG_UPDATE_CURRENT);
				channel.setIncomingIntent(pendingIntent);
			}
		}
	
		setupListView(channel);
		this.setTitle("Channel: "+channel.getFriendlyName());
		messageListView.setTranscriptMode(ListView.TRANSCRIPT_MODE_ALWAYS_SCROLL);
		messageListView.setStackFromBottom(true);
		adapter.registerDataSetObserver(new DataSetObserver() {
			@Override
			public void onChanged() {
				super.onChanged();
				messageListView.setSelection(adapter.getCount() - 1);
			}
		});
		setupInput();
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		getMenuInflater().inflate(R.menu.message, menu);
		return true;
	}
	
	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case R.id.action_settings:
			showChannelSettingsDialog();
			break;
		}
		return super.onOptionsItemSelected(item);

	}
	
	private void showChannelSettingsDialog() {
		AlertDialog.Builder builder = new AlertDialog.Builder(MessageActivity.this);
		//final AlertDialog alertDialog = builder.show();
		builder.setTitle("Select an option").setItems(EDIT_OPTIONS,
				new DialogInterface.OnClickListener() {
			public void onClick(DialogInterface dialog, int which) {
				if (which == NAME_CHANGE) {
					showChangeNameDialog();
				} else if (which == TOPIC_CHANGE) {
					channel.leave();
				} else if (which == LIST_MEMBERS) {
					Members membersObject = channel.getMemberArray();
					Member[] members = membersObject.getMembers();
					
					logger.d("member retrieved");
					StringBuffer name = new StringBuffer();
					for(int i= 0; i< members.length; i++) {
						name.append(members[i].getIdentity());
						if(i+1 <members.length) {
							name.append(" ,");
						}
					} 
					Toast toast= Toast.makeText(getApplicationContext(),name, Toast.LENGTH_LONG);
					toast.setGravity(Gravity.CENTER_HORIZONTAL, 0, 0);
					LinearLayout toastLayout = (LinearLayout) toast.getView();
					TextView toastTV = (TextView) toastLayout.getChildAt(0);
					toastTV.setTextSize(30);
					toast.show(); 
				} else if (which == INVITE_MEMBER) {			
					showInviteMemberDialog();
				} else if(which == ADD_MEMBER) {
					showAddMemberDialog();
				} else if (which == LEAVE) {
					channel.leave();
					finish();
				} else if (which == REMOVE_MEMBER) {
					showRemoveMemberDialog();
				}  
			}
		});
		
		builder.show();
	}
	
	private void showChangeNameDialog() {
		AlertDialog.Builder builder = new AlertDialog.Builder(MessageActivity.this);
		// Get the layout inflater
		LayoutInflater inflater = getLayoutInflater();

		// Inflate and set the layout for the dialog
		// Pass null as the parent view because its going in the dialog layout
		builder.setView(inflater.inflate(R.layout.dialog_edit_friendly_name, null))
				.setPositiveButton("Update", new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog, int id) {
						String friendlyName = ((EditText) editTextDialog.findViewById(R.id.update_friendly_name)).getText()
								.toString();
						logger.e(friendlyName);
						channel.updateFriendlyName(friendlyName);
					}
				}).setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int id) {
						dialog.cancel();
					}
				});
		editTextDialog = builder.create();
		editTextDialog.show();
	}
	
	private void showChangeTopicDialog() {
		AlertDialog.Builder builder = new AlertDialog.Builder(MessageActivity.this);
		// Get the layout inflater
		LayoutInflater inflater = getLayoutInflater();

		// Inflate and set the layout for the dialog
		// Pass null as the parent view because its going in the dialog layout
		builder.setView(inflater.inflate(R.layout.dialog_edit_friendly_name, null))
				.setPositiveButton("Update", new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog, int id) {
						String topic = ((EditText) editTextDialog.findViewById(R.id.update_topic)).getText()
								.toString();
						logger.e(topic);
						Map attrMap = new HashMap<String, String>();
						attrMap.put("Topic", topic);
						channel.updateAttributes(attrMap);
					}
				}).setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int id) {
						dialog.cancel();
					}
				});
		editTextDialog = builder.create();
		editTextDialog.show();
	}
	
	private void showInviteMemberDialog() {
		AlertDialog.Builder builder = new AlertDialog.Builder(MessageActivity.this);
		// Get the layout inflater
		LayoutInflater inflater = getLayoutInflater();

		// Inflate and set the layout for the dialog
		// Pass null as the parent view because its going in the dialog layout
		builder.setView(inflater.inflate(R.layout.dialog_invite_member, null))
				.setPositiveButton("Invite", new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog, int id) {
						String memberName = ((EditText) editTextDialog.findViewById(R.id.invite_member)).getText()
								.toString();
						logger.e(memberName);
						
						Members membersObject = channel.getMemberArray();
						membersObject.inviteByIdentity(memberName);
					}
				}).setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int id) {
						dialog.cancel();
					}
				});
		editTextDialog = builder.create();
		editTextDialog.show();
	}
	
	private void showAddMemberDialog() {
		AlertDialog.Builder builder = new AlertDialog.Builder(MessageActivity.this);
		// Get the layout inflater
		LayoutInflater inflater = getLayoutInflater();

		// Inflate and set the layout for the dialog
		// Pass null as the parent view because its going in the dialog layout
		builder.setView(inflater.inflate(R.layout.dialog_add_member, null))
				.setPositiveButton("Add", new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog, int id) {
						String memberName = ((EditText) editTextDialog.findViewById(R.id.add_member)).getText()
								.toString();
						logger.e(memberName);
						
						Members membersObject = channel.getMemberArray();
						membersObject.addByIdentity(memberName);
					}
				}).setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int id) {
						dialog.cancel();
					}
				});
		editTextDialog = builder.create();
		editTextDialog.show();
	}
	
	private void showRemoveMemberDialog() {
		final Members membersObject = channel.getMemberArray();
		Member[] membersArray= membersObject.getMembers();
		if(membersArray.length > 0 ) {
			members = new ArrayList<Member>(Arrays.asList(membersArray));
		}
		
		AlertDialog.Builder alertDialog = new AlertDialog.Builder(MessageActivity.this);
        LayoutInflater inflater = getLayoutInflater();
        View convertView = (View) inflater.inflate(R.layout.member_list, null);
        alertDialog.setView(convertView);
        alertDialog.setTitle("List");
        ListView lv = (ListView) convertView.findViewById(R.id.listView1);
		EasyAdapter<Member> adapterMember = new EasyAdapter<Member>(this, MemberViewHolder.class, members,
				new MemberViewHolder.OnMemberClickListener() {
					@Override
					public void onMemberClicked(Member member) {
						membersObject.removeMember(member);
						memberListDialog.dismiss();
					}
				});
		lv.setAdapter(adapterMember);
		memberListDialog = alertDialog.create();
		memberListDialog.show();
		memberListDialog.getWindow().setLayout(800, 600);
	}
	

	private void setupInput() {
		// Setup our input methods. Enter key on the keyboard or pushing the
		// send
		// button
		EditText inputText = (EditText) findViewById(R.id.messageInput);
		inputText.setOnEditorActionListener(new TextView.OnEditorActionListener() {
			@Override
			public boolean onEditorAction(TextView textView, int actionId, KeyEvent keyEvent) {
				if (actionId == EditorInfo.IME_NULL && keyEvent.getAction() == KeyEvent.ACTION_DOWN) {
					sendMessage();
				}
				return true;
			}
		});

		findViewById(R.id.sendButton).setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View view) {
				sendMessage();
			}
		});
	}


	private void setupListView(Channel channel) {
		messageListView = (ListView) findViewById(R.id.message_list_view);
		Messages messagesObject = channel.getMessages(50);
		Message[] messagesArray = messagesObject.getMessages();
		if(messagesArray.length > 0 ) {
			messages = new ArrayList<Message>(Arrays.asList(messagesArray));
			Collections.sort(messages, new CustomMessageComparator());
		}
		adapter = new EasyAdapter<Message>(this, MessageViewHolder.class, messages,
				new MessageViewHolder.OnMessageClickListener() {
					@Override
					public void onMessageClicked(final Channel channel) {
						
					}
				});
		messageListView.setAdapter(adapter);
		adapter.notifyDataSetChanged(); 
	}


	private void sendMessage() {
		inputText = (EditText) findViewById(R.id.messageInput);
		String input = inputText.getText().toString();
		if (!input.equals("")) {
			
			Messages messagesObject = this.channel.getMessages(50);
			Message message = messagesObject.createMessage(input);
			messagesObject.sendMessage(message);
		
			messages.add(message);
			adapter.notifyDataSetChanged();
			inputText.setText("");
		}
		inputText.requestFocus();
	}
	
	private class CustomMessageComparator implements Comparator<Message> {
		@Override
		public int compare(Message lhs, Message rhs) {
			return lhs.getTimeStamp().compareTo(rhs.getTimeStamp());		
		}
	}
}
