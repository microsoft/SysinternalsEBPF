/*
    SysinternalsEBPF

    Copyright (c) Microsoft Corporation

    All rights reserved.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

//====================================================================
//
// syscalls.h
//
// Linux syscall names and number of arguments, indexed by syscall
// number.  Uses designated initializers with __NR_* constants so the
// table is correct for both x86_64 and aarch64 without maintaining
// separate numeric tables.
//
//====================================================================

#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <asm/unistd.h>
#include "sysinternalsEBPF.h"

typedef struct {
    const char name[SYSCALL_NAME_LEN];
    const unsigned int numArgs;
} syscallNames;

const syscallNames syscallNumToName[SYSCALL_MAX+1] = {
    [__NR_read] = {"read", 3},
    [__NR_write] = {"write", 3},
#ifdef __NR_open
    [__NR_open] = {"open", 3},
#endif
    [__NR_close] = {"close", 1},
#ifdef __NR_stat
    [__NR_stat] = {"stat", 2},
#endif
    [__NR_fstat] = {"fstat", 2},
#ifdef __NR_lstat
    [__NR_lstat] = {"lstat", 2},
#endif
#ifdef __NR_poll
    [__NR_poll] = {"poll", 3},
#endif
    [__NR_lseek] = {"lseek", 3},
    [__NR_mmap] = {"mmap", 6},
    [__NR_mprotect] = {"mprotect", 3},
    [__NR_munmap] = {"munmap", 2},
    [__NR_brk] = {"brk", 1},
    [__NR_rt_sigaction] = {"rt_sigaction", 4},
    [__NR_rt_sigprocmask] = {"rt_sigprocmask", 4},
    [__NR_rt_sigreturn] = {"rt_sigreturn", 1},
    [__NR_ioctl] = {"ioctl", 3},
    [__NR_pread64] = {"pread64", 4},
    [__NR_pwrite64] = {"pwrite64", 4},
    [__NR_readv] = {"readv", 3},
    [__NR_writev] = {"writev", 3},
#ifdef __NR_access
    [__NR_access] = {"access", 2},
#endif
#ifdef __NR_pipe
    [__NR_pipe] = {"pipe", 1},
#endif
#ifdef __NR_select
    [__NR_select] = {"select", 5},
#endif
    [__NR_sched_yield] = {"sched_yield", 0},
    [__NR_mremap] = {"mremap", 5},
    [__NR_msync] = {"msync", 3},
    [__NR_mincore] = {"mincore", 3},
    [__NR_madvise] = {"madvise", 3},
    [__NR_shmget] = {"shmget", 3},
    [__NR_shmat] = {"shmat", 3},
    [__NR_shmctl] = {"shmctl", 3},
    [__NR_dup] = {"dup", 1},
#ifdef __NR_dup2
    [__NR_dup2] = {"dup2", 2},
#endif
#ifdef __NR_pause
    [__NR_pause] = {"pause", 0},
#endif
    [__NR_nanosleep] = {"nanosleep", 2},
    [__NR_getitimer] = {"getitimer", 2},
#ifdef __NR_alarm
    [__NR_alarm] = {"alarm", 1},
#endif
    [__NR_setitimer] = {"setitimer", 3},
    [__NR_getpid] = {"getpid", 0},
    [__NR_sendfile] = {"sendfile", 4},
    [__NR_socket] = {"socket", 3},
    [__NR_connect] = {"connect", 3},
    [__NR_accept] = {"accept", 3},
    [__NR_sendto] = {"sendto", 6},
    [__NR_recvfrom] = {"recvfrom", 6},
    [__NR_sendmsg] = {"sendmsg", 3},
    [__NR_recvmsg] = {"recvmsg", 3},
    [__NR_shutdown] = {"shutdown", 2},
    [__NR_bind] = {"bind", 3},
    [__NR_listen] = {"listen", 2},
    [__NR_getsockname] = {"getsockname", 3},
    [__NR_getpeername] = {"getpeername", 3},
    [__NR_socketpair] = {"socketpair", 4},
    [__NR_setsockopt] = {"setsockopt", 5},
    [__NR_getsockopt] = {"getsockopt", 5},
    [__NR_clone] = {"clone", 5},
#ifdef __NR_fork
    [__NR_fork] = {"fork", 0},
#endif
#ifdef __NR_vfork
    [__NR_vfork] = {"vfork", 0},
#endif
    [__NR_execve] = {"execve", 3},
    [__NR_exit] = {"exit", 1},
    [__NR_wait4] = {"wait4", 4},
    [__NR_kill] = {"kill", 2},
    [__NR_uname] = {"uname", 1},
    [__NR_semget] = {"semget", 3},
    [__NR_semop] = {"semop", 3},
    [__NR_semctl] = {"semctl", 4},
    [__NR_shmdt] = {"shmdt", 1},
    [__NR_msgget] = {"msgget", 2},
    [__NR_msgsnd] = {"msgsnd", 4},
    [__NR_msgrcv] = {"msgrcv", 5},
    [__NR_msgctl] = {"msgctl", 3},
    [__NR_fcntl] = {"fcntl", 3},
    [__NR_flock] = {"flock", 2},
    [__NR_fsync] = {"fsync", 1},
    [__NR_fdatasync] = {"fdatasync", 1},
    [__NR_truncate] = {"truncate", 2},
    [__NR_ftruncate] = {"ftruncate", 2},
#ifdef __NR_getdents
    [__NR_getdents] = {"getdents", 3},
#endif
    [__NR_getcwd] = {"getcwd", 2},
    [__NR_chdir] = {"chdir", 1},
    [__NR_fchdir] = {"fchdir", 1},
#ifdef __NR_rename
    [__NR_rename] = {"rename", 2},
#endif
#ifdef __NR_mkdir
    [__NR_mkdir] = {"mkdir", 2},
#endif
#ifdef __NR_rmdir
    [__NR_rmdir] = {"rmdir", 1},
#endif
#ifdef __NR_creat
    [__NR_creat] = {"creat", 2},
#endif
#ifdef __NR_link
    [__NR_link] = {"link", 2},
#endif
#ifdef __NR_unlink
    [__NR_unlink] = {"unlink", 1},
#endif
#ifdef __NR_symlink
    [__NR_symlink] = {"symlink", 2},
#endif
#ifdef __NR_readlink
    [__NR_readlink] = {"readlink", 3},
#endif
#ifdef __NR_chmod
    [__NR_chmod] = {"chmod", 2},
#endif
    [__NR_fchmod] = {"fchmod", 2},
#ifdef __NR_chown
    [__NR_chown] = {"chown", 3},
#endif
    [__NR_fchown] = {"fchown", 3},
#ifdef __NR_lchown
    [__NR_lchown] = {"lchown", 3},
#endif
    [__NR_umask] = {"umask", 1},
    [__NR_gettimeofday] = {"gettimeofday", 2},
    [__NR_getrlimit] = {"getrlimit", 2},
    [__NR_getrusage] = {"getrusage", 2},
    [__NR_sysinfo] = {"sysinfo", 1},
    [__NR_times] = {"times", 1},
    [__NR_ptrace] = {"ptrace", 4},
    [__NR_getuid] = {"getuid", 0},
    [__NR_syslog] = {"syslog", 3},
    [__NR_getgid] = {"getgid", 0},
    [__NR_setuid] = {"setuid", 1},
    [__NR_setgid] = {"setgid", 1},
    [__NR_geteuid] = {"geteuid", 0},
    [__NR_getegid] = {"getegid", 0},
    [__NR_setpgid] = {"setpgid", 2},
    [__NR_getppid] = {"getppid", 0},
#ifdef __NR_getpgrp
    [__NR_getpgrp] = {"getpgrp", 0},
#endif
    [__NR_setsid] = {"setsid", 0},
    [__NR_setreuid] = {"setreuid", 2},
    [__NR_setregid] = {"setregid", 2},
    [__NR_getgroups] = {"getgroups", 2},
    [__NR_setgroups] = {"setgroups", 2},
    [__NR_setresuid] = {"setresuid", 3},
    [__NR_getresuid] = {"getresuid", 3},
    [__NR_setresgid] = {"setresgid", 3},
    [__NR_getresgid] = {"getresgid", 3},
    [__NR_getpgid] = {"getpgid", 1},
    [__NR_setfsuid] = {"setfsuid", 1},
    [__NR_setfsgid] = {"setfsgid", 1},
    [__NR_getsid] = {"getsid", 1},
    [__NR_capget] = {"capget", 2},
    [__NR_capset] = {"capset", 2},
    [__NR_rt_sigpending] = {"rt_sigpending", 2},
    [__NR_rt_sigtimedwait] = {"rt_sigtimedwait", 4},
    [__NR_rt_sigqueueinfo] = {"rt_sigqueueinfo", 3},
    [__NR_rt_sigsuspend] = {"rt_sigsuspend", 2},
    [__NR_sigaltstack] = {"sigaltstack", 2},
#ifdef __NR_utime
    [__NR_utime] = {"utime", 2},
#endif
#ifdef __NR_mknod
    [__NR_mknod] = {"mknod", 3},
#endif
#ifdef __NR_uselib
    [__NR_uselib] = {"uselib", 0},
#endif
    [__NR_personality] = {"personality", 1},
#ifdef __NR_ustat
    [__NR_ustat] = {"ustat", 2},
#endif
    [__NR_statfs] = {"statfs", 2},
    [__NR_fstatfs] = {"fstatfs", 2},
#ifdef __NR_sysfs
    [__NR_sysfs] = {"sysfs", 3},
#endif
    [__NR_getpriority] = {"getpriority", 2},
    [__NR_setpriority] = {"setpriority", 3},
    [__NR_sched_setparam] = {"sched_setparam", 2},
    [__NR_sched_getparam] = {"sched_getparam", 2},
    [__NR_sched_setscheduler] = {"sched_setscheduler", 3},
    [__NR_sched_getscheduler] = {"sched_getscheduler", 1},
    [__NR_sched_get_priority_max] = {"sched_get_priority_max", 1},
    [__NR_sched_get_priority_min] = {"sched_get_priority_min", 1},
    [__NR_sched_rr_get_interval] = {"sched_rr_get_interval", 2},
    [__NR_mlock] = {"mlock", 2},
    [__NR_munlock] = {"munlock", 2},
    [__NR_mlockall] = {"mlockall", 1},
    [__NR_munlockall] = {"munlockall", 0},
    [__NR_vhangup] = {"vhangup", 0},
#ifdef __NR_modify_ldt
    [__NR_modify_ldt] = {"modify_ldt", 3},
#endif
    [__NR_pivot_root] = {"pivot_root", 2},
#ifdef __NR__sysctl
    [__NR__sysctl] = {"_sysctl", 1},
#endif
    [__NR_prctl] = {"prctl", 5},
#ifdef __NR_arch_prctl
    [__NR_arch_prctl] = {"arch_prctl", 3},
#endif
    [__NR_adjtimex] = {"adjtimex", 1},
    [__NR_setrlimit] = {"setrlimit", 2},
    [__NR_chroot] = {"chroot", 1},
    [__NR_sync] = {"sync", 0},
    [__NR_acct] = {"acct", 1},
    [__NR_settimeofday] = {"settimeofday", 2},
    [__NR_mount] = {"mount", 5},
    [__NR_umount2] = {"umount2", 2},
    [__NR_swapon] = {"swapon", 2},
    [__NR_swapoff] = {"swapoff", 1},
    [__NR_reboot] = {"reboot", 4},
    [__NR_sethostname] = {"sethostname", 2},
    [__NR_setdomainname] = {"setdomainname", 2},
#ifdef __NR_iopl
    [__NR_iopl] = {"iopl", 2},
#endif
#ifdef __NR_ioperm
    [__NR_ioperm] = {"ioperm", 3},
#endif
#ifdef __NR_create_module
    [__NR_create_module] = {"create_module", 0},
#endif
    [__NR_init_module] = {"init_module", 3},
    [__NR_delete_module] = {"delete_module", 2},
#ifdef __NR_get_kernel_syms
    [__NR_get_kernel_syms] = {"get_kernel_syms", 0},
#endif
#ifdef __NR_query_module
    [__NR_query_module] = {"query_module", 0},
#endif
    [__NR_quotactl] = {"quotactl", 4},
    [__NR_nfsservctl] = {"nfsservctl", 0},
#ifdef __NR_getpmsg
    [__NR_getpmsg] = {"getpmsg", 0},
#endif
#ifdef __NR_putpmsg
    [__NR_putpmsg] = {"putpmsg", 0},
#endif
#ifdef __NR_afs_syscall
    [__NR_afs_syscall] = {"afs_syscall", 0},
#endif
#ifdef __NR_tuxcall
    [__NR_tuxcall] = {"tuxcall", 0},
#endif
#ifdef __NR_security
    [__NR_security] = {"security", 0},
#endif
    [__NR_gettid] = {"gettid", 0},
    [__NR_readahead] = {"readahead", 3},
    [__NR_setxattr] = {"setxattr", 5},
    [__NR_lsetxattr] = {"lsetxattr", 5},
    [__NR_fsetxattr] = {"fsetxattr", 5},
    [__NR_getxattr] = {"getxattr", 4},
    [__NR_lgetxattr] = {"lgetxattr", 4},
    [__NR_fgetxattr] = {"fgetxattr", 4},
    [__NR_listxattr] = {"listxattr", 3},
    [__NR_llistxattr] = {"llistxattr", 3},
    [__NR_flistxattr] = {"flistxattr", 3},
    [__NR_removexattr] = {"removexattr", 2},
    [__NR_lremovexattr] = {"lremovexattr", 2},
    [__NR_fremovexattr] = {"fremovexattr", 2},
    [__NR_tkill] = {"tkill", 2},
#ifdef __NR_time
    [__NR_time] = {"time", 1},
#endif
    [__NR_futex] = {"futex", 6},
    [__NR_sched_setaffinity] = {"sched_setaffinity", 3},
    [__NR_sched_getaffinity] = {"sched_getaffinity", 3},
#ifdef __NR_set_thread_area
    [__NR_set_thread_area] = {"set_thread_area", 0},
#endif
    [__NR_io_setup] = {"io_setup", 2},
    [__NR_io_destroy] = {"io_destroy", 1},
    [__NR_io_getevents] = {"io_getevents", 4},
    [__NR_io_submit] = {"io_submit", 3},
    [__NR_io_cancel] = {"io_cancel", 3},
#ifdef __NR_get_thread_area
    [__NR_get_thread_area] = {"get_thread_area", 0},
#endif
    [__NR_lookup_dcookie] = {"lookup_dcookie", 3},
#ifdef __NR_epoll_create
    [__NR_epoll_create] = {"epoll_create", 1},
#endif
#ifdef __NR_epoll_ctl_old
    [__NR_epoll_ctl_old] = {"epoll_ctl_old", 0},
#endif
#ifdef __NR_epoll_wait_old
    [__NR_epoll_wait_old] = {"epoll_wait_old", 0},
#endif
    [__NR_remap_file_pages] = {"remap_file_pages", 5},
    [__NR_getdents64] = {"getdents64", 3},
    [__NR_set_tid_address] = {"set_tid_address", 1},
    [__NR_restart_syscall] = {"restart_syscall", 0},
    [__NR_semtimedop] = {"semtimedop", 4},
    [__NR_fadvise64] = {"fadvise64", 4},
    [__NR_timer_create] = {"timer_create", 3},
    [__NR_timer_settime] = {"timer_settime", 4},
    [__NR_timer_gettime] = {"timer_gettime", 2},
    [__NR_timer_getoverrun] = {"timer_getoverrun", 1},
    [__NR_timer_delete] = {"timer_delete", 1},
    [__NR_clock_settime] = {"clock_settime", 2},
    [__NR_clock_gettime] = {"clock_gettime", 2},
    [__NR_clock_getres] = {"clock_getres", 2},
    [__NR_clock_nanosleep] = {"clock_nanosleep", 4},
    [__NR_exit_group] = {"exit_group", 1},
#ifdef __NR_epoll_wait
    [__NR_epoll_wait] = {"epoll_wait", 4},
#endif
    [__NR_epoll_ctl] = {"epoll_ctl", 4},
    [__NR_tgkill] = {"tgkill", 3},
#ifdef __NR_utimes
    [__NR_utimes] = {"utimes", 2},
#endif
#ifdef __NR_vserver
    [__NR_vserver] = {"vserver", 0},
#endif
    [__NR_mbind] = {"mbind", 6},
    [__NR_set_mempolicy] = {"set_mempolicy", 3},
    [__NR_get_mempolicy] = {"get_mempolicy", 5},
    [__NR_mq_open] = {"mq_open", 4},
    [__NR_mq_unlink] = {"mq_unlink", 1},
    [__NR_mq_timedsend] = {"mq_timedsend", 5},
    [__NR_mq_timedreceive] = {"mq_timedreceive", 5},
    [__NR_mq_notify] = {"mq_notify", 2},
    [__NR_mq_getsetattr] = {"mq_getsetattr", 3},
    [__NR_kexec_load] = {"kexec_load", 4},
    [__NR_waitid] = {"waitid", 5},
    [__NR_add_key] = {"add_key", 4},
    [__NR_request_key] = {"request_key", 4},
    [__NR_keyctl] = {"keyctl", 5},
    [__NR_ioprio_set] = {"ioprio_set", 3},
    [__NR_ioprio_get] = {"ioprio_get", 2},
#ifdef __NR_inotify_init
    [__NR_inotify_init] = {"inotify_init", 0},
#endif
    [__NR_inotify_add_watch] = {"inotify_add_watch", 3},
    [__NR_inotify_rm_watch] = {"inotify_rm_watch", 2},
    [__NR_migrate_pages] = {"migrate_pages", 4},
    [__NR_openat] = {"openat", 4},
    [__NR_mkdirat] = {"mkdirat", 3},
    [__NR_mknodat] = {"mknodat", 4},
    [__NR_fchownat] = {"fchownat", 5},
#ifdef __NR_futimesat
    [__NR_futimesat] = {"futimesat", 3},
#endif
    [__NR_newfstatat] = {"newfstatat", 4},
    [__NR_unlinkat] = {"unlinkat", 3},
    [__NR_renameat] = {"renameat", 4},
    [__NR_linkat] = {"linkat", 5},
    [__NR_symlinkat] = {"symlinkat", 3},
    [__NR_readlinkat] = {"readlinkat", 4},
    [__NR_fchmodat] = {"fchmodat", 3},
    [__NR_faccessat] = {"faccessat", 3},
    [__NR_pselect6] = {"pselect6", 6},
    [__NR_ppoll] = {"ppoll", 5},
    [__NR_unshare] = {"unshare", 1},
    [__NR_set_robust_list] = {"set_robust_list", 2},
    [__NR_get_robust_list] = {"get_robust_list", 3},
    [__NR_splice] = {"splice", 6},
    [__NR_tee] = {"tee", 4},
    [__NR_sync_file_range] = {"sync_file_range", 4},
    [__NR_vmsplice] = {"vmsplice", 4},
    [__NR_move_pages] = {"move_pages", 6},
    [__NR_utimensat] = {"utimensat", 4},
    [__NR_epoll_pwait] = {"epoll_pwait", 6},
#ifdef __NR_signalfd
    [__NR_signalfd] = {"signalfd", 3},
#endif
    [__NR_timerfd_create] = {"timerfd_create", 2},
#ifdef __NR_eventfd
    [__NR_eventfd] = {"eventfd", 1},
#endif
    [__NR_fallocate] = {"fallocate", 4},
    [__NR_timerfd_settime] = {"timerfd_settime", 4},
    [__NR_timerfd_gettime] = {"timerfd_gettime", 2},
    [__NR_accept4] = {"accept4", 4},
    [__NR_signalfd4] = {"signalfd4", 4},
    [__NR_eventfd2] = {"eventfd2", 2},
    [__NR_epoll_create1] = {"epoll_create1", 1},
    [__NR_dup3] = {"dup3", 3},
    [__NR_pipe2] = {"pipe2", 2},
    [__NR_inotify_init1] = {"inotify_init1", 1},
    [__NR_preadv] = {"preadv", 5},
    [__NR_pwritev] = {"pwritev", 5},
    [__NR_rt_tgsigqueueinfo] = {"rt_tgsigqueueinfo", 4},
    [__NR_perf_event_open] = {"perf_event_open", 5},
    [__NR_recvmmsg] = {"recvmmsg", 5},
    [__NR_fanotify_init] = {"fanotify_init", 2},
    [__NR_fanotify_mark] = {"fanotify_mark", 5},
    [__NR_prlimit64] = {"prlimit64", 4},
    [__NR_name_to_handle_at] = {"name_to_handle_at", 5},
    [__NR_open_by_handle_at] = {"open_by_handle_at", 5},
    [__NR_clock_adjtime] = {"clock_adjtime", 2},
    [__NR_syncfs] = {"syncfs", 1},
    [__NR_sendmmsg] = {"sendmmsg", 4},
    [__NR_setns] = {"setns", 2},
    [__NR_getcpu] = {"getcpu", 3},
    [__NR_process_vm_readv] = {"process_vm_readv", 6},
    [__NR_process_vm_writev] = {"process_vm_writev", 6},
    [__NR_kcmp] = {"kcmp", 5},
    [__NR_finit_module] = {"finit_module", 3},
    [__NR_sched_setattr] = {"sched_setattr", 3},
    [__NR_sched_getattr] = {"sched_getattr", 4},
    [__NR_renameat2] = {"renameat2", 5},
    [__NR_seccomp] = {"seccomp", 3},
    [__NR_getrandom] = {"getrandom", 3},
    [__NR_memfd_create] = {"memfd_create", 2},
    [__NR_kexec_file_load] = {"kexec_file_load", 5},
    [__NR_bpf] = {"bpf", 3},
    [__NR_execveat] = {"execveat", 5},
    [__NR_userfaultfd] = {"userfaultfd", 1},
    [__NR_membarrier] = {"membarrier", 2},
    [__NR_mlock2] = {"mlock2", 3},
    [__NR_copy_file_range] = {"copy_file_range", 6},
    [__NR_preadv2] = {"preadv2", 6},
    [__NR_pwritev2] = {"pwritev2", 6},
    [__NR_pkey_mprotect] = {"pkey_mprotect", 0},
    [__NR_pkey_alloc] = {"pkey_alloc", 0},
    [__NR_pkey_free] = {"pkey_free", 0},
    [__NR_statx] = {"statx", 0},
    [__NR_io_pgetevents] = {"io_pgetevents", 0},
    [__NR_rseq] = {"rseq", 0},
};

#endif
