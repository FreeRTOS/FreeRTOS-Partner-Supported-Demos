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

/* This is a simple test of Xtensa overlay usage in the FreeRTOS environment.
   It creates two overlays with one function in each, and calls them from 
   two different threads. These overlay functions each call a function in a
   third overlay. The overlay functions purposely run long enough to cause
   context switches to happen while executing overlay code. This
   exercises the overlay restore mechanism when a thread is resumed.

   This test is currently set up to run on the simulator. Changes may be
   required to run on actual hardware.
*/

/* This test uses printf from the libxtutil library. This is a much smaller
   footprint than the standard C library printf, and is thread-safe.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <xtensa/overlay.h>
#include <xtensa/xtutil.h>

#include "testcommon.h"
#include "task.h"


/* Number of iterations */
#define TEST_ITER    100

/* Declare three overlay sections */

DECLARE_OVERLAY(0);
DECLARE_OVERLAY(1);
DECLARE_OVERLAY(2);


#define STACK_SIZE          ((XT_STACK_MIN_SIZE + 0x400) / sizeof(StackType_t))
#define THREAD_0_PRIO       (5 | portPRIVILEGE_BIT)
#define THREAD_1_PRIO       (4 | portPRIVILEGE_BIT)
#define THREAD_2_PRIO       (4 | portPRIVILEGE_BIT)


TaskHandle_t thread_0;
TaskHandle_t thread_1;
TaskHandle_t thread_2;

uint32_t thread_0_counter;
uint32_t thread_1_counter;
uint32_t thread_2_counter;

volatile uint32_t thread_1_done = 0;
volatile uint32_t thread_2_done = 0;


/* Thread entry points */

void    thread_0_entry(void * arg);
void    thread_1_entry(void * arg);
void    thread_2_entry(void * arg);


/* Main */
int main(void)
{
    uint32_t err;

    printf("FreeRTOS Xtensa port version %s\n", XTENSA_PORT_VERSION_STRING);
    printf("FreeRTOS overlay test starting...\n");

    /* Create threads */
    err = xTaskCreate(thread_0_entry, "Thread_0", STACK_SIZE, NULL, THREAD_0_PRIO, &thread_0);
    if (err != pdPASS) {
        puts("Thread 0 create failed");
        return 1;
    }

    err = xTaskCreate(thread_1_entry, "Thread_1", STACK_SIZE, NULL, THREAD_1_PRIO, &thread_1);
    if (err != pdPASS) {
        puts("Thread 1 create failed");
        return 1;
    }

    err = xTaskCreate(thread_2_entry, "Thread_2", STACK_SIZE, NULL, THREAD_2_PRIO, &thread_2);
    if (err != pdPASS) {
        puts("Thread 2 create failed");
        return 1;
    }

    /* Start scheduler */
    vTaskStartScheduler();

    puts("Failed to start scheduling");
    return 1;
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char * pcTaskName)
{
    puts("\nStack overflow, stopping.");
    exit(2);
}

#if configUSE_TICK_HOOK
void vApplicationTickHook(void)
{
}
#endif


/* Overlay functions */

int OVERLAY(2) ov2_func(int arg)
{
    if (xt_overlay_get_id() != 2) {
        puts("ERROR: wrong overlay ID in ov2_func()");
        exit(-1);
    }

    if (arg % 2) {
        vTaskDelay(2);
    }

    return arg + 1;
}

int OVERLAY(0) ov0_func1(int arg)
{
    int i;
    volatile int x = 1;

    if (arg) {
        x = ov2_func(arg);
        return ov0_func1(arg - 1);
    }

    putchar('0');

    for (i = 0; i < 40000; i++) {
        x += 1;
    }

    return x;
}

int OVERLAY(1) ov1_func1(int arg)
{
    int j;
    volatile int y = 1;

    if (arg) {
        y = ov2_func(arg);
        return ov1_func1(arg - 1);
    }

    putchar('1');

    for (j = 0; j < 40000; j++) {
        y = y + 1;
    }

    return y;
}


/* Test threads */

void thread_0_entry(void * arg)
{
    /* Wait for threads 1 and 2 to be done. */
    while (!thread_1_done || !thread_2_done) {
        if (thread_0_counter % 10 == 0) {
            printf("\n %8lu iterations of thread_0,  system clock = %8lu ticks\n", 
                    thread_0_counter, xTaskGetTickCount());
        }
        putchar('.');

        /* Increment the thread counter.  */
        thread_0_counter++;

        /* Sleep for 20 ticks.  */
        vTaskDelay(20);
    }

    puts("FreeRTOS overlay test PASSED");
    exit(0);
}


void thread_1_entry(void * arg)
{
    while(thread_1_counter < TEST_ITER) {
        thread_1_counter++;
        ov0_func1(4);
    }

    thread_1_done = 1;
    vTaskDelete(NULL);
}


void thread_2_entry(void * arg)
{
    while (thread_2_counter < TEST_ITER) {
        thread_2_counter++;
        ov1_func1(4);
    }

    thread_2_done = 1;
    vTaskDelete(NULL);
}

