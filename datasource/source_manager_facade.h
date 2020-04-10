#ifndef SOURCE_MANAGER_H
#define SOURCE_MANAGER_H

#include <thread>
#include <mutex>

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

    SourceManagerFacade();
    ~SourceManagerFacade();

    bool init( const SInitSettings & _settings );
    void shutdown();

    bool startListenContext( const std::string & _ctxName );
    void stopListenContext( const std::string & _ctxName );

    common_types::IObjectListeningService * getServiceOfObjectListener();


private:
    virtual void addObserver( common_types::IObjectListeningObserver * _observer ) override;
    virtual void removeObserver( common_types::IObjectListeningObserver * _observer ) override;
    virtual void runListenCycle() override;
    virtual common_types::TContextId getListenedContextId() override;
    virtual common_types::TMissionId getListenedMissionId() override;

    virtual void callbackObjectDetected( const common_types::SListenedObject & _obj ) override;

    void threadMaintenance();



    // data
    bool m_shutdownCalled;
    std::vector<common_types::IObjectListeningObserver *> m_listenedObjectsObservers;


    // service    
    std::vector<common_types::IObjectListeningService *> m_listeningServices;
    std::thread * m_trMaintenance;
    std::mutex m_muListeners;



};

#endif // SOURCE_MANAGER_H
