
//-----------------------------------------------------------------------------
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
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// This test checks that the C library per-thread context is being properly
// maintained by the OS. When a thread is running, the global C context must
// point to this thread's C library context. This test is only meaningful if
// XT_USE_THREAD_SAFE_CLIB is enabled. Thread support is only available with
// the newlib C library and the Xtensa C library (xclib).
//-----------------------------------------------------------------------------


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "asm-offsets.h"
#include "testcommon.h"

#if (XT_USE_THREAD_SAFE_CLIB > 0)
#include <sys/reent.h>
#else
#warning XT_USE_THREAD_SAFE_CLIB not defined, this test will do nothing.
#endif

#ifdef XT_BOARD
#include <xtensa/xtbsp.h>
#endif


// Task stack sizes. Can be reduced if printf() etc. are not called.
#define TASK_STK_SIZE           ((XT_STACK_MIN_SIZE + 0x800) / sizeof(StackType_t))

// Uniform prefix for reporting PASS/FAIL test results.
#define TEST_PFX                "Xtensa C library context switch test (xt_clib)"

// Number of concurrent tasks.
#define NTASKS                  4

// If MPU support is enabled these tasks will be created as privileged tasks.
// xTaskCreate does not support creating nonprivileged tasks.
#define INIT_TASK_PRIO          (20 | portPRIVILEGE_BIT)
#define TEST_TASK_PRIO          (18 | portPRIVILEGE_BIT)

uint32_t     result[NTASKS];
TaskHandle_t Task_TCB[NTASKS];

extern volatile uint32_t * volatile pxCurrentTCB;


// Task function.
void Task_Func( void * pdata )
{
    uint32_t val = (uint32_t) pdata;
    uint32_t cnt = 0;
    void *   test_p;

    srand( val );

#if (XT_USE_THREAD_SAFE_CLIB > 0)

    while ( cnt < 400 )
    {
#if XSHAL_CLIB == XTHAL_CLIB_XCLIB || XSHAL_CLIB == XTHAL_CLIB_NEWLIB
        if ( pxCurrentTCB )
        {
            // Note that _impure_ptr (newlib) is redefined as _reent_ptr in the case of
            // xclib.
            if ( _impure_ptr != (void *)(&pxCurrentTCB[TCB_IMPURE_PTR_OFF / 4]) )
            {
                // A failure might mean that the hack definition of TCB in this file, xt_clib.c,
                // is out of date with respect to the official definition in tasks.c.
                printf( "Task %d, Bad reent ptr\n", val );
                exit( 1 );
            }
        }
        else
        {
            printf( "Task %d, Bad TCB pointer!\n", val ); // This means there is some corruption
            exit( 2 );
        }
#else
  #error Unsupported C library
#endif

        test_p = malloc( (size_t)(rand() % 500) );
        if ( !test_p )
        {
            printf( "Task %d, malloc() failed\n", val );
            exit( 3 );
        }

        if ( (val == 0) && (cnt % 100 == 99) )
        {
            printf( "100...\n" );
        }

        vTaskDelay( 1 );
        free( test_p );
        cnt++;
    }

#else

    // No thread-safe library, nothing to do
    test_p = 0;
    cnt    = 0;
    vTaskDelay( 1 );

#endif

    result[val] = 1;
    vTaskDelete( NULL );
}


// Init Task.
static void Init_Task( void * pdata )
{
    uint32_t t0;
    uint32_t t1;
    int32_t i;
    int32_t busy;
    int32_t err = 0;

    UNUSED(pdata);
    for ( i = 0; i < NTASKS; ++i )
    {
        // Create the application tasks (all are lower priority so wait for us).
        err = xTaskCreate( Task_Func,
                           "Task",
                           TASK_STK_SIZE,
                           (void *) i,
                           TEST_TASK_PRIO,
                           &Task_TCB[i] );

        if ( err != pdPASS )
        {
            printf( TEST_PFX " FAILED to create Task\n" );
            goto done;
        }
    }

    // The test begins here.
    t0 = xTaskGetTickCount();

    // Simulate round-robin of the application tasks every tick.
    do
    {
        busy = 0;
        for ( i = 0; i < NTASKS; ++i )
        {
            //vTaskPrioritySet(Task_TCB[i], 21);
            vTaskDelay( NTASKS );
            //vTaskPrioritySet(Task_TCB[i], 22);
            busy |= ( result[i] == 0 );
        }
    }
    while ( busy );

    t1 = xTaskGetTickCount();
    printf( TEST_PFX " PASSED! %d ticks\n", t1 - t0 );

done:
#ifdef XT_SIMULATOR
    // Shut down simulator and report error code as exit code to host (0 = OK).
    exit( 0 );
#endif

    // Terminate this task. RTOS will continue to run timer, stats and idle tasks.
    vTaskDelete( NULL );
}


void vApplicationStackOverflowHook( TaskHandle_t xTask, char * pcTaskName )
{
    UNUSED(xTask);
    UNUSED(pcTaskName);
    puts( "\nStack overflow, stopping." );
    exit( -1 );
}


void vApplicationTickHook( void )
{
}


int main( void )
{
    int err = 0;
    int exit_code = 0;

    printf( TEST_PFX " running...\n" );

    // Create the control task initially with the high priority.
    err = xTaskCreate( Init_Task,
                       "Init_Task",
                       TASK_STK_SIZE,
                       NULL,
                       INIT_TASK_PRIO,
                       NULL );

    if ( err != pdPASS )
    {
        printf( TEST_PFX " FAILED to create Init_Task\n" );
        goto done;
    }

    // Start task scheduler.
    vTaskStartScheduler();

done:
    exit_code = err;

#ifdef XT_SIMULATOR
    // Shut down simulator and report error code as exit code to host (0 = OK).
    exit( exit_code );
#endif

    return 0;
}

