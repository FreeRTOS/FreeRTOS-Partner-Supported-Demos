/******************************************************************************
* Copyright (c) 2014 - 2020 Xilinx, Inc.  All rights reserved.
* Copyright 2025 Arm Limited and/or its affiliates <open-source-office@arm.com>
* SPDX-License-Identifier: MIT
******************************************************************************/
#if defined(__ARMCC_VERSION)
    /* Externs needed by the MPU setup code. These are defined in Scatter-Loading
    * description file (armclang.sct). */
    .set __el1_stack,		 Image$$ARM_LIB_STACK$$Base
    .set _el1_stack_end,     Image$$ARM_LIB_HEAP$$Base
#endif

#include "FreeRTOSConfig.h"

.global _prestart
.global _boot

.global __el1_stack
.global _vector_table

.set EL1_stack,		  __el1_stack

.set EL1_stack_end,	  _el1_stack_end

.set vector_base,	  _vector_table

/*
 * N_CPUS_SHIFT must equal log2(configNUMBER_OF_CORES).  It represents the
 * number of bits required to index the core that owns a particular slice
 * of the shared EL1 stack pool.
 *
 * To avoid overlapping stack regions, the code assumes
 * configNUMBER_OF_CORES is a power‑of‑two.  The static check below forces
 * a build‑time error if that assumption is broken.
 */
#if ( (configNUMBER_OF_CORES & (configNUMBER_OF_CORES - 1)) != 0 )
    #error "configNUMBER_OF_CORES must be a power‑of‑two"
#endif

/* Compute log2(configNUMBER_OF_CORES). */
#if   (configNUMBER_OF_CORES == 1)
    .set N_CPUS_SHIFT, 0
#elif (configNUMBER_OF_CORES == 2)
    .set N_CPUS_SHIFT, 1
#elif (configNUMBER_OF_CORES == 4)
    .set N_CPUS_SHIFT, 2
#else
    #error "Unsupported configNUMBER_OF_CORES value — must be a power‑of‑two up to 4"
#endif

.set SCTLR_EL1_NTWE,  18     /* SCTLR_EL1.nTWE bit position */
.set SCTLR_EL1_NTWI,  16     /* SCTLR_EL1.nTWI bit position */
.set SCTLR_EL1_CACHE, 2      /* SCTLR_EL1.C bit position */

.section .boot,"ax"

_prestart:
_boot:
start:
/* Clear all GP registers (x0–x30) for a known initial state */
	mov      x0, #0
	mov      x1, #0
	mov      x2, #0
	mov      x3, #0
	mov      x4, #0
	mov      x5, #0
	mov      x6, #0
	mov      x7, #0
	mov      x8, #0
	mov      x9, #0
	mov      x10, #0
	mov      x11, #0
	mov      x12, #0
	mov      x13, #0
	mov      x14, #0
	mov      x15, #0
	mov      x16, #0
	mov      x17, #0
	mov      x18, #0
	mov      x19, #0
	mov      x20, #0
	mov      x21, #0
	mov      x22, #0
	mov      x23, #0
	mov      x24, #0
	mov      x25, #0
	mov      x26, #0
	mov      x27, #0
	mov      x28, #0
	mov      x29, #0
	mov      x30, #0

	mrs	    x0, currentEL
	/* Check we’ve come from EL1 (currentEL==0x4), otherwise fault */
	cmp	    x0, #0x4
	beq	    InitEL1

	b 	    error
InitEL1:
	/* Set vector table base address */
	ldr	    x1, =vector_base
	msr	    VBAR_EL1,x1

	mrs	    x0, CPACR_EL1
	/* Allow FP/SIMD at both EL1 and EL0: CPACR_EL1.FPEN[21:20] = 0b11 */
	orr	    x0, x0, #(0x3 << 20)
	msr	    CPACR_EL1, x0   /* Enable FP/SIMD access at EL1 and EL0 */
	isb

	/* Clear FP status flags (FPSR) to avoid spurious exceptions on first use */
	msr  FPSR, xzr

	/* Define stack pointer for current exception level */
#if configNUMBER_OF_CORES > 1
    /* Divide the EL1 stack region equally among all cores, then set SP based on MPIDR_EL1[7:0] */
    /* x0 = log2(N_CPUS) is assumed to be a build-time constant */
    mov     x0,  N_CPUS_SHIFT       /* log2(#cores) */
    /* load overall stack limits */
    ldr     x2,  =EL1_stack         /* low  address of the shared stack pool */
    ldr     x3,  =EL1_stack_end     /* high address (one past the pool) */
    /* x1 = total size of the pool, x1 >> N_CPUS_SHIFT = size per core */
    sub     x1,  x3,  x2            /* total_stack_size */
    lsr     x1,  x1,  x0            /* slice_size = total/#cores */
    /* x4 = this CPU’s index (Aff0 field of MPIDR_EL1) */
    mrs     x4,  MPIDR_EL1
    and     x4,  x4,  #0xFF         /* core_id ∈ {0 … N_CPUS-1} */
    cmp     x4, #configNUMBER_OF_CORES
    b.hs    error
    /* x0 = slice_size * core_id  →  how far to step back from the top  */
    mul     x0,  x1,  x4
    /* sp = top_of_pool – offset   (so core 0 gets the very top) */
    sub     x3,  x3,  x0            /* x3 = initial SP for this core */
    bic     x3,  x3,  #0xF          /* keep the mandated 16-byte alignment */
    mov     sp,  x3
#else
    ldr     x2, =EL1_stack_end
    mov     sp, x2
#endif

	/* Enable ICC system-register interface (SRE=1) and disable FIQ/IRQ bypass (DFB/DIB) */
	mov     x0, #0x7
    msr     ICC_SRE_EL1, x0

	/* Invalidate I and D caches */
	ic      IALLU
	bl 	    invalidate_dcaches
	dsb	    sy
	isb

    /* Unmask SError interrupts (clear DAIF.A bit) */
    mrs 	x1,DAIF
    bic	    x1,x1,#(0x1<<8)
    msr	    DAIF,x1

	/* Configure SCTLR_EL1:
	 * - Enable data cache (C=1)
	 * - Allow EL0 to execute WFE/WFI (Set nTWE/nTWI so they don't trap)
	 */
	mrs     x1, SCTLR_EL1
	orr     x1, x1, #(1 << SCTLR_EL1_NTWE)    /* nTWE = 1 → WFE at EL0 does not trap */
	orr     x1, x1, #(1 << SCTLR_EL1_NTWI)    /* nTWI = 1 → WFI at EL0 does not trap */
	orr     x1, x1, #(1 << SCTLR_EL1_CACHE)   /* C = 1   → enable data cache */
	msr     SCTLR_EL1, x1
	isb

	/* Branch to C-level startup (zero BSS, init data, etc.) */
	bl 	    _startup

/* If we ever get here, something went wrong—hang forever */
error:
    b	    error

invalidate_dcaches:

    dmb     ISH
    mrs     x0, CLIDR_EL1          /* x0 = CLIDR */
    ubfx    w2, w0, #24, #3        /* w2 = CLIDR.LoC */
    cmp     w2, #0                 /* LoC is 0? */
    b.eq    invalidateCaches_end   /* No cleaning required */
    mov     w1, #0                 /* w1 = level iterator */

invalidateCaches_flush_level:
    add     w3, w1, w1, lsl #1     /* w3 = w1 * 3 (right-shift for cache type) */
    lsr     w3, w0, w3             /* w3 = w0 >> w3 */
    ubfx    w3, w3, #0, #3         /* w3 = cache type of this level */
    cmp     w3, #2                 /* No cache at this level? */
	b.lt    invalidateCaches_next_level

	lsl     w4, w1, #1
    msr     CSSELR_EL1, x4         /* Select current cache level in CSSELR */
    isb                            /* ISB required to reflect new CSIDR */
    mrs     x4, CCSIDR_EL1         /* w4 = CSIDR */

	ubfx    w3, w4, #0, #3
	add    	w3, w3, #2             /* w3 = log2(line size) */
	ubfx    w5, w4, #13, #15
    ubfx    w4, w4, #3, #10        /* w4 = Way number */
    clz     w6, w4                 /* w6 = 32 - log2(number of ways) */

invalidateCaches_flush_set:
	mov     w8, w4                 /* w8 = Way number */
invalidateCaches_flush_way:
	lsl     w7, w1, #1             /* Fill level field */
	lsl     w9, w5, w3
	orr     w7, w7, w9             /* Fill level field */
	lsl     w9, w8, w6
	orr     w7, w7, w9             /* Fill way field */
	dc      CISW, x7               /* Invalidate by set/way to point of coherency */
	subs    w8, w8, #1             /* Decrement way */
	b.ge    invalidateCaches_flush_way
	subs    w5, w5, #1             /* Descrement set */
	b.ge    invalidateCaches_flush_set

invalidateCaches_next_level:
	add     w1, w1, #1             /* Next level */
	cmp     w2, w1
	b.gt    invalidateCaches_flush_level

invalidateCaches_end:
	ret

.end
