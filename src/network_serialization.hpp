#ifndef NETWORK_SERIALIZATION
#define NETWORK_SERIALIZATION

#include <algorithm>
#include <array>
#include <bit>
#include <cstdint>
#include <boost/asio.hpp>

template<typename T>
constexpr T hton(const T host)
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
constexpr std::array<unsigned char, sizeof(T)> uint_to_array(const T i)
{
    return std::bit_cast<std::array<unsigned char, sizeof(T)>>(i);
}

inline std::array<unsigned char, 128>
    ipv6_to_sockaddr_storage(const boost::asio::ip::address_v6 address,
                             const uint16_t port)
{
    std::array<unsigned char, 128> result;
    result.fill('\0');

    auto it = result.begin();

    /* 10 */
    constexpr uint16_t ipv6_type_int = 10;
    constexpr std::array<unsigned char, 2> ipv6_type =
        uint_to_array(ipv6_type_int);
    it = std::ranges::copy(ipv6_type, it).out;

    /* P */
    const std::array<unsigned char, 2> port_bytes = uint_to_array(port);
    it = std::ranges::copy(port_bytes, it).out;

    /* IPv6 */
    const std::array<unsigned char, 16> ipv6_bytes = address.to_bytes();
    it = std::ranges::copy(ipv6_bytes, it).out;

    return result;
}

#endif
