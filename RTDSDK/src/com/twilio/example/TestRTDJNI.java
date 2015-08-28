package com.twilio.example;


public class TestRTDJNI {

	static {
		System.loadLibrary("twilio-rtd-native"); // Load native library at
													// runtime
	}
	
	
	private long nativeIPMessagingClientListener;
	private long nativeConfigurationProviderr;
	private long nativeNotificationClient;

	public void doTest() {
		testRTD();
	}

	
	public native void testRTD();
	public native long  getMessagingListener();
	public native long  getNotificationClient();
	public native long  getConfigurationProvider();
	

}