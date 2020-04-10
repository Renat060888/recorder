#ifndef RECORDER_CONTROLLER_H
#define RECORDER_CONTROLLER_H

#include "datasource/object_listener_objrepr.h"

class RecorderController : public INetworkObserver
{
public:
    struct SInitSettings {
        common_types::TContextId ctxId;
    };

    RecorderController();

    bool init( const SInitSettings & _settings );
    void launch();



private:
    virtual void callbackNetworkRequest( PEnvironmentRequest _request ) override;




    // data
    ObjectListenerObjrepr m_worker;

    // service
    PNetworkClient m_network;

};

#endif // RECORDER_CONTROLLER_H
