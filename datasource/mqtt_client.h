#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <string>
#include <vector>

#include <mosquitto.h>
#include <objrepr/innotransfer.h>


class MqttClient final
{
    static const char * DEFAULT_HOST;
    static const int DEFAULT_PORT;
    static const int DEFAULT_KEEP_ALIVE_SEC;
    static const int DEFAULT_MANUAL_CALLBACK_RUN_TIMEOUT_MILLISEC;
    friend void callbackMessage( mosquitto * mosq, void * userdata, const mosquitto_message * message );
public:
    enum class ERouteMode {
        MULTICAST,
        UNICAST,
        UNDEFINED
    };

    enum class EPackageType {
        MOTION,
        MOTION_BATCH,
        COVER,
        INTERACTION,
        UNDEFINED
    };

    struct SHeader {
        EPackageType packType;
        // NOTE: mqtt-брокер возвращает сообщение также и клиенту, передавшего его
        // Данный ID, чтобы отбрасывать "свои" сообщения и не обрабатывать их
        int32_t clientId;
    };

    struct SInitSettings {
        SInitSettings() :
            host(DEFAULT_HOST),
            port(DEFAULT_PORT),
            keepAliveSec(DEFAULT_KEEP_ALIVE_SEC),
            manualCallbackRunTimeoutMillisec(DEFAULT_MANUAL_CALLBACK_RUN_TIMEOUT_MILLISEC),
            internalCallbacksThread(true),
            routeMode(ERouteMode::UNDEFINED)
        {}
        std::string login;
        std::string password;
        std::string host;
        int port;
        int keepAliveSec;
        int manualCallbackRunTimeoutMillisec;
        bool internalCallbacksThread;
        ERouteMode routeMode;
    };

    class IObserver {
    public:
        virtual void mqttClientEvent( const innotransfer::Package & package, EPackageType type ) = 0;
        virtual void mqttClientBatchEvent( const innotransfer::BatchPackage & package, EPackageType type ) = 0;
    };

    static ERouteMode convertRouteModeFromStr( const std::string & _str );

    // NOTE: без задания id клиенты могут работать только на разных машинах, иначе идет конкуренция за соединение с брокером
    MqttClient( const char * id = nullptr );
    ~MqttClient();

    bool init( const SInitSettings & settings );
    void addObserver( IObserver * observer ){ _observers.push_back(observer); }
    const SInitSettings & getInitSettings(){ return _settings; }
    const std::string & getLastError(){ return _lastError; }
    void shutdown();

    void runCallbacks();

    bool subscribe( uint32_t contextId, uint32_t missionId = 0 );
    bool unsubscribe();

    void sendPackage( const innotransfer::CoverAreaData & pack );
    void sendPackage( const innotransfer::MotionData & pack );
    void sendPackage( const innotransfer::MotionBatchData & batchData );
    void sendPackage( const innotransfer::InteractionData & pack );


private:
    inline void publish( const char * ptr, int size );
    inline bool isSuccess( int ret );

    // non-copyable
    MqttClient( const MqttClient & );
    const MqttClient & operator=( const MqttClient & );

    // data
    int _clientIdNumber;
    std::string _clientIdStr;
    std::string _currentTopic;
    SInitSettings _settings;
    std::string _lastError;
    std::vector<IObserver *> _observers;
    char * motionBatchBuffer;

    // service
    mosquitto * _mosq;

};

#endif // MQTT_CLIENT_H

