
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <microservice_common/common/ms_common_utils.h>

#include "config_reader.h"

using namespace std;

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

    return true;
}

bool ConfigReader::createCommandsFromConfig( const std::string & _content ){



    return true;
}

void ConfigReader::printToStdoutConfigExampleDerive(){

    assert( false && "TODO: do" );
}
