#ifndef MULTICAST_TX_SOCKET
#define MULTICAST_TX_SOCKET

#include <functional>
#include <memory>
#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>
#include "posix_wrapper.hpp"

namespace MPingSender
{
    class MulticastTxSocket
    {
        public:
            MulticastTxSocket(
                boost::asio::any_io_executor ex,
                std::function<void(boost::system::error_code)> error_handler,
                const boost::asio::ip::udp::endpoint& endpoint,
                const boost::asio::ip::multicast::hops hops,
                const std::string& interface);

            void send_packet(const std::string& data,
                             const boost::asio::ip::udp::endpoint endpoint);

        private:
            boost::asio::ip::udp::socket _socket;
            std::function<void(boost::system::error_code)> _error_handler;
    };
} // namespace MPingSender

#endif
