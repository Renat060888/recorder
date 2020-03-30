
#include "args_parser.h"

using namespace std;

bpo::options_description ArgsParser::getArgumentsDescrTemplateMethodPart() {

    bpo::options_description desc("Available options");

    desc.add_options()
            ("help,h","show this help")
            ("version,V", "version of program")
            ("about,A", "description")
            ("main-config,C", bpo::value<std::string>(), "main config file")
            ("main-config-sample,P", "print main config sample")
            ("daemon,D", "start as daemon")
            ("start-agent", "start recorder agent")
            ("start-contoller", "start recorder controller")
            ("stop", "stop recorder agent")
            ;

    return desc;
}

void ArgsParser::checkArgumentsTemplateMethodPart( const bpo::variables_map & _varMap ) {

    if( _varMap.find("main-config") != _varMap.end() ){
        m_commmandLineArgs[EPlayerArguments::MAIN_CONFIG_PATH_FROM_CONSOLE] = _varMap["main-config"].as<std::string>();
    }

    if( _varMap.find("daemon") != _varMap.end() ){
        m_commmandLineArgs[EPlayerArguments::AS_DAEMON] = "bla-bla";
    }

    if( _varMap.find("start-agent") != _varMap.end() ){
        m_commmandLineArgs[EPlayerArguments::SHELL_COMMAND_START_RECORDER_AGENT] = "bla-bla";
    }

    if( _varMap.find("start-contoller") != _varMap.end() ){
        m_commmandLineArgs[EPlayerArguments::SHELL_COMMAND_START_RECORDER_CONTROLLER] = "bla-bla";
    }

    if( _varMap.find("stop") != _varMap.end() ){
        m_commmandLineArgs[EPlayerArguments::SHELL_COMMAND_TO_RECORDER]
            = AArgsParser::getSettings().commandConvertor->getCommandsFromProgramArgs( { {"", ""} } );
    }
}

void ArgsParser::version() {

    // TODO: do
}

void ArgsParser::about() {

    // TODO: do
}
