// SPDX-FileCopyrightText: Copyright (C) 2024-2025 Marek Küthe <m.k@mk16.de>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <iostream>
#include <span>
#include <stdexcept>
#include <cstdlib>
#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>
#include "configuration.hpp"
#include "mping_sender.hpp"

int main(int argc, char * argv[])
{
    try
    {
        const auto args = std::span(argv, static_cast<std::size_t>(argc));
        MPingSender::Configuration config(args);

        boost::asio::io_context io;

        std::function<void(boost::system::error_code)> error_handler =
            [](boost::system::error_code)
        {
            std::runtime_error("Fatal error");
        };
        const MPingSender::Sender sender(io.get_executor(),
                                         error_handler,
                                         error_handler,
                                         config.get_bind_address(),
                                         config.get_bind_port(),
                                         config.get_address(),
                                         config.get_port(),
                                         config.get_ttl(),
                                         config.get_interface_name());

        io.run();
    }
    catch (const std::exception& e)
    {
        BOOST_LOG_TRIVIAL(fatal) << "Error: " << e.what() << std::endl
                                 << "Exit program.";
        std::exit(EXIT_FAILURE); // NOLINT(concurrency-mt-unsafe)
    }
    return EXIT_SUCCESS;
}
