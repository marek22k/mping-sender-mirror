// SPDX-FileCopyrightText: Copyright (C) 2025 Marek Küthe <m.k@mk16.de>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "landlock.hpp"

#ifdef HAVE_LANDLOCK

    #include <linux/landlock.h>

LandlockRuleset::LandlockRuleset(uint64_t handled_access_fr,
                                 uint64_t handled_access_net,
                                 uint64_t scoped) :
    abi_version(LandlockRuleset::get_abi_version())
{
    if (this->abi_version < 6)
        throw std::runtime_error("Landlock is too old.");

    const struct landlock_ruleset_attr attr = {
        .handled_access_fs = handled_access_fr,
        .handled_access_net = handled_access_net,
        .scoped = scoped};

    this->ruleset = ::landlock_create_ruleset(&attr, sizeof(attr), 0);
    if (this->ruleset == -1)
        throw std::system_error(errno,
                                std::generic_category(),
                                "Failed to create landlock ruleset");
}

void LandlockRuleset::add_path_beneath_rule(uint64_t allowed_access,
                                            int32_t parent_fd) const
{
    const struct landlock_path_beneath_attr attr = {
        .allowed_access = allowed_access, .parent_fd = parent_fd};
    if (::landlock_add_rule(this->ruleset,
                            LANDLOCK_RULE_PATH_BENEATH,
                            static_cast<const void *>(&attr),
                            0) != 0)
        throw std::system_error(
            errno, std::generic_category(), "Failed to add path beneath rule");
}

void LandlockRuleset::add_net_port_rule(uint64_t allowed_access,
                                        uint64_t port) const
{
    const struct landlock_net_port_attr attr = {
        .allowed_access = allowed_access, .port = port};
    if (::landlock_add_rule(this->ruleset,
                            LANDLOCK_RULE_NET_PORT,
                            static_cast<const void *>(&attr),
                            0) != 0)
        throw std::system_error(
            errno, std::generic_category(), "Failed to add net port rule");
}

    #ifdef HAVE_LANDLOCK_RESTRICT_SELF_LOG_NEW_EXEC_ON
void LandlockRuleset::restrict_self(uint32_t flags) const
{
    uint32_t compatility_flags = flags;
    if (abi_version < 7)
        compatility_flags &= ~LANDLOCK_RESTRICT_SELF_LOG_NEW_EXEC_ON;

    if (::landlock_restrict_self(this->ruleset, compatility_flags) != 0)
        throw std::system_error(errno,
                                std::generic_category(),
                                "Failed to restrict self via landlock");
}
    #else
void LandlockRuleset::restrict_self() const
{
    if (::landlock_restrict_self(this->ruleset, 0) != 0)
        throw std::system_error(errno,
                                std::generic_category(),
                                "Failed to restrict self via landlock");
}
    #endif

[[nodiscard]] int LandlockRuleset::get_abi_version() noexcept
{
    return ::landlock_create_ruleset(
        nullptr, 0, LANDLOCK_CREATE_RULESET_VERSION);
}

#endif
