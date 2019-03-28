#pragma once
#include "rapidjson/document.h"
#include <memory>

namespace chat{
    namespace dispatcher{
        /**
         * Layer, that provides means to process messages.
         * Also it's purpose is to forward messages from DispatcherInterface clients to transport layer.
        */
        class DispatcherInterface{
        public:
            virtual ~DispatcherInterface(){}
            virtual void dispatchIncoming( std::shared_ptr<rapidjson::Document> doc ) = 0;
            virtual void send( std::shared_ptr<rapidjson::Document> doc ) = 0;
            virtual void onDisconnected( int handle ) = 0;
        };
    }
}