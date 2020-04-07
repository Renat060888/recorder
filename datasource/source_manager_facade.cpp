
#ifdef OBJREPR_LIBRARY_EXIST
#include <objrepr/reprServer.h>
#endif

#include <microservice_common/system/logger.h>
#include <microservice_common/system/objrepr_bus.h>

#include "system/config_reader.h"
#include "source_manager_facade.h"
#include "object_listener_imitator.h"
#include "object_listener_mqtt.h"
#include "object_listener_objrepr.h"

using namespace std;
using namespace common_types;

static constexpr const char * PRINT_HEADER = "SourceManagerFacade:";

SourceManagerFacade::SourceManagerFacade()
    : m_shutdownCalled(false)
    , m_trMaintenance(nullptr)
{

}

SourceManagerFacade::~SourceManagerFacade()
{
    shutdown();
}

bool SourceManagerFacade::init( const SInitSettings & _settings ){

    // I'm begins to programming...




    m_trMaintenance = new std::thread( & SourceManagerFacade::threadMaintenance, this );

    VS_LOG_INFO << PRINT_HEADER << " init success" << endl;
    return true;
}

void SourceManagerFacade::shutdown(){

    if( ! m_shutdownCalled ){
        m_shutdownCalled = true;

        VS_LOG_INFO << PRINT_HEADER << " begin shutdown" << endl;

        common_utils::threadShutdown( m_trMaintenance );



        VS_LOG_INFO << PRINT_HEADER << " shutdown success" << endl;
    }
}

void SourceManagerFacade::threadMaintenance(){

    VS_LOG_INFO << PRINT_HEADER << " start a maintenance THREAD" << endl;

    constexpr int listenIntervalMillisec = 10;

    while( ! m_shutdownCalled ){

        m_muListeners.lock();
        for( IServiceObjectListener * listener : m_listeningServices ){
            listener->runListenCycle();
        }
        m_muListeners.unlock();







        this_thread::sleep_for( chrono::milliseconds(listenIntervalMillisec) );
    }

    VS_LOG_INFO << PRINT_HEADER << " maintenance THREAD is stopped" << endl;
}

bool SourceManagerFacade::startListenContext( const std::string & _ctxName ){

    TContextId contextId = OBJREPR_BUS.getContextIdByName( _ctxName );
    contextId = 777;

    // imitator ( also may be objrepr or mqtt )
    ObjectListenerImitator::SInitSettings settings2;
    settings2.ctxId = contextId;

    ObjectListenerImitator * listener = new ObjectListenerImitator();
    if( ! listener->init(settings2) ){
        return false;
    }

    ((IServiceObjectListener *)listener)->addObserver( this );

    m_muListeners.lock();
    m_listeningServices.push_back( listener );
    m_muListeners.unlock();

    return true;
}

void SourceManagerFacade::stopListenContext( const std::string & _ctxName ){

    TContextId contextId = OBJREPR_BUS.getContextIdByName( _ctxName );
    contextId = 777;

    m_muListeners.lock();
    for( auto iter = m_listeningServices.begin(); iter != m_listeningServices.end(); ){
        IServiceObjectListener * listener = ( * iter );
        if( listener->getListenedContextId() == contextId ){
            delete listener;
            m_listeningServices.erase( iter );
            return;
        }
        else{
            ++iter;
        }
    }
    m_muListeners.unlock();
}

common_types::IServiceObjectListener * SourceManagerFacade::getServiceOfObjectListener(){
    return this;
}

// observer interface
void SourceManagerFacade::callbackObjectDetected( const common_types::SListenedObject & _obj ){

    // forward event to real consumer
    for( IListenedObjectObserver * observer : m_listenedObjectsObservers ){
        observer->callbackObjectDetected( _obj );
    }
}

// listen service interface
void SourceManagerFacade::runListenCycle(){

    assert( false && "dummy method" );
}

common_types::TContextId SourceManagerFacade::getListenedContextId(){

    assert( false && "dummy method" );
}

void SourceManagerFacade::addObserver( common_types::IListenedObjectObserver * _observer ){

    for( const IListenedObjectObserver * const observer : m_listenedObjectsObservers ){
        if( observer == _observer ){
            return;
        }
    }

    m_listenedObjectsObservers.push_back( _observer );
}

void SourceManagerFacade::removeObserver( common_types::IListenedObjectObserver * _observer ){

    for( auto iter = m_listenedObjectsObservers.begin(); iter != m_listenedObjectsObservers.end(); ){
        if( (* iter) == _observer ){
            iter = m_listenedObjectsObservers.erase( iter );
            return;
        }
        else{
            ++iter;
        }
    }
}












