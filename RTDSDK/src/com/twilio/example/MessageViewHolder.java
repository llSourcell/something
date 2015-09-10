package com.twilio.example;

import com.twilio.ipmessaging.Channel;
import com.twilio.ipmessaging.Message;

import android.view.View;
import android.widget.TextView;
import uk.co.ribot.easyadapter.ItemViewHolder;
import uk.co.ribot.easyadapter.PositionInfo;
import uk.co.ribot.easyadapter.annotations.LayoutId;
import uk.co.ribot.easyadapter.annotations.ViewId;

@LayoutId(R.layout.message_item_layout)
public class MessageViewHolder extends ItemViewHolder<Message> {

	@ViewId(R.id.author)
	TextView author;

	@ViewId(R.id.date)
	TextView date;

	@ViewId(R.id.body)
	TextView body;

	View view;

	public MessageViewHolder(View view) {
		super(view);
		this.view = view;
	}

	@Override
	public void onSetValues(Message message, PositionInfo pos) {
		author.setText(message.getAuthor());
		//TODO:
//		date.setText(message.getDateUpdated().toString());
		body.setText(message.getMessageBody());
	}
	
	public interface OnMessageClickListener {
		void onMessageClicked(Channel channel);
	}

}
