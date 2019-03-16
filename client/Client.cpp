#include "Client.hpp"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

#include <iostream>

namespace rj = rapidjson;

namespace chat{
namespace client{

ChatClient::ChatClient( const std::string& host, const std::string& port )
: socket_( ioService_ )
, r_( ioService_ )
, host_( host )
, port_( port )
, msgBuffer_( MaxSize + 1 ){
}

ChatClient::~ChatClient(){
    shutdown();
    join();
}

std::future<bool> ChatClient::run(){
    initP_ = {};
    isRunning_ = true;
    ioRunner_ = std::thread{ [ this ](){ ioService_.run(); } };
    r_.async_resolve( { host_, port_ }, [ this ]( const boost::system::error_code& rc, io::ip::tcp::resolver::iterator it ){
        if( !rc ){
            std::cerr << "11111\n";
            boost::asio::async_connect( socket_, it, [ this ]( const boost::system::error_code& rc, io::ip::tcp::resolver::iterator ){
                if( rc ){
                    std::cerr << "connect err: " << rc.message() << std::endl;
                    initP_.set_value( false );
                }else{
                    initP_.set_value( true );
                    read();
                }
            } );
        }else{
            std::cerr << "222222: " << rc.message() << std::endl;
            initP_.set_value( false );
        }
    } );
    return initP_.get_future();
}

void ChatClient::shutdown(){
    isRunning_ = false;
    socket_.close();
    ioService_.stop();
}

void ChatClient::join(){
    ioRunner_.join();
}

void ChatClient::read(){
    std::cerr << "start read\n";
    boost::asio::async_read( socket_, boost::asio::buffer( &msgBuffer_[ 0 ], MaxSize ), boost::asio::transfer_at_least( 1 ), 
    [ this ]( const boost::system::error_code& rc, size_t len ){
        if( !rc ){
            // parse
            msgBuffer_[ len ] = '\0';
            std::cerr << "Received: " << &msgBuffer_[ 0 ] << std::endl;
            read();
        }else{
            std::cerr << "read err: " << rc.message() << std::endl;
            shutdown();
        }
    });
}

void ChatClient::write( std::shared_ptr<rapidjson::Document> doc ){
    // ioService_.post( [ this, doc ]()
    {
        rj::StringBuffer buffer;
        rj::Writer<rj::StringBuffer> writer( buffer );
        doc->Accept( writer );
        boost::asio::async_write( socket_, boost::asio::buffer( buffer.GetString(), buffer.GetSize() ), [ this ]( const boost::system::error_code& rc, size_t len ){
            if( !rc ){
                std::cerr << "written " << len << " bytes\n";
                // read();
            }else{
                
                std::cerr << rc.message() << std::endl;
                shutdown();
            }
        } );
    }; 
    
}

}
}