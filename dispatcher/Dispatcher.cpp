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
    rapidjson::Value* sid = rapidjson::GetValueByPointer( *doc, "/cookie/sid" );
    assert( sid );
    rapidjson::Value* handle = rapidjson::GetValueByPointer( *doc, "/cookie/handle" );
    assert( handle );
    rapidjson::Value* method = rapidjson::GetValueByPointer( *doc, "/request/method" );
    assert( method );
    rapidjson::Value* from = rapidjson::GetValueByPointer( *doc, "/request/from" );
    assert( from );
    std::cerr << "Received " << method->GetString() << " from " << from->GetString() << ", sid: " << sid->GetString() << ", handle: " << handle->GetInt();
    auto response = MakeResponse( 200, "OK" );
    rj::SetValueByPointer( *response, "/cookie/sid", *sid );
    rj::SetValueByPointer( *response, "/cookie/handle", *handle );
    transport_.sendMessage( response );
}

}
}