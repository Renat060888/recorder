#ifndef OBJREPR_BUS_RECORDER_H
#define OBJREPR_BUS_RECORDER_H

#include <microservice_common/system/objrepr_bus.h>

class ObjreprBusRecorder : public ObjreprBus
{
public:
    ObjreprBusRecorder();

    // NOTE: stuff for derived classes ( video-server-object, player-object, etc... )



private:





};
#define OBJREPR_BUS ObjreprBusRecorder::singleton()

#endif // OBJREPR_BUS_RECORDER_H
