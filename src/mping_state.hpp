// SPDX-FileCopyrightText: Copyright (C) 2025 Marek Küthe <m.k@mk16.de>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef MPING_STATE
#define MPING_STATE

#include <chrono>
#include <limits>
#include <random>
#include <cstdint>
#include <boost/asio.hpp>
#include "network_serialization.hpp"

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
            explicit MPingState();
            explicit MPingState(const MPING_STATE_TYPE type,
                                const uint8_t ttl,
                                const boost::asio::ip::address src_host,
                                const uint16_t src_port,
                                const boost::asio::ip::address dest_host,
                                const uint16_t dest_port);
            explicit MPingState(
                const MPING_STATE_TYPE type,
                const uint8_t ttl,
                const boost::asio::ip::address src_host,
                const uint16_t src_port,
                const boost::asio::ip::address dest_host,
                const uint16_t dest_port,
                const uint32_t sequence_number,
                const uint32_t pid,
                const std::chrono::time_point<std::chrono::steady_clock> tv);

            void next_seq_no() noexcept;
            void set_current_time() noexcept;
            [[nodiscard]] std::string serialize() const;

            [[nodiscard]] MPING_STATE_TYPE get_type() const noexcept;
            void set_type(MPING_STATE_TYPE state) noexcept;
            [[nodiscard]] uint8_t get_ttl() const noexcept;
            void get_ttl(uint8_t ttl) noexcept;
            [[nodiscard]] boost::asio::ip::address
                get_src_host() const noexcept;
            [[nodiscard]] uint16_t get_src_port() const noexcept;
            [[nodiscard]] boost::asio::ip::address
                get_dest_host() const noexcept;
            [[nodiscard]] uint16_t get_dest_port() const noexcept;
            [[nodiscard]] uint32_t get_sequence_number() const noexcept;
            [[nodiscard]] uint32_t get_pid() const noexcept;
            [[nodiscard]] std::chrono::time_point<std::chrono::steady_clock>
                get_tv() const noexcept;
            [[nodiscard]] std::chrono::seconds get_seconds() const noexcept;
            [[nodiscard]] std::chrono::microseconds
                get_microseconds() const noexcept;

        private:
            MPING_STATE_TYPE _type;
            uint8_t _ttl;
            boost::asio::ip::address _src_host;
            uint16_t _src_port;
            boost::asio::ip::address _dest_host;
            uint16_t _dest_port;
            uint32_t _sequence_number;
            uint32_t _pid;
            std::chrono::time_point<std::chrono::steady_clock> _tv;
    };
} // namespace MPingSender

#endif
