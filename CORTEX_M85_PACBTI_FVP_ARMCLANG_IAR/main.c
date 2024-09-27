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

#include "tfm_ns_interface.h"

#define UFSR_REG                 ( *( ( volatile uint16_t * ) 0xe000ed2a ) )
#define UFSR_INVSTATE_BIT_POS    ( 1U )

extern uint32_t tfm_ns_interface_init( void );

static volatile uint8_t ucPacBtiFaultTracker = 0;
TaskHandle_t xPacTaskHandle = NULL;
TaskHandle_t xBtiTaskHandle = NULL;
void ( *pxCheckExceptionFunctionPtr )( void ) = NULL;

portDONT_DISCARD static void prvHandleUsageFault( uint32_t * pulFaultStackAddress );

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

portDONT_DISCARD __attribute__ ((noinline)) static void prvCorruptLR( void )
{
    __asm volatile
    (
        " pac r12, lr, sp               \n"
        " stmdb sp!, {r12, lr}          \n"
        " mov r1, #0                    \n"
        " str r1, [sp, #4]              \n"
        " ldmia.w sp!, {r12, lr}        \n"
        " aut r12, lr, sp               \n"
        " bx lr                         \n"
    );
}

static void prvPacTaskCheckException( void )
{
    configASSERT( ucPacBtiFaultTracker == 0 );
    printf("\r\nUsage Fault Exception triggered due to incorrect computed PAC, was recovered gracefully.\r\n");
    vTaskDelete( xPacTaskHandle );
}

static void prvBtiTaskCheckException( void )
{
    configASSERT( ucPacBtiFaultTracker == 0 );
    printf("\r\nUsage Fault Exception triggered due to middle of function jump, was recovered gracefully.\r\n");
    printf("\r\nPAC and BTI tasks have successfully executed, deleting tasks.\r\n");
    vTaskDelete( xBtiTaskHandle );
}

static void prvPacTask( void * arg )
{
    /* Prevent the compiler warning about the unused parameter. */
    ( void ) arg;

    printf( "PSA Framework version is: %d\n", psa_framework_version() );
    #if( configENABLE_PAC == 1 )
        /* Assign the address of prvPacTaskCheckException function to the pxCheckExceptionFunctionPtr function pointer.
        */
        pxCheckExceptionFunctionPtr = prvPacTaskCheckException;

        /* This task performs the following sequence:
        *
        * 1. Setting ucPacBtiFaultTracker variable to 1 informs the Usage Fault
        *    Exception Handler that this is an expected fault. Where in this case, the handler
        *    recovers from the expected fault gracefully by jumping to the next statement
        *    to be executed after the faulty piece of code.
        *
        * 2. Call the prvCorruptLR() function which corrupts the Link Register (LR)
        *    resulting in a Usage Fault Exception being triggered as the `aut` authentication instruction
        *    will fail to validate the computed pointer authentication code against the one generated
        *    at the entry of the function as the Link Register is corrupted.
        *
        * 3. As part of the Usage Fault Exception handler, the program counter is changed to jump to the
        *    prvPacTaskCheckException function which in turns ensure that the LR corruption did generate a Usage
        *    Fault exception by checking if the fault handler did clear the ucPacBtiFaultTracker variable, and
        *    then kills the task.
        */
        ucPacBtiFaultTracker = 1;
        printf("\r\nAttempting to corrupt Link Register (LR).\r\n");
        prvCorruptLR();
    #else
        printf("\r\nPAC is not enabled. Hence, deleteing the prvPacTask.\r\n");
        vTaskDelete( xPacTaskHandle );
    #endif /* configENABLE_PAC */
}

static void prvBtiTask( void * arg )
{
    /* Prevent the compiler warning about the unused parameter. */
    ( void ) arg;

    #if( configENABLE_BTI == 1 )
        /* Assign the address of prvBtiTaskCheckException function to the pxCheckExceptionFunctionPtr function
        * pointer.
        */
        pxCheckExceptionFunctionPtr = prvBtiTaskCheckException;

        /* This task performs the following sequence:
        *
        * 1. Setting ucPacBtiFaultTracker variable to 1 informs the Usage Fault
        *    Exception Handler that this is an expected fault. Where in this case, the handler
        *    recovers from the expected fault gracefully by jumping to the next statement
        *    to be executed after the faulty piece of code.
        *
        * 2. Load the starting address of `prvHandleUsageFault` function, increment the address by `4`
        *    to jump to the middle of the function resulting in a Usage Fault Exception being triggered
        *    as the  address jumped to is not a BTI clearing instruction `bti`.
        *
        * 3. As part of the Usage Fault Exception handler, the program counter is changed to jump to the
        *    prvBtiTaskCheckException function which in turns ensure that the jump to the middle of the function
        *    did generate a Usage Fault exception by checking if the fault handler did clear the
        *    ucPacBtiFaultTracker variable, and then kills the task.
        */

        ucPacBtiFaultTracker = 1;
        printf("\r\nAttempting to perform an indirect jump to middle of prvHandleUsageFault function.\r\n");
        __asm volatile
        (
            " ldr r1, =prvHandleUsageFault  \n"
            " add r1, #4                    \n"
            " bx r1                         \n"
        );
    #else
        printf("\r\nBTI is not enabled. Hence, deleteing the prvBtiTask.\r\n");
        vTaskDelete( xBtiTaskHandle );
    #endif /* configENABLE_BTI */
}

int main()
{
    bsp_serial_init();

    uint32_t ret = tfm_ns_interface_init();

    if( ret != 0 )
    {
        printf( "tfm_ns_interface_init() failed: %u\r\n", ret );
        return EXIT_FAILURE;
    }

    if(xTaskCreate( prvPacTask,
                    NULL,
                    configMINIMAL_STACK_SIZE,
                    NULL,
                    ( tskIDLE_PRIORITY + 2 ),
                    &xPacTaskHandle ) == pdFAIL )
    {
        return EXIT_FAILURE;
    }

    if(xTaskCreate( prvBtiTask,
                    NULL,
                    configMINIMAL_STACK_SIZE,
                    NULL,
                    ( tskIDLE_PRIORITY + 1 ),
                    &xBtiTaskHandle ) == pdFAIL )
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

portDONT_DISCARD static void prvHandleUsageFault( uint32_t * pulFaultStackAddress )
{
    volatile uint16_t ulUfsrReg;

    /* Read the UFSR (Usage Fault Status Register) register value. */
    ulUfsrReg = UFSR_REG;

    /* Is this Hard Fault triggered due to an invalid state? */
    if( ulUfsrReg >> UFSR_INVSTATE_BIT_POS == 1)
    {
        /* Is this an expected fault? */
        if( ucPacBtiFaultTracker == 1 )
        {
            /* Mark the fault as handled. */
            ucPacBtiFaultTracker = 0;

            /* Save the new program counter (recovery address) on the stack. */
            pulFaultStackAddress[ 6 ] = ( uint32_t ) pxCheckExceptionFunctionPtr;
        }
        else
        {
            /* This is an unexpected fault - loop forever. */
            for( ; ; )
            {
            }
        }
    }
    else
    {
        /* This fault is not due to an invalid state - loop forever. */
        for( ; ; )
        {
        }
    }
}

void UsageFault_Handler( void ) __attribute__((naked));
void UsageFault_Handler( void )
{
	__asm volatile
	(
		" tst lr, #4										\n"
		" ite eq											\n"
		" mrseq r0, msp										\n"
		" mrsne r0, psp										\n"
		" ldr r2, =prvHandleUsageFault					    \n"
		" bx r2												\n"
	);
}
