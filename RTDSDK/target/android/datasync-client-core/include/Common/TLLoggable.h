//
//  ITLLoggable.h
//  RTDNotificationLib
//
//  Created by Sergei Voronichev on 29/04/15.
//  Copyright (c) 2015 Twilio. All rights reserved.
//

#ifndef RTDNotificationLib_ITLLoggable_h
#define RTDNotificationLib_ITLLoggable_h

#include <string>

namespace rtd
{
    static unsigned long long LOGGABLE_OBJECT_ID = 0;

    /*!
     * Provide log scope for all inheritors.
     *
     * This class should be always inheritet as "public".
     * Otherwise logger won't have an access to it's methods.
     */
    class TLLoggable
    {
    protected:
        std::string m_name; //!< prefix of loggable object
        unsigned long long m_id; //!< id of loggable object

    public:
        explicit TLLoggable(const std::string& name = "")
            : m_name(name)
            , m_id(++LOGGABLE_OBJECT_ID)
        {}
        virtual ~TLLoggable() = default;

        /*!
         * Used inside logger, to make formatted string for loggable object.
         */
        virtual const std::string& Prefix() const
        {
            return m_name;
        }

        /*!
         * Used inside logger, to make formatted string for loggable object.
         *
         * Note: If there are several loggable objects with the same type(prefix) it is usefull to see their uniq id.
         */
        virtual unsigned long long Id() const
        {
            return m_id;
        }
    };
}

#endif
