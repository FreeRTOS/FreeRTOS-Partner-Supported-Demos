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
#include "xtensa_api.h"
#include "xtensa_timer.h"

#include <xtensa/config/core.h>

#if XT_TIMER_INDEX != 3 && XCHAL_TIMER3_INTERRUPT != XTHAL_TIMER_UNCONFIGURED
  #if XCHAL_INT_LEVEL(XCHAL_TIMER3_INTERRUPT) <= XCHAL_EXCM_LEVEL
    #undef OTHER_TIMER_INDEX
    #define OTHER_TIMER_INDEX 3
  #endif
#endif
#if XT_TIMER_INDEX != 2 && XCHAL_TIMER2_INTERRUPT != XTHAL_TIMER_UNCONFIGURED
  #if XCHAL_INT_LEVEL(XCHAL_TIMER2_INTERRUPT) <= XCHAL_EXCM_LEVEL
    #undef OTHER_TIMER_INDEX
    #define OTHER_TIMER_INDEX 2
  #endif
#endif
#if XT_TIMER_INDEX != 1 && XCHAL_TIMER1_INTERRUPT != XTHAL_TIMER_UNCONFIGURED
  #if XCHAL_INT_LEVEL(XCHAL_TIMER1_INTERRUPT) <= XCHAL_EXCM_LEVEL
    #undef OTHER_TIMER_INDEX
    #define OTHER_TIMER_INDEX 1
  #endif
#endif
#if XT_TIMER_INDEX != 0 && XCHAL_TIMER0_INTERRUPT != XTHAL_TIMER_UNCONFIGURED
  #if XCHAL_INT_LEVEL(XCHAL_TIMER0_INTERRUPT) <= XCHAL_EXCM_LEVEL
    #undef OTHER_TIMER_INDEX
    #define OTHER_TIMER_INDEX 0
  #endif
#endif

static inline void show_results_and_exit(int rc)
{
    printf("%s (%d)\n", rc == 0 ? "Passed" : "Failed", rc);
    exit(rc);
}


#define FREQ_LOW 50000000
#define FREQ_HIGH 500000000
static uint32_t clock_freq = FREQ_LOW;

uint32_t xtbsp_clock_freq_hz(void)
{
    return clock_freq;
}

#ifdef OTHER_TIMER_INDEX

#define INIT_TASK_PRIO  (4 | portPRIVILEGE_BIT)
#define TASK_STK_SIZE   ((XT_STACK_MIN_SIZE + 0x400) / sizeof(StackType_t))

struct timer_data
{
    TimerHandle_t timer;
    SemaphoreHandle_t lock;
    uint32_t ccount;
    uint32_t delta;
    uint32_t timer_cnt;
    uint32_t thread_cnt;
    uint32_t missed_delta;
    int32_t  missed_dir;
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

static inline void set_ccompare(uint32_t v)
{
    asm volatile ("wsr." CCOMPARE_NAME(OTHER_TIMER_INDEX) " %[v]" :: [v] "a" (v));
}

// Allow a 1% error margin for test purposes
#define TIMER_PERIOD 4
#define TIMER_CLOCKS (TIMER_PERIOD * configCPU_CLOCK_HZ / configTICK_RATE_HZ)
#define TICK_CLOCKS (((configCPU_CLOCK_HZ / configTICK_RATE_HZ) * 101) / 100)

static volatile int rc = 0;

static void timer(TimerHandle_t t)
{
    struct timer_data *td = pvTimerGetTimerID(t);
    uint32_t ccount = get_ccount();
    if (td->ccount) {
        /* In tickless mode, it is possible that a timer can be handled
         * one tick later than expected, with a subsequent expiration
         * handled one tick earlier.  Adjust for this possibility and 
         * only flag an error if it is not corrected in the opposite dir.
         */
        if (ccount - td->ccount > TIMER_CLOCKS + (TICK_CLOCKS - 1) / 2 &&
            ccount - td->ccount < TIMER_CLOCKS + (TICK_CLOCKS - 1)) {
            if (td->missed_delta == 0) {
                td->missed_delta = td->delta;
                td->missed_dir = 1;
                printf("%s: 1. %d, expected: %d +/- %d; delta: %d\n",
                    __func__, ccount - td->ccount, TIMER_CLOCKS, (TICK_CLOCKS - 1) / 2, td->delta);
                printf("%s: 2. (suppressing at delta %d)\n", __func__, td->delta);
            } else if (td->missed_dir == -1) {
                printf("%s: 3. (compensated at delta %d (+ to -); ignored)\n", __func__, td->delta);
                td->missed_delta = 0;
                td->missed_dir = 0;
            } else {
                printf("%s: 4. (suppressing additional + drift at delta %d\n", __func__, td->delta);
            }
        } else if (ccount - td->ccount < TIMER_CLOCKS - (TICK_CLOCKS - 1) / 2 &&
                   ccount - td->ccount > TIMER_CLOCKS - (TICK_CLOCKS - 1)) {
            if (td->missed_delta == 0) {
                td->missed_delta = td->delta;
                td->missed_dir = -1;
                printf("%s: 5. %d, expected: %d +/- %d; delta: %d\n",
                    __func__, ccount - td->ccount, TIMER_CLOCKS, (TICK_CLOCKS - 1) / 2, td->delta);
                printf("%s: 6. (suppressing at delta %d)\n", __func__, td->delta);
            } else if (td->missed_dir == 1) {
                printf("%s: 7. (compensated at delta %d (- to +); ignored)\n", __func__, td->delta);
                td->missed_delta = 0;
                td->missed_dir = 0;
            } else {
                printf("%s: 8. (suppressing additional - drift at delta %d\n", __func__, td->delta);
            }
        } else if (ccount - td->ccount > TIMER_CLOCKS + (TICK_CLOCKS - 1) ||
                   ccount - td->ccount < TIMER_CLOCKS - (TICK_CLOCKS - 1)) {
            printf("%s: 9. %d, expected: %d +/- %d; delta: %d\n",
                   __func__, ccount - td->ccount, TIMER_CLOCKS, (TICK_CLOCKS - 1) / 2, td->delta);
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

static void timer1(void *p)
{
    static uint32_t clock_idx;
    static const uint32_t clock_freq_variant[] = {
        FREQ_HIGH,
        FREQ_LOW,
    };

    (void)p;

    clock_freq = clock_freq_variant[clock_idx];
    clock_idx = 1 - clock_idx;

    set_ccompare(0);
    xt_update_clock_frequency();
}

static void Init_Task(void *pdata)
{
    struct timer_data td[1] = {};
    uint32_t delta;

    UNUSED(pdata);
    set_ccompare(0);
    xt_set_interrupt_handler(XCHAL_TIMER_INTERRUPT(OTHER_TIMER_INDEX), timer1, NULL);
    xt_interrupt_enable(XCHAL_TIMER_INTERRUPT(OTHER_TIMER_INDEX));
    td->lock = xSemaphoreCreateCounting(1, 0);
    td->timer = xTimerCreate("", TIMER_PERIOD, 1, td, timer);
    xTimerStart(td->timer, portMAX_DELAY);
    for (delta = 10; delta < TICK_CLOCKS + 1000; delta += 10) {
        if (delta > 2000 && delta < TICK_CLOCKS - 2000)
            delta = TICK_CLOCKS - 2000;
        td->delta = delta;
        set_ccompare(get_ccompare() + TIMER_CLOCKS - TICK_CLOCKS - delta);
        xSemaphoreTake(td->lock, portMAX_DELAY);
        fprintf(stderr, ".");
        ++td->thread_cnt;
    }
    printf("Done\n");
    show_results_and_exit(rc);
}

#endif

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
#ifdef OTHER_TIMER_INDEX
    int err = xTaskCreate(Init_Task, "Init_Task", TASK_STK_SIZE, NULL, INIT_TASK_PRIO, NULL);

    if (err != pdPASS) {
        printf("FAILED! main\n");
        return 1;
    }

    // Set stderr to unbuffered
    setvbuf(stderr, NULL, _IONBF, 0);
    printf("XT_TIMER_INDEX %d OTHER_TIMER_INDEX %d\n", XT_TIMER_INDEX, OTHER_TIMER_INDEX);

    vTaskStartScheduler();
    printf( "vTaskStartScheduler FAILED!\n" );
    return 1;
#else
    printf( "no acceptable timer for early wakeup test\n" );
    show_results_and_exit(0);
    return 0;
#endif
}
