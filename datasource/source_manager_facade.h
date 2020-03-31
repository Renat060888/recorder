#ifndef SOURCE_MANAGER_H
#define SOURCE_MANAGER_H

#include "common/common_types.h"
#include "mqtt_client.h"

class SourceManagerFacade : public MqttClient::IObserver
{
public:
    struct SServiceLocator {
        common_types::IServiceInternalCommunication * internalCommunication;
        common_types::IServiceExternalCommunication * externalCommunication;
    };

    struct SInitSettings {
        SServiceLocator services;
    };

    SourceManagerFacade();

    bool init( const SInitSettings & _settings );
    void shutdown();

    common_types::IServiceObjectListener * getServiceOfObjectListener();


private:
    virtual void mqttClientEvent( const innotransfer::Package & package, MqttClient::EPackageType type ) override;
    virtual void mqttClientBatchEvent( const innotransfer::BatchPackage & package, MqttClient::EPackageType type ) override;




    // data
    std::vector<common_types::IListenedObjectObserver *> m_listenedObjectsObservers;


    // service
    MqttClient m_mqttClient;
    common_types::IServiceObjectListener * m_listeningService;
};

#endif // SOURCE_MANAGER_H
