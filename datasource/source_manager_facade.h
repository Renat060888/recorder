#ifndef SOURCE_MANAGER_H
#define SOURCE_MANAGER_H

#include <thread>
#include <mutex>

#include "common/common_types.h"

class SourceManagerFacade : public common_types::IServiceObjectListener, public common_types::IListenedObjectObserver
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

    common_types::IServiceObjectListener * getServiceOfObjectListener();


private:
    virtual void addObserver( common_types::IListenedObjectObserver * _observer ) override;
    virtual void removeObserver( common_types::IListenedObjectObserver * _observer ) override;
    virtual void runListenCycle() override;
    virtual common_types::TContextId getListenedContextId() override;

    virtual void callbackObjectDetected( const common_types::SListenedObject & _obj ) override;

    void threadMaintenance();



    // data
    bool m_shutdownCalled;
    std::vector<common_types::IListenedObjectObserver *> m_listenedObjectsObservers;


    // service    
    std::vector<common_types::IServiceObjectListener *> m_listeningServices;
    std::thread * m_trMaintenance;
    std::mutex m_muListeners;



};

#endif // SOURCE_MANAGER_H
