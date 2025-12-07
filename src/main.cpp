// SPDX-FileCopyrightText: Copyright (C) 2024-2025 Marek Küthe <m.k@mk16.de>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <stdexcept>
#include <cstdlib>
#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>
#include "mping_sender.hpp"

int main()
{
    try
    {
        boost::asio::io_context io;

        std::function<void(boost::system::error_code)> error_handler =
            [](boost::system::error_code)
        {
            std::runtime_error("Fatal error");
        };
        const MPingSender::Sender sender(io.get_executor(),
                                         error_handler,
                                         error_handler,
                                         "fd00:8e13:ce5d:e::1",
                                         4321,
                                         "ff2e::42",
                                         4321,
                                         32,
                                         "lab-client01");

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
