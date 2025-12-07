#include <gtest/gtest.h>
#include <limits>
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
}

TEST(MPingState, InitializeWithCustomValues) {}
