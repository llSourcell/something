package com.twilio.example;

import java.util.ArrayList;
import java.util.List;

import com.twilio.ipmessaging.Channel;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.ListView;
import uk.co.ribot.easyadapter.EasyAdapter;

public class ChannelActivity extends Activity {
	private static final String[] CHANNEL_OPTIONS = { "Join", "Leave", "Destroy" };

	private ListView listView;
	private TestRTDJNI rtdJni;
	private List<Channel> channels = new ArrayList<Channel>();
	private EasyAdapter<Channel> adapter;

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
			// TODO:
			break;
		}
		return super.onOptionsItemSelected(item);

	}

	private void setupListView() {
		listView = (ListView) findViewById(R.id.channel_list);
		adapter = new EasyAdapter<>(this, ChannelViewHolder.class, rtdJni.getChannels(),
				new ChannelViewHolder.OnChannelClickListener() {
					@Override
					public void onChannelClicked(final Channel channel) {
						AlertDialog.Builder builder = new AlertDialog.Builder(ChannelActivity.this);
						builder.setTitle("Select an option").setItems(CHANNEL_OPTIONS,
								new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog, int which) {
								if (which == 0) {
									channel.join();
								} else if (which == 1) {
									channel.leave();
								} else if (which == 2) {
									channel.destroy();
								}
							}
						});
					}
				});
		listView.setAdapter(adapter);
		adapter.notifyDataSetChanged();
	}
}
