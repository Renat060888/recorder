#ifndef OBJECT_LISTENER_IMITATOR_H
#define OBJECT_LISTENER_IMITATOR_H

#include "common/common_types.h"

class ObjectListenerImitator : public common_types::IServiceObjectListener
{
public:
    struct SInitSettings {
        common_types::TContextId ctxId;
    };

    ObjectListenerImitator();

    bool init( const SInitSettings & _settings );



private:
    virtual void addObserver( common_types::IListenedObjectObserver * _observer ) override;
    virtual void removeObserver( common_types::IListenedObjectObserver * _observer ) override;
    virtual void runListenCycle() override;
    virtual common_types::TContextId getListenedContextId() override;



    // data
    SInitSettings m_settings;
    std::vector<common_types::IListenedObjectObserver *> m_listenedObjectsObservers;
    std::vector<common_types::SListenedTrajectory> m_objectImitations;
    int64_t m_lastActivityAtMillisec;
    int64_t m_lastStepAtMillisec;
    bool m_stepsPassed;
    int32_t m_currentStepNum;





};

#endif // OBJECT_LISTENER_IMITATOR_H
