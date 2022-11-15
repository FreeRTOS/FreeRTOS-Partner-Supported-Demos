/*
 * FreeRTOS V202112.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://aws.amazon.com/freertos
 *
 */

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"

/* Interface include files. */
#include "RegTests.h"

/* Tasks that implement register tests. */
static void prvRegisterTest1Task( void *pvParameters );
static void prvRegisterTest2Task( void *pvParameters );
static void prvRegisterTest3Task( void *pvParameters );
static void prvRegisterTest4Task( void *pvParameters );

/* Flag that will be latched to pdTRUE should any unexpected behaviour be
detected in any of the tasks. */
static volatile BaseType_t xErrorDetected = pdFALSE;

/* Counters that are incremented on each cycle of a test.  This is used to
detect a stalled task - a test that is no longer running. */
static volatile uint32_t ulRegisterTest1Counter = 0;
static volatile uint32_t ulRegisterTest2Counter = 0;
static volatile uint32_t ulRegisterTest3Counter = 0;
static volatile uint32_t ulRegisterTest4Counter = 0;
/*-----------------------------------------------------------*/

static void prvRegisterTest1Task( void *pvParameters )
{
	( void ) pvParameters;

    portTASK_USES_FLOATING_POINT();

	for( ; ; )
	{
		/* 1. Fill the registers stored as part of task context with known values.
		* 2. Force a context switch.
		* 3. Verify that all the registers contain expected values.
		* 4. If all the register contain expected values, increment ulRegisterTest1Counter.
		*/

        __asm__ __volatile__ (
            /* Fill each general purpose register with a known value. */
            "mov		x0, #0xff    \n\t"
            "mov		x1, #0x01    \n\t"
            "mov		x2, #0x02    \n\t"
            "mov		x3, #0x03    \n\t"
            "mov		x4, #0x04    \n\t"
            "mov		x5, #0x05    \n\t"
            "mov		x6, #0x06    \n\t"
            "mov		x7, #0x07    \n\t"
            "mov		x8, #0x08    \n\t"
            "mov		x9, #0x09    \n\t"
            "mov		x10, #0x10   \n\t"
            "mov		x11, #0x11   \n\t"
            "mov		x12, #0x12   \n\t"
            "mov		x13, #0x13   \n\t"
            "mov		x14, #0x14   \n\t"
            "mov		x15, #0x15   \n\t"
            "mov		x16, #0x16   \n\t"
            "mov		x17, #0x17   \n\t"
            "mov		x18, #0x18   \n\t"
            "mov		x19, #0x19   \n\t"
            "mov		x20, #0x20   \n\t"
            "mov		x21, #0x21   \n\t"
            "mov		x22, #0x22   \n\t"
            "mov		x23, #0x23   \n\t"
            "mov		x24, #0x24   \n\t"
            "mov		x25, #0x25   \n\t"
            "mov		x26, #0x26   \n\t"
            "mov		x27, #0x27   \n\t"
            "mov		x28, #0x28   \n\t"
            "mov		x29, #0x29   \n\t"
            "mov		x30, #0x30   \n\t"

            /* Fill each FPU register with a known value. */
            "fmov	v0.d[1], x0     \n\t"
            "fmov	d1, x1          \n\t"
            "fmov	d2, x2          \n\t"
            "fmov	d3, x3          \n\t"
            "fmov	d4, x4          \n\t"
            "fmov	d5, x5          \n\t"
            "fmov	d6, x6          \n\t"
            "fmov	d7, x7          \n\t"
            "fmov	d8, x8          \n\t"
            "fmov	d9, x9          \n\t"
            "fmov	d10, x10        \n\t"
            "fmov	d11, x11        \n\t"
            "fmov	d12, x12        \n\t"
            "fmov	d13, x13        \n\t"
            "fmov	d14, x14        \n\t"
            "fmov	d15, x15        \n\t"
            "fmov	d16, x16        \n\t"
            "fmov	d17, x17        \n\t"
            "fmov	d18, x18        \n\t"
            "fmov	d19, x19        \n\t"
            "fmov	d20, x20        \n\t"
            "fmov	d21, x21        \n\t"
            "fmov	d22, x22        \n\t"
            "fmov	d23, x23        \n\t"
            "fmov	d24, x24        \n\t"
            "fmov	d25, x25        \n\t"
            "fmov	d26, x26        \n\t"
            "fmov	d27, x27        \n\t"
            "fmov	d28, x28        \n\t"
            "fmov	d29, x29        \n\t"
            "fmov	v30.d[1], x30   \n\t"
            "fmov	d31, xzr        \n\t"

            /* Loop, checking each itteration that each register still contains the
            expected value. */
        "reg1_loop:                 \n\t"

            /* Test each general purpose register to check that it still contains the
            expected known value, jumping to reg1_error_loop if any register contains
            an unexpected value. */
            "cmp		x0, #0xFF               \r\n"
            "bne		reg1_error_loop         \r\n"
            "cmp		x1, #0x01               \r\n"
            "bne		reg1_error_loop         \r\n"
            "cmp		x2, #0x02               \r\n"
            "bne		reg1_error_loop         \r\n"
            "cmp		x3, #0x03               \r\n"
            "bne		reg1_error_loop         \r\n"
            "cmp		x4, #0x04               \r\n"
            "bne		reg1_error_loop         \r\n"
            "cmp		x5, #0x05               \r\n"
            "bne		reg1_error_loop         \r\n"
            "cmp		x6, #0x06               \r\n"
            "bne		reg1_error_loop         \r\n"
            "cmp		x7, #0x07               \r\n"
            "bne		reg1_error_loop         \r\n"
            "cmp		x8, #0x08               \r\n"
            "bne		reg1_error_loop         \r\n"
            "cmp		x9, #0x09               \r\n"
            "bne		reg1_error_loop         \r\n"
            "cmp		x10, #0x10              \r\n"
            "bne		reg1_error_loop         \r\n"
            "cmp		x11, #0x11              \r\n"
            "bne		reg1_error_loop         \r\n"
            "cmp		x12, #0x12              \r\n"
            "bne		reg1_error_loop         \r\n"
            "cmp		x13, #0x13              \r\n"
            "bne		reg1_error_loop         \r\n"
            "cmp		x14, #0x14              \r\n"
            "bne		reg1_error_loop         \r\n"
            "cmp		x15, #0x15              \r\n"
            "bne		reg1_error_loop         \r\n"
            "cmp		x16, #0x16              \r\n"
            "bne		reg1_error_loop         \r\n"
            "cmp		x17, #0x17              \r\n"
            "bne		reg1_error_loop         \r\n"
            "cmp		x18, #0x18              \r\n"
            "bne		reg1_error_loop         \r\n"
            "cmp		x19, #0x19              \r\n"
            "bne		reg1_error_loop         \r\n"
            "cmp		x20, #0x20              \r\n"
            "bne		reg1_error_loop         \r\n"
            "cmp		x21, #0x21              \r\n"
            "bne		reg1_error_loop         \r\n"
            "cmp		x22, #0x22              \r\n"
            "bne		reg1_error_loop         \r\n"
            "cmp		x23, #0x23              \r\n"
            "bne		reg1_error_loop         \r\n"
            "cmp		x24, #0x24              \r\n"
            "bne		reg1_error_loop         \r\n"
            "cmp		x25, #0x25              \r\n"
            "bne		reg1_error_loop         \r\n"
            "cmp		x26, #0x26              \r\n"
            "bne		reg1_error_loop         \r\n"
            "cmp		x27, #0x27              \r\n"
            "bne		reg1_error_loop         \r\n"
            "cmp		x28, #0x28              \r\n"
            "bne		reg1_error_loop         \r\n"
            "cmp		x29, #0x29              \r\n"
            "bne		reg1_error_loop         \r\n"
            "cmp		x30, #0x30              \r\n"
            "bne		reg1_error_loop         \r\n"

            /* Check every floating point register to ensure it contains the expected
            value.  First save the registers clobbered by the test. */
            "stp 	x0, x1, [sp,#-0x10]!    \r\n"

            "fmov	    x0, v0.d[1]         \n\t"
            "cmp		x0, 0xff            \n\t"
            "bne		reg1_error_loop     \n\t"
            "fmov	    x0, d1              \n\t"
            "cmp		x0, 0x01            \n\t"
            "bne		reg1_error_loop     \n\t"
            "fmov	    x0, d2              \n\t"
            "cmp		x0, 0x02            \n\t"
            "bne		reg1_error_loop     \n\t"
            "fmov	    x0, d3              \n\t"
            "cmp		x0, 0x03            \n\t"
            "bne		reg1_error_loop     \n\t"
            "fmov	    x0, d4              \n\t"
            "cmp		x0, 0x04            \n\t"
            "bne		reg1_error_loop     \n\t"
            "fmov	    x0, d5              \n\t"
            "cmp		x0, 0x05            \n\t"
            "bne		reg1_error_loop     \n\t"
            "fmov	    x0, d6              \n\t"
            "cmp		x0, 0x06            \n\t"
            "bne		reg1_error_loop     \n\t"
            "fmov	    x0, d7              \n\t"
            "cmp		x0, 0x07            \n\t"
            "bne		reg1_error_loop     \n\t"
            "fmov	    x0, d8              \n\t"
            "cmp		x0, 0x08            \n\t"
            "bne		reg1_error_loop     \n\t"
            "fmov	    x0, d9              \n\t"
            "cmp		x0, 0x09            \n\t"
            "bne		reg1_error_loop     \n\t"
            "fmov	    x0, d10             \n\t"
            "cmp		x0, 0x10            \n\t"
            "bne		reg1_error_loop     \n\t"
            "fmov	    x0, d11             \n\t"
            "cmp		x0, 0x11            \n\t"
            "bne		reg1_error_loop     \n\t"
            "fmov	    x0, d12             \n\t"
            "cmp		x0, 0x12            \n\t"
            "bne		reg1_error_loop     \n\t"
            "fmov	    x0, d13             \n\t"
            "cmp		x0, 0x13            \n\t"
            "bne		reg1_error_loop     \n\t"
            "fmov	    x0, d14             \n\t"
            "cmp		x0, 0x14            \n\t"
            "bne		reg1_error_loop     \n\t"
            "fmov	    x0, d15             \n\t"
            "cmp		x0, 0x15            \n\t"
            "bne		reg1_error_loop     \n\t"
            "fmov	    x0, d16             \n\t"
            "cmp		x0, 0x16            \n\t"
            "bne		reg1_error_loop     \n\t"
            "fmov	    x0, d17             \n\t"
            "cmp		x0, 0x17            \n\t"
            "bne		reg1_error_loop     \n\t"
            "fmov	    x0, d18             \n\t"
            "cmp		x0, 0x18            \n\t"
            "bne		reg1_error_loop     \n\t"
            "fmov	    x0, d19             \n\t"
            "cmp		x0, 0x19            \n\t"
            "bne		reg1_error_loop     \n\t"
            "fmov	    x0, d20             \n\t"
            "cmp		x0, 0x20            \n\t"
            "bne		reg1_error_loop     \n\t"
            "fmov	    x0, d21             \n\t"
            "cmp		x0, 0x21            \n\t"
            "bne		reg1_error_loop     \n\t"
            "fmov	    x0, d22             \n\t"
            "cmp		x0, 0x22            \n\t"
            "bne		reg1_error_loop     \n\t"
            "fmov	    x0, d23             \n\t"
            "cmp		x0, 0x23            \n\t"
            "bne		reg1_error_loop     \n\t"
            "fmov	    x0, d24             \n\t"
            "cmp		x0, 0x24            \n\t"
            "bne		reg1_error_loop     \n\t"
            "fmov	    x0, d25             \n\t"
            "cmp		x0, 0x25            \n\t"
            "bne		reg1_error_loop     \n\t"
            "fmov	    x0, d26             \n\t"
            "cmp		x0, 0x26            \n\t"
            "bne		reg1_error_loop     \n\t"
            "fmov	    x0, d27             \n\t"
            "cmp		x0, 0x27            \n\t"
            "bne		reg1_error_loop     \n\t"
            "fmov	    x0, d28             \n\t"
            "cmp		x0, 0x28            \n\t"
            "bne		reg1_error_loop     \n\t"
            "fmov	    x0, d29             \n\t"
            "cmp		x0, 0x29            \n\t"
            "bne		reg1_error_loop     \n\t"
            "fmov	    x0, v30.d[1]        \n\t"
            "cmp		x0, 0x30            \n\t"
            "bne		reg1_error_loop     \n\t"
            "fmov	    x0, d31             \n\t"
            "cmp		x0, 0x00            \n\t"
            "bne		reg1_error_loop     \n\t"

            /* Restore the registers clobbered by the test. */
            "ldp 	x0, x1, [sp], #0x10     \n\t"

            /* Everything passed, increment the loop counter. */
            "stp 	x0, x1, [sp,#-0x10]!            \n\t"
            "ldr	x0, =ulRegisterTest1Counter     \n\t"
            "ldr 	x1, [x0]                        \n\t"
            "adds 	x1, x1, #1                      \n\t"
            "str 	x1, [x0]                        \n\t"
            "ldp 	x0, x1, [sp], #0x10             \n\t"

            /* Start again. */
            "b reg1_loop                            \n\t"

        "reg1_error_loop:                           \n\t"
            /* If this line is hit then there was an error in a core register value.
            The loop ensures the loop counter stops incrementing. */
            "b reg1_error_loop                      \n\t"
            "nop                                    \n\t"
        );
	}
}
/*-----------------------------------------------------------*/

static void prvRegisterTest2Task( void *pvParameters )
{
	( void ) pvParameters;

    portTASK_USES_FLOATING_POINT();

	for( ; ; )
	{
		/* 1. Fill the registers stored as part of task context with known values.
		* 2. Force a context switch.
		* 3. Verify that all the registers contain expected values.
		* 4. If all the register contain expected values, increment ulRegisterTest2Counter.
		*/
        __asm__ __volatile__ (
            /* Fill each general purpose register with a known value. */
            "mov		x0, #0x1ff    \n\t"
            "mov		x1, #0x101    \n\t"
            "mov		x2, #0x102    \n\t"
            "mov		x3, #0x103    \n\t"
            "mov		x4, #0x104    \n\t"
            "mov		x5, #0x105    \n\t"
            "mov		x6, #0x106    \n\t"
            "mov		x7, #0x107    \n\t"
            "mov		x8, #0x108    \n\t"
            "mov		x9, #0x109    \n\t"
            "mov		x10, #0x110   \n\t"
            "mov		x11, #0x111   \n\t"
            "mov		x12, #0x112   \n\t"
            "mov		x13, #0x113   \n\t"
            "mov		x14, #0x114   \n\t"
            "mov		x15, #0x115   \n\t"
            "mov		x16, #0x116   \n\t"
            "mov		x17, #0x117   \n\t"
            "mov		x18, #0x118   \n\t"
            "mov		x19, #0x119   \n\t"
            "mov		x20, #0x120   \n\t"
            "mov		x21, #0x121   \n\t"
            "mov		x22, #0x122   \n\t"
            "mov		x23, #0x123   \n\t"
            "mov		x24, #0x124   \n\t"
            "mov		x25, #0x125   \n\t"
            "mov		x26, #0x126   \n\t"
            "mov		x27, #0x127   \n\t"
            "mov		x28, #0x128   \n\t"
            "mov		x29, #0x129   \n\t"
            "mov		x30, #0x130   \n\t"

            /* Fill each FPU register with a known value. */
            "fmov	v0.d[1], x0     \n\t"
            "fmov	d1, x1          \n\t"
            "fmov	d2, x2          \n\t"
            "fmov	d3, x3          \n\t"
            "fmov	d4, x4          \n\t"
            "fmov	d5, x5          \n\t"
            "fmov	d6, x6          \n\t"
            "fmov	d7, x7          \n\t"
            "fmov	d8, x8          \n\t"
            "fmov	d9, x9          \n\t"
            "fmov	d10, x10        \n\t"
            "fmov	d11, x11        \n\t"
            "fmov	d12, x12        \n\t"
            "fmov	d13, x13        \n\t"
            "fmov	d14, x14        \n\t"
            "fmov	d15, x15        \n\t"
            "fmov	d16, x16        \n\t"
            "fmov	d17, x17        \n\t"
            "fmov	d18, x18        \n\t"
            "fmov	d19, x19        \n\t"
            "fmov	d20, x20        \n\t"
            "fmov	d21, x21        \n\t"
            "fmov	d22, x22        \n\t"
            "fmov	d23, x23        \n\t"
            "fmov	d24, x24        \n\t"
            "fmov	d25, x25        \n\t"
            "fmov	d26, x26        \n\t"
            "fmov	d27, x27        \n\t"
            "fmov	d28, x28        \n\t"
            "fmov	d29, x29        \n\t"
            "fmov	v30.d[1], x30   \n\t"
            "fmov	d31, xzr        \n\t"

            /* Loop, checking each itteration that each register still contains the
            expected value. */
        "reg2_loop:                 \n\t"

            /* Test each general purpose register to check that it still contains the
            expected known value, jumping to reg2_error_loop if any register contains
            an unexpected value. */
            "cmp		x0, #0x1FF               \r\n"
            "bne		reg2_error_loop          \r\n"
            "cmp		x1, #0x101               \r\n"
            "bne		reg2_error_loop          \r\n"
            "cmp		x2, #0x102               \r\n"
            "bne		reg2_error_loop          \r\n"
            "cmp		x3, #0x103               \r\n"
            "bne		reg2_error_loop          \r\n"
            "cmp		x4, #0x104               \r\n"
            "bne		reg2_error_loop          \r\n"
            "cmp		x5, #0x105               \r\n"
            "bne		reg2_error_loop          \r\n"
            "cmp		x6, #0x106               \r\n"
            "bne		reg2_error_loop          \r\n"
            "cmp		x7, #0x107               \r\n"
            "bne		reg2_error_loop          \r\n"
            "cmp		x8, #0x108               \r\n"
            "bne		reg2_error_loop          \r\n"
            "cmp		x9, #0x109               \r\n"
            "bne		reg2_error_loop          \r\n"
            "cmp		x10, #0x110              \r\n"
            "bne		reg2_error_loop          \r\n"
            "cmp		x11, #0x111              \r\n"
            "bne		reg2_error_loop          \r\n"
            "cmp		x12, #0x112              \r\n"
            "bne		reg2_error_loop          \r\n"
            "cmp		x13, #0x113              \r\n"
            "bne		reg2_error_loop          \r\n"
            "cmp		x14, #0x114              \r\n"
            "bne		reg2_error_loop          \r\n"
            "cmp		x15, #0x115              \r\n"
            "bne		reg2_error_loop          \r\n"
            "cmp		x16, #0x116              \r\n"
            "bne		reg2_error_loop          \r\n"
            "cmp		x17, #0x117              \r\n"
            "bne		reg2_error_loop          \r\n"
            "cmp		x18, #0x118              \r\n"
            "bne		reg2_error_loop          \r\n"
            "cmp		x19, #0x119              \r\n"
            "bne		reg2_error_loop          \r\n"
            "cmp		x20, #0x120              \r\n"
            "bne		reg2_error_loop          \r\n"
            "cmp		x21, #0x121              \r\n"
            "bne		reg2_error_loop          \r\n"
            "cmp		x22, #0x122              \r\n"
            "bne		reg2_error_loop          \r\n"
            "cmp		x23, #0x123              \r\n"
            "bne		reg2_error_loop          \r\n"
            "cmp		x24, #0x124              \r\n"
            "bne		reg2_error_loop          \r\n"
            "cmp		x25, #0x125              \r\n"
            "bne		reg2_error_loop          \r\n"
            "cmp		x26, #0x126              \r\n"
            "bne		reg2_error_loop          \r\n"
            "cmp		x27, #0x127              \r\n"
            "bne		reg2_error_loop          \r\n"
            "cmp		x28, #0x128              \r\n"
            "bne		reg2_error_loop          \r\n"
            "cmp		x29, #0x129              \r\n"
            "bne		reg2_error_loop          \r\n"
            "cmp		x30, #0x130              \r\n"
            "bne		reg2_error_loop          \r\n"

            /* Check every floating point register to ensure it contains the expected
            value.  First save the registers clobbered by the test. */
            "stp 	x0, x1, [sp,#-0x10]!    \r\n"

            "fmov	    x0, v0.d[1]         \n\t"
            "cmp		x0, 0x1ff           \n\t"
            "bne		reg2_error_loop     \n\t"
            "fmov	    x0, d1              \n\t"
            "cmp		x0, 0x101           \n\t"
            "bne		reg2_error_loop     \n\t"
            "fmov	    x0, d2              \n\t"
            "cmp		x0, 0x102           \n\t"
            "bne		reg2_error_loop     \n\t"
            "fmov	    x0, d3              \n\t"
            "cmp		x0, 0x103           \n\t"
            "bne		reg2_error_loop     \n\t"
            "fmov	    x0, d4              \n\t"
            "cmp		x0, 0x104           \n\t"
            "bne		reg2_error_loop     \n\t"
            "fmov	    x0, d5              \n\t"
            "cmp		x0, 0x105           \n\t"
            "bne		reg2_error_loop     \n\t"
            "fmov	    x0, d6              \n\t"
            "cmp		x0, 0x106           \n\t"
            "bne		reg2_error_loop     \n\t"
            "fmov	    x0, d7              \n\t"
            "cmp		x0, 0x107           \n\t"
            "bne		reg2_error_loop     \n\t"
            "fmov	    x0, d8              \n\t"
            "cmp		x0, 0x108           \n\t"
            "bne		reg2_error_loop     \n\t"
            "fmov	    x0, d9              \n\t"
            "cmp		x0, 0x109           \n\t"
            "bne		reg2_error_loop     \n\t"
            "fmov	    x0, d10             \n\t"
            "cmp		x0, 0x110           \n\t"
            "bne		reg2_error_loop     \n\t"
            "fmov	    x0, d11             \n\t"
            "cmp		x0, 0x111           \n\t"
            "bne		reg2_error_loop     \n\t"
            "fmov	    x0, d12             \n\t"
            "cmp		x0, 0x112           \n\t"
            "bne		reg2_error_loop     \n\t"
            "fmov	    x0, d13             \n\t"
            "cmp		x0, 0x113           \n\t"
            "bne		reg2_error_loop     \n\t"
            "fmov	    x0, d14             \n\t"
            "cmp		x0, 0x114           \n\t"
            "bne		reg2_error_loop     \n\t"
            "fmov	    x0, d15             \n\t"
            "cmp		x0, 0x115           \n\t"
            "bne		reg2_error_loop     \n\t"
            "fmov	    x0, d16             \n\t"
            "cmp		x0, 0x116           \n\t"
            "bne		reg2_error_loop     \n\t"
            "fmov	    x0, d17             \n\t"
            "cmp		x0, 0x117           \n\t"
            "bne		reg2_error_loop     \n\t"
            "fmov	    x0, d18             \n\t"
            "cmp		x0, 0x118           \n\t"
            "bne		reg2_error_loop     \n\t"
            "fmov	    x0, d19             \n\t"
            "cmp		x0, 0x119           \n\t"
            "bne		reg2_error_loop     \n\t"
            "fmov	    x0, d20             \n\t"
            "cmp		x0, 0x120           \n\t"
            "bne		reg2_error_loop     \n\t"
            "fmov	    x0, d21             \n\t"
            "cmp		x0, 0x121           \n\t"
            "bne		reg2_error_loop     \n\t"
            "fmov	    x0, d22             \n\t"
            "cmp		x0, 0x122           \n\t"
            "bne		reg2_error_loop     \n\t"
            "fmov	    x0, d23             \n\t"
            "cmp		x0, 0x123           \n\t"
            "bne		reg2_error_loop     \n\t"
            "fmov	    x0, d24             \n\t"
            "cmp		x0, 0x124           \n\t"
            "bne		reg2_error_loop     \n\t"
            "fmov	    x0, d25             \n\t"
            "cmp		x0, 0x125           \n\t"
            "bne		reg2_error_loop     \n\t"
            "fmov	    x0, d26             \n\t"
            "cmp		x0, 0x126           \n\t"
            "bne		reg2_error_loop     \n\t"
            "fmov	    x0, d27             \n\t"
            "cmp		x0, 0x127           \n\t"
            "bne		reg2_error_loop     \n\t"
            "fmov	    x0, d28             \n\t"
            "cmp		x0, 0x128           \n\t"
            "bne		reg2_error_loop     \n\t"
            "fmov	    x0, d29             \n\t"
            "cmp		x0, 0x129           \n\t"
            "bne		reg2_error_loop     \n\t"
            "fmov	    x0, v30.d[1]        \n\t"
            "cmp		x0, 0x130           \n\t"
            "bne		reg2_error_loop     \n\t"
            "fmov	    x0, d31             \n\t"
            "cmp		x0, 0x00            \n\t"
            "bne		reg2_error_loop     \n\t"

            /* Restore the registers clobbered by the test. */
            "ldp 	x0, x1, [sp], #0x10     \n\t"

            /* Everything passed, increment the loop counter. */
            "stp 	x0, x1, [sp,#-0x10]!            \n\t"
            "ldr	x0, =ulRegisterTest2Counter     \n\t"
            "ldr 	x1, [x0]                        \n\t"
            "adds 	x1, x1, #1                      \n\t"
            "str 	x1, [x0]                        \n\t"
            "ldp 	x0, x1, [sp], #0x10             \n\t"

            /* Start again. */
            "b reg2_loop                            \n\t"

        "reg2_error_loop:                           \n\t"
            /* If this line is hit then there was an error in a core register value.
            The loop ensures the loop counter stops incrementing. */
            "b reg2_error_loop                      \n\t"
            "nop                                    \n\t"
        );
	}
}
/*-----------------------------------------------------------*/

static void prvRegisterTest3Task( void *pvParameters )
{
	( void ) pvParameters;

    portTASK_USES_FLOATING_POINT();

	for( ; ; )
	{
		/* 1. Fill the registers stored as part of task context with known values.
		* 2. Force a context switch.
		* 3. Verify that all the registers contain expected values.
		* 4. If all the register contain expected values, increment ulRegisterTest3Counter.
		*/

        __asm__ __volatile__ (
            /* Fill each general purpose register with a known value. */
            "mov		x0, #0x2ff    \n\t"
            "mov		x1, #0x201    \n\t"
            "mov		x2, #0x202    \n\t"
            "mov		x3, #0x203    \n\t"
            "mov		x4, #0x204    \n\t"
            "mov		x5, #0x205    \n\t"
            "mov		x6, #0x206    \n\t"
            "mov		x7, #0x207    \n\t"
            "mov		x8, #0x208    \n\t"
            "mov		x9, #0x209    \n\t"
            "mov		x10, #0x210   \n\t"
            "mov		x11, #0x211   \n\t"
            "mov		x12, #0x212   \n\t"
            "mov		x13, #0x213   \n\t"
            "mov		x14, #0x214   \n\t"
            "mov		x15, #0x215   \n\t"
            "mov		x16, #0x216   \n\t"
            "mov		x17, #0x217   \n\t"
            "mov		x18, #0x218   \n\t"
            "mov		x19, #0x219   \n\t"
            "mov		x20, #0x220   \n\t"
            "mov		x21, #0x221   \n\t"
            "mov		x22, #0x222   \n\t"
            "mov		x23, #0x223   \n\t"
            "mov		x24, #0x224   \n\t"
            "mov		x25, #0x225   \n\t"
            "mov		x26, #0x226   \n\t"
            "mov		x27, #0x227   \n\t"
            "mov		x28, #0x228   \n\t"
            "mov		x29, #0x229   \n\t"
            "mov		x30, #0x230   \n\t"

            /* Fill each FPU register with a known value. */
            "fmov	v0.d[1], x0     \n\t"
            "fmov	d1, x1          \n\t"
            "fmov	d2, x2          \n\t"
            "fmov	d3, x3          \n\t"
            "fmov	d4, x4          \n\t"
            "fmov	d5, x5          \n\t"
            "fmov	d6, x6          \n\t"
            "fmov	d7, x7          \n\t"
            "fmov	d8, x8          \n\t"
            "fmov	d9, x9          \n\t"
            "fmov	d10, x10        \n\t"
            "fmov	d11, x11        \n\t"
            "fmov	d12, x12        \n\t"
            "fmov	d13, x13        \n\t"
            "fmov	d14, x14        \n\t"
            "fmov	d15, x15        \n\t"
            "fmov	d16, x16        \n\t"
            "fmov	d17, x17        \n\t"
            "fmov	d18, x18        \n\t"
            "fmov	d19, x19        \n\t"
            "fmov	d20, x20        \n\t"
            "fmov	d21, x21        \n\t"
            "fmov	d22, x22        \n\t"
            "fmov	d23, x23        \n\t"
            "fmov	d24, x24        \n\t"
            "fmov	d25, x25        \n\t"
            "fmov	d26, x26        \n\t"
            "fmov	d27, x27        \n\t"
            "fmov	d28, x28        \n\t"
            "fmov	d29, x29        \n\t"
            "fmov	v30.d[1], x30   \n\t"
            "fmov	d31, xzr        \n\t"

            /* Loop, checking each itteration that each register still contains the
            expected value. */
        "reg3_loop:                 \n\t"

            /* Test each general purpose register to check that it still contains the
            expected known value, jumping to reg3_error_loop if any register contains
            an unexpected value. */
            "cmp		x0, #0x2FF               \r\n"
            "bne		reg3_error_loop          \r\n"
            "cmp		x1, #0x201               \r\n"
            "bne		reg3_error_loop          \r\n"
            "cmp		x2, #0x202               \r\n"
            "bne		reg3_error_loop          \r\n"
            "cmp		x3, #0x203               \r\n"
            "bne		reg3_error_loop          \r\n"
            "cmp		x4, #0x204               \r\n"
            "bne		reg3_error_loop          \r\n"
            "cmp		x5, #0x205               \r\n"
            "bne		reg3_error_loop          \r\n"
            "cmp		x6, #0x206               \r\n"
            "bne		reg3_error_loop          \r\n"
            "cmp		x7, #0x207               \r\n"
            "bne		reg3_error_loop          \r\n"
            "cmp		x8, #0x208               \r\n"
            "bne		reg3_error_loop          \r\n"
            "cmp		x9, #0x209               \r\n"
            "bne		reg3_error_loop          \r\n"
            "cmp		x10, #0x210              \r\n"
            "bne		reg3_error_loop          \r\n"
            "cmp		x11, #0x211              \r\n"
            "bne		reg3_error_loop          \r\n"
            "cmp		x12, #0x212              \r\n"
            "bne		reg3_error_loop          \r\n"
            "cmp		x13, #0x213              \r\n"
            "bne		reg3_error_loop          \r\n"
            "cmp		x14, #0x214              \r\n"
            "bne		reg3_error_loop          \r\n"
            "cmp		x15, #0x215              \r\n"
            "bne		reg3_error_loop          \r\n"
            "cmp		x16, #0x216              \r\n"
            "bne		reg3_error_loop          \r\n"
            "cmp		x17, #0x217              \r\n"
            "bne		reg3_error_loop          \r\n"
            "cmp		x18, #0x218              \r\n"
            "bne		reg3_error_loop          \r\n"
            "cmp		x19, #0x219              \r\n"
            "bne		reg3_error_loop          \r\n"
            "cmp		x20, #0x220              \r\n"
            "bne		reg3_error_loop          \r\n"
            "cmp		x21, #0x221              \r\n"
            "bne		reg3_error_loop          \r\n"
            "cmp		x22, #0x222              \r\n"
            "bne		reg3_error_loop          \r\n"
            "cmp		x23, #0x223              \r\n"
            "bne		reg3_error_loop          \r\n"
            "cmp		x24, #0x224              \r\n"
            "bne		reg3_error_loop          \r\n"
            "cmp		x25, #0x225              \r\n"
            "bne		reg3_error_loop          \r\n"
            "cmp		x26, #0x226              \r\n"
            "bne		reg3_error_loop          \r\n"
            "cmp		x27, #0x227              \r\n"
            "bne		reg3_error_loop          \r\n"
            "cmp		x28, #0x228              \r\n"
            "bne		reg3_error_loop          \r\n"
            "cmp		x29, #0x229              \r\n"
            "bne		reg3_error_loop          \r\n"
            "cmp		x30, #0x230              \r\n"
            "bne		reg3_error_loop          \r\n"

            /* Check every floating point register to ensure it contains the expected
            value.  First save the registers clobbered by the test. */
            "stp 	x0, x1, [sp,#-0x10]!    \r\n"

            "fmov	    x0, v0.d[1]         \n\t"
            "cmp		x0, 0x2ff           \n\t"
            "bne		reg3_error_loop     \n\t"
            "fmov	    x0, d1              \n\t"
            "cmp		x0, 0x201           \n\t"
            "bne		reg3_error_loop     \n\t"
            "fmov	    x0, d2              \n\t"
            "cmp		x0, 0x202           \n\t"
            "bne		reg3_error_loop     \n\t"
            "fmov	    x0, d3              \n\t"
            "cmp		x0, 0x203           \n\t"
            "bne		reg3_error_loop     \n\t"
            "fmov	    x0, d4              \n\t"
            "cmp		x0, 0x204           \n\t"
            "bne		reg3_error_loop     \n\t"
            "fmov	    x0, d5              \n\t"
            "cmp		x0, 0x205           \n\t"
            "bne		reg3_error_loop     \n\t"
            "fmov	    x0, d6              \n\t"
            "cmp		x0, 0x206           \n\t"
            "bne		reg3_error_loop     \n\t"
            "fmov	    x0, d7              \n\t"
            "cmp		x0, 0x207           \n\t"
            "bne		reg3_error_loop     \n\t"
            "fmov	    x0, d8              \n\t"
            "cmp		x0, 0x208           \n\t"
            "bne		reg3_error_loop     \n\t"
            "fmov	    x0, d9              \n\t"
            "cmp		x0, 0x209           \n\t"
            "bne		reg3_error_loop     \n\t"
            "fmov	    x0, d10             \n\t"
            "cmp		x0, 0x210           \n\t"
            "bne		reg3_error_loop     \n\t"
            "fmov	    x0, d11             \n\t"
            "cmp		x0, 0x211           \n\t"
            "bne		reg3_error_loop     \n\t"
            "fmov	    x0, d12             \n\t"
            "cmp		x0, 0x212           \n\t"
            "bne		reg3_error_loop     \n\t"
            "fmov	    x0, d13             \n\t"
            "cmp		x0, 0x213           \n\t"
            "bne		reg3_error_loop     \n\t"
            "fmov	    x0, d14             \n\t"
            "cmp		x0, 0x214           \n\t"
            "bne		reg3_error_loop     \n\t"
            "fmov	    x0, d15             \n\t"
            "cmp		x0, 0x215           \n\t"
            "bne		reg3_error_loop     \n\t"
            "fmov	    x0, d16             \n\t"
            "cmp		x0, 0x216           \n\t"
            "bne		reg3_error_loop     \n\t"
            "fmov	    x0, d17             \n\t"
            "cmp		x0, 0x217           \n\t"
            "bne		reg3_error_loop     \n\t"
            "fmov	    x0, d18             \n\t"
            "cmp		x0, 0x218           \n\t"
            "bne		reg3_error_loop     \n\t"
            "fmov	    x0, d19             \n\t"
            "cmp		x0, 0x219           \n\t"
            "bne		reg3_error_loop     \n\t"
            "fmov	    x0, d20             \n\t"
            "cmp		x0, 0x220           \n\t"
            "bne		reg3_error_loop     \n\t"
            "fmov	    x0, d21             \n\t"
            "cmp		x0, 0x221           \n\t"
            "bne		reg3_error_loop     \n\t"
            "fmov	    x0, d22             \n\t"
            "cmp		x0, 0x222           \n\t"
            "bne		reg3_error_loop     \n\t"
            "fmov	    x0, d23             \n\t"
            "cmp		x0, 0x223           \n\t"
            "bne		reg3_error_loop     \n\t"
            "fmov	    x0, d24             \n\t"
            "cmp		x0, 0x224           \n\t"
            "bne		reg3_error_loop     \n\t"
            "fmov	    x0, d25             \n\t"
            "cmp		x0, 0x225           \n\t"
            "bne		reg3_error_loop     \n\t"
            "fmov	    x0, d26             \n\t"
            "cmp		x0, 0x226           \n\t"
            "bne		reg3_error_loop     \n\t"
            "fmov	    x0, d27             \n\t"
            "cmp		x0, 0x227           \n\t"
            "bne		reg3_error_loop     \n\t"
            "fmov	    x0, d28             \n\t"
            "cmp		x0, 0x228           \n\t"
            "bne		reg3_error_loop     \n\t"
            "fmov	    x0, d29             \n\t"
            "cmp		x0, 0x229           \n\t"
            "bne		reg3_error_loop     \n\t"
            "fmov	    x0, v30.d[1]        \n\t"
            "cmp		x0, 0x230           \n\t"
            "bne		reg3_error_loop     \n\t"
            "fmov	    x0, d31             \n\t"
            "cmp		x0, 0x00            \n\t"
            "bne		reg3_error_loop     \n\t"

            /* Restore the registers clobbered by the test. */
            "ldp 	x0, x1, [sp], #0x10     \n\t"

            /* Everything passed, increment the loop counter. */
            "stp 	x0, x1, [sp,#-0x10]!            \n\t"
            "ldr	x0, =ulRegisterTest3Counter     \n\t"
            "ldr 	x1, [x0]                        \n\t"
            "adds 	x1, x1, #1                      \n\t"
            "str 	x1, [x0]                        \n\t"
            "ldp 	x0, x1, [sp], #0x10             \n\t"

            /* Start again. */
            "b reg3_loop                            \n\t"

        "reg3_error_loop:                           \n\t"
            /* If this line is hit then there was an error in a core register value.
            The loop ensures the loop counter stops incrementing. */
            "b reg3_error_loop                      \n\t"
            "nop                                    \n\t"
        );
	}
}
/*-----------------------------------------------------------*/

static void prvRegisterTest4Task( void *pvParameters )
{
	( void ) pvParameters;

    portTASK_USES_FLOATING_POINT();

	for( ; ; )
	{
        __asm__ __volatile__ (
            /* Fill each general purpose register with a known value. */
        "mov		x0, #0x3ff    \n\t"
        "mov		x1, #0x301    \n\t"
        "mov		x2, #0x302    \n\t"
        "mov		x3, #0x303    \n\t"
        "mov		x4, #0x304    \n\t"
        "mov		x5, #0x305    \n\t"
        "mov		x6, #0x306    \n\t"
        "mov		x7, #0x307    \n\t"
        "mov		x8, #0x308    \n\t"
        "mov		x9, #0x309    \n\t"
        "mov		x10, #0x310   \n\t"
        "mov		x11, #0x311   \n\t"
        "mov		x12, #0x312   \n\t"
        "mov		x13, #0x313   \n\t"
        "mov		x14, #0x314   \n\t"
        "mov		x15, #0x315   \n\t"
        "mov		x16, #0x316   \n\t"
        "mov		x17, #0x317   \n\t"
        "mov		x18, #0x318   \n\t"
        "mov		x19, #0x319   \n\t"
        "mov		x20, #0x320   \n\t"
        "mov		x21, #0x321   \n\t"
        "mov		x22, #0x322   \n\t"
        "mov		x23, #0x323   \n\t"
        "mov		x24, #0x324   \n\t"
        "mov		x25, #0x325   \n\t"
        "mov		x26, #0x326   \n\t"
        "mov		x27, #0x327   \n\t"
        "mov		x28, #0x328   \n\t"
        "mov		x29, #0x329   \n\t"
        "mov		x30, #0x330   \n\t"

        /* Fill each FPU register with a known value. */
        "fmov	v0.d[1], x0     \n\t"
        "fmov	d1, x1          \n\t"
        "fmov	d2, x2          \n\t"
        "fmov	d3, x3          \n\t"
        "fmov	d4, x4          \n\t"
        "fmov	d5, x5          \n\t"
        "fmov	d6, x6          \n\t"
        "fmov	d7, x7          \n\t"
        "fmov	d8, x8          \n\t"
        "fmov	d9, x9          \n\t"
        "fmov	d10, x10        \n\t"
        "fmov	d11, x11        \n\t"
        "fmov	d12, x12        \n\t"
        "fmov	d13, x13        \n\t"
        "fmov	d14, x14        \n\t"
        "fmov	d15, x15        \n\t"
        "fmov	d16, x16        \n\t"
        "fmov	d17, x17        \n\t"
        "fmov	d18, x18        \n\t"
        "fmov	d19, x19        \n\t"
        "fmov	d20, x20        \n\t"
        "fmov	d21, x21        \n\t"
        "fmov	d22, x22        \n\t"
        "fmov	d23, x23        \n\t"
        "fmov	d24, x24        \n\t"
        "fmov	d25, x25        \n\t"
        "fmov	d26, x26        \n\t"
        "fmov	d27, x27        \n\t"
        "fmov	d28, x28        \n\t"
        "fmov	d29, x29        \n\t"
        "fmov	v30.d[1], x30   \n\t"
        "fmov	d31, xzr        \n\t"

        /* Loop, checking each itteration that each register still contains the
        expected value. */
    "reg4_loop:                 \n\t"

        /* Test each general purpose register to check that it still contains the
        expected known value, jumping to reg4_error_loop if any register contains
        an unexpected value. */
        "cmp		x0, #0x3FF               \r\n"
        "bne		reg4_error_loop          \r\n"
        "cmp		x1, #0x301               \r\n"
        "bne		reg4_error_loop          \r\n"
        "cmp		x2, #0x302               \r\n"
        "bne		reg4_error_loop          \r\n"
        "cmp		x3, #0x303               \r\n"
        "bne		reg4_error_loop          \r\n"
        "cmp		x4, #0x304               \r\n"
        "bne		reg4_error_loop          \r\n"
        "cmp		x5, #0x305               \r\n"
        "bne		reg4_error_loop          \r\n"
        "cmp		x6, #0x306               \r\n"
        "bne		reg4_error_loop          \r\n"
        "cmp		x7, #0x307               \r\n"
        "bne		reg4_error_loop          \r\n"
        "cmp		x8, #0x308               \r\n"
        "bne		reg4_error_loop          \r\n"
        "cmp		x9, #0x309               \r\n"
        "bne		reg4_error_loop          \r\n"
        "cmp		x10, #0x310              \r\n"
        "bne		reg4_error_loop          \r\n"
        "cmp		x11, #0x311              \r\n"
        "bne		reg4_error_loop          \r\n"
        "cmp		x12, #0x312              \r\n"
        "bne		reg4_error_loop          \r\n"
        "cmp		x13, #0x313              \r\n"
        "bne		reg4_error_loop          \r\n"
        "cmp		x14, #0x314              \r\n"
        "bne		reg4_error_loop          \r\n"
        "cmp		x15, #0x315              \r\n"
        "bne		reg4_error_loop          \r\n"
        "cmp		x16, #0x316              \r\n"
        "bne		reg4_error_loop          \r\n"
        "cmp		x17, #0x317              \r\n"
        "bne		reg4_error_loop          \r\n"
        "cmp		x18, #0x318              \r\n"
        "bne		reg4_error_loop          \r\n"
        "cmp		x19, #0x319              \r\n"
        "bne		reg4_error_loop          \r\n"
        "cmp		x20, #0x320              \r\n"
        "bne		reg4_error_loop          \r\n"
        "cmp		x21, #0x321              \r\n"
        "bne		reg4_error_loop          \r\n"
        "cmp		x22, #0x322              \r\n"
        "bne		reg4_error_loop          \r\n"
        "cmp		x23, #0x323              \r\n"
        "bne		reg4_error_loop          \r\n"
        "cmp		x24, #0x324              \r\n"
        "bne		reg4_error_loop          \r\n"
        "cmp		x25, #0x325              \r\n"
        "bne		reg4_error_loop          \r\n"
        "cmp		x26, #0x326              \r\n"
        "bne		reg4_error_loop          \r\n"
        "cmp		x27, #0x327              \r\n"
        "bne		reg4_error_loop          \r\n"
        "cmp		x28, #0x328              \r\n"
        "bne		reg4_error_loop          \r\n"
        "cmp		x29, #0x329              \r\n"
        "bne		reg4_error_loop          \r\n"
        "cmp		x30, #0x330              \r\n"
        "bne		reg4_error_loop          \r\n"

        /* Check every floating point register to ensure it contains the expected
        value.  First save the registers clobbered by the test. */
        "stp 	x0, x1, [sp,#-0x10]!    \r\n"

        "fmov	    x0, v0.d[1]         \n\t"
        "cmp		x0, 0x3ff           \n\t"
        "bne		reg4_error_loop     \n\t"
        "fmov	    x0, d1              \n\t"
        "cmp		x0, 0x301           \n\t"
        "bne		reg4_error_loop     \n\t"
        "fmov	    x0, d2              \n\t"
        "cmp		x0, 0x302           \n\t"
        "bne		reg4_error_loop     \n\t"
        "fmov	    x0, d3              \n\t"
        "cmp		x0, 0x303           \n\t"
        "bne		reg4_error_loop     \n\t"
        "fmov	    x0, d4              \n\t"
        "cmp		x0, 0x304           \n\t"
        "bne		reg4_error_loop     \n\t"
        "fmov	    x0, d5              \n\t"
        "cmp		x0, 0x305           \n\t"
        "bne		reg4_error_loop     \n\t"
        "fmov	    x0, d6              \n\t"
        "cmp		x0, 0x306           \n\t"
        "bne		reg4_error_loop     \n\t"
        "fmov	    x0, d7              \n\t"
        "cmp		x0, 0x307           \n\t"
        "bne		reg4_error_loop     \n\t"
        "fmov	    x0, d8              \n\t"
        "cmp		x0, 0x308           \n\t"
        "bne		reg4_error_loop     \n\t"
        "fmov	    x0, d9              \n\t"
        "cmp		x0, 0x309           \n\t"
        "bne		reg4_error_loop     \n\t"
        "fmov	    x0, d10             \n\t"
        "cmp		x0, 0x310           \n\t"
        "bne		reg4_error_loop     \n\t"
        "fmov	    x0, d11             \n\t"
        "cmp		x0, 0x311           \n\t"
        "bne		reg4_error_loop     \n\t"
        "fmov	    x0, d12             \n\t"
        "cmp		x0, 0x312           \n\t"
        "bne		reg4_error_loop     \n\t"
        "fmov	    x0, d13             \n\t"
        "cmp		x0, 0x313           \n\t"
        "bne		reg4_error_loop     \n\t"
        "fmov	    x0, d14             \n\t"
        "cmp		x0, 0x314           \n\t"
        "bne		reg4_error_loop     \n\t"
        "fmov	    x0, d15             \n\t"
        "cmp		x0, 0x315           \n\t"
        "bne		reg4_error_loop     \n\t"
        "fmov	    x0, d16             \n\t"
        "cmp		x0, 0x316           \n\t"
        "bne		reg4_error_loop     \n\t"
        "fmov	    x0, d17             \n\t"
        "cmp		x0, 0x317           \n\t"
        "bne		reg4_error_loop     \n\t"
        "fmov	    x0, d18             \n\t"
        "cmp		x0, 0x318           \n\t"
        "bne		reg4_error_loop     \n\t"
        "fmov	    x0, d19             \n\t"
        "cmp		x0, 0x319           \n\t"
        "bne		reg4_error_loop     \n\t"
        "fmov	    x0, d20             \n\t"
        "cmp		x0, 0x320           \n\t"
        "bne		reg4_error_loop     \n\t"
        "fmov	    x0, d21             \n\t"
        "cmp		x0, 0x321           \n\t"
        "bne		reg4_error_loop     \n\t"
        "fmov	    x0, d22             \n\t"
        "cmp		x0, 0x322           \n\t"
        "bne		reg4_error_loop     \n\t"
        "fmov	    x0, d23             \n\t"
        "cmp		x0, 0x323           \n\t"
        "bne		reg4_error_loop     \n\t"
        "fmov	    x0, d24             \n\t"
        "cmp		x0, 0x324           \n\t"
        "bne		reg4_error_loop     \n\t"
        "fmov	    x0, d25             \n\t"
        "cmp		x0, 0x325           \n\t"
        "bne		reg4_error_loop     \n\t"
        "fmov	    x0, d26             \n\t"
        "cmp		x0, 0x326           \n\t"
        "bne		reg4_error_loop     \n\t"
        "fmov	    x0, d27             \n\t"
        "cmp		x0, 0x327           \n\t"
        "bne		reg4_error_loop     \n\t"
        "fmov	    x0, d28             \n\t"
        "cmp		x0, 0x328           \n\t"
        "bne		reg4_error_loop     \n\t"
        "fmov	    x0, d29             \n\t"
        "cmp		x0, 0x329           \n\t"
        "bne		reg4_error_loop     \n\t"
        "fmov	    x0, v30.d[1]        \n\t"
        "cmp		x0, 0x330           \n\t"
        "bne		reg4_error_loop     \n\t"
        "fmov	    x0, d31             \n\t"
        "cmp		x0, 0x00            \n\t"
        "bne		reg4_error_loop     \n\t"

        /* Restore the registers clobbered by the test. */
        "ldp 	x0, x1, [sp], #0x10     \n\t"

        /* Everything passed, increment the loop counter. */
        "stp 	x0, x1, [sp,#-0x10]!            \n\t"
        "ldr	x0, =ulRegisterTest4Counter     \n\t"
        "ldr 	x1, [x0]                        \n\t"
        "adds 	x1, x1, #1                      \n\t"
        "str 	x1, [x0]                        \n\t"
        "ldp 	x0, x1, [sp], #0x10             \n\t"

        /* Start again. */
        "b reg4_loop                            \n\t"

    "reg4_error_loop:                           \n\t"
        /* If this line is hit then there was an error in a core register value.
        The loop ensures the loop counter stops incrementing. */
        "b reg4_error_loop                      \n\t"
        "nop                                    \n\t"
        );
	}
}
/*-----------------------------------------------------------*/

void vStartRegisterTasks( UBaseType_t uxPriority )
{
	BaseType_t ret;
    TaskHandle_t xHandle1, xHandle2, xHandle3, xHandle4;

	ret = xTaskCreate( prvRegisterTest1Task, "RegTest1", configMINIMAL_STACK_SIZE, NULL, uxPriority, &xHandle1 );
    vTaskCoreAffinitySet( xHandle1,  1);
	configASSERT( ret == pdPASS );

	ret = xTaskCreate( prvRegisterTest2Task, "RegTest2", configMINIMAL_STACK_SIZE, NULL, uxPriority, &xHandle2 );
    vTaskCoreAffinitySet( xHandle2,  1);
	configASSERT( ret == pdPASS );

	ret = xTaskCreate( prvRegisterTest3Task, "RegTest3", configMINIMAL_STACK_SIZE, NULL, uxPriority, &xHandle3 );
    vTaskCoreAffinitySet( xHandle3,  1);
	configASSERT( ret == pdPASS );

	ret = xTaskCreate( prvRegisterTest4Task, "RegTest4", configMINIMAL_STACK_SIZE, NULL, uxPriority, &xHandle4 );
    vTaskCoreAffinitySet( xHandle4,  1);
	configASSERT( ret == pdPASS );
}
/*-----------------------------------------------------------*/

BaseType_t xAreRegisterTasksStillRunning( void )
{
static uint32_t ulLastRegisterTest1Counter = 0, ulLastRegisterTest2Counter = 0;
static uint32_t ulLastRegisterTest3Counter = 0, ulLastRegisterTest4Counter = 0;

	/* If the register test task is still running then we expect the loop
	 * counters to have incremented since this function was last called. */
	if( ulLastRegisterTest1Counter == ulRegisterTest1Counter )
	{
		xErrorDetected = pdTRUE;
	}

	if( ulLastRegisterTest2Counter == ulRegisterTest2Counter )
	{
		xErrorDetected = pdTRUE;
	}

	if( ulLastRegisterTest3Counter == ulRegisterTest3Counter )
	{
		xErrorDetected = pdTRUE;
	}

	if( ulLastRegisterTest4Counter == ulRegisterTest4Counter )
	{
		xErrorDetected = pdTRUE;
	}

	ulLastRegisterTest1Counter = ulRegisterTest1Counter;
	ulLastRegisterTest2Counter = ulRegisterTest2Counter;
	ulLastRegisterTest3Counter = ulRegisterTest3Counter;
	ulLastRegisterTest4Counter = ulRegisterTest4Counter;

	/* Errors detected in the task itself will have latched xErrorDetected
	 * to true. */
	return ( BaseType_t ) !xErrorDetected;
}
/*-----------------------------------------------------------*/
