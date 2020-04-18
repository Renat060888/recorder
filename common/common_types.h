#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <string>
#include <functional>

#include <microservice_common/common/ms_common_types.h>
#include <microservice_common/communication/network_interface.h>

// ---------------------------------------------------------------------------
// forwards ( outside of any namespace )
// ---------------------------------------------------------------------------
class SourceManagerFacade;
class StorageEngineFacade;
class SystemEnvironmentFacadeRecorder;
class CommunicationGatewayFacadeRecorder;

namespace common_types{

// ---------------------------------------------------------------------------
// forwards ( inside namespace )
// ---------------------------------------------------------------------------
class IListenedObjectVisitor;

// ---------------------------------------------------------------------------
// global typedefs
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// enums
// ---------------------------------------------------------------------------




// ---------------------------------------------------------------------------
// simple ADT
// ---------------------------------------------------------------------------








// ---------------------------------------------------------------------------
// exchange ADT ( component <-> store, component <-> network, etc... )
// ---------------------------------------------------------------------------
// listened object
struct SListenedObject {
    SListenedObject( TContextId _ctxId, TMissionId _missionId, TObjectId _objId )
        : ctxId(_ctxId)
        , missionId(_missionId)
        , objId(_objId)
    {}
    virtual ~SListenedObject(){}

    virtual void accept( IListenedObjectVisitor * _visitor ) const = 0;

    // TODO: highly likely a bad idea
    TContextId ctxId;
    TMissionId missionId;
    TObjectId objId;
};

struct SListenedTrajectory : SListenedObject {
    SListenedTrajectory( TContextId _ctxId, TMissionId _missionId, TObjectId _objId )
        : SListenedObject(_ctxId, _missionId, _objId){
        data.ctxId = _ctxId;
        data.missionId = _missionId;
        data.objId = _objId;
    }

    virtual void accept( IListenedObjectVisitor * _visitor ) const override;

    SPersistenceTrajectory data;
};

struct SListenedWeather : SListenedObject {
    SListenedWeather( TContextId _ctxId, TMissionId _missionId, TObjectId _objId )
        : SListenedObject(_ctxId, _missionId, _objId){
        data.ctxId = _ctxId;
        data.missionId = _missionId;
        data.objId = _objId;
    }

    virtual void accept( IListenedObjectVisitor * _visitor ) const override;

    SPersistenceWeather data;
};

struct SOnceMoreObject : SListenedObject {

    virtual void accept( IListenedObjectVisitor * _visitor ) const override;


};




// ---------------------------------------------------------------------------
// types deduction
// ---------------------------------------------------------------------------
class IListenedObjectVisitor {
public:
    virtual ~IListenedObjectVisitor(){}

    virtual void visit( const SListenedTrajectory * _listenObj ) = 0;
    virtual void visit( const SListenedWeather * _listenObj ) = 0;
    virtual void visit( const SOnceMoreObject * _listenObj ){}

};




// ---------------------------------------------------------------------------
// service interfaces
// ---------------------------------------------------------------------------

//

class IServiceInternalCommunication {
public:
    virtual ~IServiceInternalCommunication(){}

    virtual PNetworkClient getRecorderWorkerCommunicator( const std::string & _uniqueId ) = 0;
};

class IServiceExternalCommunication {
public:
    virtual ~IServiceExternalCommunication(){}

    virtual PNetworkClient getUserCommunicator( const std::string & _uniqueId ) = 0;
};
//


// source area
class IObjectListeningObserver {
public:
    virtual ~IObjectListeningObserver(){}

    virtual void callbackObjectDetected( const SListenedObject & _obj ) = 0;
};

class IObjectListeningService {
public:
    virtual ~IObjectListeningService(){}

    virtual void addObserver( IObjectListeningObserver * _observer ) = 0;
    virtual void removeObserver( IObjectListeningObserver * _observer ) = 0;
    virtual void runListenCycle() = 0;
    virtual TContextId getListenedContextId() = 0;
    virtual TMissionId getListenedMissionId() = 0;
};


// ---------------------------------------------------------------------------
// service locator
// ---------------------------------------------------------------------------
struct SIncomingCommandServices : SIncomingCommandGlobalServices {
    SIncomingCommandServices()
        : systemEnvironment(nullptr)
        , sourceManager(nullptr)
        , storageEngine(nullptr)
        , communicationGateway(nullptr)
    {}

    SystemEnvironmentFacadeRecorder * systemEnvironment;
    SourceManagerFacade * sourceManager;
    StorageEngineFacade * storageEngine;
    CommunicationGatewayFacadeRecorder * communicationGateway;
    std::function<void()> signalShutdownServer;
};

}

#endif // COMMON_TYPES_H

