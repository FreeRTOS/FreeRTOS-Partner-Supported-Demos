//-----------------------------------------------------------------------------
// Copyright (c) 2024 Cadence Design Systems, Inc.
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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "timers.h"
#include "semphr.h"

#include "xtensa_timer.h"

#define INIT_TASK_PRIO      (4 + portPRIVILEGE_BIT)
#define TASK_STK_SIZE       ((XT_STACK_MIN_SIZE + 0x400) / sizeof(StackType_t))

struct timer_data
{
    TimerHandle_t timer;
    SemaphoreHandle_t lock;
    uint32_t ccount;
    uint32_t delta;
    uint32_t timer_cnt;
    uint32_t thread_cnt;
};

static inline uint32_t get_ccount(void)
{
    uint32_t v;
    asm volatile ("rsr.ccount %[v]" : [v] "=a" (v));
    return v;
}

#define CCOMPARE_NAME(i) _CCOMPARE_NAME(i)
#define _CCOMPARE_NAME(i) "ccompare" #i

static inline uint32_t get_ccompare(void)
{
    uint32_t v;
    asm volatile ("rsr." CCOMPARE_NAME(XT_TIMER_INDEX) " %[v]" : [v] "=a" (v));
    return v;
}

static inline void show_results_and_exit(int rc)
{
    printf("%s (%d)\n", rc == 0 ? "Passed" : "Failed", rc);
    exit(rc);
}

#define TIMER_PERIOD 4
#define TIMER_CLOCKS (TIMER_PERIOD * configCPU_CLOCK_HZ / configTICK_RATE_HZ)
#define TICK_CLOCKS (configCPU_CLOCK_HZ / configTICK_RATE_HZ)
static volatile int rc = 0;

static void timer(TimerHandle_t t)
{
    struct timer_data *td = pvTimerGetTimerID(t);
    uint32_t ccount = get_ccount();
    if (td->ccount) {
        if (ccount - td->ccount > TIMER_CLOCKS + (TICK_CLOCKS - 1) / 2 ||
            ccount - td->ccount < TIMER_CLOCKS - (TICK_CLOCKS - 1) / 2) {
            printf("%s: %d, expected: %d; delta: %d\n",
                   __func__, ccount - td->ccount, TIMER_CLOCKS, td->delta);
            rc = 1;
        }
    }
    td->ccount = ccount;
    ++td->timer_cnt;
    if (td->timer_cnt - td->thread_cnt > 10) {
        printf("%s: bailing out with delta = %d, TICK_CLOCKS = %d\n",
               __func__, td->delta, TICK_CLOCKS);
        if (rc == 0) {
            printf("Done\n");
        }
        show_results_and_exit(rc);
    }
    xSemaphoreGive(td->lock);
}

static void Init_Task(void *pdata)
{
    struct timer_data td[1] = {};
    uint32_t delta;

    UNUSED(pdata);

    td->lock = xSemaphoreCreateCounting(1, 0);
    td->timer = xTimerCreate("", TIMER_PERIOD, 1, td, timer);
    xTimerStart(td->timer, portMAX_DELAY);
    for (delta = 10; delta < TICK_CLOCKS; delta += 10) {
        if (delta > 2000 && delta < TICK_CLOCKS - 2000)
            delta = TICK_CLOCKS - 2000;
        td->delta = delta;
        while (get_ccount() + delta - get_ccompare() > 100) {
        }
        xSemaphoreTake(td->lock, portMAX_DELAY);
        fprintf(stderr, ".");
        ++td->thread_cnt;
    }
    printf("Done\n");
    show_results_and_exit(rc);
}


#if configUSE_TICK_HOOK
void vApplicationTickHook(void)
{
}
#endif


void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    UNUSED(xTask);
    UNUSED(pcTaskName);
    puts("\nStack overflow, stopping.");
    show_results_and_exit(1);
}

int main(void)
{
    int err = xTaskCreate(Init_Task, "Init_Task", TASK_STK_SIZE, NULL, INIT_TASK_PRIO, NULL);

    if (err != pdPASS) {
        printf("FAILED! main\n");
        return 1;
    }

    // Set stderr to unbuffered
    setvbuf(stderr, NULL, _IONBF, 0);

    vTaskStartScheduler();
    printf( "vTaskStartScheduler FAILED!\n" );
    return 1;
}
