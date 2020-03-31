ROOT_DIR=../

TEMPLATE = app
TARGET = recorder

include($${ROOT_DIR}pri/common.pri)

CONFIG -= qt

QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -Wno-unused-variable
q
# TODO: add defines to logger, system monitor, restbed webserver, database, etc...
DEFINES += \
    SWITCH_LOGGER_SIMPLE \
#    OBJREPR_LIBRARY_EXIST \

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
    communication/commands/cmd_context_close.cpp \
    communication/commands/cmd_context_open.cpp \
    communication/commands/cmd_player_controller_ping.cpp \
    communication/commands/cmd_user_ping.cpp \
    communication/commands/cmd_user_register.cpp \
    communication/command_factory.cpp \
    datasource/mirror_object_listener_objrepr.cpp \
    datasource/object_listener_imitator.cpp \
    datasource/object_listener_mqtt.cpp \
    datasource/object_listener_objrepr.cpp \
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
    datasource/source_manager_facade.cpp

HEADERS += \
    communication/commands/cmd_context_close.h \
    communication/commands/cmd_context_open.h \
    communication/commands/cmd_player_controller_ping.h \
    communication/commands/cmd_user_ping.h \
    communication/commands/cmd_user_register.h \
    communication/command_factory.h \
    datasource/mirror_object_listener_objrepr.h \
    datasource/object_listener_imitator.h \
    datasource/object_listener_mqtt.h \
    datasource/object_listener_objrepr.h \
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
    datasource/source_manager_facade.h
