#ifndef CONTEXT_H
#define CONTEXT_H

#include "database_manager.h"

class Context : public common_types::IListenedObjectVisitor
{
public:
    Context();


    void runSystemClock();


private:
    virtual void visit( const common_types::SListenedTrajectory * _listenObj ) override;
    virtual void visit( const common_types::SListenedWeather * _listenObj ) override;


    // data
    int64_t m_lastDumpAtMillisec;
    std::vector<common_types::SListenedTrajectory> m_accumulatedTrajObjects;
    int32_t m_trajObjectsCounter;
    std::vector<common_types::SListenedWeather> m_accumulatedWeatherObjects;
    int32_t m_weatherObjectsCounter;

    // service
    DatabaseManager * m_database;




};
using PContext = std::shared_ptr<Context>;

#endif // CONTEXT_H
