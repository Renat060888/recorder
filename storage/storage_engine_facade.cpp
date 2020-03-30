
#include <microservice_common/system/logger.h>

#include "storage_engine_facade.h"

using namespace std;
using namespace common_types;

static constexpr const char * PRINT_HEADER = "StorageEngine:";

StorageEngineFacade::StorageEngineFacade()
    : m_trMaintenance(nullptr)
    , m_shutdownCalled(false)
{

}

StorageEngineFacade::~StorageEngineFacade()
{

}

bool StorageEngineFacade::init( const SInitSettings & _settings ){

    _settings.services.serviceObjectListener->addObserver( this );


    m_trMaintenance = new std::thread( & StorageEngineFacade::threadMaintenance, this );

    return true;
}

void StorageEngineFacade::shutdown(){



}

void StorageEngineFacade::threadMaintenance(){

    VS_LOG_INFO << PRINT_HEADER << " start a maintenance THREAD" << endl;

    constexpr int listenIntervalMillisec = 10;

    while( ! m_shutdownCalled ){

        m_muContexts.lock();
        for( PContext & ctx : m_contexts ){
            ctx->runSystemClock();
        }
        m_muContexts.unlock();




        this_thread::sleep_for( chrono::milliseconds(listenIntervalMillisec) );
    }

    VS_LOG_INFO << PRINT_HEADER << " maintenance THREAD is stopped" << endl;
}

void StorageEngineFacade::callbackObjectDetected( const common_types::SListenedObject & _obj ){

    m_muContexts.lock();
    auto iter = m_contextById.find( _obj.ctxId );
    if( iter != m_contextById.end() ){
        PContext & ctx = iter->second;

        _obj.accept( (IListenedObjectVisitor *)ctx.get() );
    }
    else{
        // create context
        PContext ctx = std::make_shared<Context>();

        m_contexts.push_back( ctx );
        m_contextById.insert( {_obj.ctxId, ctx} );

        _obj.accept( (IListenedObjectVisitor *)ctx.get() );
    }
    m_muContexts.unlock();
}


















