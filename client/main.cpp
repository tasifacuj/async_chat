#include "client/Client.hpp"
#include "rapidjson/pointer.h"
#include "rapidjson/document.h"

#include <iostream>
#include <string>
using namespace chat::client;
namespace rj = rapidjson;

using namespace std::literals::chrono_literals;

int main(){
    try{
        ChatClient c( "127.0.0.1", "2504" );
        auto f = c.run();
        auto status = f.wait_for( 2s );
        
        if( status != std::future_status::ready ){
            std::cerr  << "Failed to start\n";
            return -1;
        }
        std::cerr << f.get() << '\n';
        auto doc = std::make_shared<rj::Document>();
        rj::SetValueByPointer( *doc, "/cookie/sid", "bbbzzz" );
        rj::SetValueByPointer( *doc, "/request/method", "REGISTER" );
        rj::SetValueByPointer( *doc, "/request/from", "tasifacij" );
        
        c.write( doc );
        std::this_thread::sleep_for( 5s );// wait for answer
        
    }catch( const std::exception& ex ){
        std::cerr << "FFFFF: " << ex.what();
        return -1;
    }

    return 0;
}