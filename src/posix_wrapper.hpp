// SPDX-FileCopyrightText: Copyright (C) 2025 Marek Küthe <m.k@mk16.de>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef POSIX_WRAPPER
#define POSIX_WRAPPER

#include <string>
#include <system_error>
#include <net/if.h>

namespace PosixWrapper
{
    unsigned int if_nametoindex(const std::string& ifname);
}

#endif
