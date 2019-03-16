#include "Transport.hpp"
#include "ChatConnection.hpp"
#include "interfaces/DispatcherInterface.hpp"

#include "rapidjson/pointer.h"

#include <iostream>
#include <cassert>
namespace chat{
namespace transport{

Transport::Transport( boost::asio::io_service& service, const boost::asio::ip::tcp::endpoint& endp )
: ioService_( service )
, acceptor_( ioService_, endp )
, d_( nullptr )
{
}

Transport::~Transport(){
    connectionManager_.clean();
}

void Transport::startAccept(){
    std::cerr << __func__  << std::endl;
    std::shared_ptr<boost::asio::ip::tcp::socket>        socket = std::make_shared<boost::asio::ip::tcp::socket>( ioService_ );
    acceptor_.async_accept( *socket, [ this, socket ]( boost::system::error_code ec ){
        if( !ec ){
            assert( d_ );
            new ChatConnection( socket, ioService_, *d_, connectionManager_ );
        }else{
            std::cerr << "UUUUUUUUUUUUUUUUU\n";
        }
        startAccept();
    } );
}

void Transport::sendMessage( std::shared_ptr<rapidjson::Document> msg ){
    ioService_.post( [ this, msg ](){
        try{
            rapidjson::Value* handleV = rapidjson::GetValueByPointer( *msg, "/cookie/handle" );
            assert( handleV );
            Connection* conn = connectionManager_.findConnection( handleV->GetInt() );

            if( nullptr != conn ){
                conn->requestWrite( msg );
            }else{
                std::cerr << "No connection for handle " << handleV->GetInt() << std::endl;
            }
        }catch( const std::exception& ex ){
            std::cerr << ex.what() << std::endl;
        }
    } );
}

void Transport::setDispatcher( dispatcher::DispatcherInterface* d ){
    d_ = d;
}

}
}