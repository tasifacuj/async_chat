#pragma once
#include "rapidjson/document.h"
#include <memory>

namespace chat{
    namespace transport{
        class ConnectionManager;

        /**
         * Base connection class, that manages connection life time.
         * Provides means to read data and send outgoing data,
        */
        class Connection{
        public:
            Connection( int handle, ConnectionManager& mgr );
            virtual ~Connection();
        public://== virtual methods ==
            /// send message into appropriate whole.
            virtual bool requestWrite( std::shared_ptr<rapidjson::Document> ) = 0;
            /// do cleanup and kill self.
            virtual void release();
        public: // ==Methods==
            int nativeHandle(){ return nativeHandle_; }
        private: // == Members ==
            int                 nativeHandle_ = 0;
            ConnectionManager&  connectionManager_;
        };
    }
}