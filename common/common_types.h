#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <string>
#include <functional>

#include <microservice_common/common/ms_common_types.h>

// ---------------------------------------------------------------------------
// forwards
// ---------------------------------------------------------------------------
class SourceManagerFacade;
class StorageEngineFacade;
class SystemEnvironmentFacade;
class CommunicationGatewayFacadeRecorder;


namespace common_types{

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
    virtual ~SListenedObject(){}

    virtual void accept( IListenedObjectVisitor * _visitor ) const = 0;

    TContextId ctxId;
    TMissionId missionId;
    TObjectId objId;
};

struct SListenedTrajectory : SListenedObject {
    SListenedTrajectory()
    {}

    virtual void accept( IListenedObjectVisitor * _visitor ) const override;

    double latDeg;
    double lonDeg;
    double yawDeg;
};

struct SListenedWeather : SListenedObject {
    SListenedWeather()
    {}

    virtual void accept( IListenedObjectVisitor * _visitor ) const override;

    double windSpeed;
    double humidity;
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
class IContextService {
public:
    virtual ~IContextService(){}

    virtual bool open( int _contextId ) = 0;
    virtual bool close( int _contextId ) = 0;
};

class IServiceInternalCommunication {
public:
    virtual ~IServiceInternalCommunication(){}

    virtual PNetworkClient getPlayerWorkerCommunicator( const std::string & _uniqueId ) = 0;
};

class IServiceExternalCommunication {
public:
    virtual ~IServiceExternalCommunication(){}

    virtual PNetworkClient getUserCommunicator( const std::string & _uniqueId ) = 0;
};

class IUserDispatcherObserver {
public:
    virtual ~IUserDispatcherObserver(){}

    virtual void callbackUserOnline( const common_types::TUserId & _id, bool _online ) = 0;
};

class IServiceUserAuthorization {
public:
    virtual ~IServiceUserAuthorization(){}

    virtual bool isRegistered( const TUserId & _id ) = 0;
    virtual void addObserver( IUserDispatcherObserver * _observer ) = 0;
    virtual void removeObserver( IUserDispatcherObserver * _observer ) = 0;
};
//


class IListenedObjectObserver {
public:
    virtual ~IListenedObjectObserver(){}

    virtual void callbackObjectDetected( const SListenedObject & _obj ) = 0;
};

class IServiceObjectListener {
public:
    virtual ~IServiceObjectListener(){}

    virtual void addObserver( IListenedObjectObserver * _observer ) = 0;
    virtual void removeObserver( IListenedObjectObserver * _observer ) = 0;
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

    SystemEnvironmentFacade * systemEnvironment;
    SourceManagerFacade * sourceManager;
    StorageEngineFacade * storageEngine;
    CommunicationGatewayFacadeRecorder * communicationGateway;
    std::function<void()> signalShutdownServer;
};


}

#endif // COMMON_TYPES_H

