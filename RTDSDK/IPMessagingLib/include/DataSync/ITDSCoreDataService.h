#ifndef h_ITDSCoreDataService
#define h_ITDSCoreDataService

#include "ITDSTypes.h"

#include <list>

namespace rtd
{
    
/**
 * Interface of the Core Data Service, acts like a data synchronization client and performs CRUD-like operations on data entities.
 * User request ID must be passed to all methods since they are asynchronous. Request ID is persisted and returned via callback
 * method on ITDSCoreDataObserver.
 */
 
class ITDSCoreDataService
{
public:
    /**
     * Creates data entity with empty payload.
     */
    virtual void createEntity(TDSRequestId& requestId) = 0;
    /**
     * Creates data entity with specific payload. Data must contain a serialized JSON object as string.
     */
    virtual void createEntity(TDSRequestId& requestId, const std::string& entityData) = 0;
    /**
     * Creates data entity with specific payload and purpose. Data must contain a serialized JSON object as string. Purpose is just a string, to be injected into metadata.
     */
    virtual void createEntity(TDSRequestId& requestId, const std::string& entityData, const std::string& entityPurpose) = 0;
    /**
     * Deletes data entity with given URI.
     */
    virtual void deleteEntity(TDSRequestId& requestId, const std::string& entityUri) = 0;
    /**
     * Deletes data entity with given URI and expected consistency tag.
     * This call deletes the specified data entity only if the provided consistency tag matches the one currently in data store.
     */
    virtual void deleteEntity(TDSRequestId& requestId, const std::string& entityUri, const std::string& entityTag) = 0;
    /**
     * Updates data entity payload with given URI.
     * This call overwrites any previous payload of given data entity.
     */
    virtual void updateEntity(TDSRequestId& requestId, const std::string& entityUri, const std::string& entityData) = 0;
    /**
     * Updates data entity payload with given URI and consistency tag.
     * This call overwrites previous payload of specified data entity only if the provided consistency tag matches the one currently in data store.
     */
    virtual void updateEntity(TDSRequestId& requestId, const std::string& entityUri, const std::string& entityData, const std::string& entityTag) = 0;
    /**
     * Retrieves payload of data entity with given URI.
     */
    virtual void readEntity(TDSRequestId& requestId, const std::string& entityUri) = 0;

    /**
     *
     */
    virtual void createCollection(TDSRequestId& requestId, TDSCollectionType type) = 0;
    virtual void createCollection(TDSRequestId& requestId, TDSCollectionType type, const std::string& purpose) = 0;
    virtual void bindCollection(TDSRequestId& requestId, TDSCollectionType type, const std::string& collectionUri) = 0;
    /**
     *
     */
    virtual void deleteCollection(TDSRequestId& requestId, const std::string& collectionUri) = 0;
    /**
     *
     */
    virtual void appendCollection(TDSRequestId& requestId, TDSCollectionType type, const std::string& collectionEntitiesUri) = 0;
    virtual void appendCollection(TDSRequestId& requestId, TDSCollectionType type, const std::string& collectionEntitiesUri, const std::string& entityData) = 0;

    /**
     *
     */
    virtual void queryCollectionIndex(TDSRequestId& requestId, TDSCollectionType type, const std::string& collectionEntitiesUri, TDSCoreDataIndex entityId) = 0;
    virtual void queryCollectionBefore(TDSRequestId& requestId, TDSCollectionType type, const std::string& collectionEntitiesUri, TDSCoreDataIndex entityId, int count, TDSCollectionOrder order) = 0;
    virtual void queryCollectionAfter(TDSRequestId& requestId, TDSCollectionType type, const std::string& collectionEntitiesUri, TDSCoreDataIndex entityId, int count, TDSCollectionOrder order) = 0;
    virtual void queryCollectionLast(TDSRequestId& requestId, TDSCollectionType type, const std::string& collectionEntitiesUri, int count, TDSCollectionOrder order) = 0;

    /**
     * Subscribes to modifications of data entity with given URI.
     * In case the entity is modified by any client or service calls, the change shall be propagated by the service back to the subscribed client.
     * Set 'channelType' parameter to reflect the desired channel for push notification. Supported types are 'twilsock', 'apn'
     */
    virtual void createSubscription(TDSRequestId& requestId, const std::string& baseUri, const std::string& channelType) = 0;
    /**
     * Unsubscribes from modifications of data entity with given subscription URI.
     * This removes a data subscription created by createEntitySubscription() and URI returned by onEntitySubscribed().
     */
    virtual void removeSubscription(TDSRequestId& requestId, const std::string& baseUri, const std::string& subscriptionUri) = 0;
    
protected:
    virtual ~ITDSCoreDataService() = default;
};

/**
 * Interface of the Core Data Service observer, receives client events as response to asynchronous operations on ITDSoreDataService.
 */
    
class ITDSCoreDataObserver
{
public:
    /**
     *
     */
    virtual void onEntityCreated(TDSRequestId requestId, const std::string& entityUri, const std::string& entityData, const std::string& entityTag) = 0;
    /**
     *
     */
    virtual void onEntityDeleted(TDSRequestId requestId) = 0;
    /**
     *
     */
    virtual void onEntityUpdated(TDSRequestId requestId, const std::string& entityUri, const std::string& entityData, const std::string& entityTag) = 0;
    /**
     *
     */
    virtual void onEntityRead(TDSRequestId requestId, const std::string& entityUri, const std::string& entityData, const std::string& entityTag) = 0;
    /**
     *
     */
    virtual void onEntityRemotelyModified(const std::string& entityUri, const std::string& entityData, const std::string& entityTag) = 0;
    /**
     *
     */
    virtual void onErrorOccured(TDSRequestId requestId, TDSCoreDataError errorCode) = 0;

    /**
     *
     */
    virtual void onCollectionCreated(TDSRequestId requestId, const std::string& collectionUri, const std::string& collectionEntitiesUri, const std::string& collectionEventsUri) = 0;
    /**
     *
     */
    virtual void onCollectionBound(TDSRequestId requestId, const std::string& collectionUri, const std::string& collectionEntitiesUri, const std::string& collectionEventsUri) = 0;
    /**
     *
     */
    virtual void onCollectionDeleted(TDSRequestId requestId) = 0;
    /**
     *
     */
    virtual void onCollectionEntityAdded(TDSRequestId requestId, const std::string& collectionEntitiesUri, TDSCoreDataIndex entityIdx, const std::string& entityUri) = 0;
    /**
     *
     */
    virtual void onCollectionEntitiesQueried(TDSRequestId requestId, const std::string& collectionEntitiesUri, const std::list<TDSCollectionQueryResultPtr>& queryResult) = 0;
    /**
     *
     */
    virtual void onCollectionEntityRemotelyAdded(const std::string& collectionUri, TDSCoreDataIndex entityIdx, const std::string& entityUri, const std::string& entityData) = 0;
    /**
     *
     */
    virtual void onCollectionEntityRemotelyRemoved(const std::string& collectionUri, TDSCoreDataIndex entityIdx, const std::string& entityUri) = 0;
    /**
     *
     */
    virtual void onCollectionEntityRemotelyUpdated(const std::string& collectionUri, TDSCoreDataIndex entityIdx, const std::string& entityUri, const std::string& entityData) = 0;

    /**
     *
     */
    virtual void onSubscriptionCreated(TDSRequestId requestId, const std::string& subscriptionUri) = 0;
    /**
     *
     */
    virtual void onSubscriptionRemoved(TDSRequestId requestId) = 0;

    virtual ~ITDSCoreDataObserver() = default;
};

}

#endif // !defined(h_ITDSCoreDataService)
