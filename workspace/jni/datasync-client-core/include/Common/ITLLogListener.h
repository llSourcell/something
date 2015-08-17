//
//  ITLLogListener.h
//  RTDNotificationLib
//
//  Created by Sergei Voronichev on 29/04/15.
//  Copyright (c) 2015 Twilio. All rights reserved.
//

#ifndef RTDNotificationLib_ITLLogListener_h
#define RTDNotificationLib_ITLLogListener_h


namespace rtd
{
    /*!
     * Provide log listener for logger backend
     */
    class ITLLogListener
    {
    public:
        /*!
         * Used to sink formatted log items to listener.
         */
        virtual void onNewEntry(const char * c, const char* prefix) = 0;
        virtual ~ITLLogListener() = default;
    };
}


#endif
