#pragma once
#include <boost/asio.hpp>
#include <thread>
#include <string>
#include <future>
#include <memory>

#include "rapidjson/document.h"

namespace io = boost::asio;

namespace chat{
    class Message;

    namespace client{
        /**
         * boost driven tcp client
         * seds raw strings from input.
        */
        class ChatClient final{
        public: // == Constants ==
            static constexpr int MaxSize = 512;
        private: // == Memebers ==
            std::thread                 ioRunner_;      //!< io thread to avoid std input blocking.
            io::io_service              ioService_;     //!< io/task scheduler
            io::ip::tcp::socket         socket_;        // client sock.
            io::ip::tcp::resolver       r_;             // converts strig address into boost asio endpoint,
            std::string                 host_;
            std::string                 port_;
            std::vector<char>           msgBuffer_;
            bool                        isRunning_ = false;
            std::promise<bool>          initP_;
        public: // == ctors ==
            ChatClient( const std::string& host, const std::string& port );
            ~ChatClient();
        public: // == Methods ==
            std::future<bool> run();
            void write( std::shared_ptr<rapidjson::Document> msg );
            void shutdown();
            void join();
            bool isRunning(){ return isRunning_; }
        private:// == Methods ==
            void connect( io::ip::tcp::resolver::iterator it );
            void read();
        };
    }
}