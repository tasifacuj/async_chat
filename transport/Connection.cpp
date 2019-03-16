#include "Connection.hpp"
#include "ConnectionManager.hpp"

#include <iostream>
#include <cassert>

namespace chat{
namespace transport{

Connection::Connection( int handle, ConnectionManager& mgr )
: nativeHandle_( handle )
, connectionManager_( mgr ){
    std::cerr << "Connection::Connection: " << this << " -> " << nativeHandle_ << std::endl;
    assert( connectionManager_.addConnection( this ) );
}

Connection::~Connection(){
    connectionManager_.eraseConnection( this );
    std::cerr << "Connection::~Connection: " << this << " -> " << nativeHandle_ << std::endl;
}


void Connection::release(){
    delete this;
}

}// namespace transport
}// namespace chat