#include "multicast_tx_socket.hpp"

using namespace MPingSender;

MPingSender::MulticastTxSocket::MulticastTxSocket(
    boost::asio::any_io_executor ex,
    std::function<void(boost::system::error_code)> error_handler,
    const boost::asio::ip::udp::endpoint& endpoint,
    const boost::asio::ip::multicast::hops hops,
    const std::string& interface) :
    _socket(ex, endpoint.protocol()),
    _error_handler(error_handler)
{
    this->_socket.set_option(boost::asio::socket_base::reuse_address(true));
    this->_socket.set_option(boost::asio::ip::multicast::outbound_interface(
        PosixWrapper::if_nametoindex(interface)));
    this->_socket.set_option(boost::asio::ip::multicast::enable_loopback(true));
    this->_socket.set_option(hops);

    BOOST_LOG_TRIVIAL(info) << "Bind on address " << endpoint.address()
                            << " port " << endpoint.port();
    this->_socket.bind(endpoint);
}

void MPingSender::MulticastTxSocket::send_packet(
    const std::string& message, const boost::asio::ip::udp::endpoint endpoint)
{
    BOOST_LOG_TRIVIAL(trace)
        << "Sending packet to address " << endpoint.address() << " port "
        << endpoint.port();

    const auto sdata = std::make_shared<std::string>(message);
    this->_socket.async_send_to(
        boost::asio::buffer(*sdata),
        endpoint,
        [this, sdata](const boost::system::error_code ec,
                      [[maybe_unused]] const std::size_t bytes_transferred)
        {
            if (ec)
            {
                BOOST_LOG_TRIVIAL(fatal) << "Failed to send packet.";
                this->_error_handler(ec);
            }
            else
            {
                BOOST_LOG_TRIVIAL(trace) << "Packet sendet.";
            }
        });
}
