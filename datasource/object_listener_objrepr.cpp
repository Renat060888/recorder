
#include "object_listener_objrepr.h"

using namespace std;
using namespace common_types;

ObjectListenerObjrepr::ObjectListenerObjrepr()
{

}

void ObjectListenerObjrepr::runListenCycle(){


    assert( false && "TODO: do" );

    // TODO:
    // 1 traverse on dirty objects in objrepr
    // 2 notify observers about this events

}

common_types::TContextId ObjectListenerObjrepr::getListenedContextId(){
    return m_settings.ctxId;
}

common_types::TMissionId ObjectListenerObjrepr::getListenedMissionId(){
    return m_settings.missionId;
}

void ObjectListenerObjrepr::addObserver( common_types::IObjectListeningObserver * _observer ){

    for( const IObjectListeningObserver * const observer : m_listenedObjectsObservers ){
        if( observer == _observer ){
            return;
        }
    }

    m_listenedObjectsObservers.push_back( _observer );
}

void ObjectListenerObjrepr::removeObserver( common_types::IObjectListeningObserver * _observer ){

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





