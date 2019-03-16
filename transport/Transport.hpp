#pragma once
#include <boost/asio.hpp>
#include <memory>

#include "interfaces/TransportInterface.hpp"
#include "ConnectionManager.hpp"
namespace chat{
    namespace dispatcher{
        class DispatcherInterface;
    }
    namespace transport{
        /**
         * Boost-based implementation of transport layer.
        */
        class Transport : public TransportInterface{
        private: // == Members ==
            boost::asio::io_service&            ioService_;         //!< async io runner
            boost::asio::ip::tcp::acceptor      acceptor_;          //!< tcp connection acceptor
            dispatcher::DispatcherInterface*    d_ = nullptr;       //!< dispatcher layer
            ConnectionManager                   connectionManager_; //!< conn mgr.
            
        public: // == Ctors ==
            Transport( boost::asio::io_service& service, const boost::asio::ip::tcp::endpoint& endp );
            Transport( const Transport& ) = delete;
            Transport& operator=( const Transport& ) = delete;
            virtual ~Transport();
        public: // == Methods ==
            virtual void sendMessage( std::shared_ptr<rapidjson::Document> msg ) override;
            virtual void setDispatcher( dispatcher::DispatcherInterface* d ) override;
        public:// == Methods ==
            void startAccept();
        };
    }
} // chat
