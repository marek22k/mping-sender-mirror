#ifndef MPING_SENDER
#define MPING_SENDER

#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>
#include "multicast_tx_socket.hpp"

namespace MPingSender
{
    class Sender
    {
        public:
            Sender(boost::asio::any_io_executor ex,
                   std::function<void(boost::system::error_code)>
                       network_error_handler,
                   const std::string& bind_address,
                   const int bind_port,
                   const std::string& address,
                   const int port);

        private:
            void schedule_send();
            void send_packet();

            MulticastTxSocket _socket;
            boost::asio::ip::udp::endpoint _endpoint;
            boost::asio::steady_timer _timer;
    };
} // namespace MPingSender

#endif
