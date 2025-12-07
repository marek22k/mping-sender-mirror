#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <iostream>
#include <span>
#include <string>
#include <cstdint>
#include <cstdlib>
#include <boost/log/trivial.hpp>
#include <boost/program_options.hpp>

namespace MPingSender
{
    class Configuration
    {
        public:
            Configuration(const std::span<char *> args);

            [[nodiscard]] bool get_help() const noexcept;
            [[nodiscard]] std::string get_bind_address() const noexcept;
            [[nodiscard]] uint16_t get_bind_port() const noexcept;
            [[nodiscard]] std::string get_address() const noexcept;
            [[nodiscard]] uint16_t get_port() const noexcept;
            [[nodiscard]] uint8_t get_ttl() const noexcept;
            [[nodiscard]] std::string get_interface_name() const noexcept;

        private:
            bool _help;
            std::string _bind_address;
            uint16_t _bind_port;
            std::string _address;
            uint16_t _port;
            uint8_t _ttl;
            std::string _interface_name;
    };
} // namespace MPingSender

#endif
