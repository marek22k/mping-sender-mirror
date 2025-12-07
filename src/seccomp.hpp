// SPDX-FileCopyrightText: Copyright (C) 2024-2025 Marek Küthe <m.k@mk16.de>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SECCOMP_HPP
#define SECCOMP_HPP

#ifdef HAVE_SECCOMP

    #include <system_error>
    #include <cerrno>
    #include <cstdint>
    #include <seccomp.h>
    #include <sys/syscall.h>

class SeccompFilterContext
{
    public:
        explicit SeccompFilterContext(uint32_t def_action);
        void rule_add(uint32_t action, int syscall) const;
        void allow(int syscall) const;
        void kill(int syscall) const;
        void kill_chown() const;
        void kill_clock() const;
        void kill_cpu_emulation() const;
        void kill_debug() const;
        void kill_others() const;
        void kill_ipc() const;
        void kill_keyring() const;
        void kill_memlock() const;
        void kill_module() const;
        void kill_mount() const;
        void kill_obsolete() const;
        void kill_privileged() const;
        void kill_rawio() const;
        void kill_reboot() const;
        void kill_resources() const;
        void kill_setuid() const;
        void kill_signal() const;
        void kill_swap() const;
        void kill_sync() const;
        void kill_system_service() const;
        void load() const;
        void reset(uint32_t def_action) const;
        void release();
        [[nodiscard]] bool is_useable() const noexcept;

    private:
        scmp_filter_ctx ctx;
};

#endif

#endif
