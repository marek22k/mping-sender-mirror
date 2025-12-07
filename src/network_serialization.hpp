#ifndef NETWORK_SERIALIZATION
#define NETWORK_SERIALIZATION

#include <array>
#include <bit>

template<typename T>
constexpr T hton(T host)
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
constexpr std::array<unsigned char, sizeof(T)> uint_to_array(T i)
{
    return std::bit_cast<std::array<unsigned char, sizeof(T)>>(i);
}

#endif
