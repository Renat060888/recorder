#ifndef MIRROR_OBJECT_LISTENER_OBJREPR_H
#define MIRROR_OBJECT_LISTENER_OBJREPR_H

#include "common/common_types.h"

class ProxyObjectListenerObjrepr : public common_types::IObjectListeningService
{
public:
    struct SInitSettings {
        common_types::TContextId ctxId;
    };

    ProxyObjectListenerObjrepr();

    bool init( const SInitSettings & _settings );

    // NOTE: implement only if it will be necessary

private:
    virtual void addObserver( common_types::IObjectListeningObserver * _observer ) override;
    virtual void removeObserver( common_types::IObjectListeningObserver * _observer ) override;
    virtual void runListenCycle() override;



    // data
    std::vector<common_types::IObjectListeningObserver *> m_listenedObjectsObservers;


    // service





};

#endif // MIRROR_OBJECT_LISTENER_OBJREPR_H
