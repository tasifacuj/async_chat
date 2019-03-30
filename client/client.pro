TEMPLATE = app

QT += qml quick
CONFIG += c++14

gcc:QMAKE_CXXFLAGS_WARN_ON += -Wall -Wextra -Werror=return-type -Werror=reorder  -Wno-unused-parameter -Wno-unused -Wno-empty-body 

warn_error:QMAKE_CXXFLAGS_WARN_ON += -Werror

SOURCES += main.cpp \
    Client.cpp\
    GameViewModel.cpp\
    MainViewModel.cpp

HEADERS += Client.hpp\
    GameViewModel.hpp\
    MainViewModel.hpp\
    Tile.hpp\
    Client.hpp

unix {
    target.path = /home/vida/bin
    INSTALLS += target
}

RESOURCES += \
    client.qrc


INCLUDEPATH += ../3rdParty/rapidjson/include

LIBS += -lboost_system
