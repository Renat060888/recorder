
#include "object_listener_mqtt.h"

using namespace std;
using namespace common_types;

ObjectListenerMqtt::ObjectListenerMqtt()
{

}

void ObjectListenerMqtt::runListenCycle(){


    assert( false && "TODO: do" );



}

bool ObjectListenerMqtt::init(){




    return true;
}

void ObjectListenerMqtt::addObserver( common_types::IListenedObjectObserver * _observer ){

    for( const IListenedObjectObserver * const observer : m_listenedObjectsObservers ){
        if( observer == _observer ){
            return;
        }
    }

    m_listenedObjectsObservers.push_back( _observer );
}

void ObjectListenerMqtt::removeObserver( common_types::IListenedObjectObserver * _observer ){

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
