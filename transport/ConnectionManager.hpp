#pragma once
#include <map>

namespace chat{
    namespace transport{
        class Connection;

        /**
         * Storage for all connections
        */
        class ConnectionManager final{
        private: // == Members ==
            using IdMap = std::map<int, Connection*>;
            IdMap   connections_;
        public:
            ConnectionManager() = default;
            ~ConnectionManager();
            ConnectionManager( const ConnectionManager& ) = delete;
            ConnectionManager& operator=( const ConnectionManager& ) = delete;
        public: // == Methods ==
            Connection* findConnection( int id );
            bool addConnection( Connection* c );
            bool eraseConnection( Connection* c );
            void clean();
        };
    }
}