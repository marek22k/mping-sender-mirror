// SPDX-FileCopyrightText: Copyright (C) 2024-2025 Marek Küthe <m.k@mk16.de>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>
#include <array>
#include <limits>
#include <cstdint>
#include "mping_state.hpp"
#include "src/mping_state.hpp"

using namespace std::chrono_literals;

TEST(MPingStateTest, InitializeEmpty)
{
    const MPingSender::MPingState state;
    EXPECT_EQ(state.get_type(), MPingSender::MPING_STATE_TYPE::SENDER);
    EXPECT_EQ(state.get_ttl(), 0);
    EXPECT_TRUE(state.get_src_host().is_unspecified());
    EXPECT_EQ(state.get_src_port(), 0);
    EXPECT_TRUE(state.get_dest_host().is_unspecified());
    EXPECT_EQ(state.get_dest_port(), 0);
    EXPECT_EQ(state.get_sequence_number(), 0);
    EXPECT_EQ(state.get_pid(), 0);
    EXPECT_EQ(state.get_seconds(), 0s);
    EXPECT_EQ(state.get_microseconds(), 0s);
}

TEST(MPingStateTest, InitializeDefault)
{
    const auto addr1(boost::asio::ip::make_address(
        "fd72:807b:8257:bd92:80a1:32a0:f13d:b46c"));
    const auto addr2(boost::asio::ip::make_address(
        "fd72:807b:8257:bd92:80a1:32a0:f13d:b46d"));
    ASSERT_NE(addr1, addr2);

    const MPingSender::MPingState state(
        MPingSender::MPING_STATE_TYPE::SENDER, 44, addr1, 2579, addr2, 5572);
    EXPECT_EQ(state.get_type(), MPingSender::MPING_STATE_TYPE::SENDER);
    EXPECT_EQ(state.get_ttl(), 44);
    EXPECT_EQ(state.get_src_host(), addr1);
    EXPECT_EQ(state.get_src_port(), 2579);
    EXPECT_EQ(state.get_dest_host(), addr2);
    EXPECT_EQ(state.get_dest_port(), 5572);
    EXPECT_EQ(state.get_sequence_number(), 0);
    EXPECT_GE(state.get_pid(),
              std::numeric_limits<decltype(state.get_pid())>::min());
    EXPECT_LE(state.get_pid(),
              std::numeric_limits<decltype(state.get_pid())>::max());
    EXPECT_EQ(state.get_seconds(), 0s);
    EXPECT_EQ(state.get_microseconds(), 0s);

    const MPingSender::MPingState receiver_state(
        MPingSender::MPING_STATE_TYPE::RECEIVER, 0, addr1, 0, addr2, 0);
    EXPECT_EQ(receiver_state.get_type(),
              MPingSender::MPING_STATE_TYPE::RECEIVER);
}

TEST(MPingStateTest, InitializeWithCustomValues)
{
    const auto addr1(boost::asio::ip::make_address(
        "fd72:807b:8257:bd92:80a1:32a0:f13d:b46c"));
    const auto addr2(boost::asio::ip::make_address(
        "fd72:807b:8257:bd92:80a1:32a0:f13d:b46d"));
    ASSERT_NE(addr1, addr2);

    constexpr uint16_t src_port = 2252;
    constexpr uint16_t dest_port = 7597;
    ASSERT_NE(src_port, dest_port);

    constexpr uint8_t ttl = 45;
    constexpr uint32_t seq_no = 77'242'255;
    constexpr uint32_t pid = 59'592'343;

    constexpr auto seconds = std::chrono::seconds(72);
    constexpr auto microseconds = std::chrono::microseconds(24973);
    const auto time =
        std::chrono::steady_clock::time_point(seconds + microseconds);

    const MPingSender::MPingState state(MPingSender::MPING_STATE_TYPE::SENDER,
                                        ttl,
                                        addr1,
                                        src_port,
                                        addr2,
                                        dest_port,
                                        seq_no,
                                        pid,
                                        time);
    EXPECT_EQ(state.get_type(), MPingSender::MPING_STATE_TYPE::SENDER);
    EXPECT_EQ(state.get_ttl(), ttl);
    EXPECT_EQ(state.get_src_host(), addr1);
    EXPECT_EQ(state.get_src_port(), src_port);
    EXPECT_EQ(state.get_dest_host(), addr2);
    EXPECT_EQ(state.get_dest_port(), dest_port);
    EXPECT_EQ(state.get_sequence_number(), seq_no);
    EXPECT_EQ(state.get_pid(), pid);
    EXPECT_EQ(state.get_tv(), time);
    EXPECT_EQ(state.get_seconds(), seconds);
    EXPECT_EQ(state.get_microseconds(), microseconds);

    const MPingSender::MPingState receiver_state(
        MPingSender::MPING_STATE_TYPE::RECEIVER,
        ttl,
        addr1,
        src_port,
        addr2,
        dest_port,
        seq_no,
        pid,
        time);
    EXPECT_EQ(receiver_state.get_type(),
              MPingSender::MPING_STATE_TYPE::RECEIVER);
}

TEST(MPingStateTest, Serialization)
{
    const auto addr1(boost::asio::ip::make_address("fd00:8e13:ce5d:e::1"));
    const auto addr2(boost::asio::ip::make_address("ff2e::42"));
    ASSERT_NE(addr1, addr2);

    constexpr uint16_t src_port = 0;
    constexpr uint16_t dest_port = 4321;

    constexpr uint8_t ttl = 1;
    constexpr uint32_t seq_no = 3;
    constexpr uint32_t pid = 417'936;

    constexpr auto seconds = std::chrono::seconds(163'169);
    constexpr auto microseconds = std::chrono::microseconds(553'889);
    const auto time =
        std::chrono::steady_clock::time_point(seconds + microseconds);

    const MPingSender::MPingState state(MPingSender::MPING_STATE_TYPE::SENDER,
                                        ttl,
                                        addr1,
                                        src_port,
                                        addr2,
                                        dest_port,
                                        seq_no,
                                        pid,
                                        time);
    EXPECT_EQ(state.get_type(), MPingSender::MPING_STATE_TYPE::SENDER);
    EXPECT_EQ(state.get_ttl(), ttl);
    EXPECT_EQ(state.get_src_host(), addr1);
    EXPECT_EQ(state.get_src_port(), src_port);
    EXPECT_EQ(state.get_dest_host(), addr2);
    EXPECT_EQ(state.get_dest_port(), dest_port);
    EXPECT_EQ(state.get_sequence_number(), seq_no);
    EXPECT_EQ(state.get_pid(), pid);
    EXPECT_EQ(state.get_tv(), time);
    EXPECT_EQ(state.get_seconds(), seconds);
    EXPECT_EQ(state.get_microseconds(), microseconds);

    const std::string packet(
        "2."
        "0\000s\001\000\000\n\000\000\000\000\000\000\000\375\000\216\023\316]"
        "\000\016\000\000\000\000\000\000\000\001\000\000\000\000\000\000\000"
        "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
        "\000"
        "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
        "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
        "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
        "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
        "\000\000\000\000\000\000\000\000\000\000\000\n\000\020\341\000\000\000"
        "\000\377."
        "\000\000\000\000\000\000\000\000\000\000\000\000\000B\000\000\000\000"
        "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
        "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
        "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
        "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
        "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
        "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
        "\000\003\000\006`\220\000\000\000\000\000\002}"
        "a\000\000\000\000\000\bs\241",
        288);
    EXPECT_EQ(state.serialize(), packet);
}

TEST(MPingStateTest, NextSeqNoIncrement)
{
    MPingSender::MPingState state;
    EXPECT_EQ(state.get_type(), MPingSender::MPING_STATE_TYPE::SENDER);
    EXPECT_EQ(state.get_ttl(), 0);
    EXPECT_TRUE(state.get_src_host().is_unspecified());
    EXPECT_TRUE(state.get_dest_host().is_unspecified());
    EXPECT_EQ(state.get_sequence_number(), 0);
    EXPECT_EQ(state.get_pid(), 0);
    EXPECT_EQ(state.get_seconds(), 0s);
    EXPECT_EQ(state.get_microseconds(), 0s);

    state.next_seq_no();

    EXPECT_EQ(state.get_type(), MPingSender::MPING_STATE_TYPE::SENDER);
    EXPECT_EQ(state.get_ttl(), 0);
    EXPECT_TRUE(state.get_src_host().is_unspecified());
    EXPECT_TRUE(state.get_dest_host().is_unspecified());
    EXPECT_EQ(state.get_sequence_number(), 1);
    EXPECT_EQ(state.get_pid(), 0);
    EXPECT_EQ(state.get_seconds(), 0s);
    EXPECT_EQ(state.get_microseconds(), 0s);

    state.next_seq_no();

    EXPECT_EQ(state.get_type(), MPingSender::MPING_STATE_TYPE::SENDER);
    EXPECT_EQ(state.get_ttl(), 0);
    EXPECT_TRUE(state.get_src_host().is_unspecified());
    EXPECT_TRUE(state.get_dest_host().is_unspecified());
    EXPECT_EQ(state.get_sequence_number(), 2);
    EXPECT_EQ(state.get_pid(), 0);
    EXPECT_EQ(state.get_seconds(), 0s);
    EXPECT_EQ(state.get_microseconds(), 0s);
}

TEST(MPingStateTest, NextSeqNoIncrementMax)
{
    const auto addr1(boost::asio::ip::make_address(
        "fd72:807b:8257:bd92:80a1:32a0:f13d:b46c"));
    const auto addr2(boost::asio::ip::make_address(
        "fd72:807b:8257:bd92:80a1:32a0:f13d:b46d"));
    ASSERT_NE(addr1, addr2);

    constexpr uint16_t src_port = 2252;
    constexpr uint16_t dest_port = 7597;
    ASSERT_NE(src_port, dest_port);

    constexpr uint8_t ttl = 45;
    constexpr uint32_t max_seq_no = std::numeric_limits<uint32_t>::max();
    constexpr uint32_t seq_no = max_seq_no - 2;
    constexpr uint32_t pid = 59'592'343;

    constexpr auto seconds = std::chrono::seconds(72);
    constexpr auto microseconds = std::chrono::microseconds(24973);
    const auto time =
        std::chrono::steady_clock::time_point(seconds + microseconds);

    MPingSender::MPingState state(MPingSender::MPING_STATE_TYPE::SENDER,
                                  ttl,
                                  addr1,
                                  src_port,
                                  addr2,
                                  dest_port,
                                  seq_no,
                                  pid,
                                  time);
    EXPECT_EQ(state.get_type(), MPingSender::MPING_STATE_TYPE::SENDER);
    EXPECT_EQ(state.get_ttl(), ttl);
    EXPECT_EQ(state.get_src_host(), addr1);
    EXPECT_EQ(state.get_src_port(), src_port);
    EXPECT_EQ(state.get_dest_host(), addr2);
    EXPECT_EQ(state.get_dest_port(), dest_port);
    EXPECT_EQ(state.get_pid(), pid);
    EXPECT_EQ(state.get_tv(), time);
    EXPECT_EQ(state.get_seconds(), seconds);
    EXPECT_EQ(state.get_microseconds(), microseconds);

    EXPECT_EQ(state.get_sequence_number(), max_seq_no - 2);
    state.next_seq_no();
    EXPECT_EQ(state.get_sequence_number(), max_seq_no - 1);
    state.next_seq_no();
    EXPECT_EQ(state.get_sequence_number(), max_seq_no);
    state.next_seq_no();
    EXPECT_EQ(state.get_sequence_number(), 0);
    state.next_seq_no();
    EXPECT_EQ(state.get_sequence_number(), 1);
    state.next_seq_no();
    EXPECT_EQ(state.get_sequence_number(), 2);
}
