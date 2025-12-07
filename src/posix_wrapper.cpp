// SPDX-FileCopyrightText: Copyright (C) 2024-2025 Marek Küthe <m.k@mk16.de>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "posix_wrapper.hpp"

unsigned int PosixWrapper::if_nametoindex(const std::string& ifname)
{
    unsigned int result = ::if_nametoindex(ifname.c_str());
    if (result == 0)
    {
        throw std::system_error(
            errno, std::generic_category(), "Failed to get interface index.");
    }
    return result;
}
