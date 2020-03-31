
#ifdef OBJREPR_LIBRARY_EXIST
#include <objrepr/reprServer.h>
#endif

#include <microservice_common/system/logger.h>

#include "system/config_reader.h"
#include "source_manager_facade.h"

using namespace std;
using namespace common_types;

static constexpr const char * PRINT_HEADER = "SourceManagerFacade:";

SourceManagerFacade::SourceManagerFacade()
{

}

bool SourceManagerFacade::init( const SInitSettings & _settings ){

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

    //



    VS_LOG_INFO << PRINT_HEADER << " init success" << endl;
    return true;
}

void SourceManagerFacade::shutdown(){

}

void SourceManagerFacade::mqttClientEvent( const innotransfer::Package & package, MqttClient::EPackageType type ){

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
        VS_LOG_WARN << PRINT_HEADER << " unknown package type from event - " << int( type ) << endl;
    }
    }

}

void SourceManagerFacade::mqttClientBatchEvent( const innotransfer::BatchPackage & package, MqttClient::EPackageType type ){

    // TODO: do ?
}



common_types::IServiceObjectListener * SourceManagerFacade::getServiceOfObjectListener(){
    return m_listeningService;
}














