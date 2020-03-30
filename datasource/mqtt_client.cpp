
#include <string.h>
#include <iostream>

#include <boost/format.hpp>

#include "mqtt_client.h"

static constexpr const char * TOPIC_MULTICAST_TEMPLATE = "ctx%1%/mission%2%";
static constexpr const char * TOPIC_UNICAST_TEMPLATE = "ctx%1%/mission%2%/pid%3%";
static constexpr int QUALITY_OF_SERVICE = 0; // NOTE: при QOS=1 походу начинает выжирать память, пока непонятно почему
static constexpr int RETAINED_MESSAGE = false;

const char * MqttClient::DEFAULT_HOST = "localhost";
const int MqttClient::DEFAULT_PORT = 1883;
const int MqttClient::DEFAULT_KEEP_ALIVE_SEC = 60;
const int MqttClient::DEFAULT_MANUAL_CALLBACK_RUN_TIMEOUT_MILLISEC = 1000;

using namespace std;

MqttClient::ERouteMode MqttClient::convertRouteModeFromStr( const std::string & _str ){
    if( "unicast" == _str ){ return MqttClient::ERouteMode::UNICAST; }
    else if( "multicast" == _str ){ return MqttClient::ERouteMode::MULTICAST; }
    else if( "undefined" == _str ){ return MqttClient::ERouteMode::UNDEFINED; }
    else{ assert( false && "unknown route mode" ); }
}

MqttClient::MqttClient( const char * id ) :
    _clientIdNumber( getpid() ),
    _mosq(nullptr)
{
    _clientIdStr.assign( ( id ? id : "pid" + to_string(_clientIdNumber) ) );

    motionBatchBuffer = new char[ sizeof(SHeader) + sizeof(innotransfer::MotionBatchData) ];
}

MqttClient::~MqttClient()
{
    shutdown();
    delete[] motionBatchBuffer;
}

void MqttClient::shutdown(){

    if( _mosq ){
        unsubscribe();

        mosquitto_disconnect( _mosq );

        if( _settings.internalCallbacksThread ){
            const bool force = true;
            mosquitto_loop_stop( _mosq, force );
        }

        mosquitto_destroy( _mosq );
        mosquitto_lib_cleanup();

        _mosq = nullptr;
    }
}

void callbackPublish( mosquitto *, void *, int ){

    // pass
}

void callbackMessage( mosquitto *, void * userdata, const mosquitto_message * message ){

    MqttClient * client = static_cast<MqttClient *>( userdata );

    // parse
    const MqttClient::SHeader * header = static_cast<MqttClient::SHeader *>( message->payload );

    if( header->clientId == client->_clientIdNumber ){
        return;
    }

    switch( header->packType ){
    case MqttClient::EPackageType::MOTION : {
        innotransfer::Package package;
        package.motiondata = * (innotransfer::MotionData *)( (MqttClient::SHeader *)message->payload + 1 );
        for( MqttClient::IObserver * observer : client->_observers ){
            observer->mqttClientEvent( package, header->packType );
        }
        return;
    }
    case MqttClient::EPackageType::MOTION_BATCH : {
        innotransfer::BatchPackage batchPackage;
        innotransfer::MotionBatchData * mbdp = (innotransfer::MotionBatchData *)( (MqttClient::SHeader *)message->payload + 1 );
        batchPackage.motionBatchData.header = mbdp->header;
        memcpy( & batchPackage.motionBatchData.batch, mbdp->batch, sizeof(innotransfer::MotionData) * mbdp->header.size );
        for( MqttClient::IObserver * observer : client->_observers ){
            observer->mqttClientBatchEvent( batchPackage, header->packType );
        }
        return;
    }
    case MqttClient::EPackageType::COVER : {
        innotransfer::Package package;
        package.coverarea = * (innotransfer::CoverAreaData *)( (MqttClient::SHeader *)message->payload + 1 );
        for( MqttClient::IObserver * observer : client->_observers ){
            observer->mqttClientEvent( package, header->packType );
        }
        return;
    }
    case MqttClient::EPackageType::INTERACTION : {
        innotransfer::Package package;
        package.interaction = * (innotransfer::InteractionData *)( (MqttClient::SHeader *)message->payload + 1 );
        for( MqttClient::IObserver * observer : client->_observers ){
            observer->mqttClientEvent( package, header->packType );
        }
        return;
    }
    default:
        cerr << "mqtt-client unknown package type: " << (int)header->packType << endl;
        return;
    }
}

void callbackSubscribe( mosquitto *, void *, int, int, const int * ){

    // pass
}

void callbackLog( mosquitto *, void *, int level, const char * str ){

    // TODO: switch off "log callback" when the client stabilizated
    switch( level ){
    case MOSQ_LOG_WARNING : {
        cout << "mqtt-client log warn: " << str << endl;
    }
    case MOSQ_LOG_ERR : {
        cout << "mqtt-client log error: " << str << endl;
    }
    default : {
        // pass
//        cout << "mqtt-client common log: " << str << endl;
    }
    }
}

bool MqttClient::init( const SInitSettings & settings ){

    if( ERouteMode::UNDEFINED == settings.routeMode ){
        _lastError = "undefined route mode";
        return false;
    }

    mosquitto_lib_init();

    const bool cleanSession = true;
    _mosq = mosquitto_new( (_clientIdStr.empty() ? nullptr : _clientIdStr.c_str()), cleanSession, (void *)this );
    if( ! _mosq ){
        const string msg = (boost::format( "mqtt-client fail: unable to create instance - [%1%]" ) % strerror(errno) ).str();
        cerr << msg << endl;
        _lastError = msg;
        return false;
    }

    if( ! settings.login.empty() ){
        const int ret1 = mosquitto_username_pw_set( _mosq, settings.login.c_str(), settings.password.c_str() );
        if( ! isSuccess(ret1) ){
            return false;
        }
    }

    const int ret2 = mosquitto_connect(_mosq, settings.host.c_str(), settings.port, settings.keepAliveSec);
    if( ! isSuccess(ret2) ){
        return false;
    }    

    if( settings.internalCallbacksThread ){
        const int ret3 = mosquitto_loop_start( _mosq );
        if( ! isSuccess(ret3) ){
            return false;
        }
    }

//    mosquitto_connect_callback_set
    mosquitto_log_callback_set( _mosq, callbackLog );
    mosquitto_message_callback_set( _mosq, callbackMessage );
    mosquitto_publish_callback_set( _mosq, callbackPublish );
    mosquitto_subscribe_callback_set( _mosq, callbackSubscribe );

    _settings = settings;

    // TODO: print route mode, pid
    const string msg = (boost::format( "mqtt-client connected with: id [%1%] host [%2%] port [%3%] login [%4%] pass [%5%] keep alive sec [%6%] quality of service [%7%]" )
                        % _clientIdStr
                        % settings.host
                        % settings.port
                        % settings.login
                        % settings.password
                        % settings.keepAliveSec
                        % QUALITY_OF_SERVICE ).str();

    cout << msg << endl;
    return true;
}

void MqttClient::runCallbacks(){

    assert( ! _settings.internalCallbacksThread );

    const int ret = mosquitto_loop( _mosq, _settings.manualCallbackRunTimeoutMillisec, 1 );
    if( ! isSuccess(ret) ){
        return;
    }
}

bool MqttClient::subscribe( uint32_t contextId, uint32_t missionId ){

    if( 0 == contextId ){
        _lastError = "incorrect context id";
        return false;
    }

    unsubscribe();

    switch( _settings.routeMode ){
    case ERouteMode::MULTICAST : {
        _currentTopic = ( boost::format( TOPIC_MULTICAST_TEMPLATE ) % contextId % missionId ).str();
        break;
    }
    case ERouteMode::UNICAST : {
        _currentTopic = ( boost::format( TOPIC_UNICAST_TEMPLATE ) % contextId % missionId % getpid() ).str();
        break;
    }
    default : {
        _lastError = "undefined route mode";
        return false;
    }
    }

    const int ret = mosquitto_subscribe( _mosq, nullptr, _currentTopic.c_str(), QUALITY_OF_SERVICE );
    if( ! isSuccess(ret) ){
        _currentTopic.clear();
        return false;
    }

    const string msg = (boost::format( "mqtt-client subscribed to: ctx [%1%] mission [%2%]" )
                        % contextId
                        % missionId ).str();

    cout << msg << endl;

    return true;
}

bool MqttClient::unsubscribe(){

    if( _currentTopic.empty() ){
        return true;
    }

    const int ret = mosquitto_unsubscribe( _mosq, nullptr, _currentTopic.c_str() );
    if( ! isSuccess(ret) ){
        return false;
    }

    _currentTopic.clear();
    return true;
}

void MqttClient::sendPackage( const innotransfer::CoverAreaData & pack ){

    // create
    constexpr int size = sizeof(SHeader) + sizeof(innotransfer::CoverAreaData);
    char buffer[ size ];

    SHeader * header = (SHeader *)buffer;
    header->packType = EPackageType::COVER;
    header->clientId = _clientIdNumber;

    innotransfer::CoverAreaData * payload = (innotransfer::CoverAreaData *)( (SHeader *)buffer + 1 );
    ( * payload ) = pack;

    // send
    publish( buffer, size );
}

void MqttClient::sendPackage( const innotransfer::MotionData & pack ){

    // create
    constexpr int size = sizeof(SHeader) + sizeof(innotransfer::MotionData);
    char buffer[ size ];

    SHeader * header = (SHeader *)buffer;
    header->packType = EPackageType::MOTION;
    header->clientId = _clientIdNumber;

    innotransfer::MotionData * payload = (innotransfer::MotionData *)( (SHeader *)buffer + 1 );
    ( * payload ) = pack;

    // send
    publish( buffer, size );
}

void MqttClient::sendPackage( const innotransfer::InteractionData & pack )
{
    constexpr int size = sizeof(SHeader) + sizeof(innotransfer::InteractionData);
    char buffer[ size ];

    SHeader * header = (SHeader *)buffer;
    header->packType = EPackageType::INTERACTION;
    header->clientId = _clientIdNumber;

    innotransfer::InteractionData * payload = (innotransfer::InteractionData *)( (SHeader *)buffer + 1 );
    ( * payload ) = pack;

    // send
    publish( buffer, size );
}

void MqttClient::sendPackage( const innotransfer::MotionBatchData & batchData ){

    // set network header
    SHeader * header = (SHeader *)motionBatchBuffer;
    header->packType = EPackageType::MOTION_BATCH;
    header->clientId = _clientIdNumber;

    innotransfer::MotionBatchData * payload = (innotransfer::MotionBatchData *)( (SHeader *)motionBatchBuffer + 1 );

    // set data header
    payload->header = batchData.header;

    // set payload
    memcpy( payload->batch, batchData.batch, sizeof(innotransfer::MotionData) * batchData.header.size );

    // send
    const int payloadSize = sizeof(SHeader) + sizeof(innotransfer::MotionBatchData::Header) + sizeof(innotransfer::MotionData) * batchData.header.size;
    publish( motionBatchBuffer, payloadSize );
}

inline void MqttClient::publish( const char * ptr, int size ){

    assert( ! _currentTopic.empty() );

    const int ret = mosquitto_publish( _mosq, nullptr, _currentTopic.c_str(), size, ptr, QUALITY_OF_SERVICE, RETAINED_MESSAGE );
    if( ! isSuccess(ret) ){
        return;
    }
}

inline bool MqttClient::isSuccess( int ret ){

    if( mosq_err_t::MOSQ_ERR_SUCCESS != (mosq_err_t)ret ){
        const string msg = (boost::format( "mqtt-client fail, reason: [%1%]" ) % mosquitto_strerror( ret ) ).str();
        cerr << msg << endl;
        _lastError = msg;
        return false;
    }

    return true;
}







