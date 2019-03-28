#include "Dispatcher.hpp"
#include "interfaces/TransportInterface.hpp"
#include "rapidjson/pointer.h"
#include "Common.hpp"

#include <iostream>

namespace chat{
namespace dispatcher{

Dispatcher::Dispatcher( transport::TransportInterface& t,  boost::asio::io_service& service )
: transport_( t )
, ioService_( service )
{
}

Dispatcher::~Dispatcher(){}

void Dispatcher::dispatchIncoming( std::shared_ptr<rapidjson::Document> doc ){
    ioService_.post( [ this, doc ](){ slotDispatch( doc ); } );
}

void Dispatcher::slotDispatch( std::shared_ptr<rapidjson::Document> doc ){
    // rapidjson::Value* sid = rapidjson::GetValueByPointer( *doc, "/cookie/sid" );
    // assert( sid );
    // rapidjson::Value* handle = rapidjson::GetValueByPointer( *doc, "/cookie/handle" );
    // assert( handle );
    // rapidjson::Value* method = rapidjson::GetValueByPointer( *doc, "/request/method" );
    // assert( method );
    // rapidjson::Value* from = rapidjson::GetValueByPointer( *doc, "/request/from" );
    // assert( from );
    // std::cerr << "Received " << method->GetString() << " from " << from->GetString() << ", sid: " << sid->GetString() << ", handle: " << handle->GetInt();
    // auto response = MakeResponse( 200, "OK" );
    // rj::SetValueByPointer( *response, "/cookie/sid", *sid );
    // rj::SetValueByPointer( *response, "/cookie/handle", *handle );
    // transport_.sendMessage( response );
    if( rapidjson::GetValueByPointer( *doc, "/request/register" ) )
        dispatchRegister( doc );
    else if( rapidjson::GetValueByPointer( *doc, "/request/invite" ) )
        dispatchInvite( doc );
    else if( rapidjson::GetValueByPointer( *doc, "/request/message" ) )
        dispatchMessage( doc );
    else if( rapidjson::GetValueByPointer( *doc, "/request/query" ) )
        dispatchQuery( doc );
}

void Dispatcher::dispatchRegister( std::shared_ptr<rapidjson::Document> doc )try{
    int handle = ( *doc )[ "cookie" ][ "handle" ].GetInt();
    std::string from = ( *doc )[ "request" ][ "register" ][ "from" ].GetString();
    appSessions_.insert( AppSessionSet::value_type( from, handle ) );
    std::shared_ptr<rj::Document> response = std::make_shared<rj::Document>();
    rj::SetValueByPointer( *response, "/cookie/handle", handle );
    rj::SetValueByPointer( *response, "/status", "OK" );
    transport_.sendMessage( response );
}catch( const std::exception& ex ){
    std::cerr << ex.what() << std::endl;
}

void Dispatcher::dispatchInvite( std::shared_ptr<rapidjson::Document> doc )try{
    rj::Document& d = *doc;
    std::string from = d[ "request" ][ "invite" ][ "from" ].GetString();
    std::string to = d[ "request" ][ "invite" ][ "to" ].GetString();
    int handle = d[ "cookie" ][ "handle" ].GetInt();
    auto fromIt = appSessions_.left.find( from );
    auto toIt = appSessions_.left.find( to );
    std::shared_ptr<rj::Document> response = std::make_shared<rj::Document>();
    
    if( fromIt != appSessions_.left.end() and toIt != appSessions_.left.end() ){    
        rj::SetValueByPointer( *response, "/cookie/handle", handle );
        rj::SetValueByPointer( *response, "/status", "OK" );
        transport_.sendMessage( response );

        int otherHandle = toIt->second;
        rj::SetValueByPointer( *doc, "/cookie/handle", otherHandle );
        transport_.sendMessage( doc );
    }else{
        rj::SetValueByPointer( *response, "/cookie/handle", handle );
        rj::SetValueByPointer( *response, "/status", "Not found" );
        transport_.sendMessage( response );
    }
}catch( const std::exception& ex ){
    std::cerr << ex.what() << std::endl;
}

void Dispatcher::dispatchMessage( std::shared_ptr<rapidjson::Document> doc )try{
    rj::Document& d = *doc;
    std::string from = d[ "request" ][ "message" ][ "from" ].GetString();
    std::string to = d[ "request" ][ "message" ][ "to" ].GetString();
    int handle = d[ "cookie" ][ "handle" ].GetInt();
    auto fromIt = appSessions_.left.find( from );
    auto toIt = appSessions_.left.find( to );
    std::shared_ptr<rj::Document> response = std::make_shared<rj::Document>();
    
    if( fromIt != appSessions_.left.end() and toIt != appSessions_.left.end() ){    
        rj::SetValueByPointer( *response, "/cookie/handle", handle );
        rj::SetValueByPointer( *response, "/status", "OK" );
        transport_.sendMessage( response );

        int otherHandle = toIt->second;
        rj::SetValueByPointer( *doc, "/cookie/handle", otherHandle );
        transport_.sendMessage( doc );
    }else{
        rj::SetValueByPointer( *response, "/cookie/handle", handle );
        rj::SetValueByPointer( *response, "/status", "Not found" );
        transport_.sendMessage( response );
    }
}catch( const std::exception& ex ){
    std::cerr << ex.what() << std::endl;
}

void Dispatcher::dispatchQuery( std::shared_ptr<rapidjson::Document> doc )try{
    rj::Document& d = *doc;
    int handle = d[ "cookie" ][ "handle" ].GetInt();
    std::shared_ptr<rj::Document> response = std::make_shared<rj::Document>();
    rj::SetValueByPointer( *response, "/cookie/handle", handle );
    rj::Value participants( rj::kArrayType );
    // std::cerr <<  "Have "
    std::for_each( appSessions_.begin(), appSessions_.end(), [ response, &participants ]( const AppSessionSet::value_type& vt ){
        rj::Value p( vt.left.c_str(), vt.left.size(), response->GetAllocator() );
        participants.PushBack( p, response->GetAllocator() );
    } );
    response->AddMember( "participants", participants, response->GetAllocator() );
    transport_.sendMessage( response );
}catch( const std::exception& ex ){
    std::cerr << ex.what() << std::endl;
}

void Dispatcher::onDisconnected( int handle ){
    appSessions_.right.erase( handle );
}

}// dispatcher
}// chat