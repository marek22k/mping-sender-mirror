// SPDX-FileCopyrightText: Copyright (C) 2025 Marek Küthe <m.k@mk16.de>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "mping_sender.hpp"

using namespace MPingSender;

static_assert(sizeof(boost::asio::ip::multicast::hops) >= sizeof(uint8_t));

MPingSender::Sender::Sender(
    boost::asio::any_io_executor ex,
    std::function<void(boost::system::error_code)> network_error_handler,
    std::function<void(boost::system::error_code)> timer_error_handler,
    const std::string& bind_address,
    const uint16_t bind_port,
    const std::string& address,
    const uint16_t port,
    uint8_t ttl,
    const std::string& interface) :
    _socket(ex,
            std::move(network_error_handler),
            boost::asio::ip::udp::endpoint(
                boost::asio::ip::make_address(bind_address), bind_port),
            static_cast<boost::asio::ip::multicast::hops>(ttl),
            interface),
    _state(MPING_STATE_TYPE::SENDER,
           ttl,
           boost::asio::ip::make_address(bind_address),
           bind_port,
           boost::asio::ip::make_address(address),
           port),
    _timer_error_handler(std::move(timer_error_handler)),
    _endpoint(boost::asio::ip::make_address(address), port),
    _timer(ex)
{
    this->schedule_send();
}

void MPingSender::Sender::schedule_send()
{
    this->_timer.expires_after(boost::asio::chrono::seconds(1));
    this->_timer.async_wait(
        [this](const boost::system::error_code& ec)
        {
            if (ec)
            {
                BOOST_LOG_TRIVIAL(fatal) << "Failed to schedule timer.";
                this->_timer_error_handler(ec);
            }
            else
            {
                this->schedule_send();
                this->send_packet();
            }
        });
}

void MPingSender::Sender::send_packet()
{
    _state.set_current_time();
    const std::string message = _state.serialize();
    _state.next_seq_no();
    this->_socket.send_packet(message, this->_endpoint);
}
