
#include <jsoncpp/json/writer.h>

#include "cmd_user_ping.h"

using namespace std;

CommandUserPing::CommandUserPing( common_types::SIncomingCommandServices * _services )
    : ICommandExternal(_services)
{

}

bool CommandUserPing::exec(){

    Json::Value rootRecord;
    rootRecord[ "cmd_name" ] = "pong";
    rootRecord[ "player" ] = "you have a problem man!";
    rootRecord[ "error_occured" ] = true;
    rootRecord[ "code" ] = "NOT_REGISTERED";

    Json::FastWriter jsonWriter;
    sendResponse( jsonWriter.write(rootRecord) );
    return true;
}

