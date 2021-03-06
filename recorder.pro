ROOT_DIR=../

TEMPLATE = app
TARGET = recorder

include($${ROOT_DIR}pri/common.pri)

CONFIG -= qt

QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -Wno-unused-variable

# TODO: add defines to logger, system monitor, restbed webserver, database, etc...
DEFINES += \
    SWITCH_LOGGER_SIMPLE \
#    SWITCH_LOGGER_ASTRA \
#    OBJREPR_LIBRARY_EXIST \
#    RUN_UNIT_TESTS \

# NOTE: paths for dev environment ( all projects sources in one dir )
INCLUDEPATH += \
    /usr/include/libgtop-2.0 \
    /usr/include/libmongoc-1.0 \
    /usr/include/libbson-1.0 \
    $${ROOT_DIR}/microservice_common/ \

LIBS += \
    -lpthread \
    -lmongoc-1.0 \
    -lbson-1.0 \
    -lboost_regex \
    -lboost_system \
    -lboost_filesystem \
    -lboost_program_options \ # TODO: wtf?
    -ljsoncpp \
    -lmosquitto \ 
    -lmicroservice_common \

contains( DEFINES, OBJREPR_LIBRARY_EXIST ){
    message("connect 'unilog' and 'objrepr' libraries")
LIBS += \
    -lunilog \
    -lobjrepr
}

SOURCES += main.cpp \
    communication/commands/cmd_context_listen_start.cpp \
    communication/commands/cmd_context_listen_stop.cpp \
    communication/commands/cmd_player_controller_ping.cpp \
    communication/commands/cmd_user_ping.cpp \
    communication/commands/cmd_user_register.cpp \
    communication/command_factory.cpp \
    datasource/object_listener_imitator.cpp \
    datasource/object_listener_mqtt.cpp \
    datasource/object_listener_objrepr.cpp \
    datasource/proxy_object_listener_objrepr.cpp \
    recorder_controller.cpp \
    storage/database_manager.cpp \
    storage/storage_engine_facade.cpp \
    system/args_parser.cpp \
    system/config_reader.cpp \
    system/path_locator.cpp \
    recorder_agent.cpp \    
    communication/communication_gateway_facade_recorder.cpp \
    communication/unified_command_convertor_recorder.cpp \
    storage/context.cpp \
    datasource/mqtt_client.cpp \
    common/common_types.cpp \
    datasource/source_manager_facade.cpp \
    system/system_environment_facade_recorder.cpp \
    system/objrepr_bus_recorder.cpp

HEADERS += \
    communication/commands/cmd_context_listen_start.h \
    communication/commands/cmd_context_listen_stop.h \
    communication/commands/cmd_player_controller_ping.h \
    communication/commands/cmd_user_ping.h \
    communication/commands/cmd_user_register.h \
    communication/command_factory.h \
    datasource/object_listener_imitator.h \
    datasource/object_listener_mqtt.h \
    datasource/object_listener_objrepr.h \
    datasource/proxy_object_listener_objrepr.h \
    recorder_controller.h \
    storage/database_manager.h \
    storage/storage_engine_facade.h \
    system/args_parser.h \
    system/config_reader.h \
    system/path_locator.h \
    common/common_types.h \
    common/common_utils.h \
    common/common_vars.h \
    recorder_agent.h \    
    communication/communication_gateway_facade_recorder.h \
    communication/unified_command_convertor_recorder.h \
    analyze/dummy.h \
    storage/context.h \
    datasource/mqtt_client.h \
    datasource/source_manager_facade.h \
    system/system_environment_facade_recorder.h \
    system/objrepr_bus_recorder.h
