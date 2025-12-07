// SPDX-FileCopyrightText: Copyright (C) 2024-2025 Marek Küthe <m.k@mk16.de>
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "seccomp.hpp"

#ifdef HAVE_SECCOMP

SeccompFilterContext::SeccompFilterContext(uint32_t def_action) :
    ctx(seccomp_init(def_action))
{
    if (this->ctx == nullptr)
        throw std::system_error(
            errno, std::generic_category(), "Failed to initialize seccomp.");
}

void SeccompFilterContext::rule_add(uint32_t action, int syscall) const
{
    if (!this->is_useable())
        throw std::runtime_error("seccomp filter context not useable.");

    seccomp_rule_add(this->ctx, action, syscall, 0);
}

void SeccompFilterContext::allow(int syscall) const
{
    this->rule_add(SCMP_ACT_ALLOW, syscall);
}

void SeccompFilterContext::kill(int syscall) const
{
    this->rule_add(SCMP_ACT_KILL, syscall);
}

void SeccompFilterContext::kill_chown() const
{
    // #lizard forgives

    #ifdef SYS_chown
    this->kill(SCMP_SYS(chown)); // flawfinder: ignore
    #endif
    #ifdef SYS_chown32
    this->kill(SCMP_SYS(chown32)); // flawfinder: ignore
    #endif
    #ifdef SYS_fchown
    this->kill(SCMP_SYS(fchown)); // flawfinder: ignore
    #endif
    #ifdef SYS_fchown32
    this->kill(SCMP_SYS(fchown32)); // flawfinder: ignore
    #endif
    #ifdef SYS_fchownat
    this->kill(SCMP_SYS(fchownat)); // flawfinder: ignore
    #endif
    #ifdef SYS_lchown
    this->kill(SCMP_SYS(lchown)); // flawfinder: ignore
    #endif
    #ifdef SYS_lchown32
    this->kill(SCMP_SYS(lchown32)); // flawfinder: ignore
    #endif
}

void SeccompFilterContext::kill_clock() const
{
    // #lizard forgives

    #ifdef SYS_adjtimex
    this->kill(SCMP_SYS(adjtimex)); // flawfinder: ignore
    #endif
    #ifdef SYS_clock_adjtime
    this->kill(SCMP_SYS(clock_adjtime)); // flawfinder: ignore
    #endif
    #ifdef SYS_clock_settime
    this->kill(SCMP_SYS(clock_settime)); // flawfinder: ignore
    #endif
    #ifdef SYS_settimeofday
    this->kill(SCMP_SYS(settimeofday)); // flawfinder: ignore
    #endif
    #ifdef SYS_stime
    this->kill(SCMP_SYS(stime)); // flawfinder: ignore
    #endif
}

void SeccompFilterContext::kill_cpu_emulation() const
{
    // #lizard forgives

    #ifdef SYS_modify_ldt
    this->kill(SCMP_SYS(modify_ldt)); // flawfinder: ignore
    #endif
    #ifdef SYS_subpage_prot
    this->kill(SCMP_SYS(subpage_prot)); // flawfinder: ignore
    #endif
    #ifdef SYS_switch_endian
    this->kill(SCMP_SYS(switch_endian)); // flawfinder: ignore
    #endif
    #ifdef SYS_vm86
    this->kill(SCMP_SYS(vm86)); // flawfinder: ignore
    #endif
    #ifdef SYS_vm86old
    this->kill(SCMP_SYS(vm86old)); // flawfinder: ignore
    #endif
}

void SeccompFilterContext::kill_debug() const
{
    // #lizard forgives

    #ifdef SYS_lookup_dcookie
    this->kill(SCMP_SYS(lookup_dcookie)); // flawfinder: ignore
    #endif
    #ifdef SYS_perf_event_open
    this->kill(SCMP_SYS(perf_event_open)); // flawfinder: ignore
    #endif
    #ifdef SYS_pidfd_getfd
    this->kill(SCMP_SYS(pidfd_getfd)); // flawfinder: ignore
    #endif
    #ifdef SYS_process_vm_writev
    this->kill(SCMP_SYS(process_vm_writev)); // flawfinder: ignore
    #endif
    #ifdef SYS_rtas
    this->kill(SCMP_SYS(rtas)); // flawfinder: ignore
    #endif
    #ifdef SYS_s390_runtime_instr
    this->kill(SCMP_SYS(s390_runtime_instr)); // flawfinder: ignore
    #endif
    #ifdef SYS_sys_debug_setcontext
    this->kill(SCMP_SYS(sys_debug_setcontext)); // flawfinder: ignore
    #endif
}

void SeccompFilterContext::kill_others() const
{
    // #lizard forgives

    #ifdef SYS_open_by_handle_at
    this->kill(SCMP_SYS(open_by_handle_at)); // flawfinder: ignore
    #endif
    #ifdef SYS_name_to_handle_at
    this->kill(SCMP_SYS(name_to_handle_at)); // flawfinder: ignore
    #endif
    #ifdef SYS_ioprio_set
    this->kill(SCMP_SYS(ioprio_set)); // flawfinder: ignore
    #endif
    #ifdef SYS_ni_syscall
    this->kill(SCMP_SYS(ni_syscall)); // flawfinder: ignore
    #endif
    #ifdef SYS_syslog
    this->kill(SCMP_SYS(syslog)); // flawfinder: ignore
    #endif
    #ifdef SYS_fanotify_init
    this->kill(SCMP_SYS(fanotify_init)); // flawfinder: ignore
    #endif
    #ifdef SYS_add_key
    this->kill(SCMP_SYS(add_key)); // flawfinder: ignore
    #endif
    #ifdef SYS_request_key
    this->kill(SCMP_SYS(request_key)); // flawfinder: ignore
    #endif
    #ifdef SYS_mbind
    this->kill(SCMP_SYS(mbind)); // flawfinder: ignore
    #endif
    #ifdef SYS_migrate_pages
    this->kill(SCMP_SYS(migrate_pages)); // flawfinder: ignore
    #endif
    #ifdef SYS_move_pages
    this->kill(SCMP_SYS(move_pages)); // flawfinder: ignore
    #endif
    #ifdef SYS_keyctl
    this->kill(SCMP_SYS(keyctl)); // flawfinder: ignore
    #endif
    #ifdef SYS_io_setup
    this->kill(SCMP_SYS(io_setup)); // flawfinder: ignore
    #endif
    #ifdef SYS_io_destroy
    this->kill(SCMP_SYS(io_destroy)); // flawfinder: ignore
    #endif
    #ifdef SYS_io_getevents
    this->kill(SCMP_SYS(io_getevents)); // flawfinder: ignore
    #endif
    #ifdef SYS_io_submit
    this->kill(SCMP_SYS(io_submit)); // flawfinder: ignore
    #endif
    #ifdef SYS_io_cancel
    this->kill(SCMP_SYS(io_cancel)); // flawfinder: ignore
    #endif
    #ifdef SYS_remap_file_pages
    this->kill(SCMP_SYS(remap_file_pages)); // flawfinder: ignore
    #endif
    #ifdef SYS_set_mempolicyvmsplice
    this->kill(SCMP_SYS(set_mempolicyvmsplice)); // flawfinder: ignore
    #endif
    #ifdef SYS_userfaultfd
    this->kill(SCMP_SYS(userfaultfd)); // flawfinder: ignore
    #endif
    #ifdef SYS_acct
    this->kill(SCMP_SYS(acct)); // flawfinder: ignore
    #endif
    #ifdef SYS_bpf
    this->kill(SCMP_SYS(bpf)); // flawfinder: ignore
    #endif
    #ifdef SYS_nfsservctl
    this->kill(SCMP_SYS(nfsservctl)); // flawfinder: ignore
    #endif
    #ifdef SYS_setdomainname
    this->kill(SCMP_SYS(setdomainname)); // flawfinder: ignore
    #endif
    #ifdef SYS_sethostname
    this->kill(SCMP_SYS(sethostname)); // flawfinder: ignore
    #endif
    #ifdef SYS_vhangup
    this->kill(SCMP_SYS(vhangup)); // flawfinder: ignore
    #endif
    #ifdef SYS_ptrace
    this->kill(SCMP_SYS(ptrace)); // flawfinder: ignore
    #endif
    #ifdef SYS_personality
    this->kill(SCMP_SYS(personality)); // flawfinder: ignore
    #endif
    #ifdef SYS_process_vm_readv
    this->kill(SCMP_SYS(process_vm_readv)); // flawfinder: ignore
    #endif
}

void SeccompFilterContext::kill_ipc() const
{
    // #lizard forgives

    #ifdef SYS_ipc
    this->kill(SCMP_SYS(ipc)); // flawfinder: ignore
    #endif
    #ifdef SYS_memfd_create
    this->kill(SCMP_SYS(memfd_create)); // flawfinder: ignore
    #endif
    #ifdef SYS_mq_getsetattr
    this->kill(SCMP_SYS(mq_getsetattr)); // flawfinder: ignore
    #endif
    #ifdef SYS_mq_notify
    this->kill(SCMP_SYS(mq_notify)); // flawfinder: ignore
    #endif
    #ifdef SYS_mq_open
    this->kill(SCMP_SYS(mq_open)); // flawfinder: ignore
    #endif
    #ifdef SYS_mq_timedreceive
    this->kill(SCMP_SYS(mq_timedreceive)); // flawfinder: ignore
    #endif
    #ifdef SYS_mq_timedsend
    this->kill(SCMP_SYS(mq_timedsend)); // flawfinder: ignore
    #endif
    #ifdef SYS_mq_unlink
    this->kill(SCMP_SYS(mq_unlink)); // flawfinder: ignore
    #endif
    #ifdef SYS_msgctl
    this->kill(SCMP_SYS(msgctl)); // flawfinder: ignore
    #endif
    #ifdef SYS_msgget
    this->kill(SCMP_SYS(msgget)); // flawfinder: ignore
    #endif
    #ifdef SYS_msgrcv
    this->kill(SCMP_SYS(msgrcv)); // flawfinder: ignore
    #endif
    #ifdef SYS_msgsnd
    this->kill(SCMP_SYS(msgsnd)); // flawfinder: ignore
    #endif
    #ifdef SYS_process_madvise
    this->kill(SCMP_SYS(process_madvise)); // flawfinder: ignore
    #endif
    #ifdef SYS_process_vm_readv
    this->kill(SCMP_SYS(process_vm_readv)); // flawfinder: ignore
    #endif
    #ifdef SYS_process_vm_writev
    this->kill(SCMP_SYS(process_vm_writev)); // flawfinder: ignore
    #endif
    #ifdef SYS_semctl
    this->kill(SCMP_SYS(semctl)); // flawfinder: ignore
    #endif
    #ifdef SYS_semget
    this->kill(SCMP_SYS(semget)); // flawfinder: ignore
    #endif
    #ifdef SYS_semop
    this->kill(SCMP_SYS(semop)); // flawfinder: ignore
    #endif
    #ifdef SYS_semtimedop
    this->kill(SCMP_SYS(semtimedop)); // flawfinder: ignore
    #endif
    #ifdef SYS_shmat
    this->kill(SCMP_SYS(shmat)); // flawfinder: ignore
    #endif
    #ifdef SYS_shmctl
    this->kill(SCMP_SYS(shmctl)); // flawfinder: ignore
    #endif
    #ifdef SYS_shmdt
    this->kill(SCMP_SYS(shmdt)); // flawfinder: ignore
    #endif
    #ifdef SYS_pipe
    this->kill(SCMP_SYS(pipe)); // flawfinder: ignore
    #endif
    #ifdef SYS_pipe2
    this->kill(SCMP_SYS(pipe2)); // flawfinder: ignore
    #endif
}

void SeccompFilterContext::kill_keyring() const
{
    // #lizard forgives

    #ifdef SYS_add_key
    this->kill(SCMP_SYS(add_key)); // flawfinder: ignore
    #endif
    #ifdef SYS_keyctl
    this->kill(SCMP_SYS(keyctl)); // flawfinder: ignore
    #endif
    #ifdef SYS_request_key
    this->kill(SCMP_SYS(request_key)); // flawfinder: ignore
    #endif
}

void SeccompFilterContext::kill_memlock() const
{
    // #lizard forgives

    #ifdef SYS_mlock
    this->kill(SCMP_SYS(mlock)); // flawfinder: ignore
    #endif
    #ifdef SYS_mlock2
    this->kill(SCMP_SYS(mlock2)); // flawfinder: ignore
    #endif
    #ifdef SYS_mlockall
    this->kill(SCMP_SYS(mlockall)); // flawfinder: ignore
    #endif
    #ifdef SYS_munlock
    this->kill(SCMP_SYS(munlock)); // flawfinder: ignore
    #endif
    #ifdef SYS_munlockall
    this->kill(SCMP_SYS(munlockall)); // flawfinder: ignore
    #endif
}

void SeccompFilterContext::kill_module() const
{
    // #lizard forgives

    #ifdef SYS_delete_module
    this->kill(SCMP_SYS(delete_module)); // flawfinder: ignore
    #endif
    #ifdef SYS_finit_module
    this->kill(SCMP_SYS(finit_module)); // flawfinder: ignore
    #endif
    #ifdef SYS_init_module
    this->kill(SCMP_SYS(init_module)); // flawfinder: ignore
    #endif
}

void SeccompFilterContext::kill_mount() const
{
    // #lizard forgives

    #ifdef SYS_chroot
    this->kill(SCMP_SYS(chroot)); // flawfinder: ignore
    #endif
    #ifdef SYS_fsconfig
    this->kill(SCMP_SYS(fsconfig)); // flawfinder: ignore
    #endif
    #ifdef SYS_fsmount
    this->kill(SCMP_SYS(fsmount)); // flawfinder: ignore
    #endif
    #ifdef SYS_fsopen
    this->kill(SCMP_SYS(fsopen)); // flawfinder: ignore
    #endif
    #ifdef SYS_fspick
    this->kill(SCMP_SYS(fspick)); // flawfinder: ignore
    #endif
    #ifdef SYS_mount
    this->kill(SCMP_SYS(mount)); // flawfinder: ignore
    #endif
    #ifdef SYS_move_mount
    this->kill(SCMP_SYS(move_mount)); // flawfinder: ignore
    #endif
    #ifdef SYS_open_tree
    this->kill(SCMP_SYS(open_tree)); // flawfinder: ignore
    #endif
    #ifdef SYS_pivot_root
    this->kill(SCMP_SYS(pivot_root)); // flawfinder: ignore
    #endif
    #ifdef SYS_umount
    this->kill(SCMP_SYS(umount)); // flawfinder: ignore
    #endif
    #ifdef SYS_umount2
    this->kill(SCMP_SYS(umount2)); // flawfinder: ignore
    #endif
}

void SeccompFilterContext::kill_obsolete() const
{
    // #lizard forgives

    #ifdef SYS__sysctl
    this->kill(SCMP_SYS(_sysctl)); // flawfinder: ignore
    #endif
    #ifdef SYS_afs_syscall
    this->kill(SCMP_SYS(afs_syscall)); // flawfinder: ignore
    #endif
    #ifdef SYS_bdflush
    this->kill(SCMP_SYS(bdflush)); // flawfinder: ignore
    #endif
    #ifdef SYS_break
    this->kill(SCMP_SYS(break)); // flawfinder: ignore
    #endif
    #ifdef SYS_create_module
    this->kill(SCMP_SYS(create_module)); // flawfinder: ignore
    #endif
    #ifdef SYS_ftime
    this->kill(SCMP_SYS(ftime)); // flawfinder: ignore
    #endif
    #ifdef SYS_get_kernel_syms
    this->kill(SCMP_SYS(get_kernel_syms)); // flawfinder: ignore
    #endif
    #ifdef SYS_getpmsg
    this->kill(SCMP_SYS(getpmsg)); // flawfinder: ignore
    #endif
    #ifdef SYS_gtty
    this->kill(SCMP_SYS(gtty)); // flawfinder: ignore
    #endif
    #ifdef SYS_idle
    this->kill(SCMP_SYS(idle)); // flawfinder: ignore
    #endif
    #ifdef SYS_lock
    this->kill(SCMP_SYS(lock)); // flawfinder: ignore
    #endif
    #ifdef SYS_mpx
    this->kill(SCMP_SYS(mpx)); // flawfinder: ignore
    #endif
    #ifdef SYS_prof
    this->kill(SCMP_SYS(prof)); // flawfinder: ignore
    #endif
    #ifdef SYS_profil
    this->kill(SCMP_SYS(profil)); // flawfinder: ignore
    #endif
    #ifdef SYS_putpmsg
    this->kill(SCMP_SYS(putpmsg)); // flawfinder: ignore
    #endif
    #ifdef SYS_query_module
    this->kill(SCMP_SYS(query_module)); // flawfinder: ignore
    #endif
    #ifdef SYS_security
    this->kill(SCMP_SYS(security)); // flawfinder: ignore
    #endif
    #ifdef SYS_sgetmask
    this->kill(SCMP_SYS(sgetmask)); // flawfinder: ignore
    #endif
    #ifdef SYS_ssetmask
    this->kill(SCMP_SYS(ssetmask)); // flawfinder: ignore
    #endif
    #ifdef SYS_stty
    this->kill(SCMP_SYS(stty)); // flawfinder: ignore
    #endif
    #ifdef SYS_sysfs
    this->kill(SCMP_SYS(sysfs)); // flawfinder: ignore
    #endif
    #ifdef SYS_tuxcall
    this->kill(SCMP_SYS(tuxcall)); // flawfinder: ignore
    #endif
    #ifdef SYS_ulimit
    this->kill(SCMP_SYS(ulimit)); // flawfinder: ignore
    #endif
    #ifdef SYS_uselib
    this->kill(SCMP_SYS(uselib)); // flawfinder: ignore
    #endif
    #ifdef SYS_ustat
    this->kill(SCMP_SYS(ustat)); // flawfinder: ignore
    #endif
    #ifdef SYS_vserver
    this->kill(SCMP_SYS(vserver)); // flawfinder: ignore
    #endif
}

void SeccompFilterContext::kill_privileged() const
{
    // #lizard forgives

    // without capset

    #ifdef SYS__sysctl
    this->kill(SCMP_SYS(_sysctl)); // flawfinder: ignore
    #endif
    #ifdef SYS_acct
    this->kill(SCMP_SYS(acct)); // flawfinder: ignore
    #endif
    #ifdef SYS_bpf
    this->kill(SCMP_SYS(bpf)); // flawfinder: ignore
    #endif
    #ifdef SYS_chroot
    this->kill(SCMP_SYS(chroot)); // flawfinder: ignore
    #endif
    #ifdef SYS_fanotify_init
    this->kill(SCMP_SYS(fanotify_init)); // flawfinder: ignore
    #endif
    #ifdef SYS_mount
    this->kill(SCMP_SYS(mount)); // flawfinder: ignore
    #endif
    #ifdef SYS_nfsservctl
    this->kill(SCMP_SYS(nfsservctl)); // flawfinder: ignore
    #endif
    #ifdef SYS_open_by_handle_at
    this->kill(SCMP_SYS(open_by_handle_at)); // flawfinder: ignore
    #endif
    #ifdef SYS_pivot_root
    this->kill(SCMP_SYS(pivot_root)); // flawfinder: ignore
    #endif
    #ifdef SYS_quotactl
    this->kill(SCMP_SYS(quotactl)); // flawfinder: ignore
    #endif
    #ifdef SYS_setdomainname
    this->kill(SCMP_SYS(setdomainname)); // flawfinder: ignore
    #endif
    #ifdef SYS_setfsuid
    this->kill(SCMP_SYS(setfsuid)); // flawfinder: ignore
    #endif
    #ifdef SYS_setfsuid32
    this->kill(SCMP_SYS(setfsuid32)); // flawfinder: ignore
    #endif
    #ifdef SYS_setgroups
    this->kill(SCMP_SYS(setgroups)); // flawfinder: ignore
    #endif
    #ifdef SYS_setgroups32
    this->kill(SCMP_SYS(setgroups32)); // flawfinder: ignore
    #endif
    #ifdef SYS_sethostname
    this->kill(SCMP_SYS(sethostname)); // flawfinder: ignore
    #endif
    #ifdef SYS_setresuid
    this->kill(SCMP_SYS(setresuid)); // flawfinder: ignore
    #endif
    #ifdef SYS_setresuid32
    this->kill(SCMP_SYS(setresuid32)); // flawfinder: ignore
    #endif
    #ifdef SYS_setreuid
    this->kill(SCMP_SYS(setreuid)); // flawfinder: ignore
    #endif
    #ifdef SYS_setreuid32
    this->kill(SCMP_SYS(setreuid32)); // flawfinder: ignore
    #endif
    #ifdef SYS_setuid
    this->kill(SCMP_SYS(setuid)); // flawfinder: ignore
    #endif
    #ifdef SYS_setuid32
    this->kill(SCMP_SYS(setuid32)); // flawfinder: ignore
    #endif
    #ifdef SYS_umount2
    this->kill(SCMP_SYS(umount2)); // flawfinder: ignore
    #endif
    #ifdef SYS_vhangup
    this->kill(SCMP_SYS(vhangup)); // flawfinder: ignore
    #endif
}

void SeccompFilterContext::kill_rawio() const
{
    // #lizard forgives

    #ifdef SYS_ioperm
    this->kill(SCMP_SYS(ioperm)); // flawfinder: ignore
    #endif
    #ifdef SYS_iopl
    this->kill(SCMP_SYS(iopl)); // flawfinder: ignore
    #endif
    #ifdef SYS_pciconfig_iobase
    this->kill(SCMP_SYS(pciconfig_iobase)); // flawfinder: ignore
    #endif
    #ifdef SYS_pciconfig_read
    this->kill(SCMP_SYS(pciconfig_read)); // flawfinder: ignore
    #endif
    #ifdef SYS_pciconfig_write
    this->kill(SCMP_SYS(pciconfig_write)); // flawfinder: ignore
    #endif
    #ifdef SYS_s390_pci_mmio_read
    this->kill(SCMP_SYS(s390_pci_mmio_read)); // flawfinder: ignore
    #endif
    #ifdef SYS_s390_pci_mmio_write
    this->kill(SCMP_SYS(s390_pci_mmio_write)); // flawfinder: ignore
    #endif
}

void SeccompFilterContext::kill_reboot() const
{
    // #lizard forgives

    #ifdef SYS_kexec_load
    this->kill(SCMP_SYS(kexec_load)); // flawfinder: ignore
    #endif
    #ifdef SYS_kexec_file_load
    this->kill(SCMP_SYS(kexec_file_load)); // flawfinder: ignore
    #endif
    #ifdef SYS_reboot
    this->kill(SCMP_SYS(reboot)); // flawfinder: ignore
    #endif
}

void SeccompFilterContext::kill_resources() const
{
    // #lizard forgives

    #ifdef SYS_ioprio_set
    this->kill(SCMP_SYS(ioprio_set)); // flawfinder: ignore
    #endif
    #ifdef SYS_mbind
    this->kill(SCMP_SYS(mbind)); // flawfinder: ignore
    #endif
    #ifdef SYS_migrate_pages
    this->kill(SCMP_SYS(migrate_pages)); // flawfinder: ignore
    #endif
    #ifdef SYS_move_pages
    this->kill(SCMP_SYS(move_pages)); // flawfinder: ignore
    #endif
    #ifdef SYS_nice
    this->kill(SCMP_SYS(nice)); // flawfinder: ignore
    #endif
    #ifdef SYS_sched_setaffinity
    this->kill(SCMP_SYS(sched_setaffinity)); // flawfinder: ignore
    #endif
    #ifdef SYS_sched_setattr
    this->kill(SCMP_SYS(sched_setattr)); // flawfinder: ignore
    #endif
    #ifdef SYS_sched_setparam
    this->kill(SCMP_SYS(sched_setparam)); // flawfinder: ignore
    #endif
    #ifdef SYS_sched_setscheduler
    this->kill(SCMP_SYS(sched_setscheduler)); // flawfinder: ignore
    #endif
    #ifdef SYS_set_mempolicy
    this->kill(SCMP_SYS(set_mempolicy)); // flawfinder: ignore
    #endif
}

void SeccompFilterContext::kill_setuid() const
{
    // #lizard forgives

    #ifdef SYS_setgid
    this->kill(SCMP_SYS(setgid)); // flawfinder: ignore
    #endif
    #ifdef SYS_setgid32
    this->kill(SCMP_SYS(setgid32)); // flawfinder: ignore
    #endif
    #ifdef SYS_setgroups
    this->kill(SCMP_SYS(setgroups)); // flawfinder: ignore
    #endif
    #ifdef SYS_setgroups32
    this->kill(SCMP_SYS(setgroups32)); // flawfinder: ignore
    #endif
    #ifdef SYS_setregid
    this->kill(SCMP_SYS(setregid)); // flawfinder: ignore
    #endif
    #ifdef SYS_setregid32
    this->kill(SCMP_SYS(setregid32)); // flawfinder: ignore
    #endif
    #ifdef SYS_setresgid
    this->kill(SCMP_SYS(setresgid)); // flawfinder: ignore
    #endif
    #ifdef SYS_setresgid32
    this->kill(SCMP_SYS(setresgid32)); // flawfinder: ignore
    #endif
    #ifdef SYS_setresuid
    this->kill(SCMP_SYS(setresuid)); // flawfinder: ignore
    #endif
    #ifdef SYS_setresuid32
    this->kill(SCMP_SYS(setresuid32)); // flawfinder: ignore
    #endif
    #ifdef SYS_setreuid
    this->kill(SCMP_SYS(setreuid)); // flawfinder: ignore
    #endif
    #ifdef SYS_setreuid32
    this->kill(SCMP_SYS(setreuid32)); // flawfinder: ignore
    #endif
    #ifdef SYS_setuid
    this->kill(SCMP_SYS(setuid)); // flawfinder: ignore
    #endif
    #ifdef SYS_setuid32
    this->kill(SCMP_SYS(setuid32)); // flawfinder: ignore
    #endif
}

void SeccompFilterContext::kill_signal() const
{
    // #lizard forgives

    #ifdef SYS_rt_sigaction
    this->kill(SCMP_SYS(rt_sigaction)); // flawfinder: ignore
    #endif
    #ifdef SYS_rt_sigpending
    this->kill(SCMP_SYS(rt_sigpending)); // flawfinder: ignore
    #endif
    #ifdef SYS_rt_sigprocmask
    this->kill(SCMP_SYS(rt_sigprocmask)); // flawfinder: ignore
    #endif
    #ifdef SYS_rt_sigsuspend
    this->kill(SCMP_SYS(rt_sigsuspend)); // flawfinder: ignore
    #endif
    #ifdef SYS_rt_sigtimedwait
    this->kill(SCMP_SYS(rt_sigtimedwait)); // flawfinder: ignore
    #endif
    #ifdef SYS_sigaction
    this->kill(SCMP_SYS(sigaction)); // flawfinder: ignore
    #endif
    #ifdef SYS_sigaltstack
    this->kill(SCMP_SYS(sigaltstack)); // flawfinder: ignore
    #endif
    #ifdef SYS_signal
    this->kill(SCMP_SYS(signal)); // flawfinder: ignore
    #endif
    #ifdef SYS_signalfd
    this->kill(SCMP_SYS(signalfd)); // flawfinder: ignore
    #endif
    #ifdef SYS_signalfd4
    this->kill(SCMP_SYS(signalfd4)); // flawfinder: ignore
    #endif
    #ifdef SYS_sigpending
    this->kill(SCMP_SYS(sigpending)); // flawfinder: ignore
    #endif
    #ifdef SYS_sigprocmask
    this->kill(SCMP_SYS(sigprocmask)); // flawfinder: ignore
    #endif
    #ifdef SYS_sigsuspend
    this->kill(SCMP_SYS(sigsuspend)); // flawfinder: ignore
    #endif
}

void SeccompFilterContext::kill_swap() const
{
    // #lizard forgives

    #ifdef SYS_swapon
    this->kill(SCMP_SYS(swapon)); // flawfinder: ignore
    #endif
    #ifdef SYS_swapoff
    this->kill(SCMP_SYS(swapoff)); // flawfinder: ignore
    #endif
}

void SeccompFilterContext::kill_sync() const
{
    // #lizard forgives

    #ifdef SYS_fdatasync
    this->kill(SCMP_SYS(fdatasync)); // flawfinder: ignore
    #endif
    #ifdef SYS_fsync
    this->kill(SCMP_SYS(fsync)); // flawfinder: ignore
    #endif
    #ifdef SYS_msync
    this->kill(SCMP_SYS(msync)); // flawfinder: ignore
    #endif
    #ifdef SYS_sync
    this->kill(SCMP_SYS(sync)); // flawfinder: ignore
    #endif
    #ifdef SYS_sync_file_range
    this->kill(SCMP_SYS(sync_file_range)); // flawfinder: ignore
    #endif
    #ifdef SYS_sync_file_range2
    this->kill(SCMP_SYS(sync_file_range2)); // flawfinder: ignore
    #endif
    #ifdef SYS_syncfs
    this->kill(SCMP_SYS(syncfs)); // flawfinder: ignore
    #endif
}

void SeccompFilterContext::kill_system_service() const
{
    // #lizard forgives

    // without ioctl, capget, capset, getrandom, mprotect, brk

    #ifdef SYS_copy_file_range
    this->kill(SCMP_SYS(copy_file_range)); // flawfinder: ignore
    #endif
    #ifdef SYS_fadvise64
    this->kill(SCMP_SYS(fadvise64)); // flawfinder: ignore
    #endif
    #ifdef SYS_fadvise64_64
    this->kill(SCMP_SYS(fadvise64_64)); // flawfinder: ignore
    #endif
    #ifdef SYS_flock
    this->kill(SCMP_SYS(flock)); // flawfinder: ignore
    #endif
    #ifdef SYS_get_mempolicy
    this->kill(SCMP_SYS(get_mempolicy)); // flawfinder: ignore
    #endif
    #ifdef SYS_getcpu
    this->kill(SCMP_SYS(getcpu)); // flawfinder: ignore
    #endif
    #ifdef SYS_getpriority
    this->kill(SCMP_SYS(getpriority)); // flawfinder: ignore
    #endif
    #ifdef SYS_ioprio_get
    this->kill(SCMP_SYS(ioprio_get)); // flawfinder: ignore
    #endif
    #ifdef SYS_kcmp
    this->kill(SCMP_SYS(kcmp)); // flawfinder: ignore
    #endif
    #ifdef SYS_madvise
    this->kill(SCMP_SYS(madvise)); // flawfinder: ignore
    #endif
    #ifdef SYS_mremap
    this->kill(SCMP_SYS(mremap)); // flawfinder: ignore
    #endif
    #ifdef SYS_name_to_handle_at
    this->kill(SCMP_SYS(name_to_handle_at)); // flawfinder: ignore
    #endif
    #ifdef SYS_oldolduname
    this->kill(SCMP_SYS(oldolduname)); // flawfinder: ignore
    #endif
    #ifdef SYS_olduname
    this->kill(SCMP_SYS(olduname)); // flawfinder: ignore
    #endif
    #ifdef SYS_personality
    this->kill(SCMP_SYS(personality)); // flawfinder: ignore
    #endif
    #ifdef SYS_readahead
    this->kill(SCMP_SYS(readahead)); // flawfinder: ignore
    #endif
    #ifdef SYS_readdir
    this->kill(SCMP_SYS(readdir)); // flawfinder: ignore
    #endif
    #ifdef SYS_remap_file_pages
    this->kill(SCMP_SYS(remap_file_pages)); // flawfinder: ignore
    #endif
    #ifdef SYS_sched_get_priority_max
    this->kill(SCMP_SYS(sched_get_priority_max)); // flawfinder: ignore
    #endif
    #ifdef SYS_sched_get_priority_min
    this->kill(SCMP_SYS(sched_get_priority_min)); // flawfinder: ignore
    #endif
    #ifdef SYS_sched_getaffinity
    this->kill(SCMP_SYS(sched_getaffinity)); // flawfinder: ignore
    #endif
    #ifdef SYS_sched_getattr
    this->kill(SCMP_SYS(sched_getattr)); // flawfinder: ignore
    #endif
    #ifdef SYS_sched_getparam
    this->kill(SCMP_SYS(sched_getparam)); // flawfinder: ignore
    #endif
    #ifdef SYS_sched_getscheduler
    this->kill(SCMP_SYS(sched_getscheduler)); // flawfinder: ignore
    #endif
    #ifdef SYS_sched_rr_get_interval
    this->kill(SCMP_SYS(sched_rr_get_interval)); // flawfinder: ignore
    #endif
    #ifdef SYS_sched_yield
    this->kill(SCMP_SYS(sched_yield)); // flawfinder: ignore
    #endif
    #ifdef SYS_sendfile
    this->kill(SCMP_SYS(sendfile)); // flawfinder: ignore
    #endif
    #ifdef SYS_sendfile64
    this->kill(SCMP_SYS(sendfile64)); // flawfinder: ignore
    #endif
    #ifdef SYS_setfsgid
    this->kill(SCMP_SYS(setfsgid)); // flawfinder: ignore
    #endif
    #ifdef SYS_setfsgid32
    this->kill(SCMP_SYS(setfsgid32)); // flawfinder: ignore
    #endif
    #ifdef SYS_setfsuid
    this->kill(SCMP_SYS(setfsuid)); // flawfinder: ignore
    #endif
    #ifdef SYS_setfsuid32
    this->kill(SCMP_SYS(setfsuid32)); // flawfinder: ignore
    #endif
    #ifdef SYS_setpgid
    this->kill(SCMP_SYS(setpgid)); // flawfinder: ignore
    #endif
    #ifdef SYS_setsid
    this->kill(SCMP_SYS(setsid)); // flawfinder: ignore
    #endif
    #ifdef SYS_splice
    this->kill(SCMP_SYS(splice)); // flawfinder: ignore
    #endif
    #ifdef SYS_sysinfo
    this->kill(SCMP_SYS(sysinfo)); // flawfinder: ignore
    #endif
    #ifdef SYS_tee
    this->kill(SCMP_SYS(tee)); // flawfinder: ignore
    #endif
    #ifdef SYS_umask
    this->kill(SCMP_SYS(umask)); // flawfinder: ignore
    #endif
    #ifdef SYS_uname
    this->kill(SCMP_SYS(uname)); // flawfinder: ignore
    #endif
    #ifdef SYS_userfaultfd
    this->kill(SCMP_SYS(userfaultfd)); // flawfinder: ignore
    #endif
    #ifdef SYS_vmsplice
    this->kill(SCMP_SYS(vmsplice)); // flawfinder: ignore
    #endif
}

void SeccompFilterContext::load() const
{
    if (!this->is_useable())
        throw std::runtime_error("seccomp filter context not useable.");

    if (seccomp_load(this->ctx) != 0)
        throw std::system_error(
            errno, std::generic_category(), "Failed to load seccomp filter.");
}

void SeccompFilterContext::reset(uint32_t def_action) const
{
    if (!this->is_useable())
        throw std::runtime_error("seccomp filter context not useable.");

    if (seccomp_reset(this->ctx, def_action) != 0)
        throw std::system_error(
            errno, std::generic_category(), "Failed to reset seccomp filter.");
}

void SeccompFilterContext::release()
{
    if (!this->is_useable())
        throw std::runtime_error("seccomp filter context not useable.");

    seccomp_release(this->ctx);
    this->ctx = nullptr;
}

bool SeccompFilterContext::is_useable() const noexcept
{
    return this->ctx != nullptr;
}

#endif
