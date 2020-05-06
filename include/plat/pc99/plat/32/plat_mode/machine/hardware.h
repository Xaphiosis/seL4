/*
 * Copyright 2014, General Dynamics C4 Systems
 *
 * SPDX-License-Identifier: GPL-2.0-only
 */

#pragma once

#include <config.h>
#include <util.h>
#include <arch/kernel/tlb_bitmap_defs.h>

/*
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
 *      |    TLB Bitmaps    |
 *      +-------------------+
 *      | Kernel Page Table | -+
 * 2^32 +-------------------+  |
 *                             |
 *                        +----+
 *                        |
 *                        v
 *  2^32 - 2^22 +-------------------+ KDEV_BASE
 *              |  Kernel Devices   |
 *         2^32 +-------------------+
 *
 */

/* WARNING: some of these constants are also defined in linker.lds */

/* The mask here, 0xFFC00000 represents the mask for a 4MiB page. */
#define USER_TOP (PPTR_BASE & UL_CONST(0xFFC00000))

/* The first physical address to map into the kernel's physical memory
 * window */
#define PADDR_BASE UL_CONST(0x00000000)

/* The base address in virtual memory to use for the 1:1 physical memory
 * mapping */
#define PPTR_BASE UL_CONST(0xe0000000)

/* This is a page table mapping at the end of the virtual address space
 * to map objects with 4KiB pages rather than 4MiB large pages. */
#define KERNEL_PT_BASE UL_CONST(0xffc00000)

/* Calculate virtual address space reserved for TLB Bitmap. ROOT_ENTRIES
 * will be zero in the case where the bitmap is unused */
#define TLBBITMAP_PD_RESERVED (TLBBITMAP_ROOT_ENTRIES * BIT(seL4_LargePageBits))

/* Calculate virtual address space reserved for dynamic log buffer mapping */
#ifdef CONFIG_BENCHMARK_USE_KERNEL_LOG_BUFFER
#define LOGBUFFER_PD_RESERVED BIT(seL4_LargePageBits)
#else
#define LOGBUFFER_PD_RESERVED UL_CONST(0)
#endif

/* TLB bitmap is in the PD slots before the kernel page table */
#define TLBBITMAP_PPTR (KERNEL_PT_BASE - TLBBITMAP_PD_RESERVED)
/* Before TLB bitmap is the log buffer */
#define KS_LOG_PPTR (TLBBITMAP_PPTR - LOGBUFFER_PD_RESERVED)
/* The log buffer marks the end of the kernel physical memory window */
#define PPTR_TOP KS_LOG_PPTR

/* The physical memory address to use for mapping the kernel ELF */
#define KERNEL_ELF_PADDR_BASE UL_CONST(0x00100000)

/* The base address in virtual memory to use for the kernel ELF mapping */
#define KERNEL_ELF_BASE (PPTR_BASE + KERNEL_ELF_PADDR_BASE)

/* The base address in virtual memory to use for the kernel device
 * mapping region. These are mapped in the kernel page table. */
#define KDEV_BASE KERNEL_PT_BASE

/* For a 32-bit system there is no difference in how we translates
 * physical address for the kernel symbols or anything else */
#define paddr_to_kpptr(x) paddr_to_pptr(x)
#define kpptr_to_paddr(x) pptr_to_paddr(x)
