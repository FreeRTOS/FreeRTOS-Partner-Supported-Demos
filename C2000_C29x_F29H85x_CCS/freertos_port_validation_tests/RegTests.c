/*
 * FreeRTOS V202411.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates. All Rights Reserved.
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
 * https://github.com/FreeRTOS
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

/* Flag that will be latched to pdTRUE should any unexpected behaviour be
detected in any of the tasks. */
static volatile BaseType_t xErrorDetected = pdFALSE;

/* Counters that are incremented on each cycle of a test.  This is used to
detect a stalled task - a test that is no longer running. */
static volatile uint32_t ulRegisterTest1Counter = 0;
static volatile uint32_t ulRegisterTest2Counter = 0;

/*-----------------------------------------------------------*/

static void prvRegisterTest1Task( void *pvParameters )
{
    ( void ) pvParameters;

    /* 1. Fill the registers stored as part of task context with known values.*/
    __asm("reg1_loop:                       \n"                       \
          "    MV A0,  #0xFFFF              \n"                       \
          "    MV A1,  #0x10101010          \n"                       \
          "    MV A2,  #0x20202020          \n"                       \
          "    MV A3,  #0x30303030          \n"                       \
          "    MV A4,  #0x40404040          \n"                       \
          "    MV A5,  #0x50505050          \n"                       \
          "    MV A6,  #0x60606060          \n"                       \
          "    MV A7,  #0x70707070          \n"                       \
          "    MV A8,  #0x80808080          \n"                       \
          "    MV A9,  #0x90909090          \n"                       \
          "    MV A10, #0xA0A0A0A0          \n"                       \
          "    MV A11, #0xB0B0B0B0          \n"                       \
          "    MV A12, #0xC0C0C0C0          \n"                       \
          "    MV A13, #0xD0D0D0D0          \n"                       \
          "    MV A14, #0xE0E0E0E0          \n"                       \
          "    MV D0,  #0xFFFF              \n"                       \
          "    MV D1,  #0x10101010          \n"                       \
          "    MV D2,  #0x20202020          \n"                       \
          "    MV D3,  #0x30303030          \n"                       \
          "    MV D4,  #0x40404040          \n"                       \
          "    MV D5,  #0x50505050          \n"                       \
          "    MV D6,  #0x60606060          \n"                       \
          "    MV D7,  #0x70707070          \n"                       \
          "    MV D8,  #0x80808080          \n"                       \
          "    MV D9,  #0x90909090          \n"                       \
          "    MV D10, #0xA0A0A0A0          \n"                       \
          "    MV D11, #0xB0B0B0B0          \n"                       \
          "    MV D12, #0xC0C0C0C0          \n"                       \
          "    MV D13, #0xD0D0D0D0          \n"                       \
          "    MV D14, #0xE0E0E0E0          \n"                       \
          "    MV D15, #0xF0F0F0F0          \n"                       \
          "    MV M0,  #0xFFFF0000          \n"                       \
          "    MV M1,  #0x10101010          \n"                       \
          "    MV M2,  #0x20202020          \n"                       \
          "    MV M3,  #0x30303030          \n"                       \
          "    MV M4,  #0x40404040          \n"                       \
          "    MV M5,  #0x50505050          \n"                       \
          "    MV M6,  #0x60606060          \n"                       \
          "    MV M7,  #0x70707070          \n"                       \
          "    MV M8,  #0x80808080          \n"                       \
          "    MV M9,  #0x90909090          \n"                       \
          "    MV M10, #0xA0A0A0A0          \n"                       \
          "    MV M11, #0xB0B0B0B0          \n"                       \
          "    MV M12, #0xC0C0C0C0          \n"                       \
          "    MV M13, #0xD0D0D0D0          \n"                       \
          "    MV M14, #0xE0E0E0E0          \n"                       \
          "    MV M15, #0xF0F0F0F0          \n"                       \
          "    MV M16, #0xFFFFFFFF          \n"                       \
          "    MV M17, #0x01010101          \n"                       \
          "    MV M18, #0x02020202          \n"                       \
          "    MV M19, #0x03030303          \n"                       \
          "    MV M20, #0x04040404          \n"                       \
          "    MV M21, #0x05050505          \n"                       \
          "    MV M22, #0x06060606          \n"                       \
          "    MV M23, #0x07070707          \n"                       \
          "    MV M24, #0x08080808          \n"                       \
          "    MV M25, #0x09090909          \n"                       \
          "    MV M26, #0x0A0A0A0A          \n"                       \
          "    MV M27, #0x0B0B0B0B          \n"                       \
          "    MV M28, #0x0C0C0C0C          \n"                       \
          "    MV M29, #0x0D0D0D0D          \n"                       \
          "    MV M30, #0x0E0E0E0E          \n"                       \
          "    MV M31, #0x0F0F0F0F          \n"                       \
         );

    /* 2. Force a context switch (Force SW Interrupt) */
    asm("   MV	  A4,#0x300223FC    \n" \
        "   ST.8  *(ADDR1)A4, #0x1  \n" \
        "   NOP #4   \n" \
        "   NOP #4   \n" \
        "   NOP #4   \n" \
        "   NOP #4   \n" \
       );

    /* 3. Verify that all the registers contain expected values.*/
    /* 4. If all the register contain expected values, increment ulRegisterTest1Counter.*/
    __asm("    CMP.U16 A0, #0xFFFF  \n                        BC  reg1_error_loop, A.NEQ  \n"  \
          "    MV  A0, #0x10101010  \n    CMP A1,  A0  \n     BC  reg1_error_loop, A.NEQ  \n"  \
          "    MV  A0, #0x20202020  \n    CMP A2,  A0  \n     BC  reg1_error_loop, A.NEQ  \n"  \
          "    MV  A0, #0x30303030  \n    CMP A3,  A0  \n     BC  reg1_error_loop, A.NEQ  \n"  \
          "    MV  A0, #0x300223FC  \n    CMP A4,  A0  \n     BC  reg1_error_loop, A.NEQ  \n"  \
          "    MV  A0, #0x50505050  \n    CMP A5,  A0  \n     BC  reg1_error_loop, A.NEQ  \n"  \
          "    MV  A0, #0x60606060  \n    CMP A6,  A0  \n     BC  reg1_error_loop, A.NEQ  \n"  \
          "    MV  A0, #0x70707070  \n    CMP A7,  A0  \n     BC  reg1_error_loop, A.NEQ  \n"  \
          "    MV  A0, #0x80808080  \n    CMP A8,  A0  \n     BC  reg1_error_loop, A.NEQ  \n"  \
          "    MV  A0, #0x90909090  \n    CMP A9,  A0  \n     BC  reg1_error_loop, A.NEQ  \n"  \
          "    MV  A0, #0xA0A0A0A0  \n    CMP A10, A0  \n     BC  reg1_error_loop, A.NEQ  \n"  \
          "    MV  A0, #0xB0B0B0B0  \n    CMP A11, A0  \n     BC  reg1_error_loop, A.NEQ  \n"  \
          "    MV  A0, #0xC0C0C0C0  \n    CMP A12, A0  \n     BC  reg1_error_loop, A.NEQ  \n"  \
          "    MV  A0, #0xD0D0D0D0  \n    CMP A13, A0  \n     BC  reg1_error_loop, A.NEQ  \n"  \
          "    MV  A0, #0xE0E0E0E0  \n    CMP A14, A0  \n     BC  reg1_error_loop, A.NEQ  \n"  \
          "    CMP.U16 D0, #0xFFFF  \n                 \n     BC  reg1_error_loop, D.NEQ  \n"  \
          "    MV  D0, #0x10101010  \n    CMP D1,  D0  \n     BC  reg1_error_loop, D.NEQ  \n"  \
          "    MV  D0, #0x20202020  \n    CMP D2,  D0  \n     BC  reg1_error_loop, D.NEQ  \n"  \
          "    MV  D0, #0x30303030  \n    CMP D3,  D0  \n     BC  reg1_error_loop, D.NEQ  \n"  \
          "    MV  D0, #0x40404040  \n    CMP D4,  D0  \n     BC  reg1_error_loop, D.NEQ  \n"  \
          "    MV  D0, #0x50505050  \n    CMP D5,  D0  \n     BC  reg1_error_loop, D.NEQ  \n"  \
          "    MV  D0, #0x60606060  \n    CMP D6,  D0  \n     BC  reg1_error_loop, D.NEQ  \n"  \
          "    MV  D0, #0x70707070  \n    CMP D7,  D0  \n     BC  reg1_error_loop, D.NEQ  \n"  \
          "    MV  D0, #0x80808080  \n    CMP D8,  D0  \n     BC  reg1_error_loop, D.NEQ  \n"  \
          "    MV  D0, #0x90909090  \n    CMP D9,  D0  \n     BC  reg1_error_loop, D.NEQ  \n"  \
          "    MV  D0, #0xA0A0A0A0  \n    CMP D10, D0  \n     BC  reg1_error_loop, D.NEQ  \n"  \
          "    MV  D0, #0xB0B0B0B0  \n    CMP D11, D0  \n     BC  reg1_error_loop, D.NEQ  \n"  \
          "    MV  D0, #0xC0C0C0C0  \n    CMP D12, D0  \n     BC  reg1_error_loop, D.NEQ  \n"  \
          "    MV  D0, #0xD0D0D0D0  \n    CMP D13, D0  \n     BC  reg1_error_loop, D.NEQ  \n"  \
          "    MV  D0, #0xE0E0E0E0  \n    CMP D14, D0  \n     BC  reg1_error_loop, D.NEQ  \n"  \
          "    MV  D0, #0xF0F0F0F0  \n    CMP D15, D0  \n     BC  reg1_error_loop, D.NEQ  \n"  \
          "    CMPF.F16 M0, #0xFFFF\n                         BC  reg1_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x10101010 \n     CMPF M1,  M0 \n     BC  reg1_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x20202020 \n     CMPF M2,  M0 \n     BC  reg1_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x30303030 \n     CMPF M3,  M0 \n     BC  reg1_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x40404040 \n     CMPF M4,  M0 \n     BC  reg1_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x50505050 \n     CMPF M5,  M0 \n     BC  reg1_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x60606060 \n     CMPF M6,  M0 \n     BC  reg1_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x70707070 \n     CMPF M7,  M0 \n     BC  reg1_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x80808080 \n     CMPF M8,  M0 \n     BC  reg1_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x90909090 \n     CMPF M9,  M0 \n     BC  reg1_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0xA0A0A0A0 \n     CMPF M10, M0 \n     BC  reg1_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0xB0B0B0B0 \n     CMPF M11, M0 \n     BC  reg1_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0xC0C0C0C0 \n     CMPF M12, M0 \n     BC  reg1_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0xD0D0D0D0 \n     CMPF M13, M0 \n     BC  reg1_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0xE0E0E0E0 \n     CMPF M14, M0 \n     BC  reg1_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0xF0F0F0F0 \n     CMPF M15, M0 \n     BC  reg1_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0xFFFFFFFF \n     CMPF M16, M0 \n     BC  reg1_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x01010101 \n     CMPF M17, M0 \n     BC  reg1_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x02020202 \n     CMPF M18, M0 \n     BC  reg1_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x03030303 \n     CMPF M19, M0 \n     BC  reg1_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x04040404 \n     CMPF M20, M0 \n     BC  reg1_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x05050505 \n     CMPF M21, M0 \n     BC  reg1_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x06060606 \n     CMPF M22, M0 \n     BC  reg1_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x07070707 \n     CMPF M23, M0 \n     BC  reg1_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x08080808 \n     CMPF M24, M0 \n     BC  reg1_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x09090909 \n     CMPF M25, M0 \n     BC  reg1_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x0A0A0A0A \n     CMPF M26, M0 \n     BC  reg1_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x0B0B0B0B \n     CMPF M27, M0 \n     BC  reg1_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x0C0C0C0C \n     CMPF M28, M0 \n     BC  reg1_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x0D0D0D0D \n     CMPF M29, M0 \n     BC  reg1_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x0E0E0E0E \n     CMPF M30, M0 \n     BC  reg1_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x0F0F0F0F \n     CMPF M31, M0 \n     BC  reg1_error_loop, M.NEQ  \n"  \
          "    LD.32 D0,@ulRegisterTest1Counter     \n"     \
          "    ADD.U16	D0,D0,#0x1                  \n"     \
          "    ST.32	@ulRegisterTest1Counter,D0  \n"     \
          "    B   reg1_loop, UNC                   \n"     \
          "reg1_error_loop:                         \n"     \
          "    B   reg1_error_loop, UNC             \n"     \
         );
}
/*-----------------------------------------------------------*/
static void prvRegisterTest2Task( void *pvParameters )
{
    ( void ) pvParameters;

    /* 1. Fill the registers stored as part of task context with known values.*/
    __asm("reg2_loop:                       \n"                       \
          "    MV A0,  #0xAAAA              \n"                       \
          "    MV A1,  #0x11111111          \n"                       \
          "    MV A2,  #0x22222222          \n"                       \
          "    MV A3,  #0x33333333          \n"                       \
          "    MV A4,  #0x44444444          \n"                       \
          "    MV A5,  #0x55555555          \n"                       \
          "    MV A6,  #0x66666666          \n"                       \
          "    MV A7,  #0x77777777          \n"                       \
          "    MV A8,  #0x88888888          \n"                       \
          "    MV A9,  #0x99999999          \n"                       \
          "    MV A10, #0xAAAAAAAA          \n"                       \
          "    MV A11, #0xBBBBBBBB          \n"                       \
          "    MV A12, #0xCCCCCCCC          \n"                       \
          "    MV A13, #0xDDDDDDDD          \n"                       \
          "    MV A14, #0xEEEEEEEE          \n"                       \
          "    MV D0,  #0xAAAA              \n"                       \
          "    MV D1,  #0x11111111          \n"                       \
          "    MV D2,  #0x22222222          \n"                       \
          "    MV D3,  #0x33333333          \n"                       \
          "    MV D4,  #0x44444444          \n"                       \
          "    MV D5,  #0x55555555          \n"                       \
          "    MV D6,  #0x66666666          \n"                       \
          "    MV D7,  #0x77777777          \n"                       \
          "    MV D8,  #0x88888888          \n"                       \
          "    MV D9,  #0x99999999          \n"                       \
          "    MV D10, #0xAAAAAAAA          \n"                       \
          "    MV D11, #0xBBBBBBBB          \n"                       \
          "    MV D12, #0xCCCCCCCC          \n"                       \
          "    MV D13, #0xDDDDDDDD          \n"                       \
          "    MV D14, #0xEEEEEEEE          \n"                       \
          "    MV D15, #0xFFFFFFFF          \n"                       \
          "    MV M0,  #0xAAAA0000          \n"                       \
          "    MV M1,  #0x11111111          \n"                       \
          "    MV M2,  #0x22222222          \n"                       \
          "    MV M3,  #0x33333333          \n"                       \
          "    MV M4,  #0x44444444          \n"                       \
          "    MV M5,  #0x55555555          \n"                       \
          "    MV M6,  #0x66666666          \n"                       \
          "    MV M7,  #0x77777777          \n"                       \
          "    MV M8,  #0x88888888          \n"                       \
          "    MV M9,  #0x99999999          \n"                       \
          "    MV M10, #0xAAAAAAAA          \n"                       \
          "    MV M11, #0xBBBBBBBB          \n"                       \
          "    MV M12, #0xCCCCCCCC          \n"                       \
          "    MV M13, #0xDDDDDDDD          \n"                       \
          "    MV M14, #0xEEEEEEEE          \n"                       \
          "    MV M15, #0xFFFFFFFF          \n"                       \
          "    MV M16, #0xFFFFFFFF          \n"                       \
          "    MV M17, #0x11111111          \n"                       \
          "    MV M18, #0x22222222          \n"                       \
          "    MV M19, #0x33333333          \n"                       \
          "    MV M20, #0x44444444          \n"                       \
          "    MV M21, #0x55555555          \n"                       \
          "    MV M22, #0x66666666          \n"                       \
          "    MV M23, #0x77777777          \n"                       \
          "    MV M24, #0x88888888          \n"                       \
          "    MV M25, #0x99999999          \n"                       \
          "    MV M26, #0xAAAAAAAA          \n"                       \
          "    MV M27, #0xBBBBBBBB          \n"                       \
          "    MV M28, #0xCCCCCCCC          \n"                       \
          "    MV M29, #0xDDDDDDDD          \n"                       \
          "    MV M30, #0xEEEEEEEE          \n"                       \
          "    MV M31, #0xFFFFFFFF          \n"                       \
         );

    /* 2. Force a context switch (Force SW Interrupt) */
    asm("   MV	  A4,#0x300223FC     \n" \
        "   ST.8  *(ADDR1)A4, #0x1   \n" \
        "   NOP #4   \n" \
        "   NOP #4   \n" \
        "   NOP #4   \n" \
        "   NOP #4   \n" \
       );

    /* 3. Verify that all the registers contain expected values.*/
    /* 4. If all the register contain expected values, increment ulRegisterTest1Counter.*/
    __asm("    CMP.U16 A0, #0xAAAA  \n                        BC  reg2_error_loop, A.NEQ  \n"  \
          "    MV  A0, #0x11111111  \n    CMP A1,  A0  \n     BC  reg2_error_loop, A.NEQ  \n"  \
          "    MV  A0, #0x22222222  \n    CMP A2,  A0  \n     BC  reg2_error_loop, A.NEQ  \n"  \
          "    MV  A0, #0x33333333  \n    CMP A3,  A0  \n     BC  reg2_error_loop, A.NEQ  \n"  \
          "    MV  A0, #0x300223FC  \n    CMP A4,  A0  \n     BC  reg2_error_loop, A.NEQ  \n"  \
          "    MV  A0, #0x55555555  \n    CMP A5,  A0  \n     BC  reg2_error_loop, A.NEQ  \n"  \
          "    MV  A0, #0x66666666  \n    CMP A6,  A0  \n     BC  reg2_error_loop, A.NEQ  \n"  \
          "    MV  A0, #0x77777777  \n    CMP A7,  A0  \n     BC  reg2_error_loop, A.NEQ  \n"  \
          "    MV  A0, #0x88888888  \n    CMP A8,  A0  \n     BC  reg2_error_loop, A.NEQ  \n"  \
          "    MV  A0, #0x99999999  \n    CMP A9,  A0  \n     BC  reg2_error_loop, A.NEQ  \n"  \
          "    MV  A0, #0xAAAAAAAA  \n    CMP A10, A0  \n     BC  reg2_error_loop, A.NEQ  \n"  \
          "    MV  A0, #0xBBBBBBBB  \n    CMP A11, A0  \n     BC  reg2_error_loop, A.NEQ  \n"  \
          "    MV  A0, #0xCCCCCCCC  \n    CMP A12, A0  \n     BC  reg2_error_loop, A.NEQ  \n"  \
          "    MV  A0, #0xDDDDDDDD  \n    CMP A13, A0  \n     BC  reg2_error_loop, A.NEQ  \n"  \
          "    MV  A0, #0xEEEEEEEE  \n    CMP A14, A0  \n     BC  reg2_error_loop, A.NEQ  \n"  \
          "    CMP.U16 D0, #0xAAAA  \n                        BC  reg2_error_loop, D.NEQ  \n"  \
          "    MV  D0, #0x11111111  \n    CMP D1,  D0  \n     BC  reg2_error_loop, D.NEQ  \n"  \
          "    MV  D0, #0x22222222  \n    CMP D2,  D0  \n     BC  reg2_error_loop, D.NEQ  \n"  \
          "    MV  D0, #0x33333333  \n    CMP D3,  D0  \n     BC  reg2_error_loop, D.NEQ  \n"  \
          "    MV  D0, #0x44444444  \n    CMP D4,  D0  \n     BC  reg2_error_loop, D.NEQ  \n"  \
          "    MV  D0, #0x55555555  \n    CMP D5,  D0  \n     BC  reg2_error_loop, D.NEQ  \n"  \
          "    MV  D0, #0x66666666  \n    CMP D6,  D0  \n     BC  reg2_error_loop, D.NEQ  \n"  \
          "    MV  D0, #0x77777777  \n    CMP D7,  D0  \n     BC  reg2_error_loop, D.NEQ  \n"  \
          "    MV  D0, #0x88888888  \n    CMP D8,  D0  \n     BC  reg2_error_loop, D.NEQ  \n"  \
          "    MV  D0, #0x99999999  \n    CMP D9,  D0  \n     BC  reg2_error_loop, D.NEQ  \n"  \
          "    MV  D0, #0xAAAAAAAA  \n    CMP D10, D0  \n     BC  reg2_error_loop, D.NEQ  \n"  \
          "    MV  D0, #0xBBBBBBBB  \n    CMP D11, D0  \n     BC  reg2_error_loop, D.NEQ  \n"  \
          "    MV  D0, #0xCCCCCCCC  \n    CMP D12, D0  \n     BC  reg2_error_loop, D.NEQ  \n"  \
          "    MV  D0, #0xDDDDDDDD  \n    CMP D13, D0  \n     BC  reg2_error_loop, D.NEQ  \n"  \
          "    MV  D0, #0xEEEEEEEE  \n    CMP D14, D0  \n     BC  reg2_error_loop, D.NEQ  \n"  \
          "    MV  D0, #0xFFFFFFFF  \n    CMP D15, D0  \n     BC  reg2_error_loop, D.NEQ  \n"  \
          "    CMPF.F16 M0, #0xAAAA\n                         BC  reg2_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x11111111 \n     CMPF M1,  M0 \n     BC  reg2_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x22222222 \n     CMPF M2,  M0 \n     BC  reg2_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x33333333 \n     CMPF M3,  M0 \n     BC  reg2_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x44444444 \n     CMPF M4,  M0 \n     BC  reg2_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x55555555 \n     CMPF M5,  M0 \n     BC  reg2_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x66666666 \n     CMPF M6,  M0 \n     BC  reg2_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x77777777 \n     CMPF M7,  M0 \n     BC  reg2_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x88888888 \n     CMPF M8,  M0 \n     BC  reg2_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x99999999 \n     CMPF M9,  M0 \n     BC  reg2_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0xAAAAAAAA \n     CMPF M10, M0 \n     BC  reg2_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0xBBBBBBBB \n     CMPF M11, M0 \n     BC  reg2_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0xCCCCCCCC \n     CMPF M12, M0 \n     BC  reg2_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0xDDDDDDDD \n     CMPF M13, M0 \n     BC  reg2_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0xEEEEEEEE \n     CMPF M14, M0 \n     BC  reg2_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0xFFFFFFFF \n     CMPF M15, M0 \n     BC  reg2_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0xFFFFFFFF \n     CMPF M16, M0 \n     BC  reg2_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x11111111 \n     CMPF M17, M0 \n     BC  reg2_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x22222222 \n     CMPF M18, M0 \n     BC  reg2_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x33333333 \n     CMPF M19, M0 \n     BC  reg2_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x44444444 \n     CMPF M20, M0 \n     BC  reg2_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x55555555 \n     CMPF M21, M0 \n     BC  reg2_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x66666666 \n     CMPF M22, M0 \n     BC  reg2_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x77777777 \n     CMPF M23, M0 \n     BC  reg2_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x88888888 \n     CMPF M24, M0 \n     BC  reg2_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0x99999999 \n     CMPF M25, M0 \n     BC  reg2_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0xAAAAAAAA \n     CMPF M26, M0 \n     BC  reg2_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0xBBBBBBBB \n     CMPF M27, M0 \n     BC  reg2_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0xCCCCCCCC \n     CMPF M28, M0 \n     BC  reg2_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0xDDDDDDDD \n     CMPF M29, M0 \n     BC  reg2_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0xEEEEEEEE \n     CMPF M30, M0 \n     BC  reg2_error_loop, M.NEQ  \n"  \
          "    MV  M0, #0xFFFFFFFF \n     CMPF M31, M0 \n     BC  reg2_error_loop, M.NEQ  \n"  \
          "    LD.32 D0,@ulRegisterTest2Counter     \n"     \
          "    ADD.U16	D0,D0,#0x1                  \n"     \
          "    ST.32	@ulRegisterTest2Counter,D0  \n"     \
          "    B   reg2_loop, UNC                   \n"     \
          "reg2_error_loop:                         \n"     \
          "    B   reg2_error_loop, UNC             \n"     \
         );
}
/*-----------------------------------------------------------*/

void vStartRegisterTasks( UBaseType_t uxPriority )
{
	BaseType_t ret;

	ret = xTaskCreate( prvRegisterTest1Task, "RegTest1", configMINIMAL_STACK_SIZE, NULL, uxPriority, NULL );
	configASSERT( ret == pdPASS );

	ret = xTaskCreate( prvRegisterTest2Task, "RegTest2", configMINIMAL_STACK_SIZE, NULL, uxPriority, NULL );
	configASSERT( ret == pdPASS );

}
/*-----------------------------------------------------------*/

BaseType_t xAreRegisterTasksStillRunning( void )
{
static uint32_t ulLastRegisterTest1Counter = 0, ulLastRegisterTest2Counter = 0;

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


	ulLastRegisterTest1Counter = ulRegisterTest1Counter;
	ulLastRegisterTest2Counter = ulRegisterTest2Counter;

	/* Errors detected in the task itself will have latched xErrorDetected
	 * to true. */
	return ( BaseType_t ) !xErrorDetected;
}
/*-----------------------------------------------------------*/
