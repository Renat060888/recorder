
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
    m_mqttClient.subscribe( 373, 0 );


    return true;
}

void ObjectListenerMqtt::mqttClientEvent( const innotransfer::Package & package, MqttClient::EPackageType type ){

    switch( type ){
    case MqttClient::EPackageType::MOTION : {
        SListenedTrajectory traj;
        traj.ctxId = package.motiondata.contextID;
        traj.missionId = 0;
        traj.objId = package.motiondata.id;
        traj.latDeg = package.motiondata.y;
        traj.lonDeg = package.motiondata.x;
#ifdef OBJREPR_LIBRARY_EXIST
        objrepr::Orientation orient;
        orient.setQuaternion( package.motiondata.i,
                              package.motiondata.j,
                              package.motiondata.k,
                              package.motiondata.w,
                              objrepr::GeoCoord(package.motiondata.x, package.motiondata.y, package.motiondata.z),
                              4876 );
        traj.yawDeg = orient.yaw();
#endif

        for( IListenedObjectObserver * observer : m_listenedObjectsObservers ){
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

    // TODO: do ?
}

void ObjectListenerMqtt::runListenCycle(){

    m_mqttClient.runCallbacks();
}

common_types::TContextId ObjectListenerMqtt::getListenedContextId(){

    return m_settings.ctxId;
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






