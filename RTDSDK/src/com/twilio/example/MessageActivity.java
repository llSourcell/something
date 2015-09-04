package com.twilio.example;

import java.util.ArrayList;
import java.util.List;

import com.twilio.ipmessaging.Message;
import com.twilio.ipmessaging.impl.MessageImpl;

import android.app.Activity;
import android.database.DataSetObserver;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;
import uk.co.ribot.easyadapter.EasyAdapter;

public class MessageActivity extends Activity {

	private ListView listView;
	private EditText inputText;
	private EasyAdapter<Message> adapter;
	private List<Message> message = new ArrayList<Message>();
	private TestRTDJNI rtdJni;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_message);
		rtdJni = TwilioApplication.get().getRtdJni();
		listView = (ListView) findViewById(R.id.message_list_view);
		adapter = new EasyAdapter<>(getBaseContext(), MessageViewHolder.class, message);
		adapter.registerDataSetObserver(new DataSetObserver() {
			@Override
			public void onChanged() {
				super.onChanged();
				listView.setSelection(adapter.getCount() - 1);
			}
		});
		listView.setAdapter(adapter);
		setupInput();
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

	private void sendMessage() {
		inputText = (EditText) findViewById(R.id.messageInput);
		String input = inputText.getText().toString();
		if (!input.equals("")) {
			MessageImpl msg = new MessageImpl("michael", input);
			message.add(msg);
			adapter.notifyDataSetChanged();
			pushNewMessage(input);
			inputText.setText("");
		}
		inputText.requestFocus();
	}

	private void pushNewMessage(String message) {
		// TODO:
	}

}
