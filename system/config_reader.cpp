
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <microservice_common/common/ms_common_utils.h>

#include "config_reader.h"

using namespace std;
using namespace common_types;

static constexpr const char * PRINT_HEADER = "ConfigReader:";

ConfigReader::ConfigReader()
{

}


bool ConfigReader::initDerive( const SIninSettings & _settings ){

    m_parameters.baseParams = AConfigReader::m_parameters;
    return true;
}

bool ConfigReader::parse( const std::string & _content ){

    // parse base part
    boost::property_tree::ptree config;

    istringstream contentStream( _content );
    try{
        boost::property_tree::json_parser::read_json( contentStream, config );
    }
    catch( boost::property_tree::json_parser::json_parser_error & _ex ){
        PRELOG_ERR << ::PRINT_HEADER << " parse failed of [" << _content << "]" << endl
             << "Reason: [" << _ex.what() << "]" << endl;
        return false;
    }

    boost::property_tree::ptree communication = config.get_child("communication");
    boost::property_tree::ptree mqttClient = communication.get_child("mqtt_client");
    m_parameters.COMMUNICATION_MQTT_ENABLE = setParameterNew<bool>( mqttClient, "enable", false );
    m_parameters.COMMUNICATION_MQTT_SERVER_HOST = setParameterNew<std::string>( mqttClient, "host", string("localhost") );
    m_parameters.COMMUNICATION_MQTT_SERVER_PORT = setParameterNew<int32_t>( mqttClient, "port", 1883 );
    m_parameters.COMMUNICATION_MQTT_LOGIN = setParameterNew<std::string>( mqttClient, "login", string("mquser") );
    m_parameters.COMMUNICATION_MQTT_PASS = setParameterNew<std::string>( mqttClient, "pass", string("mqpass") );
    m_parameters.COMMUNICATION_MQTT_ROUTE_MODE = setParameterNew<std::string>( mqttClient, "route_mode", string("multicast") );

    boost::property_tree::ptree etherListening = config.get_child("ether_listening");
    m_parameters.RECORDING_QUANTUM_TIME_MILLISEC = setParameterNew<int32_t>( etherListening, "quantum_time_millisec", 1000 );

    return true;
}

bool ConfigReader::createCommandsFromConfig( const std::string & _content ){

    // parse whole content
    boost::property_tree::ptree config;

    istringstream contentStream( _content );
    try{
        boost::property_tree::json_parser::read_json( contentStream, config );
    }
    catch( boost::property_tree::json_parser::json_parser_error & _ex ){
        PRELOG_ERR << ::PRINT_HEADER << " parse failed of [" << _content << "]" << endl
             << "Reason: [" << _ex.what() << "]" << endl;
        return false;
    }

    // create commands from commands section
    boost::property_tree::ptree initialCommands = config.get_child("initial_commands");

    boost::property_tree::ptree listeningContexts = initialCommands.get_child("listening_contexts");
    for( auto iter = listeningContexts.begin(); iter != listeningContexts.end(); ++iter ){
        boost::property_tree::ptree arrElement = iter->second;

        ostringstream oss;
        boost::property_tree::json_parser::write_json( oss, arrElement );
        const string contextForListen = oss.str();

        RequestFromConfigPtr request = std::make_shared<RequestFromConfig>();
        request->m_incomingMessage = AConfigReader::m_commandConvertor->getCommandFromConfigFile( contextForListen );
        AConfigReader::m_parameters.INITIAL_REQUESTS.push_back( request );
    }

    return true;
}

std::string ConfigReader::getConfigExampleDerive(){

    assert( false && "TODO: do" );
    return std::string();
}








