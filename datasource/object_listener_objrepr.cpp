
#include "object_listener_objrepr.h"

using namespace std;
using namespace common_types;

ObjectListenerObjrepr::ObjectListenerObjrepr()
{

}

void ObjectListenerObjrepr::runListenCycle(){


    assert( false && "TODO: do" );

    // traverse on dirty objects in objrepr

}

common_types::TContextId ObjectListenerObjrepr::getListenedContextId(){

    return m_settings.ctxId;
}

void ObjectListenerObjrepr::addObserver( common_types::IListenedObjectObserver * _observer ){

    for( const IListenedObjectObserver * const observer : m_listenedObjectsObservers ){
        if( observer == _observer ){
            return;
        }
    }

    m_listenedObjectsObservers.push_back( _observer );
}

void ObjectListenerObjrepr::removeObserver( common_types::IListenedObjectObserver * _observer ){

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
