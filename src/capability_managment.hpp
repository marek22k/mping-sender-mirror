// SPDX-FileCopyrightText: Copyright (C) 2025 Marek Küthe <m.k@mk16.de>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CAPABILITY_HPP
#define CAPABILITY_HPP

#ifdef HAVE_LIBCAPNG
    #include <stdexcept>
    #include <cap-ng.h>
    #include <boost/log/trivial.hpp>

namespace CapabilityManagment
{
    void lock();
    void check_for_capabilites();
    void drop_all_capabilies();
} // namespace CapabilityManagment

#endif

#endif
