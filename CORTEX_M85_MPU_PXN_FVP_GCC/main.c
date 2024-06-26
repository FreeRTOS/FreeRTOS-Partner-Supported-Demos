/* Copyright 2023-2024 Arm Limited and/or its affiliates
 * <open-source-office@arm.com>
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>
#include <stdlib.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

#include "bsp_serial.h"

extern uint32_t __unprivileged_functions_start__[];
uint32_t * pulRecoveryAddress = NULL;

/**
 * @brief Memory region used to track Memory Fault intentionally caused by the
 * Pxn task.
 *
 * Pxn task sets ucPxnTaskFaultTracker[ 0 ] to 1 before executing from
 * Privileged eXecute Never memory region resulting in Memory Fault where the fault handler
 * checks ucPxnTaskFaultTracker[ 0 ] to see if this is an expected fault. We
 * recover gracefully from an expected fault by jumping to the next statement
 * after the Privileged eXecute Never marked memory region.
 *
 * @note We are declaring a region of 32 bytes even though we need only one. The
 * reason is that the size of an MPU region must be a multiple of 32 bytes.
 */
static volatile uint8_t ucPxnTaskFaultTracker[ 32 ] __attribute__( ( aligned( 32 ) ) ) = { 0 };

static void prvUnprivilegedFunction ( void ) __attribute__( ( section( "unprivileged_functions" ) ) );

portDONT_DISCARD static void prvHandleMemoryFault( uint32_t * pulFaultStackAddress );

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

/* This function is assumed to be a naked function without any stack manipulation. */
static void prvUnprivilegedFunction ( void )
{
    __asm volatile
    (
        " add r0, 1,2  \n"
    );
}

static void prvPxnTask( void * arg )
{
    /* Prevent the compiler warning about the unused parameter. */
    ( void ) arg;

    pulRecoveryAddress = &&RECOVERY_LABEL;

    for ( ;; )
    {
        /* This task performs the following sequence:
         *
         * 1. Set ucPxnTaskFaultTracker[ 0 ] to 1 before executing from
         *    the Privileged eXecute Never MPU region. Since this task is a privileged
         *    task, the execution operation would result in a Memory Fault.
         *    Setting ucPxnTaskFaultTracker[ 0 ] to 1 tells the Memory Fault
         *    Handler that this is an expected fault. The handler recovers from
         *    the expected fault gracefully by jumping to the next statement
         *    after the 32 bit Privileged eXecute Never MPU region.
         *
         * 2. Call the prvUnprivilegedFunction() which is located inside the Privileged eXecute Never
         *    MPU region resulting in a memory fault.
         *
         * 3. Ensure that the execution from  the Privileged eXecute Never
         *    MPU region did generate MemFault and the fault handler did
         *    clear the ucPxnTaskFaultTracker[ 0 ].
         */
        ucPxnTaskFaultTracker[ 0 ] = 1;
        prvUnprivilegedFunction();
        RECOVERY_LABEL:
            configASSERT( ucPxnTaskFaultTracker[ 0 ] == 0 );
        printf(" Exception recovered gracefully \r\n");
        vTaskDelay(1000);
    }
}

int main()
{
    bsp_serial_init();

    static StackType_t xPxnTaskStack[ configMINIMAL_STACK_SIZE ] __attribute__( ( aligned( 32 ) ) );

    TaskParameters_t xPxnTaskParameters =
    {
        .pvTaskCode     = prvPxnTask,
        .pcName         = NULL,
        .usStackDepth   = configMINIMAL_STACK_SIZE,
        .pvParameters   = NULL,
        .uxPriority     = ( tskIDLE_PRIORITY | portPRIVILEGE_BIT ),
        .puxStackBuffer = xPxnTaskStack,
        .xRegions       =   {
                                { ( void * ) (__unprivileged_functions_start__),   32, tskMPU_REGION_PRIVILEGED_EXECUTE_NEVER                 },
                                { ( void * ) ucPxnTaskFaultTracker,                32, tskMPU_REGION_READ_WRITE | tskMPU_REGION_EXECUTE_NEVER },
                                { 0,                                               0,  0                                                      },
                            }
    };

    if(xTaskCreateRestricted( &( xPxnTaskParameters ), NULL ) == pdFAIL)
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

    /* Code execution will never reach this line */
    return EXIT_FAILURE;
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

portDONT_DISCARD static void prvHandleMemoryFault( uint32_t * pulFaultStackAddress )
{
    /* Is this an expected fault? */
    if( ucPxnTaskFaultTracker[ 0 ] == 1 )
    {
        /* Save the new program counter (recovery address) on the stack. */
        pulFaultStackAddress[ 6 ] = ( uint32_t ) pulRecoveryAddress;

        /* Mark the fault as handled. */
        ucPxnTaskFaultTracker[ 0 ] = 0;
    }
    else
    {
        /* This is an unexpected fault - loop forever. */
        for( ; ; )
        {
        }
    }
}

void MemManage_Handler( void ) __attribute__((naked));
void MemManage_Handler( void )
{
	__asm volatile
	(
	    ".align 8					                        \n"
		" tst lr, #4										\n"
		" ite eq											\n"
		" mrseq r0, msp										\n"
		" mrsne r0, psp										\n"
		" ldr r2, =prvHandleMemoryFault					    \n"
		" bx r2												\n"
	);
}
