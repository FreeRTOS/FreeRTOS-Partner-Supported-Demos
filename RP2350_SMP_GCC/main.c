/* Copyright 2023-2026 Arm Limited and/or its affiliates <open-source-office@arm.com>
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>
#include <stdlib.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "portmacro.h"

/* Pico SDK includes. */
#include "RP2350.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"
#include "irq.h"

#define DELAY_100_MS                   ( pdMS_TO_TICKS( 100 ) )      /* 100 milliseconds delay */
#define DELAY_50_MS                    ( pdMS_TO_TICKS( 50 ) )       /* 50 milliseconds delay */
#define MIN_INTERRUPT_PRIORITY         ( 255UL )                     /* Lowest interrupt priority */
#define CORE_0_AND_CORE_1_MASK         ( 0b11 )                      /* Mask for both cores */

static SemaphoreHandle_t xMutex = NULL;
static volatile uint32_t ulSharedFlag = 0;
static TaskHandle_t prvTaskCore0Handle;
static TaskHandle_t prvTaskCore1Handle;
static int8_t cDoorbellNum = -1;

extern PRIVILEGED_DATA volatile uint8_t ucPrimaryCoreInitDoneFlag;
extern PRIVILEGED_DATA volatile uint8_t ucSecondaryCoresReadyFlags[ configNUMBER_OF_CORES - 1 ];

static void prvDoorbellInterruptHandler();
static void prvMakeSecondaryCoreReady( void );
static void prvSetupInterruptVectors( void );
static void prvSetupDoorbellInterrupt( void );
static void prvDoorbellInit ( void );

extern void SysTick_Handler( void ) PRIVILEGED_FUNCTION;
extern void PendSV_Handler( void ) __attribute__( ( naked ) ) PRIVILEGED_FUNCTION;
extern void SVC_Handler( void ) __attribute__( ( naked ) ) PRIVILEGED_FUNCTION;

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

static void prvTaskCore0( void * arg )
{
    /* Prevent the compiler warning about the unused parameter. */
    ( void ) arg;
    while( 1 )
    {
        if ( xSemaphoreTake( xMutex, portMAX_DELAY ) == pdTRUE )
        {
            if( ulSharedFlag == 0U )
            {
                printf( "Ping from Core %d\r\n", ucPortGetCoreID() );
                ulSharedFlag = 1U;
                __DSB();
            }
            xSemaphoreGive( xMutex );
            vTaskDelay( DELAY_100_MS );
        }
    }
}

static void prvTaskCore1( void * arg )
{
    /* Prevent the compiler warning about the unused parameter. */
    ( void ) arg;
    while( 1 )
    {
        if ( xSemaphoreTake( xMutex, portMAX_DELAY ) == pdTRUE )
        {
            if( ulSharedFlag == 1U )
            {
                printf( "Pong from Core %d\r\n", ucPortGetCoreID() );
                ulSharedFlag = 0U;
                __DSB();
            }
            xSemaphoreGive( xMutex );
            vTaskDelay( DELAY_100_MS );
        }
    }
}

static void prvBouncingTask( void * arg )
{
    /* Prevent the compiler warning about the unused parameter. */
    ( void ) arg;
    while( 1 )
    {
        if ( xSemaphoreTake( xMutex, portMAX_DELAY ) == pdTRUE )
        {
            printf( "Bouncing task running on core %d\r\n", ucPortGetCoreID() );
            xSemaphoreGive( xMutex );
            vTaskDelay( DELAY_50_MS );
        }
    }
}

int main()
{
    /* Initialize stdio */
    stdio_init_all();

    /* Setup Systick, PendSV, and SVC interrupt vectors */
    prvSetupInterruptVectors();

    printf( "FreeRTOS SMP Example starting on RP2350...\n" );

    if( xTaskCreate( prvTaskCore0,
        NULL,
        configMINIMAL_STACK_SIZE,
        NULL,
        ( tskIDLE_PRIORITY + 1 ),
        &prvTaskCore0Handle ) == pdFAIL )
    {
        return EXIT_FAILURE;
    }

    if( xTaskCreate( prvTaskCore1,
        NULL,
        configMINIMAL_STACK_SIZE,
        NULL,
        ( tskIDLE_PRIORITY + 1 ),
        &prvTaskCore1Handle ) == pdFAIL )
    {
        return EXIT_FAILURE;
    }

    if( xTaskCreate( prvBouncingTask,
        NULL,
        configMINIMAL_STACK_SIZE,
        NULL,
        ( tskIDLE_PRIORITY ),
        NULL ) == pdFAIL )
    {
        return EXIT_FAILURE;
    }

    vTaskCoreAffinitySet( prvTaskCore0Handle, 1UL << 0 );  /* Pin to Core 0 */
    vTaskCoreAffinitySet( prvTaskCore1Handle, 1UL << 1 );  /* Pin to Core 1 */

    xMutex = xSemaphoreCreateMutex();

    if ( xMutex == NULL )
    {
        return EXIT_FAILURE;  /* Failed to create mutex */
    }

    prvDoorbellInit(); /* Initialize doorbell for inter-core interrupts */

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

    __WEAK void vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                               StackType_t ** ppxIdleTaskStackBuffer,
                                               StackType_t * pulIdleTaskStackSize )
    {
        /* Idle task control block and stack */
        static StaticTask_t Idle_TCB = { 0 };
        static StackType_t Idle_Stack[ configMINIMAL_STACK_SIZE ] = { 0 };

        *ppxIdleTaskTCBBuffer = &Idle_TCB;
        *ppxIdleTaskStackBuffer = &Idle_Stack[ 0 ];
        *pulIdleTaskStackSize = ( uint32_t ) configMINIMAL_STACK_SIZE;
    }

/*
 * vApplicationGetTimerTaskMemory gets called when configSUPPORT_STATIC_ALLOCATION
 * equals to 1 and is required for static memory allocation support.
 */
    __WEAK void vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                                StackType_t ** ppxTimerTaskStackBuffer,
                                                StackType_t * pulTimerTaskStackSize )
    {
        /* Timer task control block and stack */
        static StaticTask_t Timer_TCB = { 0 };
        static StackType_t Timer_Stack[ configTIMER_TASK_STACK_DEPTH ] = { 0 };

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

void vInterruptCore( uint8_t ucCoreID )
{
    ( void ) ucCoreID;
    multicore_doorbell_set_other_core( cDoorbellNum );
}

static void prvDoorbellInterruptHandler( void )
{
    if ( cDoorbellNum >= 0 && multicore_doorbell_is_set_current_core( cDoorbellNum ) )
    {
        multicore_doorbell_clear_current_core( cDoorbellNum );
        /* Request a context switch */
        portYIELD_FROM_ISR( pdTRUE );
    }
}

static void prvDoorbellInit ( void )
{
    /* Claim a doorbell for inter-core interrupts to be used by both cores */
    cDoorbellNum = ( int8_t ) multicore_doorbell_claim_unused( CORE_0_AND_CORE_1_MASK, true );
    multicore_doorbell_clear_current_core( cDoorbellNum );
    multicore_doorbell_clear_other_core( cDoorbellNum );
    printf( "Core %u: Doorbell %d claimed for inter-core interrupts.\n", portGET_CORE_ID(), cDoorbellNum );
}

static void prvSetupDoorbellInterrupt( void )
{
    /* Install Doorbell handler to receive interrupt from other core */
    uint32_t ulDoorbellIRQn = multicore_doorbell_irq_num( cDoorbellNum );
    NVIC_SetPriority( ulDoorbellIRQn, MIN_INTERRUPT_PRIORITY );
    NVIC_SetVector( ulDoorbellIRQn, ( uint32_t ) prvDoorbellInterruptHandler );
    NVIC_EnableIRQ( ulDoorbellIRQn );
}

BaseType_t xWakeSecondaryCore( void )
{
    if ( ucPrimaryCoreInitDoneFlag == 1U )
    {
        printf( "Core %u: Waking up secondary core.\n", portGET_CORE_ID() );
        /* Wake secondary core with prvMakeSecondaryCoreReady as the entry point */
        multicore_launch_core1( prvMakeSecondaryCoreReady );
        return pdTRUE;
    }
    else
    {
        printf( "Core %u: Primary core initialization not done yet. Cannot wake secondary core.\n", portGET_CORE_ID() );
        return pdFALSE;
    }
}

static void prvMakeSecondaryCoreReady( void )
{
    printf( "Core %u: Secondary core started.\n", portGET_CORE_ID() );
    prvSetupInterruptVectors();
    prvSetupDoorbellInterrupt();
    vPortConfigureInterruptPriorities();

    /* Signal that this secondary core is ready */
    ucSecondaryCoresReadyFlags[ portGET_CORE_ID() - 1 ] = 1U;
    __asm volatile (
        "SVC %0 \n"
        :
        : "i" ( portSVC_START_SCHEDULER )
        : "memory"
    );
}

static void prvSetupInterruptVectors( void )
{
    NVIC_SetVector( SVCall_IRQn, ( uint32_t ) SVC_Handler );
    NVIC_SetVector( PendSV_IRQn, ( uint32_t ) PendSV_Handler );
    NVIC_SetVector( SysTick_IRQn, ( uint32_t ) SysTick_Handler );
}
