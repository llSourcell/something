package com.twilio.example;

import android.app.Application;

public class TwilioApplication extends Application {

	private static TwilioApplication instance;
	private TestRTDJNI rtdJni;
	

	public static TwilioApplication get() {
		return instance;
	}

	@Override
	public void onCreate() {
		super.onCreate();
		TwilioApplication.instance = this;
		rtdJni = new TestRTDJNI();
	}
	
	public TestRTDJNI getRtdJni() {
		return this.rtdJni;
	}
}
