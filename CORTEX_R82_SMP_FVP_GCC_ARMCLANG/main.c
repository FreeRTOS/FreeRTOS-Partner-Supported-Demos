/* Copyright 2023-2025 Arm Limited and/or its affiliates <open-source-office@arm.com>
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "portmacro.h"

/* GIC includes. */
#include "gic.h"

#define GENERIC_TIMER_IRQ          ( 30UL )                                                           /* Default IRQ for CNTP_EL0 */
#define SGI0_IRQ                   ( 0UL )                                                            /* SGI0 IRQ */
#define GENERIC_TIMER_IRQ_PRIORITY ( portLOWEST_USABLE_INTERRUPT_PRIORITY << portPRIORITY_SHIFT )     /* priority for CNTP_EL0 */
#define GENERIC_TIMER_FREQ         ( 24000000 )                                                       /* Frequency for Generic Timer */
#define TIMER_CTRL_ENABLE          ( 1UL << 0 )                                                       /* Timer ENABLE bit */
#define TIMER_CTRL_IMASK           ( 1UL << 1 )                                                       /* Timer IMASK bit */
#define DELAY_MS                   ( pdMS_TO_TICKS( 1000 ) )                                          /* Delay duration in milliseconds */

volatile uint64_t ulSharedFlag = 0;

static uint64_t prvGetTimerClockHz ( void );
static void prvSetTimerClockHz ( uint64_t ullPhysicalTimerFreq );
extern volatile uint8_t ucPortSchedulerRunning;
extern void FreeRTOS_Tick_Handler( void );
extern void FreeRTOS_SGI_Handler( void );
static TaskHandle_t prvTaskCore0Handle;
static TaskHandle_t prvTaskCore1Handle;
static SemaphoreHandle_t xSharedFlagMutex = NULL;

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
        if ( xSemaphoreTake( xSharedFlagMutex, portMAX_DELAY ) == pdTRUE )
        {
            if( ulSharedFlag == 0U )
            {
                printf( "Ping from Core %d\r\n", ucPortGetCoreID() );
                ulSharedFlag = 1U;
                __asm volatile( "dsb sy");
            }
            xSemaphoreGive( xSharedFlagMutex );
            vTaskDelay( DELAY_MS );
        }
    }
}

static void prvTaskCore1( void * arg )
{
    /* Prevent the compiler warning about the unused parameter. */
    ( void ) arg;
    while( 1 )
    {
        if ( xSemaphoreTake( xSharedFlagMutex, portMAX_DELAY ) == pdTRUE )
        {
            if( ulSharedFlag == 1U )
            {
                printf( "Pong from Core %d\r\n", ucPortGetCoreID() );
                ulSharedFlag = 0U;
                __asm volatile( "dsb sy");
            }
            xSemaphoreGive( xSharedFlagMutex );
            vTaskDelay( DELAY_MS );
        }
    }
}

int main()
{

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

    #if ( configNUMBER_OF_CORES > 1 )
        vTaskCoreAffinitySet( prvTaskCore0Handle, 1UL << 0 );  /* Pin to Core 0 */
        vTaskCoreAffinitySet( prvTaskCore1Handle, 1UL << 1 );  /* Pin to Core 1 */
    #endif

    xSharedFlagMutex = xSemaphoreCreateMutex();

    if ( xSharedFlagMutex == NULL )
    {
        return EXIT_FAILURE;  /* Failed to create mutex */
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

void vConfigureTickInterrupt( void )
{
    uint64_t ulTickInterval;
    uint32_t ulControlRegister = 0U;

    __asm volatile ( "dsb sy"     ::: "memory" );
    prvSetTimerClockHz( GENERIC_TIMER_FREQ );

    /* Calculate tick interval based on configured FreeRTOS tick rate */
    ulTickInterval = ( prvGetTimerClockHz() / configTICK_RATE_HZ );

    /* Disable CNTP timer before configuring */
    __asm volatile ( "msr cntp_ctl_el0, xzr" );

    /* Set the timer interval (time until next interrupt) */
    __asm volatile ( "msr cntp_tval_el0, %0" :: "r" ( ulTickInterval ) );

    /* Enable the timer interrupt in the GIC */
    vGIC_InitDist();
    vGIC_PowerUpRedistributor();
    vGIC_SetPriority( GENERIC_TIMER_IRQ, GENERIC_TIMER_IRQ_PRIORITY );
    vGIC_EnableIRQ( GENERIC_TIMER_IRQ );
    vGIC_EnableCPUInterface();

    /* Enable the timer without masking interrupts */
    __asm volatile ( "msr cntp_ctl_el0, %0" :: "r" ( ulControlRegister | TIMER_CTRL_ENABLE ) );
    __asm volatile ( "isb sy"     ::: "memory" );
}

static uint64_t prvGetTimerClockHz ( void )
{
    uint64_t ullPhysicalTimerFreq;
    __asm volatile ( "mrs %0, cntfrq_el0" : "=r" ( ullPhysicalTimerFreq ) );
    return ullPhysicalTimerFreq;
}

static void prvSetTimerClockHz ( uint64_t ullPhysicalTimerFreq )
{
    __asm volatile ( "msr cntfrq_el0, %0" :: "r" ( ullPhysicalTimerFreq ) );
}

void vApplicationIRQHandler( uint32_t ulICCIAR )
{
    /* The ID of the interrupt is obtained by bitwise anding the ICCIAR value
    with 0x3FF. */
    uint32_t ulInterruptID = ulICCIAR & 0x3FFUL;
    if( ulInterruptID == GENERIC_TIMER_IRQ )
    {
        FreeRTOS_Tick_Handler();
    }
    #if ( configNUMBER_OF_CORES > 1 )
        else if( ulInterruptID == SGI0_IRQ )
        {
            FreeRTOS_SGI_Handler();
        }
    #endif
    else
    {
        /* Handle other interrupts as needed. */
        printf( "Unhandled interrupt ID: %u\r\n", ulInterruptID );
    }
}

void vClearTickInterrupt( void )
{
    /* Disable CNTP timer interrupt before re-configuring */
    uint64_t ulControlRegister = 0U;
    ulControlRegister |= TIMER_CTRL_IMASK;
    __asm volatile ( "dsb sy"     ::: "memory" );
    __asm volatile ( "msr cntp_ctl_el0, %0" :: "r" ( ulControlRegister ) );

    /* Calculate tick interval based on configured FreeRTOS tick rate */
    uint64_t ulTickInterval = ( prvGetTimerClockHz() / configTICK_RATE_HZ );
    /* Set the timer interval (time until next interrupt) */
    __asm volatile ( "msr cntp_tval_el0, %0" :: "r" ( ulTickInterval ) );

    /* Enable the timer without masking interrupts */
    ulControlRegister &= ~( TIMER_CTRL_IMASK );
    ulControlRegister |=  TIMER_CTRL_ENABLE;
    __asm volatile ( "msr cntp_ctl_el0, %0" :: "r" ( ulControlRegister ) );
    __asm volatile ( "isb sy"     ::: "memory" );
}
