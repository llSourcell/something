package com.twilio.ipmessaging.demo;

import android.app.Application;

public class TwilioApplication extends Application {

	private static TwilioApplication instance;
	private BasicIPMessagingClient rtdJni;
	

	public static TwilioApplication get() {
		return instance;
	}

	@Override
	public void onCreate() {
		super.onCreate();
		TwilioApplication.instance = this;
		rtdJni = new BasicIPMessagingClient(getApplicationContext());
	}
	
	public BasicIPMessagingClient getRtdJni() {
		return this.rtdJni;
	}
}
