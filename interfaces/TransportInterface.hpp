#pragma once
#include "rapidjson/document.h"
#include <memory>

namespace chat{
    namespace dispatcher{
        class DispatcherInterface;
    }

    namespace transport{
        class TransportInterface{
        public:
            virtual ~TransportInterface(){}
            virtual void sendMessage( std::shared_ptr<rapidjson::Document> doc ) = 0;
            virtual void setDispatcher( dispatcher::DispatcherInterface* d ) = 0;
        };
    }
}