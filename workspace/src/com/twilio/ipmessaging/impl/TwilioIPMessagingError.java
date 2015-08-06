package com.twilio.ipmessaging.impl;


enum TwilioIPMessagingError
{
	// general errors
	GENERIC_ERROR(31000, "Generic error");
	
	static TwilioIPMessagingError fromCode(int code)
	{
		for ( TwilioIPMessagingError error : TwilioIPMessagingError.values() )
		{
			if ( error.getCode() == code )
				return error;
		}
		
		return null;
	}
	
	private final int code;
	private final String message;
	
	private TwilioIPMessagingError(int code, String message)
	{
		this.code = code;
		this.message = message;
	}
	
	int getCode()
	{
		return code;
	}
	
	String getMessage()
	{
		return message;
	}
}
