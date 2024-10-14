/*******************************************************************************
// Copyright (c) 2003-2024 Cadence Design Systems, Inc.
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
--------------------------------------------------------------------------------
*/

/*
*********************************************************************************************************
*
*                                            ALLOCA TEST
*
* This test is designed to trigger an alloca exception on Xtensa and see if it properly moved
* the base save area. Applicable to Windowed ABI only (else will always pass).
* It does not rely on a C library so can run on practically anything.
*
* Target  : All Xtensa configurable and Diamond preconfigured processors.
*********************************************************************************************************
*/

#include    <ctype.h>
#include    <string.h>
#include    <unistd.h>
#include    <alloca.h>
#include    <assert.h>
#include    <stdio.h>

#ifdef XT_BOARD
#include    <xtensa/xtbsp.h>
#endif

#include "testcommon.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/*
*********************************************************************************************************
*                                         SHARED DATA AND MACROS
*********************************************************************************************************
*/

/* Uniform prefix for reporting PASS/FAIL test results. */
#define TEST_PFX    "Xtensa alloca exception test (xt_alloca)"

/* Some stuff to copy to the alloca region. */
static const unsigned STUFF[16] = {
    0x11111111, 0x22222222, 0x33333333, 0x44444444,
    0x55555555, 0x66666666, 0x77777777, 0x88888888,
    0x99999999, 0xaaaaaaaa, 0Xbbbbbbbb, 0xcccccccc,
    0xdddddddd, 0xeeeeeeee, 0Xffffffff, 0x00000000,
    };

/*
*********************************************************************************************************
*                                             LOCAL FUNCTIONS
*********************************************************************************************************
*/

/* Output a simple string to the console. */
static void putstr(const char *s)
{
    char c;

    while ((c = *s) != '\0') {
        if (c == '\n') {
            outbyte('\r');
            outbyte('\n');
        }
        else if (iscntrl((int)c) && c != '\r') {
            outbyte('^');
            outbyte('@' + c);
        }
        else outbyte(c);
        ++s;
    }
}

/* Spill the register windows so alloca triggers the exception. */
static void spill(void)
{
    xthal_window_spill();
}

/* Call several levels deep, writing to the stack. */
static int factorial(int x)
{
    return x == 0 ? 1 : x * factorial(x-1);
}

/* 
Spill regs and do an alloca, copy stuff to the alloca region, call several levels deep, check if
stuff remains intact, spill any unspilled regs, then check again if the stuff remains intact.
Prevent in-lining so base-save area is that of the caller.  Task will likely crash if stuff 
overwrote its base-save area (XT-FIXME: figure out a way to make it fail nicely in that case).
Return bool success.
*/
static int test_alloca(void) __attribute__((noinline));
static int test_alloca(void)
{
    unsigned i;
    volatile unsigned *stuff;   /* hands off, optimizer! */
    volatile int f;             /* hands off, optimizer! */

    /* Spill regs to save areas and alloca space on stack. */
    spill();
    stuff = alloca(16*4);

    /* Copy stuff to the alloca region. */
    for (i=0; i<16; ++i)
        stuff[i] = STUFF[i];

    /* Overwrite stuff if alloca didn't reserve space. */
    f = factorial(4);

    /* Check if stuff remains intact. */
    for (i=0; i<16; ++i)
        if (stuff[i] != STUFF[i]) return 0;

    /* Spill any unspilled regs. */
    spill();

    /* Check if stuff remains intact. */
    for (i=0; i<16; ++i)
        if (stuff[i] != STUFF[i]) return 0;

    /* If we got this far, alloca must have worked. */
    return (f > 0) ? 1 : 0;
}

/*
*********************************************************************************************************
*                                          APP INITIALZATION TASK
*
*
*********************************************************************************************************
*/

/* Create task as privileged if MPU enabled. */
#define TASK_INIT_PRIO          (20 | portPRIVILEGE_BIT)
#define INIT_TASK_STK_SIZE      ((XT_STACK_MIN_SIZE + 0x800) / sizeof(StackType_t))

static void Init_Task(void *pdata)
{
    int ok;

    UNUSED(pdata);

    /* Call a function that does an alloca over my base save area. */
    ok = test_alloca();
    #ifdef XT_BOARD
    if (ok)
        xtbsp_display_string("xt_alloca PASSED");
    else
        xtbsp_display_string("xt_alloca FAILED");
    #endif
    if (ok)
        putstr(TEST_PFX " PASSED!\n");
    else
        putstr(TEST_PFX " FAILED!\n");

    #ifdef XT_SIMULATOR
    /* Shut down simulator and report error code as exit code to host (0 = OK). */
    _exit(!ok);
    #endif

    /* Terminate this task. OS will continue to run timer, stats and idle tasks. */
    vTaskDelete(NULL);
    // Should never come here.
    assert(0);
}

/*
*********************************************************************************************************
*                                             C ENTRY POINT
*
* Initializes FreeRTOS after the platorm's run-time system has initialized the basic platform.
* Creates at least the first task, which can then create other tasks.
* Starts multitasking.
*
*********************************************************************************************************
*/

/* Hook functions for standalone tests */
#ifdef STANDALONE

#if configUSE_TICK_HOOK
void vApplicationTickHook( void )
{
}
#endif

void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
    /* For some reason printing pcTaskName is not working */
    UNUSED(xTask);
    UNUSED(pcTaskName);
    puts("\nStack overflow, stopping.");
    exit(0);
}

int main(void)
#else
int main_xt_alloca(int argc, char *argv[])
#endif
{
    int err = 0;
    int exit_code = 0;

    #ifdef XT_BOARD
    xtbsp_display_string("xt_alloca test");
    #endif
    putstr(TEST_PFX " running...\n");

    /* Create the control task initially with the high priority. */
    err = xTaskCreate(Init_Task, "Init_Task", INIT_TASK_STK_SIZE, NULL, TASK_INIT_PRIO, NULL);
    if (err != pdPASS)
    {
        putstr(TEST_PFX " FAILED to create Init_Task\n");
        goto done;
    }

    /* Start task scheduler */
    vTaskStartScheduler();

done:
    exit_code = err;

    #ifdef XT_SIMULATOR
    /* Shut down simulator and report error code as exit code to host (0 = OK). */
    _exit(exit_code);
    #endif

    /* Does not reach here ('return' statement keeps compiler happy). */
    return 0;
}

