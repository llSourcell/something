package com.twilio.example;

public class TestRTDJNI {

	static {
		System.loadLibrary("twilio-rtd-native"); // Load native library at runtime
	}

	public native void testRTD();

}