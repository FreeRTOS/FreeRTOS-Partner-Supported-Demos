/******************************************************************************
*
* Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
* Copyright (C) 2014 - 2020 Xilinx, Inc. All rights reserved.
* Copyright 2025 Arm Limited and/or its affiliates <open-source-office@arm.com>
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
* http://www.FreeRTOS.org
* http://aws.amazon.com/freertos
*
* 1 tab == 4 spaces!
*
******************************************************************************/

.org 0
.text

/* Entry point symbol for startup; referenced by the linker */
.globl _boot

/* Base of the EL1 exception vector table (vbases for EL1t/EL1h) */
.globl _vector_table

/* Alternate vector table used once the scheduler is running */
.globl _freertos_vector_table

.org 0

/* Map this section to read-only, executable memory as the EL1 vector base */
.section .vectors, "ax"

_vector_table:

.set    VBAR, _vector_table

.org VBAR
    b    _boot

.org (VBAR + 0x80)
    b    .

.org (VBAR + 0x100)
    b    .

.org (VBAR + 0x180)
    b    .


.org (VBAR + 0x200)
    b    .

.org (VBAR + 0x280)
    b    .

.org (VBAR + 0x300)
    b    .

.org (VBAR + 0x380)
    b    .

.org (VBAR + 0x400)
    b    .

.org (VBAR + 0x480)
    b    .

.org (VBAR + 0x500)
    b    .

.org (VBAR + 0x580)
    b    .

.org (VBAR + 0x600)
    b    .

.org (VBAR + 0x680)
    b    .

.org (VBAR + 0x700)
    b    .

.org (VBAR + 0x780)
    b    .



/******************************************************************************
 * Vector table to use when FreeRTOS is running.
 *****************************************************************************/
/* Reserve 0x1000 bytes so we can switch VBAR at runtime without overlap */
.set    FREERTOS_VBAR, (VBAR+0x1000)

.org(FREERTOS_VBAR)

/* Map this section to read-only, executable memory as the EL1 vector base */
.section .vectors, "ax"

_freertos_vector_table:
    b    FreeRTOS_SWI_Handler

.org (FREERTOS_VBAR + 0x80)
    b    FreeRTOS_IRQ_Handler

.org (FREERTOS_VBAR + 0x100)
    b    FreeRTOS_IRQ_Handler

.org (FREERTOS_VBAR + 0x180)
    b    .

.org (FREERTOS_VBAR + 0x200)
    b    FreeRTOS_SWI_Handler

.org (FREERTOS_VBAR + 0x280)
    b    FreeRTOS_IRQ_Handler

.org (FREERTOS_VBAR + 0x300)
    b    FreeRTOS_IRQ_Handler

.org (FREERTOS_VBAR + 0x380)
    b    .

.org (FREERTOS_VBAR + 0x400)
    b    App_Fault_Handler

.org (FREERTOS_VBAR + 0x480)
    b    FreeRTOS_IRQ_Handler

.org (FREERTOS_VBAR + 0x500)
    b    .

.org (FREERTOS_VBAR + 0x580)
    b    .

.org (FREERTOS_VBAR + 0x600)
    b    .

.org (FREERTOS_VBAR + 0x680)
    b    FreeRTOS_IRQ_Handler

.org (FREERTOS_VBAR + 0x700)
    b    .

.org (FREERTOS_VBAR + 0x780)
    b    .

.org (FREERTOS_VBAR + 0x800)

.end
