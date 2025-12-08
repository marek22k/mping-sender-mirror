// SPDX-FileCopyrightText: Copyright (C) 2025 Marek Küthe <m.k@mk16.de>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef NETWORK_SERIALIZATION
#define NETWORK_SERIALIZATION

#include <algorithm>
#include <array>
#include <bit>
#include <cstdint>
#include <boost/asio.hpp>

namespace NetworkSerialization
{
    template<typename T>
    constexpr T to_bigendian(const T host)
        requires(std::endian::native == std::endian::big ||
                 std::endian::native == std::endian::little)
    {
        if constexpr (std::endian::native == std::endian::little)
        {
            return std::byteswap(host);
        }
        else
        {
            return host;
        }
    }

    template<typename T>
    constexpr T to_littleendian(const T host)
        requires(std::endian::native == std::endian::big ||
                 std::endian::native == std::endian::little)
    {
        if constexpr (std::endian::native == std::endian::big)
        {
            return std::byteswap(host);
        }
        else
        {
            return host;
        }
    }

    template<typename T>
    constexpr std::array<unsigned char, sizeof(T)> uint_to_array(const T i)
    {
        return std::bit_cast<std::array<unsigned char, sizeof(T)>>(i);
    }

    inline std::array<unsigned char, 128>
        ipv6_to_sockaddr_storage(boost::asio::ip::address_v6 address,
                                 uint16_t port)
    {
        std::array<unsigned char, 128> result = {};
        result.fill('\0');

        auto * it = result.begin();

        /* 10 */
        constexpr uint16_t ipv6_type_int = 10;
        constexpr std::array<unsigned char, 2> ipv6_type =
            uint_to_array(to_littleendian(ipv6_type_int));
        it = std::ranges::copy(ipv6_type, it).out;

        /* P */
        const std::array<unsigned char, 2> port_bytes =
            uint_to_array(to_bigendian(port));
        it = std::ranges::copy(port_bytes, it).out;

        std::advance(it, 4);

        /* IPv6 */
        const std::array<unsigned char, 16> ipv6_bytes = address.to_bytes();
        std::ranges::copy(ipv6_bytes, it);

        return result;
    }

    inline std::array<unsigned char, 128>
        ipv4_to_sockaddr_storage(boost::asio::ip::address_v4 address,
                                 uint16_t port)
    {
        std::array<unsigned char, 128> result = {};
        result.fill('\0');

        auto * it = result.begin();

        /* 10 */
        constexpr uint16_t ipv4_type_int = 2;
        constexpr std::array<unsigned char, 2> ipv4_type =
            uint_to_array(to_littleendian(ipv4_type_int));
        it = std::ranges::copy(ipv4_type, it).out;

        /* P */
        const std::array<unsigned char, 2> port_bytes =
            uint_to_array(to_bigendian(port));
        it = std::ranges::copy(port_bytes, it).out;

        std::advance(it, 4);

        /* IPv4 */
        const std::array<unsigned char, 4> ipv4_bytes = address.to_bytes();
        std::ranges::copy(ipv4_bytes, it);

        return result;
    }
} // namespace NetworkSerialization

#endif
