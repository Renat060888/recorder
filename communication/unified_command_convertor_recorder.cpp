
#include "unified_command_convertor_recorder.h"

using namespace std;

UnifiedCommandConvertorRecorder::UnifiedCommandConvertorRecorder()
{

}

std::string UnifiedCommandConvertorRecorder::getCommandsFromProgramArgs( const std::map<common_types::TCommandLineArgKey, common_types::TCommandLineArgVal> & _args ){

    assert( false && "TODO: do" );
}

std::string UnifiedCommandConvertorRecorder::getCommandsFromConfigFile( const std::string & _commands ){

    assert( false && "TODO: do" );
}

std::string UnifiedCommandConvertorRecorder::getCommandsFromHTTPRequest( const std::string & _httpMethod,
                                        const std::string & _uri,
                                        const std::string & _queryString,
                                        const std::string & _body ){

    assert( false && "TODO: do" );
}
