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

#if defined(__ARMCC_VERSION)
    /* Externs needed by the MPU setup code. These are defined in Scatter-Loading
    * description file (armclang.sct). */
    extern uint32_t Image$$flash$$Base;
    extern uint32_t Image$$ER_IROM_NS_PRIVILEGED_ALIGN$$Limit;
    extern uint32_t Image$$ER_IROM_NS_FREERTOS_SYSTEM_CALLS$$Base;
    extern uint32_t Image$$ER_IROM_NS_FREERTOS_SYSTEM_CALLS_ALIGN$$Limit;
    extern uint32_t Image$$ER_IROM_NS_PRIVILEGED_EXECUTE_NEVER$$Base;
    extern uint32_t Image$$ER_IROM_NS_PRIVILEGED_EXECUTE_NEVER_ALIGN$$Limit;
    extern uint32_t Image$$ER_IROM_NS_UNPRIVILEGED$$Base;
    extern uint32_t Image$$ER_IROM_NS_UNPRIVILEGED_ALIGN$$Limit;

    extern uint32_t Image$$ER_IRAM_NS_PRIVILEGED$$Base;
    extern uint32_t Image$$ER_IRAM_NS_PRIVILEGED_ALIGN$$Limit;
    extern uint32_t Image$$ER_IRAM_NS_UNPRIVILEGED$$Base;
    extern uint32_t Image$$ER_IRAM_NS_UNPRIVILEGED_ALIGN$$Limit;

    /* Privileged flash. */
    const uint32_t * __privileged_functions_start__		= ( uint32_t * ) &( Image$$flash$$Base );
    const uint32_t * __privileged_functions_end__		= ( uint32_t * ) ( ( uint32_t ) &( Image$$ER_IROM_NS_PRIVILEGED_ALIGN$$Limit ) - 0x1 ); /* Last address in privileged Flash region. */

    /* Flash containing system calls. */
    const uint32_t * __syscalls_flash_start__			= ( uint32_t * ) &( Image$$ER_IROM_NS_FREERTOS_SYSTEM_CALLS$$Base );
    const uint32_t * __syscalls_flash_end__				= ( uint32_t * ) ( ( uint32_t ) &( Image$$ER_IROM_NS_FREERTOS_SYSTEM_CALLS_ALIGN$$Limit ) - 0x1 ); /* Last address in Flash region containing system calls. */

    /* Unprivileged executed functions. Note that the section contains functions
    * that cannot be executed by a privileged task */
    const uint32_t * __privileged_execute_never_start__		= ( uint32_t * ) &( Image$$ER_IROM_NS_PRIVILEGED_EXECUTE_NEVER$$Base );
    const uint32_t * __privileged_execute_never_end__	    = ( uint32_t * ) ( ( uint32_t ) &( Image$$ER_IROM_NS_PRIVILEGED_EXECUTE_NEVER_ALIGN$$Limit ) - 0x1 ); /* Last address in privileged execute never Flash region. */

    /* Unprivileged flash. Note that the section containing system calls is
    * unprivileged so that unprivileged tasks can make system calls. */
    const uint32_t * __unprivileged_flash_start__		= ( uint32_t * ) &( Image$$ER_IROM_NS_UNPRIVILEGED$$Base );
    const uint32_t * __unprivileged_flash_end__			= ( uint32_t * ) ( ( uint32_t ) &( Image$$ER_IROM_NS_UNPRIVILEGED_ALIGN$$Limit ) - 0x1 ); /* Last address in un-privileged Flash region. */

    /* RAM with priviledged access only. This contains kernel data. */
    const uint32_t * __privileged_sram_start__			= ( uint32_t * ) &( Image$$ER_IRAM_NS_PRIVILEGED$$Base );
    const uint32_t * __privileged_sram_end__			= ( uint32_t * ) ( ( uint32_t ) &( Image$$ER_IRAM_NS_PRIVILEGED_ALIGN$$Limit ) - 0x1 ); /* Last address in privileged RAM. */

    /* Unprivileged RAM. */
    const uint32_t * __unprivileged_sram_start__		= ( uint32_t * ) &( Image$$ER_IRAM_NS_UNPRIVILEGED$$Base );
    const uint32_t * __unprivileged_sram_end__			= ( uint32_t * ) ( ( uint32_t ) &( Image$$ER_IRAM_NS_UNPRIVILEGED_ALIGN$$Limit ) - 0x1 ); /* Last address in un-privileged RAM. */
#else
    extern uint32_t __privileged_execute_never_start__[];
    extern uint32_t __privileged_execute_never_end__[];
#endif /* defined(__CC_ARM) */


/* FreeRTOS tasks handles */
TaskHandle_t xPacTaskHandle = NULL;
TaskHandle_t xBtiTaskHandle = NULL;
TaskHandle_t xPxnTaskHandle = NULL;

/**
 * @brief Memory region used to track Memory, and Usage faults intentionally caused by the
 * Pxn, Pac, and Bti tasks.
 *
 * @note We are declaring two regions of 32 bytes even though we need only one byte for each region. The
 * reason why, is that the size of an MPU region must be a multiple of 32 bytes.
 */
static volatile uint8_t ucPxnTaskFaultTracker[ 32 ] __attribute__( ( aligned( 32 ) ) ) = { 0 };

static volatile uint8_t ucPacBtiFaultTracker[ 32 ] __attribute__( ( aligned( 32 ) ) ) = { 0 };

void ( *pxCheckExceptionFunctionPtr )( void ) = NULL;

portDONT_DISCARD static void prvHandleUsageFault( uint32_t * pulFaultStackAddress );
portDONT_DISCARD static void prvHandleMemoryFault( uint32_t * pulFaultStackAddress );

extern uint32_t tfm_ns_interface_init( void );

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
portDONT_DISCARD __attribute__ ( ( noinline ) ) __attribute__ ( ( section( "privileged_execute_never_functions" ) ) ) static void prvUnprivilegedFunction ( void ) 
{
    __asm volatile
    (
        " mov r1, #0                   \n"
    );
}

portDONT_DISCARD __attribute__ ( ( noinline ) ) static void prvCorruptLR( void )
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
    configASSERT( ucPacBtiFaultTracker[0] == 0 );
    printf("\r\nUsage Fault Exception triggered due to incorrect computed PAC, was recovered gracefully.\r\n");
    vTaskDelete( xPacTaskHandle );
}

static void prvBtiTaskCheckException( void )
{
    configASSERT( ucPacBtiFaultTracker[0] == 0 );
    printf("\r\nUsage Fault Exception triggered due to middle of function jump, was recovered gracefully.\r\n");
    vTaskDelete( xBtiTaskHandle );
}

static void prvPxnTaskCheckException( void )
{
    configASSERT( ucPxnTaskFaultTracker[ 0 ] == 0 );
    printf("\r\nMemory Fault Exception triggered due to unpermitted privleged execution, was recovered gracefully.\r\n");
    printf("\r\nAll tasks have successfully executed, deleting existing tasks.\r\n");
    vTaskDelete( xPxnTaskHandle );
}

static void prvPacTask( void * arg )
{
    /* Prevent the compiler warning about the unused parameter. */
    ( void ) arg;

    printf( "PSA Framework version is: %d\n", psa_framework_version() );
    #if( configENABLE_PAC == 1 )
        /* Assign the address of prvPacTaskCheckException function to the pxCheckExceptionFunctionPtr function
        * pointer.
        */
        pxCheckExceptionFunctionPtr = prvPacTaskCheckException;

        /* This task performs the following sequence:
        *
        * 1. Setting ucPacBtiFaultTracker[0] variable to 1 informs the Usage Fault
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
        *    Fault exception by checking if the fault handler did clear the ucPacBtiFaultTracker[0] variable, and
        *    then kills the task.
        */
        ucPacBtiFaultTracker[0] = 1;
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
        * 1. Setting ucPacBtiFaultTracker[0] variable to 1 informs the Usage Fault
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
        *    ucPacBtiFaultTracker[0] variable, and then kills the task.
        */

        ucPacBtiFaultTracker[0] = 1;
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

static void prvPxnTask( void * arg )
{
    /* Prevent the compiler warning about the unused parameter. */
    ( void ) arg;

    /* Assign the address of prvPxnTaskCheckException function to the pxCheckExceptionFunctionPtr function
    * pointer.
    */
    pxCheckExceptionFunctionPtr = prvPxnTaskCheckException;

    /* This task performs the following sequence:
    *
    * 1. Setting ucPxnFaultTracker[0] variable to 1 informs the Memory Fault
    *    Exception Handler that this is an expected fault. Where in this case, the handler
    *    recovers from the expected fault gracefully by jumping to the next statement
    *    after the 32 bit Privileged eXecute Never MPU region.
    *
    * 2. Call the prvUnprivilegedFunction() which is located inside the Privileged eXecute Never
    *    MPU region resulting into a memory fault.
    *
    * 3. As part of the Memory Fault Exception handler, the program counter is changed to jump to the
    *    prvPxnTaskCheckException function which in turns ensure that the execution of a function placed
    *    inside a MPU region marked as Privileged Execute Never by a privileged task did generate a
    *    Memory Fault exception by checking if the fault handler did clear the
    *    ucPxnFaultTracker[0] variable, and then kills the task.
    */
    ucPxnTaskFaultTracker[ 0 ] = 1;
    printf("\r\nAttempting to execute a privileged execute never function.\r\n");
    prvUnprivilegedFunction();
}

int main()
{
    bsp_serial_init();

    uint32_t ret = tfm_ns_interface_init();

    /* FreeRTOS tasks' stacks */
    static StackType_t xPacTaskStack[ configMINIMAL_STACK_SIZE ] __attribute__( ( aligned( 32 ) ) );
    static StackType_t xBtiTaskStack[ configMINIMAL_STACK_SIZE ] __attribute__( ( aligned( 32 ) ) );
    static StackType_t xPxnTaskStack[ configMINIMAL_STACK_SIZE ] __attribute__( ( aligned( 32 ) ) );

    if( ret != 0 )
    {
        printf( "tfm_ns_interface_init() failed: %u\r\n", ret );
        return EXIT_FAILURE;
    }

    TaskParameters_t xPacTaskParameters =
    {
        .pvTaskCode     = prvPacTask,
        .pcName         = NULL,
        .usStackDepth   = configMINIMAL_STACK_SIZE,
        .pvParameters   = NULL,
        .uxPriority     = ( ( tskIDLE_PRIORITY + 3 ) | portPRIVILEGE_BIT ),
        .puxStackBuffer = xPacTaskStack,
        .xRegions       =   {
                                { ( void * ) ucPacBtiFaultTracker,                 32, tskMPU_REGION_READ_WRITE | tskMPU_REGION_EXECUTE_NEVER },
                                { 0,                                               0,  0                                                      },
                                { 0,                                               0,  0                                                      },
                            }
    };

    if(xTaskCreateRestricted( &( xPacTaskParameters ), &xPacTaskHandle ) == pdFAIL)
    {
        return EXIT_FAILURE;
    }

    TaskParameters_t xBtiTaskParameters =
    {
        .pvTaskCode     = prvBtiTask,
        .pcName         = NULL,
        .usStackDepth   = configMINIMAL_STACK_SIZE,
        .pvParameters   = NULL,
        .uxPriority     = ( ( tskIDLE_PRIORITY + 2 ) | portPRIVILEGE_BIT ),
        .puxStackBuffer = xBtiTaskStack,
        .xRegions       =   {
                                { ( void * ) ucPacBtiFaultTracker,                 32, tskMPU_REGION_READ_WRITE | tskMPU_REGION_EXECUTE_NEVER },
                                { 0,                                               0,  0                                                      },
                                { 0,                                               0,  0                                                      },
                            }
    };

    if(xTaskCreateRestricted( &( xBtiTaskParameters ), &xBtiTaskHandle ) == pdFAIL)
    {
        return EXIT_FAILURE;
    }

    TaskParameters_t xPxnTaskParameters =
    {
        .pvTaskCode     = prvPxnTask,
        .pcName         = NULL,
        .usStackDepth   = configMINIMAL_STACK_SIZE,
        .pvParameters   = NULL,
        .uxPriority     = ( ( tskIDLE_PRIORITY + 1 ) | portPRIVILEGE_BIT ),
        .puxStackBuffer = xPxnTaskStack,
        .xRegions       =   {
                                { ( void * ) (__privileged_execute_never_start__), 32, tskMPU_REGION_PRIVILEGED_EXECUTE_NEVER                 },
                                { ( void * ) ucPxnTaskFaultTracker,                32, tskMPU_REGION_READ_WRITE | tskMPU_REGION_EXECUTE_NEVER },
                                { 0,                                               0,  0                                                      },
                            }
    };

    if(xTaskCreateRestricted( &( xPxnTaskParameters ), &xPxnTaskHandle ) == pdFAIL)
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
    uint16_t ulUfsrReg;

    /* Read the UFSR (Usage Fault Status Register) register value. */
    ulUfsrReg = UFSR_REG;

    /* Is this Hard Fault triggered due to an invalid state? */
    if( ulUfsrReg >> UFSR_INVSTATE_BIT_POS == 1)
    {
        /* Is this an expected fault? */
        if( ucPacBtiFaultTracker[0] == 1 )
        {
            /* Mark the fault as handled. */
            ucPacBtiFaultTracker[0] = 0;

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

portDONT_DISCARD static void prvHandleMemoryFault( uint32_t * pulFaultStackAddress )
{
    /* Is this an expected fault? */
    if( ucPxnTaskFaultTracker[ 0 ] == 1 )
    {
        /* Mark the fault as handled. */
        ucPxnTaskFaultTracker[ 0 ] = 0;

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

void MemManage_Handler( void ) __attribute__((naked));
void MemManage_Handler( void )
{
	__asm volatile
	(
		" tst lr, #4										\n"
		" ite eq											\n"
		" mrseq r0, msp										\n"
		" mrsne r0, psp										\n"
		" ldr r2, =prvHandleMemoryFault					    \n"
		" bx r2												\n"
	);
}
