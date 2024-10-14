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

#include    <stdio.h>
#include    <stdlib.h>

#include    <xtensa/hal.h>
#include <xtensa/tie/xt_core.h>
#ifdef XT_BOARD
#include    <xtensa/xtbsp.h>
#endif
#include <xtensa/xtutil.h>

#include "FreeRTOS.h"

#include "xtensa_api.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"

/* Task priorities. */
/*
 * NOTE: the consumer runs at a higher priority than the producer so as to
 * avoid overflowing the message queue. Also note: if privileged/user mode
 * is enabled, these tasks are set up to run in privileged mode only.
*/
#define INIT_TASK_PRIO    (5 | portPRIVILEGE_BIT)
#define TASK_0_PRIO       (6 | portPRIVILEGE_BIT)
#define TASK_1_PRIO       (8 | portPRIVILEGE_BIT)
#define TASK_2_PRIO       (7 | portPRIVILEGE_BIT)

/* Test iterations */
#define TEST_ITER         1000

#if XCHAL_HAVE_XEA2 && (XCHAL_NUM_INTERRUPTS <= 32)
/* Uncomment this to exercise the s/w prioritization */
#define XT_USE_SWPRI      1
#endif

#define INT_LO_PRI        (XCHAL_NUM_INTLEVELS - 2)
#define INT_HI_PRI        (XCHAL_NUM_INTLEVELS - 1)

/* SW interrupt number (computed at runtime) */
uint32_t uiSwIntNum = 0;

#if defined(XT_USE_SWPRI) || XCHAL_HAVE_XEA3
/* Second (higher priority) SW interrupt number */
uint32_t uiSwInt2Num = 0;
volatile int iFlag = 0;
#endif

/* Variables used by exception test */
volatile int iExcCount = 0;
volatile int junk;

/* Stack size for tasks that do not use the C library. */
#define     TASK_STK_SIZE_MIN       ((XT_STACK_MIN_SIZE) / sizeof(StackType_t))
/* Stack size for tasks that use the C library and/or the coprocessors */
#define     TASK_STK_SIZE_STD       ((XT_STACK_MIN_SIZE + 0x400) / sizeof(StackType_t))

/* Queue for passing count. */
#define     QUEUE_SIZE              16
#define     SEM_CNT                 32

QueueHandle_t xQueue;

/* Semaphore for test. */
SemaphoreHandle_t xSem;

/* Define the counters used in the demo application...  */

uint32_t  uiTask0Counter;
uint32_t  uiTask1Counter;
uint32_t  uiTask1MessagesSent;
uint32_t  uiTask2Counter;
uint32_t  uiTask2MessagesReceived;

/* Task handles */
TaskHandle_t      xTask0Handle;
TaskHandle_t      xTask1Handle;
TaskHandle_t      xTask2Handle;

/*
*******************************************************************************
* Illegal instruction handler.
*******************************************************************************
*/
void illegalInstHandler(XtExcFrame *frame)
{
    /* Move the PC past the illegal (3-byte) instruction */
    frame->pc += 3;
    puts("e");
}


/*
*******************************************************************************
* SW interrupt handler. Argument is pointer to semaphore handle.
* Interrupt is cleared in dispatcher so no need to do that here.
*******************************************************************************
*/
void softwareIntHandler(void* arg)
{
    SemaphoreHandle_t s = (SemaphoreHandle_t)arg;

    /* Signal the semaphore */
    xSemaphoreGiveFromISR(s, NULL);

#if defined(XT_USE_SWPRI) || XCHAL_HAVE_XEA3
    if (uiSwInt2Num) {
        iFlag = 44;
        putchar('<');
        /* Higher priority handler should run right away and change flag */
        xt_interrupt_trigger(uiSwInt2Num);
        /* Eat a few cycles here to ensure interrupt is taken */
        XT_RSYNC();
        junk++;
        if (iFlag == 44) {
            puts("Error: higher priority handler not run");
            exit(-1);
        }
        putchar('>');
    }
#endif
}


#if defined(XT_USE_SWPRI) || XCHAL_HAVE_XEA3
/* Handler for higher priority interrupt (at same level but prioritized
   higher in software).
*/
void softwareHighHandler(void* arg)
{
    UNUSED(arg);
    iFlag = 55;
    putchar('H');
}
#endif


/*
*******************************************************************************
* Task 0
*******************************************************************************
*/
static void Task0(void* pvData)
{
    //int err;

    /* This task simply sits in while-forever-sleep loop.  */
    UNUSED(pvData);
    while(1)
    {
        #ifdef DEMO_USE_PRINTF
        if (uiTask0Counter % 10 == 0) {
            printf(" %8u iterations of task_0,  system clock = %8u ticks\n",
                    uiTask0Counter, xTaskGetTickCount());
        }
        printf("."); fflush(stdout);
        #endif

        /* Increment the task counter.  */
        uiTask0Counter++;

        /* Sleep for 10 ticks.  */
        vTaskDelay(10);
    }
}


/*
*******************************************************************************
* Task 1
*******************************************************************************
*/
static void Task1(void *pvData)
{
    BaseType_t err = pdPASS;
    uint32_t i;

    UNUSED(pvData);

    /* Set up interrupt handling and enable interrupt */
    xt_set_interrupt_handler(uiSwIntNum, softwareIntHandler, (void*)xSem);
    xt_interrupt_enable(uiSwIntNum);

#if defined(XT_USE_SWPRI) || XCHAL_HAVE_XEA3
    /* Set up the higher priority interrupt if available */
    if (uiSwInt2Num) {
        xt_set_interrupt_handler(uiSwInt2Num, softwareHighHandler, 0);
        xt_interrupt_enable(uiSwInt2Num);
    }
#endif

    /* Now send messages to task 2 and signal it */
    for (i = 0; i < TEST_ITER; i++) {
        /* Increment the task counter */
        uiTask1Counter++;

        /* Send message to queue 0 */
        err = xQueueSend(xQueue, &uiTask1MessagesSent, portMAX_DELAY);
        if (err != pdPASS)
            break;

        uiTask1MessagesSent++;
        putchar('+');

        /* Now trigger the interrupt handler */
        xt_interrupt_trigger(uiSwIntNum);
    }

#ifdef DEMO_USE_PRINTF
    printf("Thread_1: sent %u messages\n", uiTask1MessagesSent);
#else
    puts("task 1 finish");
#endif
    vTaskDelete(NULL);
}


/*
*******************************************************************************
* Task 2
*******************************************************************************
*/
static void Task2(void* pvData)
{
    uint32_t  uiReceivedMessage;
    uint32_t  i;
    uint32_t  iok = 0;
    uint32_t  eok = 0;
    BaseType_t err = pdPASS;

    /* This task retrieves messages placed on the queue by task 1 */
    UNUSED(pvData);
    for (i = 0; i < TEST_ITER; i++) {
        /* Wait for the semaphore to be signalled */
        xSemaphoreTake(xSem, portMAX_DELAY);
        /* Increment the task counter */
        uiTask2Counter++;

        /* Retrieve a message from the queue */
        err = xQueueReceive(xQueue, &uiReceivedMessage, portMAX_DELAY);

        /* Check completion status and make sure the message is what we
           expected */
        if ((err != pdPASS) || (uiReceivedMessage != uiTask2MessagesReceived))
            break;

        /* Otherwise, all is okay.  Increment the received message count */
        uiTask2MessagesReceived++;
        putchar('-');
    }

#ifdef DEMO_USE_PRINTF
    printf("Thread 2: recd %u messages\n", uiTask2MessagesReceived);
#else
    puts("task 2 finish");
#endif

    if (uiTask1MessagesSent == uiTask2MessagesReceived) {
        puts("Interrupt Test PASS");
        iok = 1;
    }
    else {
        puts("Interrupt Test FAIL");
    }

    /* Now test exception handling */

    /* Install handler */
#if XCHAL_HAVE_XEA2
    xt_set_exception_handler(EXCCAUSE_ILLEGAL, illegalInstHandler);
#else
    xt_set_exception_handler(EXCCAUSE_INSTRUCTION, illegalInstHandler);
#endif

    /* Force an illegal instruction. The 3-byte form of the illegal
       instruction should be present in all configs. The handler will
       push the PC past the offending instruction, so the loop should
       complete successfully.
     */
    for (i = 0; i < 10; i++) {
        asm volatile
           ("movi    a4, iExcCount\n"
            "_ill\n"
            "l32i    a5, a4, 0\n"
            "addi    a5, a5, 1\n"
            "s32i    a5, a4, 0\n" : :: "a4", "a5");
    }

    if (iExcCount == 10) {
        puts("Exception Test PASS");
        eok = 1;
    }
    else {
        puts("Exception Test FAIL");
    }

    if (iok && eok) {
        puts("Xtensa interrupt/exception test (xt_intr) PASSED!");
    }

    exit(0);
}


/*
*********************************************************************************************************
*                                          APP INITIALZATION TASK
*
*********************************************************************************************************
*/

static void initTask(void* pvData)
{
    BaseType_t err = 0;

    UNUSED(pvData);

    /* Create test semaphore. */
    xSem = xSemaphoreCreateCounting( SEM_CNT, 0 );
    /* Create queue for sequence of counts. */
    xQueue = xQueueCreate(QUEUE_SIZE, 1 * sizeof(uint32_t));

    /* Create the 3 test tasks. */
    err = xTaskCreate(Task0, "Task0", TASK_STK_SIZE_STD, (void*)0, TASK_0_PRIO, &xTask0Handle);

    if (err != pdPASS)
    {
        puts("Task 0 create failed.");
        goto done;
    }

    err = xTaskCreate(Task1, "Task1", TASK_STK_SIZE_STD, (void*)0, TASK_1_PRIO, &xTask1Handle);

    if (err != pdPASS)
    {
        puts("Task 1 create failed.");
        goto done;
    }

    err = xTaskCreate(Task2, "Task2", TASK_STK_SIZE_STD, (void*)0, TASK_2_PRIO, &xTask2Handle);

    if (err != pdPASS)
    {
        puts("Task 2 create failed.");
        goto done;
    }

done:
    /* Clean up and shut down. */
    if (err != pdPASS) {
        exit(err);
    }

    vTaskDelete(NULL);
}

/*
*********************************************************************************************************
*                                             C ENTRY POINT
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
int main_xt_intr(int argc, char *argv[])
#endif
{
    int32_t x = -1;
    int32_t y = -1;
    int32_t i;

    /* Unbuffer stdout */
    setbuf(stdout, 0);

    puts("Xtensa interrupt/exception test (xt_intr) running...");

    /* Find one or two sw interrupts <= XCHAL_EXCM_LEVEL */
    for (i = 0; i < XCHAL_NUM_INTERRUPTS; i++) {
        if ((Xthal_inttype[i] == XTHAL_INTTYPE_SOFTWARE) &&
			(Xthal_intlevel[i] <= XCHAL_EXCM_LEVEL)) {
            printf("interrupt %d\n", i);
            if (x == -1) {
                x = i;
            }
            else {
                y = i;
                if (Xthal_intlevel[y] != Xthal_intlevel[x])
                    break;
            }
        }
    }

    if (x == -1) {
        printf("No software interrupt found.\n");
        return 0;
    }

    /* Set default */
    uiSwIntNum = (uint32_t)x;

    if (y == -1) {
        printf("Second sw interrupt not found, nested test will not run.\n");
    }
    else {
#if XCHAL_HAVE_XEA2 && defined(XT_USE_SWPRI)
        if (Xthal_intlevel[x] == Xthal_intlevel[y]) {
            printf("Both interrupts at same priority, nested test will not run.\n");
            uiSwIntNum = (uint32_t)x;
        }
        else {
            uiSwIntNum  = (uint32_t)(Xthal_intlevel[x] > Xthal_intlevel[y] ? y : x);
            uiSwInt2Num = (uint32_t)(Xthal_intlevel[x] > Xthal_intlevel[y] ? x : y);
        }
#endif
#if XCHAL_HAVE_XEA3
        xthal_interrupt_pri_set(x, INT_LO_PRI);
        xthal_interrupt_pri_set(y, INT_HI_PRI);
        uiSwIntNum  = x;
        uiSwInt2Num = y;
#endif
    }

    xTaskCreate( initTask, "initTask", configMINIMAL_STACK_SIZE, (void *)NULL, INIT_TASK_PRIO, NULL );
    /* Finally start the scheduler. */
    vTaskStartScheduler();
    /* Will only reach here if there is insufficient heap available to start
       the scheduler. */
    for( ;; );
    return 0;
}

