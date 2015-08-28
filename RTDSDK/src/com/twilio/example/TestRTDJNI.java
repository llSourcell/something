package com.twilio.example;


public class TestRTDJNI {

	static {
		System.loadLibrary("twilio-rtd-native"); // Load native library at
													// runtime
	}
	
	
	private long nativeIPMessagingClientListener;
	private long nativeNotificationClientObserver;
	private long nativeConfigurationProvider;
	private long nativeNotificationClient;
	private long nativeMessagingClient;
	private String capabilityToken;

	public void doTest(String capabilityToken) {
		
		this.nativeIPMessagingClientListener = getMessagingListener();
		this.nativeNotificationClientObserver = getNotificationClientObserver();
		this.nativeConfigurationProvider = getConfigurationProvider(this.nativeNotificationClientObserver);
		this.nativeNotificationClient = getNotificationClient(this.nativeConfigurationProvider);
		this.nativeMessagingClient = getMessagingClient(this.nativeIPMessagingClientListener, this.nativeConfigurationProvider, this.nativeNotificationClient);
		System.out.println("Test test test");

	}

	
	public native void testRTD();	
	public native void init();
	public native void shutdown();
	public native long  getMessagingListener();	
	public native long  getNotificationClientObserver();
	public native long  getConfigurationProvider(long nativeNotificationClientObserver);	
	public native long  getNotificationClient(long cfgProvider);
	public native long  getMessagingClient(long nativeMsgClient, long nativeConfigProvider, long nativeNotificationClient);

}