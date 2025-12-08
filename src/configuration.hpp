// SPDX-FileCopyrightText: Copyright (C) 2025 Marek Küthe <m.k@mk16.de>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <iostream>
#include <span>
#include <string>
#include <cstdint>
#include <cstdlib>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <boost/program_options.hpp>

namespace MPingSender
{
    class Configuration
    {
        public:
            explicit Configuration(const std::span<char *> args);

            [[nodiscard]] bool get_help() const noexcept;
            [[nodiscard]] boost::log::trivial::severity_level
                get_log_level() const noexcept;
            [[nodiscard]] std::string get_bind_address() const noexcept;
            [[nodiscard]] uint16_t get_bind_port() const noexcept;
            [[nodiscard]] std::string get_address() const noexcept;
            [[nodiscard]] uint16_t get_port() const noexcept;
            [[nodiscard]] uint8_t get_ttl() const noexcept;
            [[nodiscard]] std::string get_interface_name() const noexcept;

            void apply_log_level() const;

        private:
            bool _help;
            boost::log::trivial::severity_level _log_level;
            std::string _bind_address;
            uint16_t _bind_port;
            std::string _address;
            uint16_t _port;
            uint8_t _ttl;
            std::string _interface_name;
    };
} // namespace MPingSender

#endif
