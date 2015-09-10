package com.twilio.example;

import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
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
	private Channel[] channels;

	public interface LoginListener {
		public void onLoginStarted();

		public void onLoginFinished();

		public void onLoginError(String errorMessage);

		public void onLogoutFinished();
	}
	
	private class CustomComparator implements Comparator<Channel> {
		@Override
		public int compare(Channel lhs, Channel rhs) {
			return lhs.getFriendlyName().compareTo(rhs.getFriendlyName());		
		}
	}

	public String getCapabilityToken() {
		return capabilityToken;
	}

	public void setCapabilityToken(String capabilityToken) {
		this.capabilityToken = capabilityToken;
	}

	// This seems to be holding on to the object references. But
	// notificationClientPtr->Init(notificationClientObserver);
	// results in errorcode 7 (TwilsockError). Currently my test device is
	// connected to TwilioAV and tested using PROD.
	public void doTest(String capabilityToken, LoginListener listener) {

		System.out.println("Test starting .....");

		long i = this.init(this.capabilityToken);
		long statusCode = this.createMessagingClient(this.capabilityToken);

	
		this.channels = this.getChannels();

		if(this.channels != null) {
			Collections.sort(Arrays.asList(this.channels), new CustomComparator());
			System.out.println("Test Done : " + channels.length);
		}
		
		if (statusCode == 0 && listener != null) {
			listener.onLoginFinished();
		} else {
			listener.onLoginError("some error");
		}

	}

	public void cleanupTest() {
		this.shutDown();
	}
	
	public List<Channel> getChannelList() {
		List<Channel> list = Arrays.asList(this.channels);
		return list;
	}
	
	public long getNativeClientParam() {
		return nativeClientParam;
	}

	public void setNativeClientParam(long nativeClientParam) {
		this.nativeClientParam = nativeClientParam;
	}
	
	public native long init(String token);

	public native void shutDown();

	public native long createMessagingClient(String token);

	public native Channel[] getChannels();

	public native void addChannel(String name);

	public native void removeChannel(Channel channel);
	
	public native void createMessage(String message);

	
}