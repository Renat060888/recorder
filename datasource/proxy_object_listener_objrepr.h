#ifndef MIRROR_OBJECT_LISTENER_OBJREPR_H
#define MIRROR_OBJECT_LISTENER_OBJREPR_H

#include "common/common_types.h"

class ProxyObjectListenerObjrepr : public common_types::IServiceObjectListener
{
public:
    struct SInitSettings {
        common_types::TContextId ctxId;
    };

    ProxyObjectListenerObjrepr();

    bool init( const SInitSettings & _settings );



private:
    virtual void addObserver( common_types::IListenedObjectObserver * _observer ) override;
    virtual void removeObserver( common_types::IListenedObjectObserver * _observer ) override;
    virtual void runListenCycle() override;



    // data
    std::vector<common_types::IListenedObjectObserver *> m_listenedObjectsObservers;




};

#endif // MIRROR_OBJECT_LISTENER_OBJREPR_H
