package com.twilio.example;

import java.util.List;

import com.twilio.ipmessaging.Channel;

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
	 private LoginListener loginListener;
	
	public interface LoginListener
    {
        public void onLoginStarted();
        public void onLoginFinished();
        public void onLoginError(String errorMessage);
        public void onLogoutFinished();
    }


	public String getCapabilityToken() {
		return capabilityToken;
	}


	public void setCapabilityToken(String capabilityToken) {
		this.capabilityToken = capabilityToken;
	}
	
	//This seems to be holding on to the object references. But notificationClientPtr->Init(notificationClientObserver);
	//results in errorcode 7 (TwilsockError). Currently my test device is connected to TwilioAV and tested using PROD.
	public void doTest(String capabilityToken,  LoginListener listener) {
		
		System.out.println("Test starting .....");
		
		long i = this.init(this.capabilityToken);
		long statusCode = createMessagingClient(this.capabilityToken);
		
		if (statusCode == 0 && listener != null) {
			listener.onLoginFinished();
		} else {
			listener.onLoginError("some error");
		}

		System.out.println("Test Done");

	}
	
	public void cleanupTest() {
		this.shutDown();
	}

	
	
	public native long init(String token);
	public native void shutDown();
	public native long createMessagingClient(String token);
	public native List<Channel> getChannels();

}