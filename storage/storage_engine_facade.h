#ifndef STORAGE_ENGINE_FACADE_H
#define STORAGE_ENGINE_FACADE_H

#include <thread>
#include <mutex>

#include "common/common_types.h"
#include "context.h"

class StorageEngineFacade : public common_types::IObjectListeningObserver
{
public:
    struct SServiceLocator {
        common_types::IObjectListeningService * serviceObjectListener;
    };

    struct SInitSettings {
        SServiceLocator services;
    };

    StorageEngineFacade();
    ~StorageEngineFacade();

    bool init( const SInitSettings & _settings );
    void shutdown();


private:
    virtual void callbackObjectDetected( const common_types::SListenedObject & _obj ) override;

    void threadMaintenance();




    // data
    bool m_shutdownCalled;
    std::vector<PContext> m_contexts;
    std::unordered_map<common_types::TContextId, PContext> m_contextById;


    // service
    std::thread * m_trMaintenance;
    std::mutex m_muContexts;




};

#endif // STORAGE_ENGINE_FACADE_H

