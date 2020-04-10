#ifndef OBJECT_LISTENER_MQTT_H
#define OBJECT_LISTENER_MQTT_H

#include "common/common_types.h"
#include "mqtt_client.h"

class ObjectListenerMqtt : public common_types::IObjectListeningService, public MqttClient::IObserver
{
public:
    struct SInitSettings {
        common_types::TContextId ctxId;
        common_types::TMissionId missionId;
    };

    ObjectListenerMqtt();

    bool init( const SInitSettings & _settings );


private:
    virtual void mqttClientEvent( const innotransfer::Package & package, MqttClient::EPackageType type ) override;
    virtual void mqttClientBatchEvent( const innotransfer::BatchPackage & package, MqttClient::EPackageType type ) override;

    virtual void addObserver( common_types::IObjectListeningObserver * _observer ) override;
    virtual void removeObserver( common_types::IObjectListeningObserver * _observer ) override;
    virtual void runListenCycle() override;
    virtual common_types::TContextId getListenedContextId() override;
    virtual common_types::TMissionId getListenedMissionId() override;



    // data
    SInitSettings m_settings;
    std::vector<common_types::IObjectListeningObserver *> m_listenedObjectsObservers;


    // service
    MqttClient m_mqttClient;
};

#endif // OBJECT_LISTENER_MQTT_H
