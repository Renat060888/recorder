
#include <jsoncpp/json/writer.h>

#include "cmd_user_register.h"

using namespace std;

CommandUserRegister::CommandUserRegister( common_types::SIncomingCommandServices * _services )
    : ICommandExternal(_services)
{

}

bool CommandUserRegister::exec(){

    Json::Value rootRecord;
    rootRecord[ "user_id" ] = "bla-bla";

    Json::FastWriter jsonWriter;
    sendResponse( jsonWriter.write(rootRecord) );
    return true;
}

