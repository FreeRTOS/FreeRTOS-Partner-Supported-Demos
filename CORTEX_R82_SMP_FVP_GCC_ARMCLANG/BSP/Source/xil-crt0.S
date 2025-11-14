/******************************************************************************
* Copyright (C) 2014 - 2020 Xilinx, Inc.  All rights reserved.
* Copyright 2025 Arm Limited and/or its affiliates <open-source-office@arm.com>
* SPDX-License-Identifier: MIT
******************************************************************************/

#if defined(__ARMCC_VERSION)
    .set __bss_start__,		       Image$$ER_BSS$$Base
    .set __bss_end__,		       Image$$ARM_LIB_STACK$$Base
    .set __data_start__,		   Image$$ER_DATA$$Base
    .set __data_end__,		       Image$$ER_DATA$$Limit
	.set _sidata,                  Load$$ER_DATA$$Base
#endif

#include "FreeRTOSConfig.h"
	.file	"xil-crt0.S"
	.align	2

.extern _vector_table
.extern ucSecondaryCoresReadyFlags
.extern ucPrimaryCoreInitDoneFlag
.extern _freertos_vector_table

.set freertos_vector_base, _freertos_vector_table

	.text

/* Store the addresses of .bss start/end so they can be cleared if needed */
.Lbss_start:
	.quad	__bss_start__

.Lbss_end:
	.quad	__bss_end__

/*
 * _startup: early runtime init
 *   - For CORE0: copy .data, clear .bss
 *   - For all cores: hand off to C code
 */
	.globl	_startup
_startup:

/* Only core0 initializes RAM image; others skip to c_init_end */
#if configNUMBER_OF_CORES > 1
	/* Get CPU Id */
	mrs  x0, MPIDR_EL1
	and  x0, x0, #0xFF
	cmp  x0, #0
	bne  c_init_end
#endif

    /* Copy .data section from ROM to RAM */
    ldr     x0, =_sidata        /* Source: ROM load address */
    ldr     x1, =__data_start__ /* Destination: RAM */
    ldr     x2, =__data_end__   /* End of destination */

/* Copy initialised data in 8-byte words for performance/alignment */
1:  cmp     x1, x2
    b.ge    .Lenclbss           /* Done copying */
    ldr     x3, [x0], #8        /* Load 64-bit word from ROM */
    str     x3, [x1], #8        /* Store to RAM */
    b       1b

/* Zero `.bss` in 8-byte words; efficient bulk clear of uninitialised data */
.Lenclbss:
	/* clear bss */
	ldr	x1,.Lbss_start		/* calculate beginning of the BSS */
	ldr	x2,.Lbss_end		/* calculate end of the BSS */

.Lloop_bss:
	mov x0, #0
	cmp	x1,x2
	bge	libc_init    /* If no BSS, no clearing required */
	str	x0, [x1], #8
	b	.Lloop_bss

libc_init:
#if defined( __GNUC__ ) && !defined( __clang__ )
	/* Creates the _reent structure that holds errno,
	 * the three static FILE objects (stdin, stdout, stderr).
	 * Runs any functions placed in .preinit_array and calls _init().
	 */
	bl __libc_init_array
    /* Executes three semihosting SYS_OPEN SWIs to open the host's
	 * special file :tt three times, obtaining descriptors 0, 1 and 2
	 * and writes those into stdin->_file, stdout->_file, stderr->_file.
	 * It also sets the __SWR (write-enabled) flag in each stream.
	 */
	bl initialise_monitor_handles
#endif /* if defined( __GNUC__ ) && !defined( __clang__ ) */

c_init_end:
#if configNUMBER_OF_CORES > 1
	/* Get CPU Id */
	mrs     x0, MPIDR_EL1
	and     x0, x0, #0xFF
	cbnz    x0, secondary_cores_hold
#endif
	b	    jump_to_main

secondary_cores_hold:
    ldr     x0, =ucPrimaryCoreInitDoneFlag
    ldr     w1, [x0]               			/* Has the primary core set the flag? */
    cmp     w1, #1
	b.eq 	make_secondary_cores_ready	    /* One? → Secondary cores released */
    wfe                            			/* Sleep until any event */
    b       secondary_cores_hold   			/* Re-test the flag */

make_secondary_cores_ready:
	ldr  X1, = freertos_vector_base
	msr  VBAR_EL1, X1
	DSB  SY
	ISB  SY
	bl   vGIC_SetupSgi0
	ldr  x0, =ucSecondaryCoresReadyFlags
	mrs  x1, MPIDR_EL1
	and  x1, x1, #0xFF
	sub  x1, x1, #1             /* Core 1 is index 0 in the array */
	add	 x0, x0, x1
	mov  w1, #1
	strb w1, [x0]               /* Mark this core as ready */
	SVC 106                     /* Start the first task on the secondary core */

jump_to_main:
	ldr     X1, = freertos_vector_base
	msr     VBAR_EL1, X1				 /* Set VBAR_EL1 to Freertos vector table */
	DSB     SY
	ISB     SY
	mov     x0, #0
	mov     x1, #0
	bl      main

.Lexit:
	b .Lexit
