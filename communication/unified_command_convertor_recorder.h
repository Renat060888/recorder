#ifndef UNIFIED_COMMAND_CONVERTOR_RECORDER_H
#define UNIFIED_COMMAND_CONVERTOR_RECORDER_H

#include <microservice_common/communication/unified_command_convertor.h>

class UnifiedCommandConvertorRecorder : public UnifiedCommandConvertor
{
public:
    static UnifiedCommandConvertorRecorder & singleton(){
        static UnifiedCommandConvertorRecorder instance;
        return instance;
    }


    virtual std::string getCommandFromProgramArgs( const std::map<common_types::TCommandLineArgKey, common_types::TCommandLineArgVal> & _args ) override;
    virtual std::string getCommandFromConfigFile( const std::string & _command ) override;
    virtual std::string getCommandFromHTTPRequest( const std::string & _httpMethod,
                                                    const std::string & _uri,
                                                    const std::string & _queryString,
                                                    const std::string & _body ) override;

private:
    UnifiedCommandConvertorRecorder();




};
#define UNIFIED_COMMAND_CONVERTOR UnifiedCommandConvertorRecorder::singleton()

#endif // UNIFIED_COMMAND_CONVERTOR_RECORDER_H
