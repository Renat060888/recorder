#ifndef CONTEXT_H
#define CONTEXT_H

#include <mutex>

#include <microservice_common/storage/database_manager_base.h>

#include "common/common_types.h"

class Context : public common_types::IListenedObjectVisitor
{
public:
    struct SInitSettings {
        common_types::TContextId ctxId;
        common_types::TMissionId missionId;
        int64_t updateIntervalMilllisec;
    };

    Context();

    bool init( const SInitSettings & _settings );
    void runSystemClock();


private:
    virtual void visit( const common_types::SListenedTrajectory * _listenObj ) override;
    virtual void visit( const common_types::SListenedWeather * _listenObj ) override;

    void createNewMetadata( const SInitSettings & _settings );

    // data
    SInitSettings m_settings;
    int64_t m_lastDumpAtMillisec;
    int32_t m_trajObjectsCounter;
    int32_t m_weatherObjectsCounter;
    std::vector<common_types::SListenedTrajectory> m_accumulatedTrajObjects;
    std::vector<common_types::SListenedWeather> m_accumulatedWeatherObjects;
    common_types::TPersistenceSetId m_persId;

    // service
    DatabaseManagerBase * m_database;
    std::mutex m_muAccumulatedObjects;



};
using PContext = std::shared_ptr<Context>;

#endif // CONTEXT_H
