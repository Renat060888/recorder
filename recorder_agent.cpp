
#include <microservice_common/common/ms_common_utils.h>
#include <microservice_common/system/logger.h>
#include <microservice_common/system/process_launcher.h>
#include <microservice_common/system/wal.h>
#include <microservice_common/system/system_monitor.h>

#include "system/path_locator.h"
#include "system/config_reader.h"
#include "recorder_agent.h"

using namespace std;

boost::signals2::signal<void()> RecorderAgent::m_unixInterruptSignal;
static constexpr const char * PRINT_HEADER = "RecorderAgent:";

RecorderAgent::RecorderAgent()
    : m_systemEnvironment(nullptr)
    , m_communicateGateway(nullptr)
    , m_storageEngine(nullptr)
    , m_sourceManager(nullptr)
    , m_shutdownCalled(false)
{
    // system facility
    m_systemEnvironment = new SystemEnvironmentFacadeRecorder();

    // I data generation
    m_sourceManager = new SourceManagerFacade();

    // II data store
    m_storageEngine = new StorageEngineFacade();

    // III data analyze ( NOTE: Recorder does not analyze data )

    // IV communication between actors
    m_commandServices.sourceManager = m_sourceManager;
    m_commandServices.storageEngine = m_storageEngine;
    m_commandServices.systemEnvironment = m_systemEnvironment;
    m_commandServices.signalShutdownServer = std::bind( & RecorderAgent::shutdown, this );

    m_communicateGateway = new CommunicationGatewayFacadeRecorder();
}

RecorderAgent::~RecorderAgent()
{
    shutdown();
}

bool RecorderAgent::init( const SInitSettings & _settings ){

    VS_LOG_INFO << PRINT_HEADER << " ============================ START INIT ========================" << endl;

    PATH_LOCATOR.removePreviousSessionSocketFiles();

    const SystemMonitor::STotalInfo info = SYSTEM_MONITOR.getTotalSnapshot();
    SYSTEM_MONITOR.printOnScreen( info );

    SystemEnvironmentFacadeRecorder::SInitSettings settings0;
    settings0.databaseHost = CONFIG_PARAMS.baseParams.MONGO_DB_ADDRESS;
    settings0.databaseName = CONFIG_PARAMS.baseParams.MONGO_DB_NAME;
    settings0.restoreSystemAfterInterrupt = CONFIG_PARAMS.baseParams.SYSTEM_RESTORE_INTERRUPTED_SESSION;
    settings0.uniqueLockFileFullPath = PATH_LOCATOR.getUniqueLockFile();
    if( ! m_systemEnvironment->init(settings0) ){
        return false;
    }

    CommunicationGatewayFacadeRecorder::SInitSettings settings1;
    settings1.requestsFromConfig = CONFIG_PARAMS.baseParams.INITIAL_REQUESTS;
    settings1.requestsFromWAL = m_systemEnvironment->serviceForWriteAheadLogging()->getInterruptedOperations();
    settings1.asyncNetwork = true;
    settings1.services = m_commandServices;
    if( ! m_communicateGateway->init(settings1) ){
        return false;
    }

    SourceManagerFacade::SInitSettings settings2;
    settings2.services.internalCommunication = m_communicateGateway->serviceForInternalCommunication();
    settings2.services.externalCommunication = m_communicateGateway->serviceForExternalCommunication();
    if( ! m_sourceManager->init(settings2) ){
        return false;
    }

    StorageEngineFacade::SInitSettings settings3;
    settings3.services.ObjectListeningService = m_sourceManager->getServiceOfObjectListening();
    if( ! m_storageEngine->init(settings3) ){
        return false;
    }

    // "CTRL + C" handler
    common_utils::connectInterruptSignalHandler( (common_utils::TSignalHandlerFunction) & RecorderAgent::callbackUnixInterruptSignal );
    m_unixInterruptSignal.connect( boost::bind( & RecorderAgent::shutdownByUnixInterruptSignal, this) );

    checkForSelfShutdown();

    // NOTE: if initialize successfully done, then the history of interrupted operations is completely cleared,
    // and their renewal will be started with a command fetch in the RecorderAgent::launch()
    m_systemEnvironment->serviceForWriteAheadLogging()->cleanJournal();

    VS_LOG_INFO << PRINT_HEADER << " ============================ INIT SUCCESS ============================" << endl;
    return true;
}

void RecorderAgent::launch(){

    while( ! m_shutdownCalled.load() ){

        if( ! m_communicateGateway->getInitSettings().asyncNetwork ){
            m_communicateGateway->runNetworkCallbacks();
        }

        if( m_communicateGateway->isCommandAvailable() ){
            PCommand command = m_communicateGateway->nextIncomingCommand();
            command->exec();
        }

        // TODO: may be throught about CV ?
        std::this_thread::sleep_for( std::chrono::milliseconds(10) );
    }
}

void RecorderAgent::checkForSelfShutdown(){

    if( CONFIG_PARAMS.baseParams.SYSTEM_SELF_SHUTDOWN_SEC != 0 ){
        m_selfShutdownFuture = std::async( std::launch::async, [this](){
            VS_LOG_WARN << PRINT_HEADER
                        << " ------------------------------ (!) SELF-SHUTDOWN AFTER"
                        << " [" << CONFIG_PARAMS.baseParams.SYSTEM_SELF_SHUTDOWN_SEC << "] sec"
                        << " (!) ------------------------------"
                     << endl;

            std::this_thread::sleep_for( std::chrono::seconds(CONFIG_PARAMS.baseParams.SYSTEM_SELF_SHUTDOWN_SEC) );

            VS_LOG_WARN << PRINT_HEADER
                        << " ------------------------------ (!) SELF-SHUTDOWN INITIATE (!) ------------------------------"
                        << endl;

            m_shutdownCalled.store( true );
        });
    }
}

void RecorderAgent::shutdown(){

    // NOTE: order of system shutdown following:
    // - first fo all - interrupt communication with outside world;
    // - then destroy source consumers;
    // - and finally close connections to sources;
    m_communicateGateway->shutdown();
    m_sourceManager->shutdown();
    m_storageEngine->shutdown();

    delete m_communicateGateway;
    m_communicateGateway = nullptr;
    delete m_storageEngine;
    m_storageEngine = nullptr;
    delete m_sourceManager;
    m_sourceManager = nullptr;
    delete m_systemEnvironment;
    m_systemEnvironment = nullptr;

    PROCESS_LAUNCHER.shutdown();

    VS_LOG_INFO << PRINT_HEADER << " ============================ SHUTDOWN SUCCESS ========================" << endl;
}

void RecorderAgent::shutdownByUnixInterruptSignal(){

    m_shutdownCalled.store( true );
}

void RecorderAgent::callbackUnixInterruptSignal(){

    VS_LOG_INFO << PRINT_HEADER << " ============================ catched SIGINT, initiate shutdown ============================" << endl;
    m_unixInterruptSignal();
}







