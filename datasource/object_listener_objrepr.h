#ifndef OBJECT_LISTENER_OBJREPR_H
#define OBJECT_LISTENER_OBJREPR_H

#include "common/common_types.h"

// NOTE: this component must be in separate process
class ObjectListenerObjrepr : public common_types::IObjectListeningService
{
public:
    struct SInitSettings {
        common_types::TContextId ctxId;
        common_types::TMissionId missionId;
    };

    ObjectListenerObjrepr();

    bool init( const SInitSettings & _settings );

    // NOTE: implement only if it will be necessary

private:
    virtual void addObserver( common_types::IObjectListeningObserver * _observer ) override;
    virtual void removeObserver( common_types::IObjectListeningObserver * _observer ) override;
    virtual void runListenCycle() override;
    virtual common_types::TContextId getListenedContextId() override;
    virtual common_types::TMissionId getListenedMissionId() override;



    // data
    SInitSettings m_settings;
    std::vector<common_types::IObjectListeningObserver *> m_listenedObjectsObservers;




};

#endif // OBJECT_LISTENER_OBJREPR_H
