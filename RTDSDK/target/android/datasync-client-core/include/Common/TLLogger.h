/*! \file TLLogger.h */
//
//  TLLogger.h
//  RTDNotificationLib
//
//  Created by Sergei Voronichev on 29/04/15.
//  Copyright (c) 2015 Twilio. All rights reserved.
//

#ifndef __RTDNotificationLib__TLLogger__
#define __RTDNotificationLib__TLLogger__

#include <type_traits>
#include <sstream>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <thread>
#include <mutex>
#include <vector>

#include "ITLLogListener.h"
#include "TLLoggable.h"
#include "ITDLogger.h"

#define RTD_ENABLE_LOGGING  1

namespace rtd
{
    /*!
     * Rtd Logger backend.
     *
     * Route log entries.
     */
    class TLLogger
    {
        ITLLogListener * m_listener;
        explicit TLLogger()
            : m_listener(nullptr)
        {}
        TLLogger(const TLLogger&) = default;
        TLLogger& operator=(TLLogger const&) = default;
        ~TLLogger() = default;

    public:
        static TLLogger& instance();

        /*!
         * Route log entry to defined listener or to default output.
         */
        void AddLine(const std::string& line, const std::string& prefix);

        /*!
         * Set log listener which can be used intead of default output.
         */
        void SetLogListener(ITLLogListener * listener)
        {
            m_listener = listener;
        }
    };


    /**
     * Log string formatting frontend.
     */
    static const std::vector<std::string> TLLogLevelArr = {"FATAL", "CRITICAL", "WARNING", "INFO", "DEBUG"};

    /*!
     * Log entries formatter.
     */
    class TLLog
    {
    public:
        enum class TLLogLevel: unsigned int
        {
            Fatal = 0, /**< Log level is used to show errors, wich mean that application doesn't work properly. */
            Critical = 1, /**< It means something bad happend. */
            Warning = 2, /**< Log level which is used to pay attention. */
            Info = 3, /**< Info log level. Most common log level. */
            Debug = 4 /**< This level is used to for debbuging. */
        };

    public:
        explicit TLLog(const std::string& prefix, unsigned long long objId, TLLogLevel level, bool displayId)
        : m_level(level)
        , m_prefix(prefix)
        , m_id(objId)
        , m_buffer()
        , m_displayId(displayId)
        {}

        TLLog(const TLLog&) = delete;
        TLLog& operator=(TLLog const&) = delete;
        TLLog(TLLog&&) = delete;
        TLLog& operator=(TLLog&&) = delete;
        
        virtual ~TLLog()
        {
            std::stringstream buffer;
            formatLog(buffer);
            TLLogger::instance().AddLine(buffer.str(), m_prefix);
        }
        
        /*!
         * Used to add data to the log item.
         */
        template <typename T>
        TLLog& operator<<(const T& entry)
        {
            m_buffer << entry;
            return *this;
        }

    protected:
        /*!
         * Entry formatting performs here.
         */
        void formatLog(std::stringstream& buffer)
        {
            std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
            auto now_c = std::chrono::system_clock::to_time_t(now);
            auto duration = now.time_since_epoch();
            auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() % 1000;
            char milliseconds_str[4];
            snprintf(milliseconds_str, sizeof(milliseconds_str), "%03lld", milliseconds);

            buffer.width(18);
            buffer << std::this_thread::get_id() << " | ";
            buffer << std::put_time(std::localtime(&now_c), "%m/%d/%T.")  << milliseconds_str << " | ";
            buffer.width(8);
            buffer << std::left << TLLogLevelArr[static_cast<unsigned int>(m_level)] << " | ";
            if (m_displayId)
            {
                buffer.width(6);
                buffer << m_id << " | ";
            }
            buffer.width(0);
            buffer << m_prefix << " | " << m_buffer.str();
        }
        
    protected:
        TLLogLevel m_level;
        std::string m_prefix;
        unsigned long long m_id;
        std::stringstream m_buffer;
        bool m_displayId;
    };

    /*!
     * Helper class which used for using log formatter and loggable objects together.
     *
     * \sa #rtd::TLLog, \sa #rtd::TLLoggable
     */
    template <typename T>
    class TLLogLineHelper
        : public TLLog
    {
    public:
        TLLogLineHelper(T * p, TLLogLevel level = TLLogLevel::Debug)
            : TLLog( ( (std::is_convertible<T, const TLLoggable>::value) ? dynamic_cast<const TLLoggable*>(p)->Prefix() : "")
                    ,( (std::is_convertible<T, const TLLoggable>::value) ? dynamic_cast<const TLLoggable*>(p)->Id() : 0)
                    , level, true)
        {}

        TLLogLineHelper(const TLLogLineHelper&) = delete;
        TLLogLineHelper& operator=(TLLogLineHelper const&) = delete;
        TLLogLineHelper(TLLog&&) = delete;
        TLLogLineHelper& operator=(TLLogLineHelper&&) = delete;
        virtual ~TLLogLineHelper() = default;

        /*!
         * Used for creating log item with default prefix or custom prefix.
         */
        TLLog& operator()(const std::string& prefix = "")
        {
            if(!prefix.empty())
                m_prefix = prefix;

            return *this;
        }
    };

    /*!
     * Helper class wich used for using log formatter inside plain functions and non-polymorfic classes.
     */
    class TLPlainLogLineHelper
        : public TLLog
    {
    public:
        TLPlainLogLineHelper(TLLogLevel level = TLLogLevel::Debug)
            : TLLog("", 0, level, false)
        {}

        TLPlainLogLineHelper(const TLPlainLogLineHelper&) = delete;
        TLPlainLogLineHelper& operator=(TLPlainLogLineHelper const&) = delete;
        TLPlainLogLineHelper(TLLog&&) = delete;
        TLPlainLogLineHelper& operator=(TLPlainLogLineHelper&&) = delete;
        virtual ~TLPlainLogLineHelper() = default;

        /*!
         * Used to create log item with empty prefix or custom prefix.
         */
        TLLog& operator()(const std::string& prefix = "")
        {
            if(!prefix.empty())
                m_prefix = prefix;

            return *this;
        }
    };

    /*!
     * Helper class which is used to switch off logging.
     */
    class TLLogLineHollow
    {
    public:
        TLLogLineHollow() = default;
        TLLogLineHollow(const TLLogLineHollow&) = delete;
        TLLogLineHollow& operator=(TLLogLineHollow const&) = delete;
        TLLogLineHollow(TLLog&&) = delete;
        TLLogLineHollow& operator=(TLLogLineHollow&&) = delete;
        virtual ~TLLogLineHollow() = default;

        TLLogLineHollow& operator()(const std::string& prefix = "")
        {
            return *this;
        }

        template <typename T>
        TLLogLineHollow& operator<<(const T&)
        {
            return *this;
        }
    };
}

#ifdef RTD_ENABLE_LOGGING

/**
 * \def LOG
 * Macroses for logging inside classes inherited from TLLoggable.
 *
 * Usage example: LOG() << "Some log info"; //Formatted item will contein prefix and id according the loggable object.
 * Usage example: LOG("Custom prefix") << "Some log info";/ /Formatted item will contein "Custom prefix" as the prefix and id according the loggable object.
 * \sa #rtd::TLLog::TLLogLevel::Info
*/
/**
 * \def LOG_D
 * Macroses for logging inside classes inherited from TLLoggable.
 *
 * Usage example: LOG_D() << "Some log info"; //Formatted item will contein prefix and id according the loggable object.
 * Usage example: LOG_D("Custom prefix") << "Some log info";/ /Formatted item will contein "Custom prefix" as the prefix and id according the loggable object.
 * \sa #rtd::TLLog::TLLogLevel::Debug
 */
/**
 * \def LOG_W
 * Macroses for logging inside classes inherited from TLLoggable.
 *
 * Usage example: LOG_W() << "Some log info"; //Formatted item will contein prefix and id according the loggable object.
 * Usage example: LOG_W("Custom prefix") << "Some log info";/ /Formatted item will contein "Custom prefix" as the prefix and id according the loggable object.
 * \sa #rtd::TLLog::TLLogLevel::Warning
 */
/**
 * \def LOG_C
 * Macroses for logging inside classes inherited from TLLoggable.
 *
 * Usage example: LOG_C() << "Some log info"; //Formatted item will contein prefix and id according the loggable object.
 * Usage example: LOG_C("Custom prefix") << "Some log info";/ /Formatted item will contein "Custom prefix" as the prefix and id according the loggable object.
 * \sa #rtd::TLLog::TLLogLevel::Critical
 */
/**
 * \def LOG_F
 * Macroses for logging inside classes inherited from TLLoggable.
 *
 * Usage example: LOG_F() << "Some log info"; //Formatted item will contein prefix and id according the loggable object.
 * Usage example: LOG_F("Custom prefix") << "Some log info";/ /Formatted item will contein "Custom prefix" as the prefix and id according the loggable object.
 * \sa #rtd::TLLog::TLLogLevel::Fatal
 */

#define LOG   rtd::TLLogLineHelper<std::pointer_traits<decltype(this)>::element_type >(this, rtd::TLLog::TLLogLevel::Info)
#define LOG_D rtd::TLLogLineHelper<std::pointer_traits<decltype(this)>::element_type >(this, rtd::TLLog::TLLogLevel::Debug)
#define LOG_W rtd::TLLogLineHelper<std::pointer_traits<decltype(this)>::element_type >(this, rtd::TLLog::TLLogLevel::Warning)
#define LOG_C rtd::TLLogLineHelper<std::pointer_traits<decltype(this)>::element_type >(this, rtd::TLLog::TLLogLevel::Critical)
#define LOG_F rtd::TLLogLineHelper<std::pointer_traits<decltype(this)>::element_type >(this, rtd::TLLog::TLLogLevel::Fatal)

/*!
 * \def LOGP
 * Macroses for logging inside plain functions and non-polymorphic classes.
 *
 * Usage example: LOGP() << "Some log info"; //Formatted item will contein empty prefix and zero id.
 * Usage example: LOGP("Custom prefix") << "Some log info";/ /Formatted item will contein "Custom prefix" as the prefix and zero id.
 * \sa #rtd::TLLog::TLLogLevel::Info
 */
/*!
 * \def LOGP_D
 * Macroses for logging inside plain functions and non-polymorphic classes.
 *
 * Usage example: LOGP_D() << "Some log info"; //Formatted item will contein empty prefix and zero id.
 * Usage example: LOGP_D("Custom prefix") << "Some log info";/ /Formatted item will contein "Custom prefix" as the prefix and zero id.
 * \sa #rtd::TLLog::TLLogLevel::Debug
 */
/*!
 * \def LOGP_W
 * Macroses for logging inside plain functions and non-polymorphic classes.
 *
 * Usage example: LOGP_W() << "Some log info"; //Formatted item will contein empty prefix and zero id.
 * Usage example: LOGP_W("Custom prefix") << "Some log info";/ /Formatted item will contein "Custom prefix" as the prefix and zero id.
 * \sa #rtd::TLLog::TLLogLevel::Warning
 */
/*!
 * \def LOGP_C
 * Macroses for logging inside plain functions and non-polymorphic classes.
 *
 * Usage example: LOGP_C() << "Some log info"; //Formatted item will contein empty prefix and zero id.
 * Usage example: LOGP_C("Custom prefix") << "Some log info";/ /Formatted item will contein "Custom prefix" as the prefix and zero id.
 * \sa #rtd::TLLog::TLLogLevel::Critical
 */
/*!
 * \def LOGP_F
 * Macroses for logging inside plain functions and non-polymorphic classes.
 *
 * Usage example: LOGP_F() << "Some log info"; //Formatted item will contein empty prefix and zero id.
 * Usage example: LOGP_F("Custom prefix") << "Some log info";/ /Formatted item will contein "Custom prefix" as the prefix and zero id.
 * \sa #rtd::TLLog::TLLogLevel::Fatal
 */

#define LOGP   rtd::TLPlainLogLineHelper(rtd::TLLog::TLLogLevel::Info)
#define LOGP_D rtd::TLPlainLogLineHelper(rtd::TLLog::TLLogLevel::Debug)
#define LOGP_W rtd::TLPlainLogLineHelper(rtd::TLLog::TLLogLevel::Warning)
#define LOGP_C rtd::TLPlainLogLineHelper(rtd::TLLog::TLLogLevel::Critical)
#define LOGP_F rtd::TLPlainLogLineHelper(rtd::TLLog::TLLogLevel::Fatal)

#else

/*!
 \def LOG
 * Macroses to disable logging.
 *
 * All log data will be lost.
 */

#define LOG   rtd::TLLogLineHollow()
#define LOG_D rtd::TLLogLineHollow()
#define LOG_W rtd::TLLogLineHollow()
#define LOG_C rtd::TLLogLineHollow()
#define LOG_F rtd::TLLogLineHollow()
#define LOGP   rtd::TLLogLineHollow()
#define LOGP_D rtd::TLLogLineHollow()
#define LOGP_W rtd::TLLogLineHollow()
#define LOGP_C rtd::TLLogLineHollow()
#define LOGP_F rtd::TLLogLineHollow()

#endif

#endif /* defined(__RTDNotificationLib__TLLogger__) */
