/*
 * FreeRTOS V202212.00
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
 * https://github.com/FreeRTOS
 *
 */

/* Platform include files. */
#include "Ifx_Types.h"
#include <stdio.h>

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

/* Flag that will be latched to pdTRUE should any unexpected behavior be
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
    /* Value written into all registers (decremented at each iteration) */
    static uint32_t value_in = 0xFFFFFFF;

    /* Values read from the Lower Context Registers will be read into these variables... */
    static uint32_t d0_out = 0, d1_out = 0, d6_out = 0, d7_out = 0;

    /* Values read from the Upper Context Registers will be read into these variables... */
    static uint32_t d8_out = 0, d9_out = 0, d14_out = 0, d15_out = 0;

    /* Will be set to TRUE if there is a mismatch in value read back from any register */
    static boolean mismatch = FALSE;

    ( void ) pvParameters;

    vTaskDelay(pdMS_TO_TICKS(100));
    printf("prvRegisterTest1Task() start...\r\n");
    vTaskDelay(pdMS_TO_TICKS(100));

    for( ; ; )
    {
        mismatch = FALSE;

        /* Write to Lower Context Registers */
        __asm("LD.W d0,[%0]" : : "a" (&value_in));
        __asm("LD.W d1,[%0]" : : "a" (&value_in));
        __asm("LD.W d6,[%0]" : : "a" (&value_in));
        __asm("LD.W d7,[%0]" : : "a" (&value_in));

        /* Write to Upper Context Register */
        __asm("LD.W d8,[%0]" : : "a" (&value_in));
        __asm("LD.W d9,[%0]" : : "a" (&value_in));
        __asm("LD.W d14,[%0]" : : "a" (&value_in));
        __asm("LD.W d15,[%0]" : : "a" (&value_in));

        /* Force context switch */
        taskYIELD();

        /* Read Lower Context Registers */
        __asm("st.w [%0],d0" : : "a" (&d0_out));
        __asm("st.w [%0],d1" : : "a" (&d1_out));
        __asm("st.w [%0],d6" : : "a" (&d6_out));
        __asm("st.w [%0],d7" : : "a" (&d7_out));

        /* Read Upper Context Registers */
        __asm("st.w [%0],d8" : : "a" (&d8_out));
        __asm("st.w [%0],d9" : : "a" (&d9_out));
        __asm("st.w [%0],d14" : : "a" (&d14_out));
        __asm("st.w [%0],d15" : : "a" (&d15_out));


        if (d0_out != value_in) {
            printf("D[0] mismatch: 0x%X != 0x%X\r\n", value_in, d0_out);
            mismatch = TRUE;
        }

        if (d1_out != value_in) {
            printf("D[1] mismatch: 0x%X != 0x%X\r\n", value_in, d1_out);
            mismatch = TRUE;
        }

        if (d6_out != value_in) {
            printf("D[6] mismatch: 0x%X != 0x%X\r\n", value_in, d6_out);
            mismatch = TRUE;
        }

        if (d7_out != value_in) {
            printf("D[7] mismatch: 0x%X != 0x%X\r\n", value_in, d7_out);
            mismatch = TRUE;
        }

        if (d8_out != value_in) {
            printf("D[8] mismatch: 0x%X != 0x%X\r\n", value_in, d8_out);
            mismatch = TRUE;
        }

        if (d9_out != value_in) {
            printf("D[9] mismatch: 0x%X != 0x%X\r\n", value_in, d9_out);
            mismatch = TRUE;
        }

        if (d14_out != value_in) {
            printf("D[14] mismatch: 0x%X != 0x%X\r\n", value_in, d14_out);
            mismatch = TRUE;
        }

        if (d15_out != value_in) {
            printf("D[7] mismatch: 0x%X != 0x%X\r\n", value_in, d15_out);
            mismatch = TRUE;
        }

        if (!mismatch) {
            ulRegisterTest1Counter++;
        }
    }
}
/*-----------------------------------------------------------*/

static void prvRegisterTest2Task( void *pvParameters )
{
    /* Value written into all registers (decremented at each iteration) */
    static uint32_t value_in = 0xFFFFFFF;

    /* Values read from the Lower Context Registers will be read into these variables... */
    static uint32_t d0_out = 0, d1_out = 0, d6_out = 0, d7_out = 0;

    /* Values read from the Upper Context Registers will be read into these variables... */
    static uint32_t d8_out = 0, d9_out = 0, d14_out = 0, d15_out = 0;

    /* Will be set to TRUE if there is a mismatch in value read back from any register */
    static boolean mismatch = FALSE;

    ( void ) pvParameters;

    vTaskDelay(pdMS_TO_TICKS(200));
    printf("prvRegisterTest2Task() start...\r\n");
    vTaskDelay(pdMS_TO_TICKS(200));

    for( ; ; )
    {
        mismatch = FALSE;

        /* Write to Lower Context Registers */
        __asm("LD.W d0,[%0]" : : "a" (&value_in));
        __asm("LD.W d1,[%0]" : : "a" (&value_in));
        __asm("LD.W d6,[%0]" : : "a" (&value_in));
        __asm("LD.W d7,[%0]" : : "a" (&value_in));

        /* Write to Upper Context Register */
        __asm("LD.W d8,[%0]" : : "a" (&value_in));
        __asm("LD.W d9,[%0]" : : "a" (&value_in));
        __asm("LD.W d14,[%0]" : : "a" (&value_in));
        __asm("LD.W d15,[%0]" : : "a" (&value_in));

        /* Force context switch */
        taskYIELD();

        /* Read Lower Context Registers */
        __asm("st.w [%0],d0" : : "a" (&d0_out));
        __asm("st.w [%0],d1" : : "a" (&d1_out));
        __asm("st.w [%0],d6" : : "a" (&d6_out));
        __asm("st.w [%0],d7" : : "a" (&d7_out));

        /* Read Upper Context Registers */
        __asm("st.w [%0],d8" : : "a" (&d8_out));
        __asm("st.w [%0],d9" : : "a" (&d9_out));
        __asm("st.w [%0],d14" : : "a" (&d14_out));
        __asm("st.w [%0],d15" : : "a" (&d15_out));


        if (d0_out != value_in) {
            printf("D[0] mismatch: 0x%X != 0x%X\r\n", value_in, d0_out);
            mismatch = TRUE;
        }

        if (d1_out != value_in) {
            printf("D[1] mismatch: 0x%X != 0x%X\r\n", value_in, d1_out);
            mismatch = TRUE;
        }

        if (d6_out != value_in) {
            printf("D[6] mismatch: 0x%X != 0x%X\r\n", value_in, d6_out);
            mismatch = TRUE;
        }

        if (d7_out != value_in) {
            printf("D[7] mismatch: 0x%X != 0x%X\r\n", value_in, d7_out);
            mismatch = TRUE;
        }

        if (d8_out != value_in) {
            printf("D[8] mismatch: 0x%X != 0x%X\r\n", value_in, d8_out);
            mismatch = TRUE;
        }

        if (d9_out != value_in) {
            printf("D[9] mismatch: 0x%X != 0x%X\r\n", value_in, d9_out);
            mismatch = TRUE;
        }

        if (d14_out != value_in) {
            printf("D[14] mismatch: 0x%X != 0x%X\r\n", value_in, d14_out);
            mismatch = TRUE;
        }

        if (d15_out != value_in) {
            printf("D[7] mismatch: 0x%X != 0x%X\r\n", value_in, d15_out);
            mismatch = TRUE;
        }

        if (!mismatch) {
            ulRegisterTest2Counter++;
        }
    }
}
/*-----------------------------------------------------------*/

static void prvRegisterTest3Task( void *pvParameters )
{
    /* Value written into all registers (decremented at each iteration) */
    static uint32_t value_in = 0xFFFFFFF;

    /* Values read from the Lower Context Registers will be read into these variables... */
    static uint32_t d0_out = 0, d1_out = 0, d6_out = 0, d7_out = 0;

    /* Values read from the Upper Context Registers will be read into these variables... */
    static uint32_t d8_out = 0, d9_out = 0, d14_out = 0, d15_out = 0;

    /* Will be set to TRUE if there is a mismatch in value read back from any register */
    static boolean mismatch = FALSE;

    ( void ) pvParameters;

    vTaskDelay(pdMS_TO_TICKS(300));
    printf("prvRegisterTest3Task() start...\r\n");
    vTaskDelay(pdMS_TO_TICKS(300));

    for( ; ; )
    {
        mismatch = FALSE;

        /* Write to Lower Context Registers */
        __asm("LD.W d0,[%0]" : : "a" (&value_in));
        __asm("LD.W d1,[%0]" : : "a" (&value_in));
        __asm("LD.W d6,[%0]" : : "a" (&value_in));
        __asm("LD.W d7,[%0]" : : "a" (&value_in));

        /* Write to Upper Context Register */
        __asm("LD.W d8,[%0]" : : "a" (&value_in));
        __asm("LD.W d9,[%0]" : : "a" (&value_in));
        __asm("LD.W d14,[%0]" : : "a" (&value_in));
        __asm("LD.W d15,[%0]" : : "a" (&value_in));

        /* Force context switch */
        taskYIELD();

        /* Read Lower Context Registers */
        __asm("st.w [%0],d0" : : "a" (&d0_out));
        __asm("st.w [%0],d1" : : "a" (&d1_out));
        __asm("st.w [%0],d6" : : "a" (&d6_out));
        __asm("st.w [%0],d7" : : "a" (&d7_out));

        /* Read Upper Context Registers */
        __asm("st.w [%0],d8" : : "a" (&d8_out));
        __asm("st.w [%0],d9" : : "a" (&d9_out));
        __asm("st.w [%0],d14" : : "a" (&d14_out));
        __asm("st.w [%0],d15" : : "a" (&d15_out));


        if (d0_out != value_in) {
            printf("D[0] mismatch: 0x%X != 0x%X\r\n", value_in, d0_out);
            mismatch = TRUE;
        }

        if (d1_out != value_in) {
            printf("D[1] mismatch: 0x%X != 0x%X\r\n", value_in, d1_out);
            mismatch = TRUE;
        }

        if (d6_out != value_in) {
            printf("D[6] mismatch: 0x%X != 0x%X\r\n", value_in, d6_out);
            mismatch = TRUE;
        }

        if (d7_out != value_in) {
            printf("D[7] mismatch: 0x%X != 0x%X\r\n", value_in, d7_out);
            mismatch = TRUE;
        }

        if (d8_out != value_in) {
            printf("D[8] mismatch: 0x%X != 0x%X\r\n", value_in, d8_out);
            mismatch = TRUE;
        }

        if (d9_out != value_in) {
            printf("D[9] mismatch: 0x%X != 0x%X\r\n", value_in, d9_out);
            mismatch = TRUE;
        }

        if (d14_out != value_in) {
            printf("D[14] mismatch: 0x%X != 0x%X\r\n", value_in, d14_out);
            mismatch = TRUE;
        }

        if (d15_out != value_in) {
            printf("D[7] mismatch: 0x%X != 0x%X\r\n", value_in, d15_out);
            mismatch = TRUE;
        }

        if (!mismatch) {
            ulRegisterTest3Counter++;
        }
    }
}
/*-----------------------------------------------------------*/

static void prvRegisterTest4Task( void *pvParameters )
{
    /* Value written into all registers (decremented at each iteration) */
    static uint32_t value_in = 0xFFFFFFF;

    /* Values read from the Lower Context Registers will be read into these variables... */
    static uint32_t d0_out = 0, d1_out = 0, d6_out = 0, d7_out = 0;

    /* Values read from the Upper Context Registers will be read into these variables... */
    static uint32_t d8_out = 0, d9_out = 0, d14_out = 0, d15_out = 0;

    /* Will be set to TRUE if there is a mismatch in value read back from any register */
    static boolean mismatch = FALSE;

    ( void ) pvParameters;

    vTaskDelay(pdMS_TO_TICKS(400));
    printf("prvRegisterTest4Task() start...\r\n");
    vTaskDelay(pdMS_TO_TICKS(400));

    for( ; ; )
    {
        mismatch = FALSE;

        /* Write to Lower Context Registers */
        __asm("LD.W d0,[%0]" : : "a" (&value_in));
        __asm("LD.W d1,[%0]" : : "a" (&value_in));
        __asm("LD.W d6,[%0]" : : "a" (&value_in));
        __asm("LD.W d7,[%0]" : : "a" (&value_in));

        /* Write to Upper Context Register */
        __asm("LD.W d8,[%0]" : : "a" (&value_in));
        __asm("LD.W d9,[%0]" : : "a" (&value_in));
        __asm("LD.W d14,[%0]" : : "a" (&value_in));
        __asm("LD.W d15,[%0]" : : "a" (&value_in));

        /* Force context switch */
        taskYIELD();

        /* Read Lower Context Registers */
        __asm("st.w [%0],d0" : : "a" (&d0_out));
        __asm("st.w [%0],d1" : : "a" (&d1_out));
        __asm("st.w [%0],d6" : : "a" (&d6_out));
        __asm("st.w [%0],d7" : : "a" (&d7_out));

        /* Read Upper Context Registers */
        __asm("st.w [%0],d8" : : "a" (&d8_out));
        __asm("st.w [%0],d9" : : "a" (&d9_out));
        __asm("st.w [%0],d14" : : "a" (&d14_out));
        __asm("st.w [%0],d15" : : "a" (&d15_out));


        if (d0_out != value_in) {
            printf("D[0] mismatch: 0x%X != 0x%X\r\n", value_in, d0_out);
            mismatch = TRUE;
        }

        if (d1_out != value_in) {
            printf("D[1] mismatch: 0x%X != 0x%X\r\n", value_in, d1_out);
            mismatch = TRUE;
        }

        if (d6_out != value_in) {
            printf("D[6] mismatch: 0x%X != 0x%X\r\n", value_in, d6_out);
            mismatch = TRUE;
        }

        if (d7_out != value_in) {
            printf("D[7] mismatch: 0x%X != 0x%X\r\n", value_in, d7_out);
            mismatch = TRUE;
        }

        if (d8_out != value_in) {
            printf("D[8] mismatch: 0x%X != 0x%X\r\n", value_in, d8_out);
            mismatch = TRUE;
        }

        if (d9_out != value_in) {
            printf("D[9] mismatch: 0x%X != 0x%X\r\n", value_in, d9_out);
            mismatch = TRUE;
        }

        if (d14_out != value_in) {
            printf("D[14] mismatch: 0x%X != 0x%X\r\n", value_in, d14_out);
            mismatch = TRUE;
        }

        if (d15_out != value_in) {
            printf("D[7] mismatch: 0x%X != 0x%X\r\n", value_in, d15_out);
            mismatch = TRUE;
        }

        if (!mismatch) {
            ulRegisterTest4Counter++;
        }
    }
}
/*-----------------------------------------------------------*/

void vStartRegisterTasks( UBaseType_t uxPriority )
{
	BaseType_t ret;

	ret = xTaskCreate( prvRegisterTest1Task, "RegTest1", configMINIMAL_STACK_SIZE, NULL, uxPriority, NULL );
	configASSERT( ret == pdPASS );

	ret = xTaskCreate( prvRegisterTest2Task, "RegTest2", configMINIMAL_STACK_SIZE, NULL, uxPriority, NULL );
	configASSERT( ret == pdPASS );

	ret = xTaskCreate( prvRegisterTest3Task, "RegTest3", configMINIMAL_STACK_SIZE, NULL, uxPriority, NULL );
	configASSERT( ret == pdPASS );

	ret = xTaskCreate( prvRegisterTest4Task, "RegTest4", configMINIMAL_STACK_SIZE, NULL, uxPriority, NULL );
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
	    printf("ulRegisterTest1Counter error (%d, %d)\r\n", ulLastRegisterTest1Counter, ulRegisterTest1Counter);
		xErrorDetected = pdTRUE;
	}

	if( ulLastRegisterTest2Counter == ulRegisterTest2Counter )
	{
	    printf("ulRegisterTest2Counter error (%d, %d)\r\n", ulLastRegisterTest2Counter, ulRegisterTest2Counter);
		xErrorDetected = pdTRUE;
	}

	if( ulLastRegisterTest3Counter == ulRegisterTest3Counter )
	{
	    printf("ulRegisterTest3Counter error (%d, %d)\r\n", ulLastRegisterTest3Counter, ulRegisterTest3Counter);
		xErrorDetected = pdTRUE;
	}

	if( ulLastRegisterTest4Counter == ulRegisterTest4Counter )
	{
	    printf("ulRegisterTest4Counter error (%d, %d)\r\n", ulLastRegisterTest4Counter, ulRegisterTest4Counter);
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
