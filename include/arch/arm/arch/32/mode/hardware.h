/*
 * Copyright 2020, Data61, CSIRO (ABN 41 687 119 230)
 *
 * SPDX-License-Identifier: GPL-2.0-only
 */

#pragma once

#include <config.h>
#include <util.h>
#ifndef __ASSEMBLER__
#include <arch/machine/hardware.h>
#include <sel4/plat/api/constants.h>
#endif

/*
 * 0xffe00000 asid id slot (arm/arch/kernel/vspace.h)
 * 0xfff00000 devices      (plat/machine/devices.h)
 * 0xffff0000 vectors      (arch/machine/hardware.h)
 * 0xffffc000 global page  (arch/machine/hardware.h)
 *
 *
 *  0x0 +-------------------+
 *      |                   |
 *      |       User        |
 *      |                   |
 *      +-------------------+ USER_TOP / PPTR_BASE / KERNEL_ELF_BASE
 *      |    Kernel ELF     |
 *      +-------------------+
 *      |                   |
 *      |  Physical Memory  |
 *      |       Window      |
 *      |                   |
 *      +-------------------+ PPTR_TOP
 *      |    Log Buffer     |
 *      +-------------------+
 *      | Kernel Page Table | -+
 * 2^32 +-------------------+  |
 *                             |
 *                        +----+
 *                        |
 *                        v
 *  2^32 - 2^20 +-------------------+ KDEV_BASE
 *              |  Kernel Devices   |
 *              +-------------------+
 *              |      Vectors      |
 *              +-------------------+
 *              |    Global Page    |
 *         2^32 +-------------------+
 */

/* last accessible virtual address in user space */
#define USER_TOP seL4_UserTop

/* The first physical address to map into the kernel's physical memory
 * window */
#define PADDR_BASE physBase

/* The base address in virtual memory to use for the 1:1 physical memory
 * mapping */
#define PPTR_BASE seL4_UserTop

/* Calculate virtual address space reserved for dynamic log buffer mapping */
#ifdef CONFIG_BENCHMARK_USE_KERNEL_LOG_BUFFER
#define PPTR_TOP UL_CONST(0xffe00000)
#define KS_LOG_PPTR PPTR_TOP
#else
#define PPTR_TOP UL_CONST(0xfff00000)
#endif

/* The physical memory address to use for mapping the kernel ELF */
#define KERNEL_ELF_PADDR_BASE PADDR_BASE

/* The base address in virtual memory to use for the kernel ELF mapping */
#define KERNEL_ELF_BASE (USER_TOP + (KERNEL_ELF_PADDR_BASE & MASK(22)))

/* This is a page table mapping at the end of the virtual address space
 * to map objects with 4KiB pages rather than 4MiB large pages. */
#define KERNEL_PT_BASE UL_CONST(0xfff00000)

/* The base address in virtual memory to use for the kernel device
 * mapping region. These are mapped in the kernel page table. */
#define KDEV_BASE KERNEL_PT_BASE

#ifndef __ASSEMBLER__
#include <plat/machine/hardware.h>
#endif

