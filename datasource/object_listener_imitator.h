#ifndef OBJECT_LISTENER_IMITATOR_H
#define OBJECT_LISTENER_IMITATOR_H

#include "common/common_types.h"

class ObjectListenerImitator : public common_types::IObjectListeningService
{
public:
    struct SInitSettings {
        common_types::TContextId ctxId;
        common_types::TMissionId missionId;
    };

    ObjectListenerImitator();

    bool init( const SInitSettings & _settings );



private:
    virtual void addObserver( common_types::IObjectListeningObserver * _observer ) override;
    virtual void removeObserver( common_types::IObjectListeningObserver * _observer ) override;
    virtual void runListenCycle() override;
    virtual common_types::TContextId getListenedContextId() override;
    virtual common_types::TMissionId getListenedMissionId() override;



    // data
    SInitSettings m_settings;
    std::vector<common_types::IObjectListeningObserver *> m_listenedObjectsObservers;
    std::vector<common_types::SListenedTrajectory> m_objectImitations;
    int64_t m_lastActivityAtMillisec;
    int64_t m_lastStepAtMillisec;
    bool m_stepsPassed;
    int32_t m_currentStepNum;





};

#endif // OBJECT_LISTENER_IMITATOR_H
