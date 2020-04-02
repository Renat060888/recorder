
#include <microservice_common/system/logger.h>

#include "context.h"

using namespace std;

Context::Context()
    : m_trajObjectsCounter(0)
    , m_weatherObjectsCounter(0)
    , m_lastDumpAtMillisec(0)
{

}

void Context::runSystemClock(){



    // TODO: do



}

void Context::visit( const common_types::SListenedTrajectory * _listenObj ){

    VS_LOG_INFO << "listen traj object: " << _listenObj->objId << endl;
}

void Context::visit( const common_types::SListenedWeather * _listenObj ){

    VS_LOG_INFO << "listen weather object: " << _listenObj->objId << endl;


}
