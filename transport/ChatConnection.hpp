#pragma once
#include <vector>
#include <boost/asio.hpp>

#include "Connection.hpp"

namespace chat{
    namespace dispatcher{
        class DispatcherInterface;
    }
    namespace transport{
        class ConnectionManager;

        //boost-based tcp connection
        class ChatConnection : public Connection{
        public: // == Constants ==
            static constexpr int MaxSize = 512;
        private: // ==Members ==
            std::shared_ptr<boost::asio::ip::tcp::socket>   socket_;        //!<< accepted socket
            boost::asio::io_service&                        ioService_;     //!< io runner
            dispatcher::DispatcherInterface&                d_;             //!< dispatching layer
            std::vector<char>                               msgBuffer_;
        public: // == Ctors ==
            ChatConnection(  std::shared_ptr<boost::asio::ip::tcp::socket> s, boost::asio::io_service& ioService, dispatcher::DispatcherInterface& d, ConnectionManager& cm );
            ChatConnection( const ChatConnection& ) = delete;
            ChatConnection& operator=( const ChatConnection& ) = delete;
            virtual ~ChatConnection();
        public: // == Methods ==
            virtual bool requestWrite( std::shared_ptr<rapidjson::Document> msg )   override;
            virtual void release()                                                  override;
        private:
            void processRxAll();
        };
    }
}
