#include "Service.hpp"
namespace chat{

Service::Service()
: transport_( ioService_, boost::asio::ip::tcp::endpoint( boost::asio::ip::tcp::v4(), 2504 ) )
, dispatcher_( std::make_unique<dispatcher::Dispatcher>( transport_, ioService_ ) ){
    transport_.setDispatcher( dispatcher_.get() );
}

void Service::run(){
    transport_.startAccept();
    ioService_.run();
}

}