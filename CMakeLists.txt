cmake_minimum_required( VERSION 2.8 )

project( chat_server )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -g" )

find_package( Boost 1.58 COMPONENTS system REQUIRED )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} ${Boost_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/rapidjson/include )

set( SRC_LIST dispatcher/Dispatcher.cpp
    transport/Connection.cpp
    transport/ConnectionManager.cpp
    transport/ChatConnection.cpp
    transport/Transport.cpp
    Service.cpp
    chat.cpp
)

add_executable( chat_server ${SRC_LIST} )
target_link_libraries( chat_server LINK_PUBLIC ${Boost_LIBRARIES} )

