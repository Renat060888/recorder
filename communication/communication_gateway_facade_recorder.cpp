
#include <microservice_common/common/ms_common_utils.h>
#include <microservice_common/communication/amqp_client_c.h>

#include "communication_gateway_facade_recorder.h"
#include "command_factory.h"
#include "system/config_reader.h"

using namespace std;

CommunicationGatewayFacadeRecorder::CommunicationGatewayFacadeRecorder()
    : CommunicationGatewayFacade()
{

}

CommunicationGatewayFacadeRecorder::~CommunicationGatewayFacadeRecorder(){

}

bool CommunicationGatewayFacadeRecorder::init( const SInitSettings & _settings ){

    SAmqpRouteParameters initialRoute;
    initialRoute.predatorExchangePointName = "dss_dx_player";
    initialRoute.predatorQueueName = "dss_q_player_mailbox";
    initialRoute.predatorRoutingKeyName = "dss_rk_to_player";

    m_settings = _settings;
    m_settings.paramsForInitialAmqp.route = initialRoute;
    m_settings.paramsForInitialAmqp.enable = CONFIG_PARAMS.baseParams.COMMUNICATION_AMQP_ENABLE;
    m_settings.paramsForInitialAmqp.host = CONFIG_PARAMS.baseParams.COMMUNICATION_AMQP_SERVER_HOST;
    m_settings.paramsForInitialAmqp.virtHost = CONFIG_PARAMS.baseParams.COMMUNICATION_AMQP_VIRTUAL_HOST;
    m_settings.paramsForInitialAmqp.port = CONFIG_PARAMS.baseParams.COMMUNICATION_AMQP_SERVER_PORT;
    m_settings.paramsForInitialAmqp.login = CONFIG_PARAMS.baseParams.COMMUNICATION_AMQP_LOGIN;
    m_settings.paramsForInitialAmqp.pass = CONFIG_PARAMS.baseParams.COMMUNICATION_AMQP_PASS;
    m_settings.specParams.factory = new CommandFactory( m_settings.services );

    if( ! CommunicationGatewayFacade::init(m_settings) ){
        return false;
    }

    // configure communicators by amqp controllers





    const string nodeWorkerName = "simula_ctx0_mis0";
    const string nodeAgentName = "simula_ip0_pid0";

    // player agent & workers...




    // node worker -> core
    SAmqpRouteParameters routes6;
    routes6.predatorExchangePointName = "dss_dx_node_workers";
    routes6.predatorQueueName = "dss_q_node_worker_mailbox_" + nodeWorkerName;
    routes6.predatorRoutingKeyName = "dss_rk_to_node_worker_" + nodeWorkerName;
    routes6.targetExchangePointName = "dss_dx_core";
    routes6.targetRoutingKeyName = "dss_rk_to_core";

    // node agent -> core
    SAmqpRouteParameters routes5;
    routes5.predatorExchangePointName = "dss_dx_node_agents";
    routes5.predatorQueueName = "dss_q_node_agent_mailbox_" + nodeAgentName;
    routes5.predatorRoutingKeyName = "dss_rk_to_node_agent_" + nodeAgentName;
    routes5.targetExchangePointName = "dss_dx_core";
    routes5.targetRoutingKeyName = "dss_rk_to_core";





    return true;
}

common_types::IServiceExternalCommunication * CommunicationGatewayFacadeRecorder::serviceForExternalCommunication(){
    return this;
}

common_types::IServiceInternalCommunication * CommunicationGatewayFacadeRecorder::serviceForInternalCommunication(){
    return this;
}

PNetworkClient CommunicationGatewayFacadeRecorder::getRecorderWorkerCommunicator( const std::string & _uniqueId ){

    auto iter = m_playerWorkerCommunicatorsById.find( _uniqueId );
    if( iter != m_playerWorkerCommunicatorsById.end() ){
        return iter->second;
    }

    // recorder agent -> recorder controller ( worker )
    SAmqpRouteParameters route;
    route.predatorExchangePointName = "dss_dx_recorder";
    route.predatorQueueName = "dss_q_recorder_mailbox";
    route.predatorRoutingKeyName = "dss_rk_to_recorder";
    route.targetExchangePointName = "dss_dx_recorder_workers";
    route.targetRoutingKeyName = "dss_rk_to_recorder_worker_" + _uniqueId;

    PNetworkClient connection = CommunicationGatewayFacade::getInitialAmqpConnection();
    PAmqpClient amqpClient = std::dynamic_pointer_cast<AmqpClient>( connection );

    AmqpController::SInitSettings settings;
    settings.client = amqpClient;
    settings.route = route;

    PAmqpController controller = std::make_shared<AmqpController>( CommunicationGatewayFacade::getConnectionId() );
    if( ! controller->init( settings ) ){
        m_lastError = controller->getState().lastError;
        return nullptr;
    }

    m_playerWorkerCommunicatorsById.insert( {_uniqueId, controller} );
    return controller;
}

PNetworkClient CommunicationGatewayFacadeRecorder::getUserCommunicator( const std::string & _uniqueId ){

    auto iter = m_userCommunicatorsById.find( _uniqueId );
    if( iter != m_userCommunicatorsById.end() ){
        return iter->second;
    }

    // core -> client
    SAmqpRouteParameters route;
    route.predatorExchangePointName = "dss_dx_core";
    route.predatorQueueName = "dss_q_core_mailbox";
    route.predatorRoutingKeyName = "dss_rk_to_core";
    route.targetExchangePointName = "dss_dx_users";
    route.targetRoutingKeyName = "dss_rk_to_user_" + _uniqueId;

    PNetworkClient connection = CommunicationGatewayFacade::getInitialAmqpConnection();
    PAmqpClient amqpClient = std::dynamic_pointer_cast<AmqpClient>( connection );

    AmqpController::SInitSettings settings;
    settings.client = amqpClient;
    settings.route = route;

    PAmqpController controller = std::make_shared<AmqpController>( CommunicationGatewayFacade::getConnectionId() );
    if( ! controller->init( settings ) ){
        m_lastError = controller->getState().lastError;
        return nullptr;
    }

    m_userCommunicatorsById.insert( {_uniqueId, controller} );
    return controller;
}




















