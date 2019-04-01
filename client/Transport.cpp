#include "Transport.hpp"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/error/en.h"

#include <QDebug>

#include <memory>

namespace rj = rapidjson;

namespace FiveInRow{
namespace transport{

Transport::Transport( const std::string& host, const std::string& port )
: socket_( ioService_ )
, r_( ioService_ )
, host_( host )
, port_( port )
, msgBuffer_( MaxSize + 1 ){
}

Transport::~Transport(){
   
}

void Transport::run(){
    isRunning_ = true;
    qDebug() << __func__ << "enter";
    ioRunner_ = std::thread{ [ this ](){ ioService_.run(); } };
    r_.async_resolve( { host_, port_ }, [ this ]( const boost::system::error_code& rc, io::ip::tcp::resolver::iterator it ){
        if( !rc ){
            boost::asio::async_connect( socket_, it, [ this ]( const boost::system::error_code& rc, io::ip::tcp::resolver::iterator ){
                if( rc ){
                    qCritical() << "connect err: " << rc.message().c_str();
                }else{
                    rif_->onConnected();
                    startRead();
                }
            } );
        }else{
            qCritical() << "222222: " << rc.message().c_str();
        }
    } );
    qDebug() << __func__ << "exit";
}

void Transport::shutdown(){
    isRunning_ = false;
    socket_.close();
    ioService_.stop();
}

void Transport::join(){
    if( ioRunner_.joinable() )
        ioRunner_.join();
}

void Transport::startRead(){
    namespace rj = rapidjson;
    qDebug() << "start read\n";
    boost::asio::async_read( socket_, boost::asio::buffer( &msgBuffer_[ 0 ], MaxSize ), boost::asio::transfer_at_least( 1 ), 
    [ this ]( const boost::system::error_code& rc, size_t len ){
        if( !rc ){
            // parse
            msgBuffer_[ len ] = '\0';

            std::string src = &msgBuffer_[ 0 ] ;

            int braceCount = 0;
            int sz = src.size();

            for( int idx = 0, prevIdx = 0; idx < sz; idx++ ){
                if( src[ idx ] == '{' )
                    braceCount++;
                else if( src[ idx ] == '}' )
                    braceCount--;

                if( braceCount == 0 ){
                    std::string cmd = src.substr( prevIdx, idx - prevIdx + 1 );
                    qDebug() << "Received doc src: " << cmd.c_str() ;
                    prevIdx = idx + 1;
                    rj::Document doc;
                    doc.Parse( cmd.c_str() );

                    if( doc.HasParseError() ){
                        rif_->onParseError( rj::GetParseError_En( doc.GetParseError() ) );
                    }else{
                        rif_->onReadCompleted( doc );
                    }
                }
            }


            startRead();
        }else{
            qCritical() << "read err: " << rc.message().c_str() ;
        }
    });
}

void Transport::send( std::shared_ptr<rapidjson::Document> doc ){
    ioService_.post( [ doc, this ](){
        q_.push_back( doc );
        startWrite();
    } );
}

void Transport::startWrite(){
    if( !q_.empty() ){
        rj::StringBuffer buffer;
        rj::Writer<rj::StringBuffer> writer( buffer );
        auto doc = q_.front();
        q_.pop_front();
        doc->Accept( writer );
        boost::asio::async_write( socket_, boost::asio::buffer( buffer.GetString(), buffer.GetSize() ), [ this ]( const boost::system::error_code& rc, size_t len ){
            if( !rc ){
                qDebug() << "written " << len << " bytes";
                startWrite();
            }else{
                rif_->onWriteError( rc.message().c_str() );
            }
        } );
    }
}

}
}
