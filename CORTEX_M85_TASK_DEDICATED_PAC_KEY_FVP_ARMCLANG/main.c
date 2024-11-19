/* Copyright 2023-2024 Arm Limited and/or its affiliates
 * <open-source-office@arm.com>
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

#include "bsp_serial.h"

#define PAC_KEY_SIZE_IN_BYTES    ( 4U )

TaskHandle_t xFirstTaskHandle  = NULL;
TaskHandle_t xSecondTaskHandle = NULL;

void vAssertCalled( const char * pcFile,
                    unsigned long ulLine )
{
    printf( "ASSERT failed! file %s:%lu, \r\n", pcFile, ulLine );

    taskENTER_CRITICAL();
    {
        volatile unsigned long looping = 0;

        /* Use the debugger to set ul to a non-zero value in order to step out
         *      of this function to determine why it was called. */
        while( looping == 0LU )
        {
            portNOP();
        }
    }
    taskEXIT_CRITICAL();
}

static void prvFirstTask( void * arg )
{
    /* Prevent the compiler warning about the unused parameter. */
    ( void ) arg;
    
    #if( configENABLE_PAC == 1 )
        /* This task performs the following sequence:
        *
        *     1. Fetch the task's randomly generated dedicated PAC key once it starts running.
        *     2. Move the task to blocked state by calling `vTaskDelay()` function.
        *     3. As part of the context switching handler, the task's PAC key is unstacked after the task is unblocked, where the value of the PAC key before and after the blockage should be the same to prove the soundess of the task's dedicated PAC key stacking/unstacking procedure.
        *     4. If the task's PAC key before and after the blockage are not the same, a `configASSERT()` statement would be triggered.
        */
        uint32_t ulFirstTaskPacKey[PAC_KEY_SIZE_IN_BYTES];
        uint32_t ulRestoredFirstTaskPacKey[PAC_KEY_SIZE_IN_BYTES];

        __asm volatile ( "mrs %0, PAC_KEY_P_0"  : "=r" ( ulFirstTaskPacKey[0] ) );
        __asm volatile ( "mrs %0, PAC_KEY_P_1"  : "=r" ( ulFirstTaskPacKey[1] ) );
        __asm volatile ( "mrs %0, PAC_KEY_P_2"  : "=r" ( ulFirstTaskPacKey[2] ) );
        __asm volatile ( "mrs %0, PAC_KEY_P_3"  : "=r" ( ulFirstTaskPacKey[3] ) );

        for ( uint8_t i = 0; i < PAC_KEY_SIZE_IN_BYTES; i++ )
        {
            printf( "\r\nPAC_KEY_P_%d for first Task is = 0x%x\r\n", i, ulFirstTaskPacKey[i] );
        }

        vTaskDelay(100);

        __asm volatile ( "mrs %0, PAC_KEY_P_0"  : "=r" ( ulRestoredFirstTaskPacKey[0] ) );
        __asm volatile ( "mrs %0, PAC_KEY_P_1"  : "=r" ( ulRestoredFirstTaskPacKey[1] ) );
        __asm volatile ( "mrs %0, PAC_KEY_P_2"  : "=r" ( ulRestoredFirstTaskPacKey[2] ) );
        __asm volatile ( "mrs %0, PAC_KEY_P_3"  : "=r" ( ulRestoredFirstTaskPacKey[3] ) );

        for ( uint8_t i = 0; i < PAC_KEY_SIZE_IN_BYTES; i++ )
        {
            printf( "\r\nRestored PAC_KEY_P_%d for the first Task is = 0x%x\r\n", i, ulRestoredFirstTaskPacKey[i] );
            configASSERT( ulFirstTaskPacKey[i] == ulRestoredFirstTaskPacKey[i] );
        }
        printf( "\r\nFirst task's PAC keys matched successfully. Hence, deleteing the task.\r\n" );
        vTaskDelete( xFirstTaskHandle );
    #else
        printf( "\r\nPAC is not enabled. Hence, deleteing the prvFirstTask.\r\n" );
        vTaskDelete( xFirstTaskHandle );
    #endif
}

static void prvSecondTask( void * arg )
{
    /* Prevent the compiler warning about the unused parameter. */
    ( void ) arg;

    #if( configENABLE_PAC == 1 )
        /* This task performs the following sequence:
        *
        *     1. Fetch the task's randomly generated dedicated PAC key once it starts running.
        *     2. Move the task to blocked state by calling `vTaskDelay()` function.
        *     3. As part of the context switching handler, the task's PAC key is unstacked after the task is unblocked, where the value of the PAC key before and after the blockage should be the same to prove the soundess of the task's dedicated PAC key stacking/unstacking procedure.
        *     4. If the task's PAC key before and after the blockage are not the same, a `configASSERT()` statement would be triggered.
        */
        uint32_t ulSecondTaskPacKey[PAC_KEY_SIZE_IN_BYTES];
        uint32_t ulRestoredSecondTaskPacKey[PAC_KEY_SIZE_IN_BYTES];

        __asm volatile ( "mrs %0, PAC_KEY_P_0"  : "=r" ( ulSecondTaskPacKey[0] ) );
        __asm volatile ( "mrs %0, PAC_KEY_P_1"  : "=r" ( ulSecondTaskPacKey[1] ) );
        __asm volatile ( "mrs %0, PAC_KEY_P_2"  : "=r" ( ulSecondTaskPacKey[2] ) );
        __asm volatile ( "mrs %0, PAC_KEY_P_3"  : "=r" ( ulSecondTaskPacKey[3] ) );

        for ( uint8_t i = 0; i < PAC_KEY_SIZE_IN_BYTES; i++ )
        {
            printf( "\r\nPAC_KEY_P_%d for second Task is = 0x%x\r\n", i, ulSecondTaskPacKey[i] );
        }

        vTaskDelay(100);

        __asm volatile ( "mrs %0, PAC_KEY_P_0"  : "=r" ( ulRestoredSecondTaskPacKey[0] ) );
        __asm volatile ( "mrs %0, PAC_KEY_P_1"  : "=r" ( ulRestoredSecondTaskPacKey[1] ) );
        __asm volatile ( "mrs %0, PAC_KEY_P_2"  : "=r" ( ulRestoredSecondTaskPacKey[2] ) );
        __asm volatile ( "mrs %0, PAC_KEY_P_3"  : "=r" ( ulRestoredSecondTaskPacKey[3] ) );

        for ( uint8_t i = 0; i < PAC_KEY_SIZE_IN_BYTES; i++ )
        {
            printf( "\r\nRestored PAC_KEY_P_%d for the second Task is = 0x%x\r\n", i, ulRestoredSecondTaskPacKey[i] );
            configASSERT( ulSecondTaskPacKey[i] == ulRestoredSecondTaskPacKey[i] );
        }
        printf( "\r\nSecond task's PAC keys matched successfully. Hence, deleteing the task.\r\n" );
        vTaskDelete( xSecondTaskHandle );
    #else
        printf( "\r\nPAC is not enabled. Hence, deleteing the prvSecondTask.\r\n" );
        vTaskDelete( xSecondTaskHandle );
    #endif
}

int main()
{
    bsp_serial_init();

    printf( "In non-secure world main ...\n" );

    if(xTaskCreate( prvFirstTask,
                    NULL,
                    configMINIMAL_STACK_SIZE,
                    NULL,
                    ( tskIDLE_PRIORITY + 2 ),
                    &xFirstTaskHandle ) == pdFAIL )
    {
        return EXIT_FAILURE;
    }

    if(xTaskCreate( prvSecondTask,
                    NULL,
                    configMINIMAL_STACK_SIZE,
                    NULL,
                    ( tskIDLE_PRIORITY + 1 ),
                    &xSecondTaskHandle ) == pdFAIL )
    {
        return EXIT_FAILURE;
    }

    vTaskStartScheduler();

    /* If all is well, the scheduler will now be running, and the following
     * line will never be reached.  If the following line does execute, then
     * there was insufficient FreeRTOS heap memory available for the idle and/or
     * timer tasks	to be created.  See the memory management section on the
     * FreeRTOS web site for more details.  NOTE: This demo uses static allocation
     * for the idle and timer tasks so this line should never execute. */
    for( ; ; )
    {
    }
}

/**
 * Dummy implementation of the callback function vApplicationStackOverflowHook().
 */
#if ( configCHECK_FOR_STACK_OVERFLOW > 0 )
    void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                        char * pcTaskName )
    {
        ( void ) xTask;
        ( void ) pcTaskName;

        /* Assert when stack overflow is enabled but no application defined function exists */
        configASSERT( 0 );
    }
#endif

/*---------------------------------------------------------------------------*/
#if ( configSUPPORT_STATIC_ALLOCATION == 1 )

/*
 * vApplicationGetIdleTaskMemory gets called when configSUPPORT_STATIC_ALLOCATION
 * equals to 1 and is required for static memory allocation support.
 */
    void vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                        StackType_t ** ppxIdleTaskStackBuffer,
                                        uint32_t * pulIdleTaskStackSize )
    {
        /* Idle task control block and stack */
        static StaticTask_t Idle_TCB;
        static StackType_t Idle_Stack[ configMINIMAL_STACK_SIZE ];

        *ppxIdleTaskTCBBuffer = &Idle_TCB;
        *ppxIdleTaskStackBuffer = &Idle_Stack[ 0 ];
        *pulIdleTaskStackSize = ( uint32_t ) configMINIMAL_STACK_SIZE;
    }

/*
 * vApplicationGetTimerTaskMemory gets called when configSUPPORT_STATIC_ALLOCATION
 * equals to 1 and is required for static memory allocation support.
 */
    void vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                         StackType_t ** ppxTimerTaskStackBuffer,
                                         uint32_t * pulTimerTaskStackSize )
    {
        /* Timer task control block and stack */
        static StaticTask_t Timer_TCB;
        static StackType_t Timer_Stack[ configTIMER_TASK_STACK_DEPTH ];

        *ppxTimerTaskTCBBuffer = &Timer_TCB;
        *ppxTimerTaskStackBuffer = &Timer_Stack[ 0 ];
        *pulTimerTaskStackSize = ( uint32_t ) configTIMER_TASK_STACK_DEPTH;
    }
#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */

void vApplicationTickHook( void )
{
    /* Provide a stub for this function. */
}

void vApplicationIdleHook( void )
{
    const TickType_t xKitHitCheckPeriod = pdMS_TO_TICKS( 1000UL );
    static TickType_t xTimeNow, xLastTimeCheck = 0;

    if( ( xTimeNow - xLastTimeCheck ) > xKitHitCheckPeriod )
    {
        xLastTimeCheck = xTimeNow;
    }

    /* Exit. Just a stub. */
}

void vApplicationMallocFailedHook( void )
{
    /* Provide a stub for this function. */
}

#if( configENABLE_PAC == 1 )
    void vApplicationGenerateTaskRandomPacKey( uint32_t * pulTaskPacKey )
    {
        static BaseType_t isSeeded = pdFALSE;
        if ( isSeeded == pdFALSE )
        {
            srand(time(NULL));
            isSeeded = pdTRUE;
        }

        for ( uint8_t i = 0; i < 4; i++ )
        {
            pulTaskPacKey[i] = rand();
        }
    }
#endif /* configENABLE_PAC == 1 */
