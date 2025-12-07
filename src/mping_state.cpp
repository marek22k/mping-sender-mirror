#include "mping_state.hpp"
#include <chrono>
#include <limits>
#include <stdexcept>
#include "network_serialization.hpp"

using namespace MPingSender;

MPingSender::MPingState::MPingState() :
    _type(MPING_STATE_TYPE::SENDER),
    _ttl(0),
    _src_host(),
    _src_port(0),
    _dest_host(),
    _dest_port(0),
    _sequence_number(0),
    _pid(0),
    _tv()
{
}

MPingSender::MPingState::MPingState(const MPING_STATE_TYPE type,
                                    const uint8_t ttl,
                                    const boost::asio::ip::address src_host,
                                    const uint16_t src_port,
                                    const boost::asio::ip::address dest_host,
                                    const uint16_t dest_port) :
    _type(type),
    _ttl(ttl),
    _src_host(src_host),
    _src_port(src_port),
    _dest_host(dest_host),
    _dest_port(dest_port),
    _sequence_number(0),
    _tv()
{
    static thread_local std::mt19937 generator{std::random_device()()};
    using pid_type = decltype(this->_pid);
    static thread_local std::uniform_int_distribution<pid_type> distribution(
        std::numeric_limits<pid_type>::min(),
        std::numeric_limits<pid_type>::max());
    this->_pid = distribution(generator);
}

MPingSender::MPingState::MPingState(
    const MPING_STATE_TYPE type,
    const uint8_t ttl,
    const boost::asio::ip::address src_host,
    const uint16_t src_port,
    const boost::asio::ip::address dest_host,
    const uint16_t dest_port,
    const uint32_t sequence_number,
    const uint32_t pid,
    const std::chrono::time_point<std::chrono::steady_clock> tv) :
    _type(type),
    _ttl(ttl),
    _src_host(src_host),
    _src_port(src_port),
    _dest_host(dest_host),
    _dest_port(dest_port),
    _sequence_number(sequence_number),
    _pid(pid),
    _tv(tv)
{
}

void MPingSender::MPingState::next_seq_no() noexcept
{
    if (this->_sequence_number ==
        std::numeric_limits<decltype(this->_sequence_number)>::max())
    {
        this->_sequence_number = 0;
    }
    else
    {
        this->_sequence_number++;
    }
}

void MPingSender::MPingState::set_current_time() noexcept
{
    this->_tv = std::chrono::steady_clock::now();
}

/*
MPing packet =
    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |  Ver  |T|L| 0 |                                               |
    |                                                               |
    |                     Source Host                               |
    |                                                               |
    |               |                                               |
    |                                                               |
    |                     Destination Host                          |
    |                                                               |
    |               | Seq   | PID   | Seconds       | Microseconds  |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

    Ver = VERSION = "2.0\0"
    T = TYPE
    +----------+-------+
    | T = TYPE | VALUE |
    +----------+-------+
    | SENDER   | "s"   |
    | RECEIVER | "r"   |
    +----------+-------+
    L = TTL
    Source Host = Host
    Destination Host = Host
    Seq = Sequence number
    PID = PID or Random Number
    Seconds = Seconds (Monotonic clock)
    Microseconds = Microseconds (Monotonic clock)

    IPv6 Host =
    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |10 | P |   0   | IPv6 Address                  | 0             |
    |                            0                                  |
    |                            0                                  |
    |                            0                                  |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    10 = 10 (little endian)
    P = Port

    IPv4 Host =
    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    | 2 | P | IPv4  |                   0                           |
    |                            0                                  |
    |                            0                                  |
    |                            0                                  |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    2 = 2 (little endian)

    otherwise bigendian
*/

std::string MPingSender::MPingState::serialize() const
{
    std::string result;
    result.reserve(288);

    /* Ver */
    constexpr std::array<char, 4> mstate_version =
        std::to_array(MPING_STATE_VERSION);
    result.append(mstate_version.begin(), mstate_version.end());

    /* T */
    result.push_back(static_cast<char>(this->_type));

    /* L */
    auto ttl = NetworkSerialization::uint_to_array(
        NetworkSerialization::to_bigendian(this->_ttl));
    result.append(ttl.begin(), ttl.end());

    constexpr std::array<unsigned char, 2> two_null_characters = {'\0', '\0'};
    result.append(two_null_characters.begin(), two_null_characters.end());

    /* Source Host */
    if (this->_src_host.is_v6())
    {
        const auto ipv6_sockaddr_storage =
            NetworkSerialization::ipv6_to_sockaddr_storage(
                this->_src_host.to_v6(), this->_src_port);
        result.append(ipv6_sockaddr_storage.begin(),
                      ipv6_sockaddr_storage.end());
    }
    else if (this->_src_host.is_v4())
    {
        const auto ipv4_sockaddr_storage =
            NetworkSerialization::ipv4_to_sockaddr_storage(
                this->_src_host.to_v4(), this->_src_port);
        result.append(ipv4_sockaddr_storage.begin(),
                      ipv4_sockaddr_storage.end());
    }
    else [[unlikely]]
    {
        throw std::invalid_argument("Invalid source address");
    }

    /* Destination Host */
    if (this->_dest_host.is_v6())
    {
        const auto ipv6_sockaddr_storage =
            NetworkSerialization::ipv6_to_sockaddr_storage(
                this->_dest_host.to_v6(), this->_dest_port);
        result.append(ipv6_sockaddr_storage.begin(),
                      ipv6_sockaddr_storage.end());
    }
    else if (this->_dest_host.is_v4())
    {
        const auto ipv4_sockaddr_storage =
            NetworkSerialization::ipv4_to_sockaddr_storage(
                this->_dest_host.to_v4(), this->_dest_port);
        result.append(ipv4_sockaddr_storage.begin(),
                      ipv4_sockaddr_storage.end());
    }
    else [[unlikely]]
    {
        throw std::invalid_argument("Invalid destination address");
    }

    /* Seq */
    auto seq = NetworkSerialization::uint_to_array(
        NetworkSerialization::to_bigendian(this->_sequence_number));
    result.append(seq.begin(), seq.end());

    /* PID */
    auto pid = NetworkSerialization::uint_to_array(
        NetworkSerialization::to_bigendian(this->_pid));
    result.append(pid.begin(), pid.end());

    /* Seconds */
    auto seconds = NetworkSerialization::uint_to_array(
        NetworkSerialization::to_bigendian(this->get_seconds().count()));
    result.append(seconds.begin(), seconds.end());

    /* Microseconds */
    auto microseconds = NetworkSerialization::uint_to_array(
        NetworkSerialization::to_bigendian(this->get_microseconds().count()));
    result.append(microseconds.begin(), microseconds.end());

    return result;
}

MPING_STATE_TYPE MPingSender::MPingState::get_type() const noexcept
{
    return this->_type;
}

uint8_t MPingSender::MPingState::get_ttl() const noexcept
{
    return this->_ttl;
}

boost::asio::ip::address MPingSender::MPingState::get_src_host() const noexcept
{
    return this->_src_host;
}

uint16_t MPingSender::MPingState::get_src_port() const noexcept
{
    return this->_src_port;
}

boost::asio::ip::address MPingSender::MPingState::get_dest_host() const noexcept
{
    return this->_dest_host;
}

uint16_t MPingSender::MPingState::get_dest_port() const noexcept
{
    return this->_dest_port;
}

uint32_t MPingSender::MPingState::get_sequence_number() const noexcept
{
    return this->_sequence_number;
}

uint32_t MPingSender::MPingState::get_pid() const noexcept
{
    return this->_pid;
}

std::chrono::time_point<std::chrono::steady_clock>
    MPingSender::MPingState::get_tv() const noexcept
{
    return this->_tv;
}

std::chrono::seconds MPingSender::MPingState::get_seconds() const noexcept
{
    return std::chrono::duration_cast<std::chrono::seconds>(
        this->_tv.time_since_epoch());
}

std::chrono::microseconds
    MPingSender::MPingState::get_microseconds() const noexcept
{
    auto duration = this->_tv.time_since_epoch();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
    auto remaining = duration - seconds;
    return std::chrono::duration_cast<std::chrono::microseconds>(remaining);
}
