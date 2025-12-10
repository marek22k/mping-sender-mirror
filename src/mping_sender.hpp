// SPDX-FileCopyrightText: Copyright (C) 2025 Marek Küthe <m.k@mk16.de>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef MPING_SENDER
#define MPING_SENDER

#include <utility>
#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>
#include "mping_state.hpp"
#include "multicast_tx_socket.hpp"

namespace MPingSender
{
    class Sender
    {
        public:
            explicit Sender(boost::asio::any_io_executor ex,
                            std::function<void(boost::system::error_code)>
                                network_error_handler,
                            std::function<void(boost::system::error_code)>
                                timer_error_handler,
                            const std::string& bind_address,
                            const uint16_t bind_port,
                            const std::string& address,
                            const uint16_t port,
                            uint8_t ttl,
                            const std::string& interface);

        private:
            void schedule_send();
            void send_packet();

            MulticastTxSocket _socket;
            MPingState _state;
            std::function<void(boost::system::error_code)> _timer_error_handler;
            boost::asio::ip::udp::endpoint _endpoint;
            boost::asio::steady_timer _timer;
    };
} // namespace MPingSender

#endif
