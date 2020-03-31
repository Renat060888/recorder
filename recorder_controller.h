#ifndef RECORDER_CONTROLLER_H
#define RECORDER_CONTROLLER_H

#include "datasource/object_listener_objrepr.h"

class RecorderController
{
public:
    struct SInitSettings {

    };

    RecorderController();

    bool init( const SInitSettings & _settings );
    void launch();



private:

    ObjectListenerObjrepr m_worker;


};

#endif // RECORDER_CONTROLLER_H
