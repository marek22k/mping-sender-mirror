#include "mping_sender.hpp"
#include <boost/asio/ip/address.hpp>
#include <boost/log/trivial.hpp>

using namespace MPingSender;

MPingSender::Sender::Sender(
    boost::asio::any_io_executor ex,
    std::function<void(boost::system::error_code)> network_error_handler,
    std::function<void(boost::system::error_code)> timer_error_handler,
    const std::string& bind_address,
    const int bind_port,
    const std::string& address,
    const int port,
    boost::asio::ip::multicast::hops hops,
    const std::string& interface) :
    _socket(ex,
            network_error_handler,
            boost::asio::ip::udp::endpoint(
                boost::asio::ip::make_address(bind_address), bind_port),
            hops,
            interface),
    _timer_error_handler(timer_error_handler),
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
                this->send_packet();
                this->schedule_send();
            }
        });
}

void MPingSender::Sender::send_packet()
{
    std::string message = "test";
    BOOST_LOG_TRIVIAL(trace)
        << "Sending packet to address " << this->_endpoint.address() << " port "
        << this->_endpoint.port();
    this->_socket.send_packet(message, this->_endpoint);
}
