package com.twilio.ipmessaging;

import com.twilio.ipmessaging.impl.Logger;
import com.twilio.ipmessaging.impl.TwilioIPMessagingSDKImpl;
import com.twilio.ipmessaging.impl.TwilioIpMessagingClientServiceImpl;

import android.app.Service;
import android.content.Intent;
import android.os.Binder;
import android.os.IBinder;


public class TwilioIPMessagingClientService extends Service
{
	private static final Logger logger = Logger.getLogger(TwilioIPMessagingClientService.class);
	private TwilioIpMessagingClientServiceImpl serviceImpl;
	private final TwilioBinder binder = new TwilioBinder();
	private int lastStartId = -1;
	
	public class TwilioBinder extends Binder
	{
		public TwilioIPMessagingSDKImpl getTwiloIPMessagingClient()
		{
			return serviceImpl.getTwilioIPMessagingClient();
		}
	}
	
	@Override
	public void onCreate()
	{
		serviceImpl = new TwilioIpMessagingClientServiceImpl();
		serviceImpl.initialize(this);
	}
	
	@Override
	public int onStartCommand(Intent intent, int flags, int startId)
	{	
		if ((flags & Service.START_FLAG_REDELIVERY) != 0)
			serviceImpl.restoreState(intent, binder);
		
		if (lastStartId != -1)
			stopSelfResult(lastStartId);
		lastStartId = startId;
		return Service.START_REDELIVER_INTENT;
	}

	@Override
	public void onDestroy()
	{
		super.onDestroy();
		serviceImpl.destroy();
		serviceImpl = null;
	}

	@Override
	public IBinder onBind(Intent intent)
	{
		return binder;
	}
}
