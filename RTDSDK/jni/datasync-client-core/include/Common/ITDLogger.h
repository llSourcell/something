#ifndef h_ITDLogger
#define h_ITDLogger

#include <Poco/Logger.h>
#include <set>
#include <string>
#include "TLLogger.h"

namespace rtd
{

class ITDLogger : public ITLLogListener
{
public:
    //! Initialize logging. Must be called before the RTD notification/datasync libraries are accessed
    static void initialize(bool logToConsole = false, const char** filter = nullptr, unsigned int filter_size = 0, const char* filename = nullptr);
    //! Shutdown logging. Must be called after RTD notification/datasync libraries are fully shut down
    static void shutdown();
    //! Retrieve logger instance. NB! this is not thread safe when shutting down.
    static Poco::Logger* instance();
    //! Returns true/false if logging is enabled/disabled in runtime.
    static bool enabled();
    

    //! Hook for actually writing the string to the logger backend.
    virtual void onNewEntry(const char* c, const char* prefix) override;

private:
    static bool _logToConsole;
    static std::set<std::string> m_filter;
};

}

#endif // h_ITDLogger
