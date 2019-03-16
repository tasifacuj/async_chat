#include "Service.hpp"

#include <iostream>

using namespace chat;

int main(){
    try{
        Service service;
        service.run();
    }catch( const std::exception& ex ){
        std::cerr << ex.what() << std::endl;
        return -1;
    }
    return 0;
}