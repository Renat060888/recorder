
#include "proxy_object_listener_objrepr.h"

using namespace std;
using namespace common_types;

ProxyObjectListenerObjrepr::ProxyObjectListenerObjrepr()
{

}

void ProxyObjectListenerObjrepr::runListenCycle(){


    assert( false && "TODO: do" );

    // TODO:
    // 1 catch all events accumulated from ObjreprListener (which is in another process) via network
    // 2 notify observers about this events

}

void ProxyObjectListenerObjrepr::addObserver( common_types::IObjectListeningObserver * _observer ){

    for( const IObjectListeningObserver * const observer : m_listenedObjectsObservers ){
        if( observer == _observer ){
            return;
        }
    }

    m_listenedObjectsObservers.push_back( _observer );
}

void ProxyObjectListenerObjrepr::removeObserver( common_types::IObjectListeningObserver * _observer ){

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
