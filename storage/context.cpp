
#include <thread>

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
            common_types::SPersistenceMetadataRaw persistenceFromRaw = meta.persistenceFromRaw.front();
            persistenceFromRaw.lastRecordedSession++;

            m_persId = m_database->writePersistenceSetMetadata( persistenceFromRaw );
            assert( m_persId != common_types::SPersistenceMetadataDescr::INVALID_PERSISTENCE_ID );
        }
        // make changes in metadata section of database (1st case)
        else{
            createNewMetadata( _settings );
        }
    }
    // make changes in metadata section of database (2nd case)
    else{
        createNewMetadata( _settings );
    }

    return true;
}

void Context::createNewMetadata( const SInitSettings & _settings ){

    common_types::SPersistenceMetadataRaw rawMetadata;
    rawMetadata.contextId = _settings.ctxId;
    rawMetadata.missionId = _settings.missionId;
    rawMetadata.lastRecordedSession = 1;
    rawMetadata.sourceType = common_types::EPersistenceSourceType::AUTONOMOUS_RECORDER;
    rawMetadata.timeStepIntervalMillisec = _settings.updateIntervalMilllisec;

    m_persId = m_database->writePersistenceSetMetadata( rawMetadata );
    assert( m_persId != common_types::SPersistenceMetadataDescr::INVALID_PERSISTENCE_ID );
}

void Context::runSystemClock(){

    VS_LOG_INFO << "runSystemClock() - thread id: " << std::this_thread::get_id() << endl;


    // TODO: dump accumulated objects every N milliseconds

    m_muAccumulatedObjects.lock();

    for( const common_types::SListenedTrajectory & traj : m_accumulatedTrajObjects ){

    }

    vector<common_types::SPersistenceTrajectory> data;
    m_database->writeTrajectoryData( m_persId, data );

    m_muAccumulatedObjects.unlock();
}

void Context::visit( const common_types::SListenedTrajectory * _listenObj ){

    VS_LOG_INFO << "visit() - thread id: " << std::this_thread::get_id() << endl;

    VS_LOG_INFO << "listen traj object: " << _listenObj->objId << endl;



    // TODO: accumulate detected objects

    m_muAccumulatedObjects.lock();
    m_accumulatedTrajObjects.push_back( (* _listenObj) );
    m_muAccumulatedObjects.unlock();

}

void Context::visit( const common_types::SListenedWeather * _listenObj ){

    VS_LOG_INFO << "listen weather object: " << _listenObj->objId << endl;


    // TODO: accumulate detected objects



}
