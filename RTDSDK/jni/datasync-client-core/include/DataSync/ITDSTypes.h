#ifndef h_ITDSTypes_h
#define h_ITDSTypes_h

#include <memory>
#include <string>

namespace rtd
{

class ITDSCoreDataFactory;
class ITDSCoreDataObserver;
class ITDSCoreDataService;
class TDSCoreDataEntity;

using ITDSCoreDataObserverPtr = std::shared_ptr<ITDSCoreDataObserver>;
using ITDSCoreDataServicePtr = std::shared_ptr<ITDSCoreDataService>;
using ITDSCoreDataFactoryPtr = std::shared_ptr<ITDSCoreDataFactory>;

using TDSRequestId = unsigned int;
using TDSCoreDataIndex = uint64_t; // because Dima said 32-bit is not enough for everyone!

/**
 * Declares known error codes
 */
enum class TDSCoreDataError : int
{
    InternalClientError,
    BadRequest,
    Unauthorized,
    NotFound,
    AccessConflict,
    InternalServerError,
    Unknown,
};

/**
 *
 */
enum class TDSCollectionType
{
    EventStream
};

struct TDSCollectionQueryResult
{
    TDSCoreDataIndex entityId;
    std::string entityUri;
    std::string entityData;
};

using TDSCollectionQueryResultPtr = std::shared_ptr<TDSCollectionQueryResult>;

enum class TDSCollectionOrder
{
    Ascending,
    Descending
};

}

#endif
