
#include <microservice_common/system/logger.h>

#include "context.h"

using namespace std;

Context::Context()
    : m_lastDumpAtMillisec(0)
    , m_trajObjectsCounter(0)
    , m_weatherObjectsCounter(0)
{

}

bool Context::init( const SInitSettings & _settings ){

    m_settings = _settings;

    //


    //
    const std::vector<common_types::SPersistenceMetadata> ctxPersistenceMetadatas = m_database->getPersistenceSetMetadata( _settings.ctxId );

    if( ! ctxPersistenceMetadatas.empty() ){
        assert( ctxPersistenceMetadatas.size() == 1 );

        // update existing metadata
        const common_types::SPersistenceMetadata & meta = ctxPersistenceMetadatas[ 0 ];
        if( ! meta.persistenceFromRaw.empty() ){

        }
        // make changes in metadata section of database (1st case)
        else{

        }
    }
    // make changes in metadata section of database (2nd case)
    else{

    }


    return true;
}

void Context::runSystemClock(){



    // TODO: dump accumulated objects every N milliseconds



}

void Context::visit( const common_types::SListenedTrajectory * _listenObj ){

    VS_LOG_INFO << "listen traj object: " << _listenObj->objId << endl;



    // TODO: accumulate detected objects



}

void Context::visit( const common_types::SListenedWeather * _listenObj ){

    VS_LOG_INFO << "listen weather object: " << _listenObj->objId << endl;


    // TODO: accumulate detected objects



}
