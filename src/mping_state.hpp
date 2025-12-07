#ifndef MPING_STATE
#define MPING_STATE

#include <chrono>
#include <limits>
#include <random>
#include <cstdint>
#include <boost/asio.hpp>

#define MPING_STATE_VERSION "2.0"

namespace MPingSender
{
    enum class MPING_STATE_TYPE
    {
        SENDER = 's',
        RECEIVER = 'r'
    };

    class MPingState
    {
        public:
            MPingState();
            MPingState(const MPING_STATE_TYPE type,
                       const uint8_t ttl,
                       const boost::asio::ip::address src_host,
                       const boost::asio::ip::address dest_host);
            MPingState(
                const MPING_STATE_TYPE type,
                const uint8_t ttl,
                const boost::asio::ip::address src_host,
                const boost::asio::ip::address dest_host,
                const uint32_t _sequence_number,
                const uint32_t _pid,
                const std::chrono::time_point<std::chrono::steady_clock> _tv);

            void next_seq_no() noexcept;
            void set_current_time() noexcept;

            [[nodiscard]] MPING_STATE_TYPE get_type() const noexcept;
            void set_type(MPING_STATE_TYPE state) noexcept;
            [[nodiscard]] uint8_t get_ttl() const noexcept;
            void get_ttl(uint8_t ttl) noexcept;
            [[nodiscard]] boost::asio::ip::address
                get_src_host() const noexcept;
            [[nodiscard]] boost::asio::ip::address
                get_dest_host() const noexcept;
            [[nodiscard]] uint32_t get_sequence_number() const noexcept;
            [[nodiscard]] uint32_t get_pid() const noexcept;
            [[nodiscard]] std::chrono::time_point<std::chrono::steady_clock>
                get_tv() const noexcept;
            [[nodiscard]] std::chrono::seconds get_seconds() const noexcept;
            [[nodiscard]] std::chrono::microseconds
                get_microseconds() const noexcept;

        private:
            MPING_STATE_TYPE _type;
            uint8_t _ttl; /* unsigned char ~ 1 byte */
            boost::asio::ip::address _src_host; /* sockaddr_storage */
            boost::asio::ip::address _dest_host; /* sockaddr_storage */
            uint32_t _sequence_number; /* unsigned int ~ 4 bytes */
            uint32_t _pid; /* pid_t ~ 4 bytes */
            std::chrono::time_point<std::chrono::steady_clock>
                _tv; /* struct timeval */
    };
} // namespace MPingSender

#endif
