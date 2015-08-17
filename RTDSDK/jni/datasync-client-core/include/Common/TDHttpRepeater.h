#ifndef h_RTDDataSyncLib
#define h_RTDDataSyncLib

#include "TLLogger.h"

#include <functional>

#define HTTP_CLIENT_MAX_RETRIES         3
#define HTTP_CLIENT_INITIAL_TIMEOUT     7500

namespace rtd
{

enum class HttpClientResult { Success, Failed, Retry };

inline bool httpClientRetry(const char* name, std::function<HttpClientResult(int timeOutMs)> func)
{
    int timeOutMs = HTTP_CLIENT_INITIAL_TIMEOUT;
    for (int i = 0; i < HTTP_CLIENT_MAX_RETRIES; i++)
    {
        switch (func(timeOutMs))
        {
        case HttpClientResult::Success:
            return true;
        case HttpClientResult::Retry:
            continue;
        case HttpClientResult::Failed:
            LOGP_C(name) << "permanent failure, aborting";
            return false;
        }
        // increment timeout
        timeOutMs *= 2;
    }
    LOGP_C(name) << "retry count exceeded, aborting";
    return false;
}

}

#endif
