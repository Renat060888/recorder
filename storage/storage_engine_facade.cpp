
#include <microservice_common/system/logger.h>

#include "system/config_reader.h"
#include "storage_engine_facade.h"

using namespace std;
using namespace common_types;

static constexpr const char * PRINT_HEADER = "StorageEngineFacade:";

StorageEngineFacade::StorageEngineFacade()
    : m_trMaintenance(nullptr)
    , m_shutdownCalled(false)
{

}

StorageEngineFacade::~StorageEngineFacade()
{
    shutdown();
}

bool StorageEngineFacade::init( const SInitSettings & _settings ){

    _settings.services.ObjectListeningService->addObserver( this );







    m_trMaintenance = new std::thread( & StorageEngineFacade::threadMaintenance, this );

    VS_LOG_INFO << PRINT_HEADER << " init success" << endl;
    return true;
}

void StorageEngineFacade::shutdown(){

    if( ! m_shutdownCalled ){
        m_shutdownCalled = true;

        VS_LOG_INFO << PRINT_HEADER << " begin shutdown" << endl;








        VS_LOG_INFO << PRINT_HEADER << " shutdown success" << endl;
    }
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
        Context::SInitSettings settings;
        settings.ctxId = _obj.ctxId;
        settings.missionId = _obj.missionId;
        settings.quantumIntervalMilllisec = CONFIG_PARAMS.RECORDING_QUANTUM_TIME_MILLISEC;

        PContext ctx = std::make_shared<Context>();
        if( ctx->init( settings ) ){
            m_contexts.push_back( ctx );
            m_contextById.insert( {_obj.ctxId, ctx} );
        }

        _obj.accept( (IListenedObjectVisitor *)ctx.get() );
    }
    m_muContexts.unlock();
}


















