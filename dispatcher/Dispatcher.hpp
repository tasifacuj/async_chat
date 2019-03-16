#pragma once
#include "interfaces/DispatcherInterface.hpp"
#include <boost/asio.hpp>

namespace chat{
    /// Forward declarations.
    namespace transport{
        class TransportInterface;
    }
    namespace dispatcher{
        /**
         * Main purpose of this class is to manage incoming messages.
         * Select appropriate session, based on session params, provided inside message.
         * Session is to be implemented
        */
        class Dispatcher : public DispatcherInterface{
        private: // == Members ==
            transport::TransportInterface&  transport_;     //!< transport layer.
            boost::asio::io_service&        ioService_;     //!< async task scheduler.
        public:// == Ctors ==
            Dispatcher( transport::TransportInterface& t, boost::asio::io_service& service );
            Dispatcher( const Dispatcher& ) = delete;
            Dispatcher& operator=( const Dispatcher& ) = delete;
            virtual ~Dispatcher();
        public: // == Methods ==
            /// accept incoming message and schedule for further processing.
            virtual void dispatchIncoming( std::shared_ptr<rapidjson::Document> doc) override;
            /// provides client of DispatcherInterface an opportunity to send requests.
            /// Does requests interception. 
            virtual void send( std::shared_ptr<rapidjson::Document>  ) override{}
        private:
            void slotDispatch( std::shared_ptr<rapidjson::Document> doc );    //!< handler routine.
        };
    }
}