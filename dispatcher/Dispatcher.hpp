#pragma once
#include "interfaces/DispatcherInterface.hpp"

#include <boost/asio.hpp>
#include <boost/bimap.hpp>
#include <boost/bimap/set_of.hpp>

namespace chat{
    /// Forward declarations.
    namespace transport{
        class TransportInterface;
    }
    namespace dispatcher{
        namespace rj = rapidjson;
        /**
         * Main purpose of this class is to manage incoming messages.
         * Select appropriate session, based on session params, provided inside message.
         * Session is to be implemented
        */
        class Dispatcher : public DispatcherInterface{
        private: // == Members ==
            transport::TransportInterface&  transport_;     //!< transport layer.
            boost::asio::io_service&        ioService_;     //!< async task scheduler.
            using AppSessionSet = boost::bimaps::bimap
            <
              boost::bimaps::set_of<std::string>,
              boost::bimaps::set_of<int>
            >;
            AppSessionSet                   appSessions_;       //!< application sessions storage.
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
            virtual void onDisconnected( int handle ) override;
        private:
            void slotDispatch( std::shared_ptr<rapidjson::Document> doc );    //!< handler routine.
            void dispatchRegister( std::shared_ptr<rapidjson::Document> doc );
            void dispatchInvite( std::shared_ptr<rapidjson::Document> doc );
            void dispatchMessage( std::shared_ptr<rapidjson::Document> doc );
            void dispatchQuery( std::shared_ptr<rapidjson::Document> doc );
        };
    }
}