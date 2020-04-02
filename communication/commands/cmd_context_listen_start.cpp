
#include "cmd_context_listen_start.h"
#include "datasource/source_manager_facade.h"

using namespace std;

CommandContextListenStart::CommandContextListenStart( common_types::SIncomingCommandServices * _services )
    : ICommandExternal(_services)
{

}

bool CommandContextListenStart::exec(){

    SourceManagerFacade * srcManager = ((common_types::SIncomingCommandServices *)m_services)->sourceManager;

    if( srcManager->startListenContext( m_contextName ) ){





        return true;
    }
    else{




        return false;
    }
}


