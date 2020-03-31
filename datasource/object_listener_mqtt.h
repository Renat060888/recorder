#ifndef OBJECT_LISTENER_MQTT_H
#define OBJECT_LISTENER_MQTT_H

#include "common/common_types.h"
#include "mqtt_client.h"

class ObjectListenerMqtt : public common_types::IServiceObjectListener
{
public:
    ObjectListenerMqtt();

    bool init();


private:
    virtual void addObserver( common_types::IListenedObjectObserver * _observer ) override;
    virtual void removeObserver( common_types::IListenedObjectObserver * _observer ) override;
    virtual void runListenCycle() override;



    // data
    std::vector<common_types::IListenedObjectObserver *> m_listenedObjectsObservers;


    // service
    MqttClient m_mqttClient;
};

#endif // OBJECT_LISTENER_MQTT_H
