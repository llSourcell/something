#ifndef h_LoggerInitializer
#define h_LoggerInitializer

#include "ITDLogger.h"

#include <Poco/Logger.h>
#include <Poco/PatternFormatter.h>
#include <Poco/FormattingChannel.h>
#include <Poco/FileChannel.h>
#include <Poco/Message.h>

#define RTD_LOGGER "RTDLogger"
#define LOGGER_FILE "rtd.log"

namespace rtd
{

bool ITDLogger::_logToConsole = false;
std::set<std::string> ITDLogger::m_filter = std::set<std::string>();

static Poco::Logger* gLoggerPocoBackend = nullptr;
static Poco::FormattingChannel* gLoggerFileChannel;

void ITDLogger::initialize(bool logToConsole, const char** filter, unsigned int filter_size, const char* filename)
{

#if (TARGET_OS_IPHONE != 1) && (TARGET_IPHONE_SIMULATOR != 1)
    gLoggerFileChannel = new Poco::FormattingChannel(new Poco::PatternFormatter("%t"));
    gLoggerFileChannel->setChannel(new Poco::FileChannel(nullptr == filename ? LOGGER_FILE : filename));
    gLoggerFileChannel->open();
    
    gLoggerPocoBackend = &Poco::Logger::create(RTD_LOGGER, gLoggerFileChannel, Poco::Message::PRIO_DEBUG);
#endif

    _logToConsole = logToConsole;

    for(auto i = 0; i < filter_size; ++i)
        m_filter.insert(filter[i]);

    static ITDLogger instance;
    TLLogger::instance().SetLogListener(&instance);
}

void ITDLogger::shutdown()
{
    TLLogger::instance().SetLogListener(nullptr);
    gLoggerPocoBackend->shutdown();
    gLoggerPocoBackend = nullptr;
}

Poco::Logger* ITDLogger::instance()
{
    return gLoggerPocoBackend;
}

bool ITDLogger::enabled()
{
    return gLoggerPocoBackend != nullptr;
}

void ITDLogger::onNewEntry(const char* c, const char* prefix)
{
    if(m_filter.size() > 0 && m_filter.find(prefix) == m_filter.end())
        return;

    if (_logToConsole)
    {
        static std::mutex mutex;
        std::lock_guard<std::mutex> lock(mutex);
        std::cout << c << std::endl;
    }

    if (ITDLogger::enabled())
    {
        ITDLogger::instance()->information(c);
    }
}

TLLogger& TLLogger::instance()
{
    static TLLogger logger;
    return logger;
}

void TLLogger::AddLine(const std::string& line, const std::string& prefix)
{
    if(m_listener)
    {
        m_listener->onNewEntry(line.c_str(), prefix.c_str());
    }
    else
    {
        static std::mutex mutex;
        std::lock_guard<std::mutex> lock(mutex);
        std::cout << line << std::endl;
    }
}

}

#endif
