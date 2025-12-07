#include <gtest/gtest.h>
#include <limits>
#include <cstdint>
#include "mping_state.hpp"
#include "src/mping_state.hpp"

using namespace std::chrono_literals;

TEST(MPingState, InitializeEmpty)
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
}

TEST(MPingState, InitializeDefault)
{
    const auto addr1(boost::asio::ip::make_address(
        "fd72:807b:8257:bd92:80a1:32a0:f13d:b46c"));
    const auto addr2(boost::asio::ip::make_address(
        "fd72:807b:8257:bd92:80a1:32a0:f13d:b46d"));
    ASSERT_NE(addr1, addr2);

    MPingSender::MPingState state(
        MPingSender::MPING_STATE_TYPE::SENDER, 0, addr1, addr2);
    EXPECT_EQ(state.get_type(), MPingSender::MPING_STATE_TYPE::SENDER);
    EXPECT_EQ(state.get_ttl(), 0);
    EXPECT_EQ(state.get_src_host(), addr1);
    EXPECT_EQ(state.get_dest_host(), addr2);
    EXPECT_EQ(state.get_sequence_number(), 0);
    EXPECT_GE(state.get_pid(),
              std::numeric_limits<decltype(state.get_pid())>::min());
    EXPECT_LE(state.get_pid(),
              std::numeric_limits<decltype(state.get_pid())>::max());
    EXPECT_EQ(state.get_seconds(), 0s);
    EXPECT_EQ(state.get_microseconds(), 0s);

    MPingSender::MPingState receiver_state(
        MPingSender::MPING_STATE_TYPE::RECEIVER, 0, addr1, addr2);
    EXPECT_EQ(receiver_state.get_type(),
              MPingSender::MPING_STATE_TYPE::RECEIVER);
}

TEST(MPingState, InitializeWithCustomValues)
{
    const auto addr1(boost::asio::ip::make_address(
        "fd72:807b:8257:bd92:80a1:32a0:f13d:b46c"));
    const auto addr2(boost::asio::ip::make_address(
        "fd72:807b:8257:bd92:80a1:32a0:f13d:b46d"));
    ASSERT_NE(addr1, addr2);

    constexpr uint8_t ttl = 45;
    constexpr uint32_t seq_no = 77'242'255;
    constexpr uint32_t pid = 59'592'343;

    constexpr auto seconds = std::chrono::seconds(72);
    constexpr auto microseconds = std::chrono::microseconds(24973);
    const auto time =
        std::chrono::steady_clock::time_point(seconds + microseconds);

    MPingSender::MPingState state(MPingSender::MPING_STATE_TYPE::SENDER,
                                  ttl,
                                  addr1,
                                  addr2,
                                  seq_no,
                                  pid,
                                  time);
    EXPECT_EQ(state.get_type(), MPingSender::MPING_STATE_TYPE::SENDER);
    EXPECT_EQ(state.get_ttl(), ttl);
    EXPECT_EQ(state.get_src_host(), addr1);
    EXPECT_EQ(state.get_dest_host(), addr2);
    EXPECT_EQ(state.get_sequence_number(), seq_no);
    EXPECT_EQ(state.get_pid(), pid);
    EXPECT_EQ(state.get_tv(), time);
    EXPECT_EQ(state.get_seconds(), seconds);
    EXPECT_EQ(state.get_microseconds(), microseconds);

    MPingSender::MPingState receiver_state(
        MPingSender::MPING_STATE_TYPE::RECEIVER,
        ttl,
        addr1,
        addr2,
        seq_no,
        pid,
        time);
    EXPECT_EQ(receiver_state.get_type(),
              MPingSender::MPING_STATE_TYPE::RECEIVER);
}

TEST(MPingState, NextSeqNoIncrement)
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

TEST(MPingState, NextSeqNoIncrementMax)
{
    const auto addr1(boost::asio::ip::make_address(
        "fd72:807b:8257:bd92:80a1:32a0:f13d:b46c"));
    const auto addr2(boost::asio::ip::make_address(
        "fd72:807b:8257:bd92:80a1:32a0:f13d:b46d"));
    ASSERT_NE(addr1, addr2);

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
                                  addr2,
                                  seq_no,
                                  pid,
                                  time);
    EXPECT_EQ(state.get_type(), MPingSender::MPING_STATE_TYPE::SENDER);
    EXPECT_EQ(state.get_ttl(), ttl);
    EXPECT_EQ(state.get_src_host(), addr1);
    EXPECT_EQ(state.get_dest_host(), addr2);
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
