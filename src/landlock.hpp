// SPDX-FileCopyrightText: Copyright (C) 2024-2025 Marek Küthe <m.k@mk16.de>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef LANDLOCK_HPP
#define LANDLOCK_HPP

#ifdef HAVE_LANDLOCK

    #include <stdexcept>
    #include <cstdint>
    #include <linux/landlock.h>

    #if defined(NO_LANDLOCK_CREATE_RULESET) || \
        defined(NO_LANDLOCK_ADD_RULE) || defined(NO_LANDLOCK_RESTRICT_SELF)

        #include <unistd.h>
        #include <sys/syscall.h>

        #ifdef NO_LANDLOCK_CREATE_RULESET
inline int landlock_create_ruleset(const struct landlock_ruleset_attr * attr,
                                   size_t size,
                                   uint32_t flags)
{
    return static_cast<int>(
        syscall(SYS_landlock_create_ruleset, attr, size, flags));
}
        #endif

        #ifdef NO_LANDLOCK_ADD_RULE
inline int landlock_add_rule(int ruleset_fd,
                             enum landlock_rule_type rule_type,
                             const void * rule_attr,
                             uint32_t flags)
{
    return static_cast<int>(syscall(
        SYS_landlock_add_rule, ruleset_fd, rule_type, rule_attr, flags));
}
        #endif

        #ifdef NO_LANDLOCK_RESTRICT_SELF
inline int landlock_restrict_self(int ruleset_fd, uint32_t flags)
{
    return static_cast<int>(
        syscall(SYS_landlock_restrict_self, ruleset_fd, flags));
}
        #endif

    #endif

class LandlockRuleset
{
    public:
        explicit LandlockRuleset(uint64_t handled_access_fr,
                                 uint64_t handled_access_net);
        void add_path_beneath_rule(uint64_t allowed_access,
                                   int32_t parent_fd) const;
        void restrict_self() const;

        [[nodiscard]] static int abi_version() noexcept;

    private:
        int ruleset;
};

#endif

#endif
