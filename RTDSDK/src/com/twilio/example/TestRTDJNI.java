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

	//This approach does not work, object references are lost when returned from the JNI layer  
	public void doTestPrevious(String capabilityToken) {
		this.init(this.capabilityToken);
		this.nativeIPMessagingClientListener = getMessagingListener();
		this.nativeNotificationClientObserver = getNotificationClientObserver();
		this.nativeConfigurationProvider = getConfigurationProvider(this.nativeNotificationClientObserver);
		this.nativeNotificationClient = getNotificationClient(this.capabilityToken, this.nativeConfigurationProvider, this.nativeNotificationClientObserver, this.nativeIPMessagingClientListener);
		this.nativeMessagingClient = getMessagingClient(this.capabilityToken, this.nativeIPMessagingClientListener, this.nativeConfigurationProvider, this.nativeNotificationClient);
		

		System.out.println("Test test test");

	}
	
	//This seems to be holding on to the object references. But notificationClientPtr->Init(notificationClientObserver);
	//results in errorcode 7 (TwilsockError). Currently my test device is connected to TwilioAV and tested using PROD.
	public void doTest(String capabilityToken) {
		
		System.out.println("Test starting .....");
		
		long i = this.init(this.capabilityToken);
		this.nativeMessagingClient = createMessagingClient(this.capabilityToken);
		

		System.out.println("Test Done");

	}

	
	public native void testRTD();	
	public native long init(String token);
	public native void shutdown();
	public native long  getMessagingListener();	
	public native long  getNotificationClientObserver();
	public native long  getConfigurationProvider(long nativeNotificationClientObserver);	
	public native long  getNotificationClient(String token, long cfgProvider, long notificationClientObserver, long nativeMsgClientListener);
	public native long  getMessagingClient(String token, long nativeMsgClient, long nativeConfigProvider, long nativeNotificationClient);
	public native long  createMessagingClient(String token);

}