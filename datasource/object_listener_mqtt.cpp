#ifdef OBJREPR_LIBRARY_EXIST
#include <objrepr/reprServer.h>
#endif

#include <microservice_common/system/logger.h>

#include "object_listener_mqtt.h"
#include "system/config_reader.h"

using namespace std;
using namespace common_types;

ObjectListenerMqtt::ObjectListenerMqtt()
{

}

bool ObjectListenerMqtt::init( const SInitSettings & _settings ){

    //
    MqttClient::SInitSettings mqttSettings;
    mqttSettings.host = CONFIG_PARAMS.COMMUNICATION_MQTT_SERVER_HOST;
    mqttSettings.port = CONFIG_PARAMS.COMMUNICATION_MQTT_SERVER_PORT;
    mqttSettings.login = CONFIG_PARAMS.COMMUNICATION_MQTT_LOGIN;
    mqttSettings.password = CONFIG_PARAMS.COMMUNICATION_MQTT_PASS;
    mqttSettings.routeMode = MqttClient::convertRouteModeFromStr( CONFIG_PARAMS.COMMUNICATION_MQTT_ROUTE_MODE );
    mqttSettings.internalCallbacksThread = false;
    if( ! m_mqttClient.init(mqttSettings) ){
        return false;
    }

    m_mqttClient.addObserver( this );
    m_mqttClient.subscribe( _settings.ctxId, _settings.missionId );


    return true;
}

void ObjectListenerMqtt::mqttClientEvent( const innotransfer::Package & package, MqttClient::EPackageType type ){

    switch( type ){
    case MqttClient::EPackageType::MOTION : {
        SListenedTrajectory traj( package.motiondata.contextID, 0, package.motiondata.id );
        traj.data.latDeg = package.motiondata.y;
        traj.data.lonDeg = package.motiondata.x;
        traj.data.yawDeg = 0; // convert quaternion to euler angles
#ifdef OBJREPR_LIBRARY_EXIST
        objrepr::Orientation orient;
        orient.setQuaternion( package.motiondata.i,
                              package.motiondata.j,
                              package.motiondata.k,
                              package.motiondata.w,
                              objrepr::GeoCoord(package.motiondata.x, package.motiondata.y, package.motiondata.z),
                              4876 );
        traj.data.yawDeg = orient.yaw();
#endif

        for( IObjectListeningObserver * observer : m_listenedObjectsObservers ){
            observer->callbackObjectDetected( traj );
        }
        break;
    }
    case MqttClient::EPackageType::COVER : {

        package.coverarea;
        break;
    }
    case MqttClient::EPackageType::INTERACTION : {

        package.interaction;
        break;
    }
    default : {
        VS_LOG_WARN << "unknown package type from event - " << int( type ) << endl;
    }
    }

}

void ObjectListenerMqtt::mqttClientBatchEvent( const innotransfer::BatchPackage & package, MqttClient::EPackageType type ){

    assert( false && "TODO: do ?" );
}

void ObjectListenerMqtt::runListenCycle(){

    m_mqttClient.runCallbacks();
}

common_types::TContextId ObjectListenerMqtt::getListenedContextId(){
    return m_settings.ctxId;
}

common_types::TContextId ObjectListenerMqtt::getListenedMissionId(){
    return m_settings.missionId;
}

void ObjectListenerMqtt::addObserver( common_types::IObjectListeningObserver * _observer ){

    for( const IObjectListeningObserver * const observer : m_listenedObjectsObservers ){
        if( observer == _observer ){
            return;
        }
    }

    m_listenedObjectsObservers.push_back( _observer );
}

void ObjectListenerMqtt::removeObserver( common_types::IObjectListeningObserver * _observer ){

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






