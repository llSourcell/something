package com.twilio.ipmessaging.demo;

import com.google.android.gms.gcm.GcmListenerService;

import android.os.Bundle;
import android.util.Log;

public class GCMListenerService extends GcmListenerService {

	public static final String TAG = "GCMListenerService";
	
	@Override
	public void onMessageReceived(String from, Bundle data) {
		Log.e(TAG, "GCM - onMessageReceived");
		for( String key : data.keySet() ){
			Log.e(TAG, "GCM " + key + " exists in bundle");
		}
		
	}
}
