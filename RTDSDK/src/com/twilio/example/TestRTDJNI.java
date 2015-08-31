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

	public String getCapabilityToken() {
		return capabilityToken;
	}


	public void setCapabilityToken(String capabilityToken) {
		this.capabilityToken = capabilityToken;
	}


	public void doTest(String capabilityToken) {
		this.init();
		this.nativeIPMessagingClientListener = getMessagingListener();
		this.nativeNotificationClientObserver = getNotificationClientObserver();
		this.nativeConfigurationProvider = getConfigurationProvider(this.nativeNotificationClientObserver);
		this.nativeNotificationClient = getNotificationClient(this.capabilityToken, this.nativeConfigurationProvider, this.nativeNotificationClientObserver);
		this.nativeMessagingClient = getMessagingClient(this.capabilityToken, this.nativeIPMessagingClientListener, this.nativeConfigurationProvider, this.nativeNotificationClient);
		

		System.out.println("Test test test");

	}

	
	public native void testRTD();	
	public native void init();
	public native void shutdown();
	public native long  getMessagingListener();	
	public native long  getNotificationClientObserver();
	public native long  getConfigurationProvider(long nativeNotificationClientObserver);	
	public native long  getNotificationClient(String token, long cfgProvider, long notificationClientObserver);
	public native long  getMessagingClient(String toen, long nativeMsgClient, long nativeConfigProvider, long nativeNotificationClient);

}