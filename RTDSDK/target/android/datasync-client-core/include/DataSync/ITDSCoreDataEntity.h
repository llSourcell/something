#ifndef h_ITDSCoreDataEntity
#define h_ITDSCoreDataEntity

#include "ITDSTypes.h"

#include <string>

namespace rtd
{

class TDSCoreDataEntityImpl;

// Higher level user extended object API.
// Here we expect the developer to inherit from TDSCoreDataEntity and expect teh data to sync

class TDSCoreDataEntity
{
public:
    /**
     * Creates a new entity with empty object payload and synchronizes it to cloud storage.
     * factory contains pointer to ITDSCoreDataFactory factory object
     */
    TDSCoreDataEntity(ITDSCoreDataFactoryPtr factory);
    /**
     * Creates a new entity with pre-populated object payload and synchronizes it to cloud storage.
     * factory contains pointer to ITDSCoreDataFactory factory object
     * objectData contains JSON object serialized to string.
     */
    TDSCoreDataEntity(ITDSCoreDataFactoryPtr factory, const char* objectData);
    /**
     * Creates a new entity with pre-populated object payload, specific purpose and synchronizes it to cloud storage.
     * factory contains pointer to ITDSCoreDataFactory factory object
     * objectData contains JSON object serialized to string.
     * objectPurpose contains string that identifies object purpose for data metainfo, can be used by application.
     */
    TDSCoreDataEntity(ITDSCoreDataFactoryPtr factory, const char* objectData, const char* objectPurpose);
    /**
     * Creates an entity from existing copy and synchronizes it to cloud storage.
     * factory contains pointer to ITDSCoreDataFactory factory object
     * entityUri contains location of existing entity in cloud storage.
     */
    TDSCoreDataEntity(const char* entityUri, ITDSCoreDataFactoryPtr factory);
    virtual ~TDSCoreDataEntity();
    
public:
    /**
     * Retrieves URI that identifies location of the data entity in the cloud.
     */
    std::string getURI();
    /**
     * Retrieves user data contained in the data object.
     * If cloud synchronization is not yet completed, this call will return a locally cached copy instead.
     */
    std::string getData();
    /**
     * Returns true if current data state is "dirty", meaning it is known that it is locally modified
     * and a write operation is pending, making it desynchronized from the cloud copy.
     */
    bool isDirty();
    /**
     * Writes user data to the object.
     * This also triggers data synchronization to update entity stored in the cloud.
     * Provide optional 'contextId' parameter to identify this asynchronous operation, which will be used in onResultObjectUpdated() or onResultErrorOccured() events.
     */
    void setData(const char* objectData, int contextId);
    /**
     * Forces synchronization with object data stored in the cloud. In case the local copy is out of date, it will be overwritten with the new content.
     * Provide optional 'contextId' parameter to identify this asynchronous operation, which will be used in onResultObjectUpdated() or onResultErrorOccured() events.
     */
    void forceSync(int contextId);
    /**
     * Subscribes to remote updates of the object data in the cloud.
     * This will instruct the service to push updates automatically if the object data is modified externally.
     * Provide optional 'contextId' parameter to identify this asynchronous operation, which will be used in onResultSubscriptionUpdated() or onResultErrorOccured() events.
     */
    void subscribeUpdates(const char* channelType, int contextId);
    /**
     * Unsubscribes from remote updates of the object data in the cloud.
     * This will instruct the service to stop pushing updates if the object data is modified externally.
     * Provide optional 'contextId' parameter to identify this asynchronous operation, which will be used in onResultSubscriptionUpdated() or onResultErrorOccured() events.
     */
    void unsubscribeUpdates(int contextId);
    /**
     * Deletes the entity entirely.
     * Removes the object data from local and cloud data store.
     * Provide optional 'contextId' parameter to identify this asynchronous operation, which will be used in onResultEntityDeleted() or onResultErrorOccured() events.
     */
    void deleteEntity(int contextId);
    /**
     * Detaches from entity and releases internal hooks, guarantees that no callbacks are going to be called after the method returns.
     * User should call this before the destructor kicks in. Unsubscription will be automatically executed as well, in case a subscription has been established previously.
     */
    void detach();
    
public:
    /**
     * This notification is called when entity is successfully created or bound to existing resource in cloud storage.
     * 'entityUri' parameter contains URI of the data entity where user object is stored.
     * 'objectData' parameter contains serialized JSON object as null terminated string.
     */
    virtual void onEntityBound(const char* entityUri, const char* objectData);

    /**
     * This notification is called when entity is successfully deleted from cloud storage.
     * 'contextId' parameter contains an identifier of the original operation.
     */
    virtual void onResultEntityDeleted(int contextId);
    /**
     * This notification is called when object data is updated in the cloud storage due to a local (e.g. setData() or forceSync()) operation.
     * 'objectData' parameter contains serialized JSON object as null terminated string.
     * 'contextId' parameter contains an identifier of the original operation.
     */
    virtual void onResultObjectUpdated(int contextId, const char* objectData);
    /**
     * This notification is called when push subscription to remote data update is successfully added or removed.
     * 'contextId' parameter contains an identifier of the original operation.
     * 'subscribed' parameter is set to true: given object is ready to receive push updates when the data is changed in the cloud storage.
     * 'subscribed' parameter is set to false: given object no longer receives changes from the cloud storage.
     */
    virtual void onResultSubscriptionUpdated(int contextId, bool subscribed);
    /**
     * This notification is called whenever an error condition occurs.
     * 'contextId' parameter contains an identifier of the original operation.
     * 'errorCode' parameter contains an error identifier.
     */
    virtual void onResultErrorOccured(int contextId, TDSCoreDataError errorCode);

    /**
     * This notification is called when object data is updated in the cloud storage due to a remote operation, e.g. other service or client writing to the object.
     * 'objectData' parameter contains serialized JSON object as string.
     */
    virtual void onRemoteObjectUpdated(const char* objectData);
    /**
     * This notification is called whenever an error condition occurs.
     * 'errorCode' parameter contains an error identifier.
     */
    virtual void onRemoteErrorOccured(TDSCoreDataError errorCode);
    
private:
    TDSCoreDataEntityImpl* _impl;
};
    
}

#endif
