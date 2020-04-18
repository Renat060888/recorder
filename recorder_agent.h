#ifndef PLAYER_AGENT_H
#define PLAYER_AGENT_H

#include <string>
#include <future>

#include <boost/signals2.hpp>

#include "common/common_types.h"
#include "system/system_environment_facade_recorder.h"
#include "communication/communication_gateway_facade_recorder.h"
#include "storage/storage_engine_facade.h"
#include "datasource/source_manager_facade.h"

class RecorderAgent
{
    static void callbackUnixInterruptSignal();
    static boost::signals2::signal<void()> m_unixInterruptSignal;
public:
    struct SInitSettings {

    };

    struct SState {
        SInitSettings settings;
        std::string lastError;
    };

    RecorderAgent();
    ~RecorderAgent();

    bool init( const SInitSettings & _settings );
    const SState & getState(){ return m_state; }

    void launch();


private:
    void shutdown();
    void shutdownByUnixInterruptSignal();
    void checkForSelfShutdown();

    // data
    SState m_state;
    common_types::SIncomingCommandServices m_commandServices;
    std::atomic<bool> m_shutdownCalled;
    std::future<void> m_selfShutdownFuture;

    // service
    SystemEnvironmentFacadeRecorder * m_systemEnvironment;
    CommunicationGatewayFacadeRecorder * m_communicateGateway;
    StorageEngineFacade * m_storageEngine;
    SourceManagerFacade * m_sourceManager;
};

#endif // PLAYER_AGENT_H
