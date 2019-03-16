#pragma once
#include "transport/Transport.hpp"
#include "dispatcher/Dispatcher.hpp"

#include <memory>

namespace chat{
    /**
     * Simple service launcher.
    */
    class Service{
    private: // == Members==
        boost::asio::io_service                 ioService_;     //!< io runner.
        transport::Transport                    transport_;     //!< transport layer impl.
        std::unique_ptr<dispatcher::Dispatcher> dispatcher_;    //!< dispatcher layer impl.
    public:// ==Ctors==
        Service();
        void run();
    };
}