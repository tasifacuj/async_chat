#include "ConnectionManager.hpp"
#include "Connection.hpp"

#include <list>
#include <algorithm>

namespace chat{
namespace transport{

ConnectionManager::~ConnectionManager(){
    clean();
}

Connection* ConnectionManager::findConnection( int id ){
    auto it = connections_.find( id );
    return it != connections_.end() ? it->second : nullptr;
}

bool ConnectionManager::addConnection( Connection* c ){
    auto rv = connections_.insert( { c->nativeHandle(), c } );
    return rv.second;
}

bool ConnectionManager::eraseConnection( Connection* c ){
    auto cnt = connections_.erase( c->nativeHandle() );
    return cnt > 0;
}

void ConnectionManager::clean(){
    if( !connections_.empty() ){
        std::list<Connection*> to_delete;
        std::for_each( connections_.begin(), connections_.end(), [ &to_delete ](auto& p){ to_delete.push_back( p.second ); } );
        std::for_each( to_delete.begin(), to_delete.end(), []( Connection* c ){ c->release(); } );
    }
}

}
}