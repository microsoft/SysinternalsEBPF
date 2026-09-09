/*
    SysinternalsEBPF

    Copyright (c) Microsoft Corporation

    All rights reserved.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

//====================================================================
//
// sysinternalsEBPF_common.h
//
// Defines and maps needed by sysinternalsEBPF_helpers.c
//
//====================================================================

#ifndef SYSINTERNALS_EBPF_COMMON_H
#define SYSINTERNALS_EBPF_COMMON_H

#ifndef EBPF_CO_RE
#include <linux/version.h>
#include <linux/bpf.h>
#include <linux/string.h>
#include <linux/limits.h>
#include <asm/ptrace.h>
#else
#define PATH_MAX        4096        // Missing def
#endif

#ifndef EBPF_CO_RE
#include <stdint.h>
#endif
#include <bpf_helpers.h>
#include <asm/unistd.h>
#include <sysinternalsEBPFshared.h>

// debug tracing can be found using:
// #cat /sys/kernel/debug/tracing/trace_pipe

#ifdef DEBUG_K
#define BPF_PRINTK( format, ... ) \
    char fmt[] = format; \
    bpf_trace_printk(fmt, sizeof(fmt), ##__VA_ARGS__ );
#else
#define BPF_PRINTK ((void)0);
#endif

// missing stddef.h defines
#define NULL ((void *)0)
#define true 1
#define false 0

// Architecture-dependent syscall pt_regs access macros.
//
// On arm64 the UAPI header (asm/ptrace.h) only exposes struct user_pt_regs
// (regs[31], sp, pc, pstate) — there is no UAPI struct pt_regs.  The kernel-
// internal struct pt_regs starts with user_pt_regs followed by orig_x0
// (the preserved first syscall argument, because the kernel overwrites
// regs[0] with the return value).
//
// CO-RE path   – define a pt_regs___arm64 "flavor" struct with
//                __attribute__((preserve_access_index)) so CO-RE relocations
//                resolve orig_x0 against the target kernel's real pt_regs.
// Non-CO-RE    – define struct pt_regs ourselves (arm64 UAPI omits it) so
//                the macros compile.  Layout must match the kernel.

#if defined(__TARGET_ARCH_arm64)

#ifdef EBPF_CO_RE
// CO-RE flavor type matching the kernel's struct pt_regs on arm64.
// The triple-underscore suffix is the libbpf CO-RE naming convention.
struct pt_regs___arm64 {
    unsigned long regs[31];
    unsigned long sp;
    unsigned long pc;
    unsigned long pstate;
    unsigned long orig_x0;
} __attribute__((preserve_access_index));

#define SYSCALL_PT_REGS_PARM1(x) (((const struct pt_regs___arm64 *)(x))->orig_x0)
#define SYSCALL_PT_REGS_PARM2(x) (((const struct pt_regs___arm64 *)(x))->regs[1])
#define SYSCALL_PT_REGS_PARM3(x) (((const struct pt_regs___arm64 *)(x))->regs[2])
#define SYSCALL_PT_REGS_PARM4(x) (((const struct pt_regs___arm64 *)(x))->regs[3])
#define SYSCALL_PT_REGS_PARM5(x) (((const struct pt_regs___arm64 *)(x))->regs[4])
#define SYSCALL_PT_REGS_PARM6(x) (((const struct pt_regs___arm64 *)(x))->regs[5])
#define SYSCALL_PT_REGS_RC(x)    (((const struct pt_regs___arm64 *)(x))->regs[0])

#else /* non-CO-RE arm64 */

// arm64 UAPI only exposes struct user_pt_regs.  Define the kernel-internal
// struct pt_regs layout so the macros below can access orig_x0.
struct pt_regs {
    __u64 regs[31];
    __u64 sp;
    __u64 pc;
    __u64 pstate;
    __u64 orig_x0;
};

#define SYSCALL_PT_REGS_PARM1(x) ((x)->orig_x0)
#define SYSCALL_PT_REGS_PARM2(x) ((x)->regs[1])
#define SYSCALL_PT_REGS_PARM3(x) ((x)->regs[2])
#define SYSCALL_PT_REGS_PARM4(x) ((x)->regs[3])
#define SYSCALL_PT_REGS_PARM5(x) ((x)->regs[4])
#define SYSCALL_PT_REGS_PARM6(x) ((x)->regs[5])
#define SYSCALL_PT_REGS_RC(x)    ((x)->regs[0])

#endif /* EBPF_CO_RE */

#else /* x86_64 */

#ifdef EBPF_CO_RE
#define SYSCALL_PT_REGS_PARM1(x) ((x)->di)
#define SYSCALL_PT_REGS_PARM2(x) ((x)->si)
#define SYSCALL_PT_REGS_PARM3(x) ((x)->dx)
#define SYSCALL_PT_REGS_RC(x)    ((x)->ax)
#else
#define SYSCALL_PT_REGS_PARM1(x) ((x)->rdi)
#define SYSCALL_PT_REGS_PARM2(x) ((x)->rsi)
#define SYSCALL_PT_REGS_PARM3(x) ((x)->rdx)
#define SYSCALL_PT_REGS_RC(x)    ((x)->rax)
#endif
#define SYSCALL_PT_REGS_PARM4(x) ((x)->r10)
#define SYSCALL_PT_REGS_PARM5(x) ((x)->r8)
#define SYSCALL_PT_REGS_PARM6(x) ((x)->r9)

#endif /* __TARGET_ARCH_arm64 */

#define CMDLINE_MAX_LEN 16384 // must be power of 2
#define MAX_FDS 65535

#define MAX_MEM_DUMP 32768

#define ARG_ARRAY_SIZE 8
#define ARG_MASK 7

// Event arguments structure
typedef struct {
    unsigned long      syscallId;
    unsigned long      a[8]; // Should only be 6 but this helps with verifier
    unsigned long      returnCode;
} argsStruct;



// bpf_raw_tracepoint_args definition from /usr/src/linux/include/uapi/linux/bpf.h
struct bpf_our_raw_tracepoint_args {
    __u64 args[0];
};

// generic sys_enter argument struct for traditional tracepoints. Note that
// some or all of the 'a' array can't be derefenced depending on how many
// arguments a syscall expects; attempts to do so will cause the verifier
// to reject it.
struct tracepoint__syscalls__sys_enter {
    __u64 pad;
    __u32 __syscall_nr;
    __u32 pad2;
    __u64 a[6];
};


// all sys_exit arguments are the same for traditional tracepoints.
struct tracepoint__syscalls__sys_exit {
    __u64 pad;
    __u32 __syscall_nr;
    __u32 pad2;
    long ret;
};



#define MAX_PROC 512
#define ARGS_HASH_SIZE 10240

#ifndef BPF_F_CURRENT_CPU
#define BPF_F_CURRENT_CPU 0xffffffffULL
#endif

// create a map to transport events to userland via perf ring buffer
struct {
    __uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
    __uint(key_size, sizeof(int));
    __uint(value_size, sizeof(uint32_t));
    __uint(max_entries, MAX_PROC);
} eventMap SEC(".maps");


// create a map to hold the configuration
// only one entry, which is the config struct
struct {
    __uint(type, BPF_MAP_TYPE_ARRAY);
    __type(key, uint32_t);
    __type(value, ebpfConfig);
    __uint(max_entries, 1);
} configMap SEC(".maps");

// create a map to hold a temporary filepath as we build it - too big for stack
// one entry per cpu
struct {
    __uint(type, BPF_MAP_TYPE_ARRAY);
    __uint(key_size, sizeof(uint32_t));
    __uint(value_size, PATH_MAX * 2);
    __uint(max_entries, MAX_PROC);
} temppathArray SEC(".maps");

// create a hash to hold event arguments between sys_enter and sys_exit
// shared by all cpus because sys_enter and sys_exit could be on different cpus
struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, ARGS_HASH_SIZE);
    __type(key, uint64_t);
    __type(value, argsStruct);
} argsHash SEC(".maps");

// create a map to hold perf_ring_buffer errors
struct {
    __uint(type, BPF_MAP_TYPE_ARRAY);
    __type(key, uint32_t);
    __type(value, perfError);
    __uint(max_entries, PERF_ERRORS_MAX + 2);
} perfErrorsMap SEC(".maps");


#endif
