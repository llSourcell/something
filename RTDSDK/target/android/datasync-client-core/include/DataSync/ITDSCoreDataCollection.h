#ifndef h_ITDSCoreDataCollection
#define h_ITDSCoreDataCollection

#include "ITDSTypes.h"

#include <string>
#include <list>

namespace rtd
{

class TDSCoreDataEventStreamImpl;

class TDSCoreDataEventStream
{
public:
    /**
     * Creates a new event stream collection with zero events and synchronizes it to cloud storage.
     * factory contains pointer to ITDSCoreDataFactory factory object
     */
    TDSCoreDataEventStream(ITDSCoreDataFactoryPtr factory);
    /**
     * Creates a new event stream collection with zero events, specific purpose and synchronizes it to cloud storage.
     * factory contains pointer to ITDSCoreDataFactory factory object
     * objectPurpose contains string that identifies object purpose for data metainfo, can be used by application.
     */
    TDSCoreDataEventStream(ITDSCoreDataFactoryPtr factory, const char* objectPurpose);
    /**
     * Creates a data event stream collection from existing resource and synchronizes it to cloud storage.
     * factory contains pointer to ITDSCoreDataFactory factory object
     * collectionUri contains location of existing event stream in cloud storage.
     */
    TDSCoreDataEventStream(const char* collectionUri, ITDSCoreDataFactoryPtr factory);

    virtual ~TDSCoreDataEventStream();
   
public:
    /**
     * Forces synchronization with entity data stored in the cloud. In case the local copy is out of date, it will be overwritten with the new content.
     * Provide optional 'contextId' parameter to identify this asynchronous operation, which will be used in onResultEntityUpdated() or onResultErrorOccured() callbacks.
     */
    void forceSync(int contextId, TDSCoreDataIndex idx);
    /**
     * Subscribes to remote updates of the collection data in the cloud.
     * This will instruct the service to push updates automatically if the collection data is modified externally.
     * Provide optional 'contextId' parameter to identify this asynchronous operation, which will be used in onResultSubscriptionUpdated() or onResultErrorOccured() callbacks.
     */
    void subscribeUpdates(int contextId);
    /**
     * Unsubscribes from remote updates of the collection data in the cloud.
     * This will instruct the service to stop pushing updates if the entity data is added, removed or modified externally.
     * Provide optional 'contextId' parameter to identify this asynchronous operation, which will be used in onResultSubscriptionUpdated() or onResultErrorOccured() callbacks.
     */
    void unsubscribeUpdates(int contextId);
    /**
     * Deletes the collection entirely.
     * Removes the collection from local and cloud data store.
     * Provide optional 'contextId' parameter to identify this asynchronous operation, which will be used in onResultCollectionDeleted() or onResultErrorOccured() callbacks.
     */
    void deleteCollection(int contextId);

    /**
     * Retrieves URI that identifies location of the collection in the cloud.
     */
    std::string getURI();
    /**
     * Retrieves a specific entity from local cache using the known index.
     * 'idx' parameter specifies the index of the entity to be retrieved.
     * Returns true if entity data is filled successfully.
     * Returns false if entity with specified index is not cached.
     */
    bool getEntity(TDSCoreDataIndex idx, std::string& entityData);
    /**
     * Appends new empty entity to the event stream collection.
     * Provide optional 'contextId' parameter to identify this asynchronous operation, which will be used in onResultEntityAdded() or onResultErrorOccured() callbacks.
     */
    void addEntity(int contextId);
    /**
     * Appends new pre-populated entity from provided JSON string.
     * Provide optional 'contextId' parameter to identify this asynchronous operation, which will be used in onResultEntityAdded() or onResultErrorOccured() callbacks.
     */
    void addEntity(const char* entityData, int contextId);
    /**
     * Writes user data to an existing entity in the event stream collection.
     * This also triggers data synchronization to update entity stored in the cloud.
     * 'entityData' parameter contains JSON object as a null terminated string.
     * Provide optional 'contextId' parameter to identify this asynchronous operation, which will be used in onResultEntityUpdated() or onResultErrorOccured() callbacks.
     */
    void setEntity(TDSCoreDataIndex idx, const char* entityData, int contextId);
    /**
     * Removes the entity from the event stream collection given the known index.
     * Provide optional 'contextId' parameter to identify this asynchronous operation, which will be used in onResultEntityRemoved() or onResultErrorOccured() callbacks.
     */
    void removeEntity(TDSCoreDataIndex idx, int contextId);
    /**
     * Retrieves a number of entities from the event stream located in ordering before the given entity (original insertion order is maintained).
     * 'idx' parameter specifies the index of an anchor entity up until which the entities are returned.
     * 'count' parameter specifies the maximum number of entities to be returned. In case given collection contains less items than specifed, only the available ones will be returned.
     * Provide optional 'contextId' parameter to identify this asynchronous operation, which will be used in onResultQueryCompleted() or onResultErrorOccured() callbacks.
     */
    void queryEntitiesBefore(TDSCoreDataIndex idx, int count, TDSCollectionOrder order, int contextId);
    /**
     * Retrieves a number of entities from the stream located in ordering after the given entity (original insertion order is maintained).
     * 'idx' parameter specifies the index of an anchor entity down from which the entities are returned.
     * 'count' parameter specifies the maximum number of entities to be returned. In case given collection contains less items than specifed, only the available ones will be returned.
     * Provide optional 'contextId' parameter to identify this asynchronous operation, which will be used in onResultQueryCompleted() or onResultErrorOccured() callbacks.
     */
    void queryEntitiesAfter(TDSCoreDataIndex idx, int count, TDSCollectionOrder order, int contextId);
    /**
     * Retrieves a number of entities located at the end of the stream in their original insertion order.
     * 'count' parameter specifies the maximum number of entities to be returned. In case given collection contains less items than specifed, only the available ones will be returned.
     * Provide optional 'contextId' parameter to identify this asynchronous operation, which will be used in onResultQueryCompleted() or onResultErrorOccured() callbacks.
     */
    void queryLastEntities(int count, TDSCollectionOrder order, int contextId);
    /**
     * Detaches from collection and releases internal hooks, guarantees that no callbacks are going to be called after the method returns.
     * User should call this before the destructor kicks in. Unsubscription will be automatically executed as well, in case a subscription has been established previously.
     */
    void detach();
    
public:
    /**
     * This notification is called when collection is successfully created or bound to existing resource in cloud storage.
     * 'collectionUri' parameter contains URI of the event stream where user objects are stored.
     */
    virtual void onCollectionBound(const std::string& collectionUri);

    /**
     * This notification is called when a new event is successuflly added to event stream collection as a result of addEntity() call.
     * 'contextId' parameter contains an identifier of the original operation.
     */
    virtual void onResultEntityAdded(int contextId, TDSCoreDataIndex idx);
    /**
     * This notification is called when an existing entity is successfully deleted from event stream collection as a result of removeEntity() call.
     * 'contextId' parameter contains an identifier of the original operation.
     */
    virtual void onResultEntityRemoved(int contextId, TDSCoreDataIndex idx);
    /**
     * This notification is called when entity data is updated in the cloud storage due to a local operation, i.e. pushChanges() or forceSync().
     * 'contextId' parameter contains an identifier of the original operation.
     */
    virtual void onResultEntityUpdated(int contextId, TDSCoreDataIndex idx);
    /**
     * This notification is called when push subscription to remote data update is successfully added or removed.
     * 'contextId' parameter contains an identifier of the original operation.
     * 'subscribed' parameter is set to true: given collection is ready to receive push updates when the data is changed in the cloud storage.
     * 'subscribed' parameter is set to false: given collection no longer receives changes from the cloud storage.
     */
    virtual void onResultSubscriptionUpdated(int contextId, bool subscribed);
    /**
     * This notification is called when collection is successfully deleted from cloud storage.
     * 'contextId' parameter contains an identifier of the original operation.
     */
    virtual void onResultCollectionDeleted(int contextId);
    /**
     * This notification is called when entities are retrieved from the stream as a result of a query operation.
     * The entity objects are also stored in local cache for quick access later.
     */
    virtual void onResultQueryCompleted(int contextId, const std::list<TDSCoreDataIndex>& entities);
    /**
     * This notification is called whenever an error condition occurs.
     * 'contextId' parameter contains an identifier of the original operation.
     * 'errorCode' parameter contains an error identifier.
     */
    virtual void onResultErrorOccured(int contextId, TDSCoreDataError errorCode);
    
    /**
     * This notification is called when a nested key is added to the given entity of the event stream collection, resulting from a remote operation -- such as service or another client update.
     */
    virtual void onRemoteEntityAdded(TDSCoreDataIndex idx, const char* entityData);
    /**
     * This notification is called when a nested key is removed from the given entity of the event stream collection, resulting from a remote operation -- such as service or another client update.
     */
    virtual void onRemoteEntityRemoved(TDSCoreDataIndex idx);
    /**
     * This notification is called when a nested key is modified in the given entity of the event stream collection, resulting from a remote operation -- such as service or another client update.
     */
    virtual void onRemoteEntityUpdated(TDSCoreDataIndex idx, const char* entityData);
    /**
     * This notification is called whenever an error condition occurs.
     * 'errorCode' parameter contains an error identifier.
     */
    virtual void onRemoteErrorOccured(TDSCoreDataError errorCode);
    
private:
    TDSCoreDataEventStreamImpl* _impl;
};

}

#endif
