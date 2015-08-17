#ifndef h_ITDSCoreDataFactory
#define h_ITDSCoreDataFactory

#include <Common/ITDConfiguration.h>
#include <Notification/ITNNotificator.h>
#include <Notification/ITNTransportState.h>

#include "ITDSTypes.h"

namespace rtd
{

class ITDSCoreDataFactory
{
public:
    virtual bool initialize(ITNNotificatorPtr notificator, ITNTransportStatePtr transport, ITDConfigurationProviderPtr configProvider) = 0;
    virtual void uninitialize() = 0;
    virtual void setAuthToken(const char* authToken) = 0;

    virtual ITDSCoreDataServicePtr createClient(ITDSCoreDataObserverPtr observer) = 0;

protected:
    virtual ~ITDSCoreDataFactory() = default;
    
public:
    static ITDSCoreDataFactoryPtr create();
};

}

#endif
