#ifndef OBJECT_LISTENER_OBJREPR_H
#define OBJECT_LISTENER_OBJREPR_H

#include "common/common_types.h"

class ObjectListenerObjrepr : public common_types::IServiceObjectListener
{
public:
    struct SInitSettings {
        common_types::TContextId ctxId;
    };

    ObjectListenerObjrepr();

    bool init( const SInitSettings & _settings );




private:
    virtual void addObserver( common_types::IListenedObjectObserver * _observer ) override;
    virtual void removeObserver( common_types::IListenedObjectObserver * _observer ) override;
    virtual void runListenCycle() override;
    virtual common_types::TContextId getListenedContextId() override;



    // data
    SInitSettings m_settings;
    std::vector<common_types::IListenedObjectObserver *> m_listenedObjectsObservers;




};

#endif // OBJECT_LISTENER_OBJREPR_H
