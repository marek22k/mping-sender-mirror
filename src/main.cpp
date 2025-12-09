// SPDX-FileCopyrightText: Copyright (C) 2025 Marek Küthe <m.k@mk16.de>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <iostream>
#include <span>
#include <stdexcept>
#include <cstdlib>
#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>
#include "capability_managment.hpp"
#include "configuration.hpp"
#include "landlock.hpp"
#include "mping_sender.hpp"
#include "seccomp.hpp"

int main(int argc, char * argv[])
{
    try
    {
#ifdef HAVE_LIBCAPNG
        CapabilityManagment::check_for_capabilites();
        CapabilityManagment::lock();
        CapabilityManagment::drop_all_capabilies();
#endif
#ifdef HAVE_LANDLOCK
        const LandlockRuleset landlock_ruleset_init(
            LANDLOCK_ACCESS_FS_EXECUTE | LANDLOCK_ACCESS_FS_WRITE_FILE |
                LANDLOCK_ACCESS_FS_READ_FILE | LANDLOCK_ACCESS_FS_TRUNCATE |
                LANDLOCK_ACCESS_FS_READ_DIR | LANDLOCK_ACCESS_FS_REMOVE_DIR |
                LANDLOCK_ACCESS_FS_REMOVE_FILE | LANDLOCK_ACCESS_FS_MAKE_CHAR |
                LANDLOCK_ACCESS_FS_MAKE_DIR | LANDLOCK_ACCESS_FS_MAKE_REG |
                LANDLOCK_ACCESS_FS_MAKE_SOCK | LANDLOCK_ACCESS_FS_MAKE_FIFO |
                LANDLOCK_ACCESS_FS_MAKE_BLOCK | LANDLOCK_ACCESS_FS_MAKE_SYM |
                LANDLOCK_ACCESS_FS_REFER,
            LANDLOCK_ACCESS_NET_BIND_TCP | LANDLOCK_ACCESS_NET_CONNECT_TCP);
    #ifdef HAVE_LANDLOCK_RESTRICT_SELF_LOG_NEW_EXEC_ON
        landlock_ruleset_init.restrict_self(
            LANDLOCK_RESTRICT_SELF_LOG_NEW_EXEC_ON);
    #else
        landlock_ruleset_init.restrict_self();
    #endif
#endif
#ifdef HAVE_SECCOMP
        const SeccompFilterContext seccomp_context(SCMP_ACT_ALLOW);
        // see also
        // https://lists.boost.org/archives/list/boost-users@lists.boost.org/thread/YJ5RTK25HLPFEZ3XVBBFQDJOSPIIOBNA/
        // and https://sourceforge.net/p/asio/mailman/message/59260797/
        // due to complexity use seccomp blacklist
        seccomp_context.kill_chown();
        seccomp_context.kill_clock();
        seccomp_context.kill_cpu_emulation();
        seccomp_context.kill_debug();
        seccomp_context.kill_others();
        seccomp_context.kill_ipc();
        seccomp_context.kill_keyring();
        seccomp_context.kill_memlock();
        seccomp_context.kill_module();
        seccomp_context.kill_obsolete();
        seccomp_context.kill_privileged();
        seccomp_context.kill_rawio();
        seccomp_context.kill_reboot();
        seccomp_context.kill_resources();
        seccomp_context.kill_setuid();
        seccomp_context.kill_swap();
        seccomp_context.kill_sync();
        seccomp_context.kill_system_service();
        seccomp_context.kill_signal();
        seccomp_context.load();
#endif

        const auto args = std::span(argv, static_cast<std::size_t>(argc));
        const MPingSender::Configuration config(args);

        BOOST_LOG_TRIVIAL(info)
            << "Boost version (compile time): " << (BOOST_VERSION / 100'000)
            << "." << (BOOST_VERSION / 100 % 1000) << "."
            << (BOOST_VERSION % 100);

#ifdef HAVE_LIBCAPNG
        BOOST_LOG_TRIVIAL(info) << "libcapng: true";
#else
        BOOST_LOG_TRIVIAL(info) << "libcapng: false";
#endif

#ifdef HAVE_SECCOMP
        BOOST_LOG_TRIVIAL(info) << "seccomp: true";
        const auto * seccomp_ver = seccomp_version();
        BOOST_LOG_TRIVIAL(info)
            << "seccomp version (runtime): " << seccomp_ver->major << "."
            << seccomp_ver->minor << "." << seccomp_ver->micro;
#else
        BOOST_LOG_TRIVIAL(info) << "seccomp: false";
#endif

#ifdef HAVE_LANDLOCK
        BOOST_LOG_TRIVIAL(info) << "Landlock: true";
        BOOST_LOG_TRIVIAL(info)
            << "Landlock ABI version: " << LandlockRuleset::get_abi_version();
#else
        BOOST_LOG_TRIVIAL(info) << "Landlock: false";
#endif

        boost::asio::io_context io;

        const std::function<void(boost::system::error_code)> error_handler =
            [](boost::system::error_code)
        {
            throw std::runtime_error("Fatal error");
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
