// SPDX-FileCopyrightText: Copyright (C) 2024-2025 Marek Küthe <m.k@mk16.de>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "capability_managment.hpp"

#ifdef HAVE_LIBCAPNG

void CapabilityManagment::lock()
{
    if (capng_lock() != 0)
        throw std::system_error(
            errno, std::generic_category(), "Failed to lock capabilities");
}

void CapabilityManagment::check_for_capabilites()
{
    if (capng_have_capability(CAPNG_PERMITTED, CAP_SETPCAP) != 1)
        throw std::runtime_error("Missing CAP_SETPCAP capability");
}

void CapabilityManagment::drop_all_capabilies()
{
    capng_clear(CAPNG_SELECT_ALL);

    if (capng_apply(CAPNG_SELECT_ALL) != 0)
        throw std::system_error(
            errno, std::generic_category(), "Failed to apply capabilities");
}

#endif
