
#include <thread>

#include <microservice_common/system/logger.h>
#include <microservice_common/common/ms_common_utils.h>

#include "system/config_reader.h"
#include "context.h"

using namespace std;

static constexpr const char * PRINT_HEADER = "ContextForRecord:";

static void printMetadataInfo( const common_types::SPersistenceMetadataRaw & _meta ){

    VS_LOG_INFO << PRINT_HEADER
                << " begin working with"
                << " ctx id: " << _meta.contextId
                << " mission id: " << _meta.missionId
                << " session num: " << _meta.lastRecordedSession
                << " quantum time (millisec): " << _meta.timeStepIntervalMillisec
                << endl;
}

Context::Context()
    : m_lastDumpAtMillisec(0)
    , m_trajObjectsCounter(0)
    , m_weatherObjectsCounter(0)
    , m_database(nullptr)
    , m_currentSessionNum(0)
    , m_currentLogicTime(-1)
{

}

bool Context::init( const SInitSettings & _settings ){

    // TODO: temporary input validation
    assert( _settings.ctxId > 0 && _settings.missionId >= 0 && _settings.updateIntervalMilllisec > 0 );

    m_settings = _settings;

    // database
    DatabaseManagerBase::SInitSettings settings;
    settings.host = CONFIG_PARAMS.baseParams.MONGO_DB_ADDRESS;
    settings.databaseName = CONFIG_PARAMS.baseParams.MONGO_DB_NAME;

    m_database = DatabaseManagerBase::getInstance();
    if( ! m_database->init(settings) ){
        return false;
    }

    // update metadata
    const std::vector<common_types::SPersistenceMetadata> ctxPersistenceMetadatas = m_database->getPersistenceSetMetadata( _settings.ctxId );

    if( ! ctxPersistenceMetadatas.empty() ){
        assert( ctxPersistenceMetadatas.size() == 1 && "ONE record for the ONE context" );

        // update existing metadata
        const common_types::SPersistenceMetadata & meta = ctxPersistenceMetadatas[ 0 ];
        if( ! meta.persistenceFromRaw.empty() ){
            common_types::SPersistenceMetadataRaw persistenceFromRaw = meta.persistenceFromRaw.front();

            persistenceFromRaw.lastRecordedSession++;

            m_persId = m_database->writePersistenceSetMetadata( persistenceFromRaw );
            assert( m_persId != common_types::SPersistenceMetadataDescr::INVALID_PERSISTENCE_ID );

            m_currentSessionNum = persistenceFromRaw.lastRecordedSession;

            printMetadataInfo( persistenceFromRaw );
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

    m_currentSessionNum = rawMetadata.lastRecordedSession;

    printMetadataInfo( rawMetadata );
}

void Context::runSystemClock(){

    // dump accumulated objects every N milliseconds
    if( (common_utils::getCurrentTimeMillisec() - m_lastDumpAtMillisec) > m_settings.updateIntervalMilllisec ){
        m_lastDumpAtMillisec = common_utils::getCurrentTimeMillisec();

        // increase time despite on object presence
        m_currentLogicTime++;

        m_muAccumulatedObjects.lock();
        if( m_accumulatedTrajObjects.empty() ){
            m_muAccumulatedObjects.unlock();
            return;
        }

        vector<common_types::SPersistenceTrajectory> data;
        data.resize( m_accumulatedTrajObjects.size() );

        for( int i = 0; i < m_accumulatedTrajObjects.size(); i++ ){
            const common_types::SListenedTrajectory & traj = m_accumulatedTrajObjects[ i ];
            data[ i ] = traj.data;
            data[ i ].sessionNum = m_currentSessionNum;
            data[ i ].logicTime = m_currentLogicTime;
        }

        m_database->writeTrajectoryData( m_persId, data );
        m_accumulatedTrajObjects.clear();

        m_muAccumulatedObjects.unlock();
    }
}

void Context::visit( const common_types::SListenedTrajectory * _listenObj ){

    VS_LOG_INFO << "listen traj object: "
                << _listenObj->objId
                << " lat: " << _listenObj->data.latDeg
                << " lon: " << _listenObj->data.lonDeg
                << endl;

    // accumulate detected objects
    m_muAccumulatedObjects.lock();
    m_accumulatedTrajObjects.push_back( (* _listenObj) );
    m_muAccumulatedObjects.unlock();

}

void Context::visit( const common_types::SListenedWeather * _listenObj ){

    VS_LOG_INFO << "listen weather object: " << _listenObj->objId << endl;


    // TODO: accumulate detected objects



}













