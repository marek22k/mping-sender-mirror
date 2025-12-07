#include "mping_state.hpp"
#include <chrono>
#include <limits>

using namespace MPingSender;

MPingSender::MPingState::MPingState() :
    _type(MPING_STATE_TYPE::SENDER),
    _ttl(0),
    _src_host(),
    _dest_host(),
    _sequence_number(0),
    _pid(0),
    _tv()
{
}

MPingSender::MPingState::MPingState(const MPING_STATE_TYPE type,
                                    const uint8_t ttl,
                                    const boost::asio::ip::address src_host,
                                    const boost::asio::ip::address dest_host) :
    _type(type),
    _ttl(ttl),
    _src_host(src_host),
    _dest_host(dest_host),
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
    const boost::asio::ip::address dest_host,
    const uint32_t sequence_number,
    const uint32_t pid,
    const std::chrono::time_point<std::chrono::steady_clock> tv) :
    _type(type),
    _ttl(ttl),
    _src_host(src_host),
    _dest_host(dest_host),
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
*/

std::string MPingSender::MPingState::serialize() const
{
    constexpr std::array<char, 4> mstate_version =
        std::to_array(MPING_STATE_VERSION);
    std::string result;
    result.resize(288);
    return "";
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

boost::asio::ip::address MPingSender::MPingState::get_dest_host() const noexcept
{
    return this->_dest_host;
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
