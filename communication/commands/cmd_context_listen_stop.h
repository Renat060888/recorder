#ifndef CMD_CONTEXT_CLOSE_H
#define CMD_CONTEXT_CLOSE_H

#include <microservice_common/communication/i_command_external.h>

#include "common/common_types.h"

class CommandContextListenStop : public ICommandExternal
{
    friend class CommandFactory;
public:
    CommandContextListenStop( common_types::SIncomingCommandServices * _services );

    virtual bool exec() override;


private:
    std::string m_contextName;


};
using PCommandContextListenStop = std::shared_ptr<CommandContextListenStop>;

#endif // CMD_CONTEXT_CLOSE_H
