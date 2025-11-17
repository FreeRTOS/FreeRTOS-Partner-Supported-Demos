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

#define portINITIAL_PSTATE_EL0     ( 0x0ULL )                                                         /* EL0, SP_EL0 */
#define GENERIC_TIMER_IRQ          ( 30UL )                                                           /* Default IRQ for CNTP_EL0 */
#define SGI0_IRQ                   ( 0UL )                                                            /* SGI0 IRQ */
#define GENERIC_TIMER_IRQ_PRIORITY ( portLOWEST_USABLE_INTERRUPT_PRIORITY << portPRIORITY_SHIFT )     /* priority for CNTP_EL0 */
#define GENERIC_TIMER_FREQ         ( 24000000 )                                                       /* Frequency for Generic Timer */
#define TIMER_CTRL_ENABLE          ( 1UL << 0 )                                                       /* Timer ENABLE bit */
#define TIMER_CTRL_IMASK           ( 1UL << 1 )                                                       /* Timer IMASK bit */
#define LOG_QUEUE_LENGTH           ( 100 )                                                            /* Length of the logging queue */
#define LOG_QUEUE_IDX              ( 0 )                                                              /* Index of the logging queue handle in the shared array */
#define NUMBER_OF_SHARED_ELEMENTS  ( 10 )                                                             /* Number of shared elements in the shared array */
#define FIRST_TASK_HANDLE_IDX      ( 1 )                                                              /* Index assigned to the first task handle */
#define SECOND_TASK_HANDLE_IDX     ( 2 )                                                              /* Index assigned to the second task handle */
#define THIRD_TASK_HANDLE_IDX      ( 3 )                                                              /* Index assigned to the third task handle */

typedef struct
{
    uint8_t     ucText[128];
    uint8_t     ucCore;
} LogMsg_t;

#if defined(__ARMCC_VERSION)
    /* Externs needed by the MPU setup code. These are defined in Scatter-Loading
     * description file (armclang.sct). */
    extern uint64_t Image$$ER_ROM_CODE$$Base;
    extern uint64_t Image$$ER_IROM_PRIVILEGED_ALIGN$$Limit;
    extern uint64_t Image$$ER_IROM_FREERTOS_SYSTEM_CALLS$$Base;
    extern uint64_t Image$$ER_IROM_FREERTOS_SYSTEM_CALLS_ALIGN$$Limit;
    extern uint64_t Image$$ER_IROM_UNPRIVILEGED$$Base;
    extern uint64_t Image$$ER_IROM_UNPRIVILEGED_ALIGN$$Limit;

    extern uint64_t Image$$ER_IRAM_PRIVILEGED$$Base;
    extern uint64_t Image$$ER_IRAM_PRIVILEGED_ALIGN$$Limit;
    extern uint64_t Image$$ER_IRAM_BSS_UNPRIVILEGED$$Base;
    extern uint64_t Image$$ER_IRAM_BSS_UNPRIVILEGED_ALIGN$$Limit;

    /* Privileged flash. */
    const uint64_t * __privileged_functions_start__		= ( uint64_t * ) &( Image$$ER_ROM_CODE$$Base );
    const uint64_t * __privileged_functions_end__		= ( uint64_t * ) ( ( uint64_t ) &( Image$$ER_IROM_PRIVILEGED_ALIGN$$Limit ) - 0x1 ); /* Last address in privileged Flash region. */

    /* Flash containing system calls. */
    const uint64_t * __syscalls_flash_start__			= ( uint64_t * ) &( Image$$ER_IROM_FREERTOS_SYSTEM_CALLS$$Base );
    const uint64_t * __syscalls_flash_end__				= ( uint64_t * ) ( ( uint64_t ) &( Image$$ER_IROM_FREERTOS_SYSTEM_CALLS_ALIGN$$Limit ) - 0x1 ); /* Last address in Flash region containing system calls. */

    /* Unprivileged flash. Note that the section containing system calls is
     * unprivileged so that unprivileged tasks can make system calls. */
    const uint64_t * __unprivileged_flash_start__		= ( uint64_t * ) &( Image$$ER_IROM_UNPRIVILEGED$$Base );
    const uint64_t * __unprivileged_flash_end__			= ( uint64_t * ) ( ( uint64_t ) &( Image$$ER_IROM_UNPRIVILEGED_ALIGN$$Limit ) - 0x1 ); /* Last address in un-privileged Flash region. */

    /* RAM with priviledged access only. This contains kernel data. */
    const uint64_t * __privileged_sram_start__			= ( uint64_t * ) &( Image$$ER_IRAM_PRIVILEGED$$Base );
    const uint64_t * __privileged_sram_end__			= ( uint64_t * ) ( ( uint64_t ) &( Image$$ER_IRAM_PRIVILEGED_ALIGN$$Limit ) - 0x1 ); /* Last address in privileged RAM. */
#else
    /* When building with GCC, these symbols come from gnu_linker_script.ld. */
    extern uint64_t __privileged_flash_start__[];
    extern uint64_t __privileged_flash_end__[];
    extern uint64_t __syscalls_flash_start__[];
    extern uint64_t __syscalls_flash_end__[];
    extern uint64_t __unprivileged_flash_start__[];
    extern uint64_t __unprivileged_flash_end__[];
    extern uint64_t __privileged_sram_start__[];
    extern uint64_t __privileged_sram_end__[];
#endif

static TaskHandle_t xFirstTaskHandle      = NULL;
static TaskHandle_t xSecondTaskHandle     = NULL;
static TaskHandle_t xThirdTaskHandle      = NULL;
static TaskHandle_t xLoggerTaskHandle     = NULL;

/* User defined task stack. */
static StackType_t xFirstTaskStack[ configMINIMAL_STACK_SIZE ]  __attribute__( ( aligned( 64 ) ) );
static StackType_t xSecondTaskStack[ configMINIMAL_STACK_SIZE ] __attribute__( ( aligned( 64 ) ) );
static StackType_t xThirdTaskStack[ configMINIMAL_STACK_SIZE ] __attribute__( ( aligned( 64 ) ) );

/* User defined shared queue handles array. */
static QueueHandle_t xQueueHandlesArray[ NUMBER_OF_SHARED_ELEMENTS ] __attribute__( ( aligned( 64 ) ) );

static uint64_t prvGetTimerClockHz ( void );
static void prvSetTimerClockHz ( uint64_t ullPhysicalTimerFreq );
extern void FreeRTOS_Tick_Handler( void );
extern void FreeRTOS_SGI_Handler( void );

#if ( configNUMBER_OF_CORES == 1 )
    extern TaskHandle_t pxCurrentTCB;
#else
    extern TaskHandle_t pxCurrentTCBs[ configNUMBER_OF_CORES ];
#endif

void vAssertCalled( const char * pcFile,
                    unsigned long ulLine )
{
    printf( "ASSERT failed! file %s:%lu, \r\n", pcFile, ulLine );

    taskENTER_CRITICAL();
    {
        volatile unsigned long looping = 0;

        /* Use the debugger to set ul to a non-zero value in order to step out
         * of this function to determine why it was called. */
        while( looping == 0LU )
        {
            portNOP();
        }
    }
    taskEXIT_CRITICAL();
}

static void * prvTinyMemCpy( void * pvDst, const void * pvSrc, size_t xNumberOfElements )
{
    uint8_t * pucDestination = ( uint8_t * ) pvDst;
    const uint8_t * pucSource = ( const uint8_t * ) pvSrc;
    while ( xNumberOfElements-- ) {
        *pucDestination++ = *pucSource++;
    }
    return pucDestination;
}

static size_t prvTinyStrnlen( const char * pcString, size_t xMaxLength )
{
    size_t xLength = 0;

    if( pcString == NULL )
    {
        return 0;
    }

    while( ( xLength < xMaxLength ) && ( pcString[ xLength ] != '\0' ) )
    {
        xLength++;
    }

    return xLength;
}

static void prvLogMsg( const char * pcMessage )
{
    LogMsg_t xLogMsg;

    /* Leave room for the terminating NUL character. */
    size_t xMaxCopyLength = sizeof( xLogMsg.ucText ) - 1U;
    size_t xMsgLength = prvTinyStrnlen( pcMessage, xMaxCopyLength );
    prvTinyMemCpy( xLogMsg.ucText, pcMessage, xMsgLength );
    xLogMsg.ucText[ xMsgLength ] = '\0';
    xLogMsg.ucCore = ucPortGetCoreID();

    (void) xQueueSend( xQueueHandlesArray[ LOG_QUEUE_IDX ], &xLogMsg, portMAX_DELAY );
}

/* Privileged logger task: receives log messages and prints them. */
static void prvLoggerTask( void * arg )
{
    ( void ) arg;
    LogMsg_t xLogMsg;

    while( 1)
    {
        if( xQueueReceive( xQueueHandlesArray[ LOG_QUEUE_IDX ], &xLogMsg, portMAX_DELAY ) == pdPASS )
        {
            printf( "[Core: %d] %s\r\n", xLogMsg.ucCore, xLogMsg.ucText );
        }
    }
}

void vRecoverFromException( void )
{
    TaskHandle_t xCurrentTaskHandle = MPU_xTaskGetCurrentTaskHandle();

    if ( xCurrentTaskHandle == ( TaskHandle_t ) FIRST_TASK_HANDLE_IDX )
    {
        prvLogMsg( "First Task: Recovered from the data abortion exception gracefully." );
    }
    else if ( xCurrentTaskHandle == ( TaskHandle_t ) SECOND_TASK_HANDLE_IDX )
    {
        prvLogMsg( "Second Task: Recovered from the data abortion exception gracefully." );
    }
    else if ( xCurrentTaskHandle == ( TaskHandle_t ) THIRD_TASK_HANDLE_IDX )
    {
        prvLogMsg( "Third Task: Recovered from the data abortion exception gracefully." );
    }
    else
    {
        prvLogMsg( "Unknown Task: Recovered from the data abortion exception gracefully." );
    }

    /* Delete the offending task. */
    __asm__ volatile (
        "SVC %0         \n" /* SVC to vTaskDelete. */
        :
        : "i" ( portSVC_DELETE_CURRENT_TASK )
        : "memory"
    );
}

static void prvFirstTask( void * arg )
{
    /* Prevent the compiler warning about the unused parameter. */
    ( void ) arg;
    while( 1 )
    {
        prvLogMsg( "First Task: Attempting to raise its privilege level by accessing its privileged data." );
        /* First task deliberately attempts to raise its privilege level by modifying its own TCB,
         * which resides in privileged RAM. This should trigger an MPU violation exception where
         * the exception will be recovered gracefully and the task will be deleted by
         * vRecoverFromException function.
         */
        #if ( configNUMBER_OF_CORES == 1 )
            xMPU_SETTINGS * pxMpuSettings = xTaskGetMPUSettings( pxCurrentTCB );
        #else
            xMPU_SETTINGS * pxMpuSettings = xTaskGetMPUSettings( pxCurrentTCBs[ ucPortGetCoreID() ] );
        #endif

        pxMpuSettings->ullTaskFlags |= portTASK_IS_PRIVILEGED_FLAG;
    }
}

static void prvSecondTask( void * arg )
{
    /* Prevent the compiler warning about the unused parameter. */
    ( void ) arg;
    while( 1 )
    {
        prvLogMsg( "Second Task: Attempting to access its own unprivileged stack." );
        /* Second task attempts to access its own stack, this should be successful
         * as the second task has access to its own stack.
         */
        xSecondTaskStack[0] = 0xDEADBEEF;
        prvLogMsg( "Second Task: Successfully accessed its own unprivileged stack." );

        prvLogMsg( "Second Task: Attempting to access first task unprivileged stack." );
        /* Second task attempts to access the stack of the first task, which
         * it does not have access to. This should trigger an MPU violation exception where the
         * exception will be recovered gracefully and the task will be
         * deleted by vRecoverFromException function.
         */
        xFirstTaskStack[0] = 0xDEADBEEF;
    }
}

static void prvThirdTask( void * arg )
{
    /* Prevent the compiler warning about the unused parameter. */
    ( void ) arg;
    while( 1 )
    {
        prvLogMsg( "Third Task: Attempting to write to a read-only region." );
        /* Third task attempts to write to a read-only region in unprivileged flash.
         * This should trigger an MPU violation exception where the exception
         * will be recovered gracefully and the task will be deleted by
         * vRecoverFromException function.
         */
        * ( ( uint64_t * ) __unprivileged_flash_start__ ) = 0xDEADBEEF;
    }
}

int main()
{
    printf( "Starting MPU demo with exception recovery.\r\n\n" );

    TaskParameters_t xFirstTaskParameters =
    {
        .pvTaskCode     = prvFirstTask,
        .pcName         = NULL,
        .usStackDepth   = configMINIMAL_STACK_SIZE,
        .pvParameters   = NULL,
        .uxPriority     = ( tskIDLE_PRIORITY + 1 ),
        .puxStackBuffer = xFirstTaskStack,
        .xRegions       =
        {
            /* Base address, size, parameters. */
            { xQueueHandlesArray, NUMBER_OF_SHARED_ELEMENTS * sizeof( QueueHandle_t ), tskMPU_REGION_READ_WRITE | tskMPU_REGION_EXECUTE_NEVER | tskMPU_REGION_INNER_SHAREABLE },
        }
    };

    if(xTaskCreateRestricted( &( xFirstTaskParameters ), &xFirstTaskHandle ) == pdFAIL)
    {
        return EXIT_FAILURE;
    }

    TaskParameters_t xSecondTaskParameters =
    {
        .pvTaskCode     = prvSecondTask,
        .pcName         = NULL,
        .usStackDepth   = configMINIMAL_STACK_SIZE,
        .pvParameters   = NULL,
        .uxPriority     = ( tskIDLE_PRIORITY + 1 ),
        .puxStackBuffer = xSecondTaskStack,
        .xRegions       =
        {
            /* Base address, size, parameters. */
            { xQueueHandlesArray, NUMBER_OF_SHARED_ELEMENTS * sizeof( QueueHandle_t ), tskMPU_REGION_READ_WRITE | tskMPU_REGION_EXECUTE_NEVER | tskMPU_REGION_INNER_SHAREABLE },
        }
    };

    if(xTaskCreateRestricted( &( xSecondTaskParameters ), &xSecondTaskHandle ) == pdFAIL)
    {
        return EXIT_FAILURE;
    }

    TaskParameters_t xThirdTaskParameters =
    {
        .pvTaskCode     = prvThirdTask,
        .pcName         = NULL,
        .usStackDepth   = configMINIMAL_STACK_SIZE,
        .pvParameters   = NULL,
        .uxPriority     = ( tskIDLE_PRIORITY + 1 ),
        .puxStackBuffer = xThirdTaskStack,
        .xRegions       =
        {
            /* Base address, size, parameters. */
            { xQueueHandlesArray, NUMBER_OF_SHARED_ELEMENTS * sizeof( QueueHandle_t ), tskMPU_REGION_READ_WRITE | tskMPU_REGION_EXECUTE_NEVER | tskMPU_REGION_INNER_SHAREABLE },
        }
    };

    if(xTaskCreateRestricted( &( xThirdTaskParameters ), &xThirdTaskHandle ) == pdFAIL)
    {
        return EXIT_FAILURE;
    }

    if(xTaskCreate( prvLoggerTask,
        NULL,
        configMINIMAL_STACK_SIZE,
        NULL,
        ( ( tskIDLE_PRIORITY + 2 ) | portPRIVILEGE_BIT ),
        &xLoggerTaskHandle ) == pdFAIL )
    {
        return EXIT_FAILURE;
    }

    /* Dedicated logging queue.. */
    xQueueHandlesArray[ LOG_QUEUE_IDX ] = xQueueCreate( LOG_QUEUE_LENGTH, sizeof( LogMsg_t ) );

    if( xQueueHandlesArray[ LOG_QUEUE_IDX ] == NULL )
    {
        return EXIT_FAILURE;
    }

    vTaskStartScheduler();

    /* If all is well, the scheduler will now be running, and the following
     * line will never be reached.  If the following line does execute, then
     * there was insufficient FreeRTOS heap memory available for the idle and/or
     * timer tasks	to be created.  See the memory management section on the
     * FreeRTOS web site for more details.  NOTE: This demo uses static allocation
     * for the idle and timer tasks so this line should never execute.
     */
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

void App_Fault_Handler( void )
{
    /* This handler deals with data aborts caused by MPU violations.
     * It is placed in the vector table in place of the SWI handler.
     * The handler checks the exception class to ensure it is a data
     * abort from a lower exception level, then adjusts the return
     * address to skip the faulting instruction before returning
     * from the exception.
     */
    __asm volatile (
        " STP X0, X1, [SP, #-0x10]!                          \n"     /* Save X0 and X1 temporarily as they are used in the handler. */
        " MRS X0, ESR_EL1                                    \n"
        " LSR X1, X0, # 26                                   \n"     /* Extract exception class. */
        " CMP X1, # 0x24                                     \n"     /* 0x24 = Data abortion from low level. */
        " B.NE svc_handler                                   \n"     /* Indicate a svc. */
        " MSR SPSR_EL1, %0                                   \n"     /* Set PSTATE to EL0, SP_EL0. */
        " adrp    X0, vRecoverFromException                  \n"
        " add     X0, X0, :lo12:vRecoverFromException        \n"     /* X0 = &vRecoverFromException */
        " MSR ELR_EL1, X0                                    \n"
        " LDP X0, X1, [SP], #0x10                            \n"     /* Restore X0 and X1. */
        " ERET                                               \n"
        " svc_handler:                                       \n"     /* Route SVC calls to FreeRTOS_SWI_Handler. */
        " CMP X1, # 0x15                                     \n"
        " B.NE fault_loop                                    \n"
        " LDP X0, X1, [SP], #0x10                            \n"     /* Restore X0 and X1. */
        " B FreeRTOS_SWI_Handler                             \n"
        " fault_loop:                                        \n"
        " B fault_loop                                       \n"
        :
        : "r" ( portINITIAL_PSTATE_EL0 )
        : "memory", "x0", "x1"
    );
}
