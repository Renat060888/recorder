#ifndef SOURCE_MANAGER_H
#define SOURCE_MANAGER_H

#include <thread>
#include <mutex>
#include <unordered_map>

#include "common/common_types.h"

class SourceManagerFacade : public common_types::IObjectListeningService, public common_types::IObjectListeningObserver
{
public:
    struct SServiceLocator {
        common_types::IServiceInternalCommunication * internalCommunication;
        common_types::IServiceExternalCommunication * externalCommunication;
    };

    struct SInitSettings {
        SServiceLocator services;
    };

    struct SState {
        SInitSettings m_settings;
        std::string lastError;
    };

    SourceManagerFacade();
    ~SourceManagerFacade();

    bool init( const SInitSettings & _settings );
    const SState & getState(){ return m_state; }
    void shutdown();

    bool startListenContext( const std::string & _ctxName );
    void stopListenContext( const std::string & _ctxName );

    common_types::IObjectListeningService * getServiceOfObjectListening();


private:
    virtual void addObserver( common_types::IObjectListeningObserver * _observer ) override;
    virtual void removeObserver( common_types::IObjectListeningObserver * _observer ) override;
    virtual void runListenCycle() override;
    virtual common_types::TContextId getListenedContextId() override;
    virtual common_types::TMissionId getListenedMissionId() override;

    virtual void callbackObjectDetected( const common_types::SListenedObject & _obj ) override;

    void threadMaintenance();



    // data
    SState m_state;
    bool m_shutdownCalled;    
    std::vector<common_types::IObjectListeningObserver *> m_listenedObjectsObservers;


    // service    
    std::vector<common_types::IObjectListeningService *> m_listeningServices;
    std::unordered_map<common_types::TContextId, common_types::IObjectListeningService *> m_listeningServicesByCtxId;
    std::thread * m_trMaintenance;
    std::mutex m_muListeners;



};

#endif // SOURCE_MANAGER_H
