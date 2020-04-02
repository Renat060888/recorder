#ifndef OBJECT_LISTENER_MQTT_H
#define OBJECT_LISTENER_MQTT_H

#include "common/common_types.h"
#include "mqtt_client.h"

class ObjectListenerMqtt : public common_types::IServiceObjectListener, public MqttClient::IObserver
{
public:
    struct SInitSettings {
        common_types::TContextId ctxId;
    };

    ObjectListenerMqtt();

    bool init( const SInitSettings & _settings );


private:
    virtual void mqttClientEvent( const innotransfer::Package & package, MqttClient::EPackageType type ) override;
    virtual void mqttClientBatchEvent( const innotransfer::BatchPackage & package, MqttClient::EPackageType type ) override;

    virtual void addObserver( common_types::IListenedObjectObserver * _observer ) override;
    virtual void removeObserver( common_types::IListenedObjectObserver * _observer ) override;
    virtual void runListenCycle() override;
    virtual common_types::TContextId getListenedContextId() override;



    // data
    SInitSettings m_settings;
    std::vector<common_types::IListenedObjectObserver *> m_listenedObjectsObservers;


    // service
    MqttClient m_mqttClient;
};

#endif // OBJECT_LISTENER_MQTT_H
