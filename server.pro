TEMPLATE = app

CONFIG += c++14

gcc:QMAKE_CXXFLAGS_WARN_ON += -Wall -Wextra -Werror=return-type -Werror=reorder  -Wno-unused-parameter -Wno-unused -Wno-empty-body 

warn_error:QMAKE_CXXFLAGS_WARN_ON += -Werror

SOURCES += dispatcher/Dispatcher.cpp\
    transport/Connection.cpp\
    transport/ConnectionManager.cpp\
    transport/ChatConnection.cpp\
    transport/Transport.cpp\
    Service.cpp\
    chat.cpp\

HEADERS +=\
    transport/Connection.hpp\
    transport/ConnectionManager.hpp\
    transport/ChatConnection.hpp\
    transport/Transport.hpp\
    Service.hpp\

unix {
    target.path = /home/vida/bin
    INSTALLS += target
}

INCLUDEPATH += 3rdParty/rapidjson/include

LIBS += -lboost_system

DISTFILES += \
    transport/doc/api.json
