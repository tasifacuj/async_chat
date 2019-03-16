#include "ChatConnection.hpp"
#include "ConnectionManager.hpp"
#include "interfaces/DispatcherInterface.hpp"
#include "Common.hpp"

#include "rapidjson/error/en.h"
#include "rapidjson/pointer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#include <iostream>

namespace rj = rapidjson;

namespace chat{
namespace transport{

ChatConnection::ChatConnection(std::shared_ptr<boost::asio::ip::tcp::socket>        s, boost::asio::io_service& ioService, dispatcher::DispatcherInterface& d, ConnectionManager& cm )
: Connection( s->native_handle(), cm )
, socket_( s )
, ioService_( ioService )
, d_( d ){
    std::cerr << "ChatConnection::ChatConnection: " << this << " -> " << socket_->native_handle() << std::endl;
    processRxAll();
}

ChatConnection::~ChatConnection(){
    std::cerr << "ChatConnection::~ChatConnection: " << this << " -> " << socket_->native_handle() << std::endl;
}

void ChatConnection::processRxAll(){
    msgBuffer_.clear();
    msgBuffer_.resize( MaxSize );
    std::cerr << "Start read from " << nativeHandle() << std::endl;
    
    boost::asio::async_read( *socket_, boost::asio::buffer( &msgBuffer_[ 0 ], MaxSize ), boost::asio::transfer_at_least( 1 ),
    [ this ]( const boost::system::error_code& ec, std::size_t len ){
        if( !ec ){
            // msgBuffer_[ len ] = '\0';

 
            try{
                auto doc = std::make_shared<rj::Document>();
                doc->Parse( &msgBuffer_[ 0 ] );
                
                if( doc->HasParseError() ){
                    throw std::runtime_error( rj::GetParseError_En( doc->GetParseError() ) );
                }else{


                    if( !doc->HasMember( "cookie" ) )
                        throw std::runtime_error( "No cookies" );

                    rj::SetValueByPointer( *doc, "/cookie/handle", nativeHandle() );
                    d_.dispatchIncoming( doc );
                }
            }catch( const std::exception& ex ){
                std::cerr << ex.what() << std::endl;
            }
            processRxAll();
        }else{
            std::cerr << "ALARM!!!!!!!!!!!!!!!\n";
            release();
        }
    });

}

bool ChatConnection::requestWrite( std::shared_ptr<rapidjson::Document> doc ){
    rj::EraseValueByPointer( *doc, "/cookie/handle" );
    rj::StringBuffer buffer;
    rj::Writer<rj::StringBuffer> writer( buffer );
    doc->Accept( writer );
    std::cerr << "send response " << buffer.GetString() << std::endl;
    boost::asio::async_write( *socket_, boost::asio::buffer( buffer.GetString(), buffer.GetLength() ),
    [ this ]( boost::system::error_code ec, std::size_t len ){
        if( !ec ){
            std::cerr << "sent " << len << " bytes into " << nativeHandle() << std::endl;
        }
    });
}

}
}