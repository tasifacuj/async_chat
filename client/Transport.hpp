#pragma once
#include <boost/asio.hpp>
#include <thread>
#include <string>
#include <future>
#include <memory>
#include <deque>

#include "rapidjson/document.h"
#include "TransportResponseInterface.hpp"

namespace io = boost::asio;

namespace FiveInRow{
    namespace transport{
        /**
         * boost driven tcp client
         * seds raw strings from input.
        */
        class Transport final{
        public: // == Constants ==
            static constexpr int MaxSize = 512;
            using ByteArray = std::vector<char>;
            using DocPtr = std::shared_ptr<rapidjson::Document>;
            using MessageQ = std::deque<DocPtr>;
        private: // == Memebers ==
            std::thread                 ioRunner_;      //!< io thread to avoid std input blocking.
            io::io_service              ioService_;     //!< io/task scheduler
            io::ip::tcp::socket         socket_;        // client sock.
            io::ip::tcp::resolver       r_;             // converts strig address into boost asio endpoint,
            std::string                 host_;
            std::string                 port_;
            ByteArray                   msgBuffer_;
            bool                        isRunning_ = false;
            TransportResponseInterface* rif_ = nullptr;
            MessageQ                    q_;
        public: // == ctors ==
            Transport( const std::string& host, const std::string& port );
            ~Transport();
        public: // == Methods ==
            void run();
            void  send( std::shared_ptr<rapidjson::Document> msg );
            void shutdown();
            void join();
            bool isRunning(){ return isRunning_; }
            void registerRif( TransportResponseInterface* rif ){
              rif_ = rif;
            }

        private:// == Methods ==
            void connect( io::ip::tcp::resolver::iterator it );
            void startRead();
            void startWrite();
        };
    }
}
