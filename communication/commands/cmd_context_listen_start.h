#ifndef CMD_CONTEXT_OPEN_H
#define CMD_CONTEXT_OPEN_H

#include <microservice_common/communication/i_command_external.h>

#include "common/common_types.h"

class CommandContextListenStart : public ICommandExternal
{
    friend class CommandFactory;
public:
    CommandContextListenStart( common_types::SIncomingCommandServices * _services );

    virtual bool exec() override;


private:    
    std::string m_contextName;
    std::string m_missionName;



};
using PCommandContextListenStart = std::shared_ptr<CommandContextListenStart>;

#endif // CMD_CONTEXT_OPEN_H
