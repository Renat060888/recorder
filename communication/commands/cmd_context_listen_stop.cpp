
#include "datasource/source_manager_facade.h"
#include "cmd_context_listen_stop.h"

using namespace std;

CommandContextListenStop::CommandContextListenStop( common_types::SIncomingCommandServices * _services )
    : ICommandExternal(_services)
{

}

bool CommandContextListenStop::exec(){

    SourceManagerFacade * srcManager = ((common_types::SIncomingCommandServices *)m_services)->sourceManager;

    srcManager->stopListenContext( m_contextName );





    return true;
}
