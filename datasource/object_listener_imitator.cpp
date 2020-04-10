
#include <microservice_common/system/logger.h>

#include "common/common_utils.h"
#include "object_listener_imitator.h"

using namespace std;
using namespace common_types;

static constexpr int64_t ACTIVITY_INTERVAL_MILLISEC = 1000 * 60 * 1;
static constexpr int64_t STEP_INTERVAL_MILLISEC = 1000;
static constexpr int32_t STEPS_LIMIT = 20;

ObjectListenerImitator::ObjectListenerImitator()
    : m_lastActivityAtMillisec(0)
    , m_lastStepAtMillisec(0)
    , m_stepsPassed(false)
    , m_currentStepNum(0)
{

}

bool ObjectListenerImitator::init( const SInitSettings & _settings ){

    m_settings = _settings;



    return true;
}

void ObjectListenerImitator::runListenCycle(){

    // either make steps
    if( ! m_stepsPassed ){
        if( m_currentStepNum < STEPS_LIMIT ){
            if( (common_utils::getCurrentTimeMillisec() - m_lastStepAtMillisec) > STEP_INTERVAL_MILLISEC ){
                m_lastStepAtMillisec = common_utils::getCurrentTimeMillisec();
                m_currentStepNum++;

                // ...
                VS_LOG_INFO << "step num: " << m_currentStepNum << endl;

                for( SListenedTrajectory & obj : m_objectImitations ){
                    // change obj state

                    // notify
                    for( IObjectListeningObserver * observer : m_listenedObjectsObservers ){
                        observer->callbackObjectDetected( obj );
                    }
                }
            }
        }
        else{
            VS_LOG_INFO << "steps passed" << endl;

            // count up 20 steps, then 'stepsPassed' = true
            m_stepsPassed = true;
            m_lastActivityAtMillisec = common_utils::getCurrentTimeMillisec();
        }
    }
    // or pause activity
    else{
        // pause up to N minutes
        if( (common_utils::getCurrentTimeMillisec() - m_lastActivityAtMillisec) < ACTIVITY_INTERVAL_MILLISEC ){
            return;
        }
        // let makes steps again
        else{
            VS_LOG_INFO << "let's make steps" << endl;
            m_lastActivityAtMillisec = 0;
            m_currentStepNum = 0;
            m_stepsPassed = false;
        }
    }
}

common_types::TContextId ObjectListenerImitator::getListenedContextId(){
    return m_settings.ctxId;
}

common_types::TMissionId ObjectListenerImitator::getListenedMissionId(){
    return m_settings.missionId;
}

void ObjectListenerImitator::addObserver( common_types::IObjectListeningObserver * _observer ){

    for( const IObjectListeningObserver * const observer : m_listenedObjectsObservers ){
        if( observer == _observer ){
            return;
        }
    }

    m_listenedObjectsObservers.push_back( _observer );
}

void ObjectListenerImitator::removeObserver( common_types::IObjectListeningObserver * _observer ){

    for( auto iter = m_listenedObjectsObservers.begin(); iter != m_listenedObjectsObservers.end(); ){
        if( (* iter) == _observer ){
            iter = m_listenedObjectsObservers.erase( iter );
            return;
        }
        else{
            ++iter;
        }
    }
}
















