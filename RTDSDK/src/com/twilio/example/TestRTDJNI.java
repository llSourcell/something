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
	private long nativeClientParam;

	public String getCapabilityToken() {
		return capabilityToken;
	}


	public void setCapabilityToken(String capabilityToken) {
		this.capabilityToken = capabilityToken;
	}
	
	//This seems to be holding on to the object references. But notificationClientPtr->Init(notificationClientObserver);
	//results in errorcode 7 (TwilsockError). Currently my test device is connected to TwilioAV and tested using PROD.
	public void doTest(String capabilityToken) {
		
		System.out.println("Test starting .....");
		
		long i = this.init(this.capabilityToken);
		this.nativeMessagingClient = createMessagingClient(this.capabilityToken);
		

		System.out.println("Test Done");

	}

	
	
	public native long init(String token);
	public native void shutdown();
	public native long  createMessagingClient(String token);

}