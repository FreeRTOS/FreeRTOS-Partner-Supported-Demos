/*
 * FreeRTOS V202212.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 * Copyright 2024-2025 Arm Limited and/or its affiliates <open-source-office@arm.com>
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

/******************************************************************************
*   See http://www.freertos.org/a00110.html for an explanation of the
*   definitions contained in this file.
******************************************************************************/

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
* Application specific definitions.
*
* These definitions should be adjusted for your particular hardware and
* application requirements.
*
* THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
* FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
* https://www.FreeRTOS.org/a00110.html
*----------------------------------------------------------*/

/* Ensure definitions are only used by the compiler, and not by the assembler. */
#if !defined(__ASSEMBLER__)
    #if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
        extern uint32_t SystemCoreClock;
        void vAssertCalled( const char * pcFile, unsigned long ulLine );
    #endif
#endif

/* See https://freertos.org/a00110.html#configPROTECTED_KERNEL_OBJECT_POOL_SIZE for details. */
#define configPROTECTED_KERNEL_OBJECT_POOL_SIZE   150
/* See https://freertos.org/a00110.html#configSYSTEM_CALL_STACK_SIZE for details. */
#define configSYSTEM_CALL_STACK_SIZE              128

/* Cortex M33 port configuration. */
#define configENABLE_MPU                                 1
#define configTOTAL_MPU_REGIONS                          16
#define configENABLE_FPU                                 1
#define configUSE_TASK_FPU_SUPPORT                       2
#define configENABLE_TRUSTZONE                           0
#define configENABLE_MVE                                 0

/* Run FreeRTOS on the secure side and never jump to the non-secure side. */
#define configRUN_FREERTOS_SECURE_ONLY                   0

/* Constants related to the behaviour or the scheduler. */
#define configUSE_PORT_OPTIMISED_TASK_SELECTION          0
#define configUSE_PREEMPTION                             1
#define configUSE_TIME_SLICING                           1
#define configMAX_PRIORITIES                             ( 10 )
#define configIDLE_SHOULD_YIELD                          1
#define configTICK_TYPE_WIDTH_IN_BITS                    TICK_TYPE_WIDTH_64_BITS

/* Constants that describe the hardware and memory usage. */
#define configCPU_CLOCK_HZ                               SystemCoreClock
#define configMINIMAL_STACK_SIZE                         ( ( uint16_t ) 512 )
#define configMAX_TASK_NAME_LEN                          ( 12 )
#define configTOTAL_HEAP_SIZE                            ( 0x20000 )
#define configAPPLICATION_ALLOCATED_HEAP                 0

/* Constants that build features in or out. */
#define configUSE_MUTEXES                                1
#define configUSE_TICKLESS_IDLE                          0
#define configUSE_APPLICATION_TASK_TAG                   0
#define configUSE_NEWLIB_REENTRANT                       0
#define configUSE_COUNTING_SEMAPHORES                    1
#define configUSE_RECURSIVE_MUTEXES                      1
#define configUSE_QUEUE_SETS                             0
#define configUSE_TASK_NOTIFICATIONS                     1
#define configUSE_TRACE_FACILITY                         1
#define configNUM_TX_DESCRIPTORS                         15
#define configSTREAM_BUFFER_TRIGGER_LEVEL_TEST_MARGIN    2

/* Constants that define which hook (callback) functions should be used. */
#define configUSE_IDLE_HOOK                              1
#define configUSE_TICK_HOOK                              1
#define configUSE_MALLOC_FAILED_HOOK                     1

/* Constants provided for debugging and optimisation assistance. */
#define configCHECK_FOR_STACK_OVERFLOW                   2
#define configASSERT( x )    if( ( x ) == 0 )            vAssertCalled( __FILE__, __LINE__ );
#define configQUEUE_REGISTRY_SIZE                        20

/* Software timer definitions. */
#define configUSE_TIMERS                                 1
#define configTIMER_TASK_PRIORITY                        ( configMAX_PRIORITIES - 1 )
#define configTIMER_QUEUE_LENGTH                         20
#define configTIMER_TASK_STACK_DEPTH                     ( configMINIMAL_STACK_SIZE * 2 )

/* Set the following definitions to 1 to include the API function, or zero
 * to exclude the API function.  NOTE:  Setting an INCLUDE_ parameter to 0 is
 * only necessary if the linker does not automatically remove functions that are
 * not referenced anyway. */
#define INCLUDE_vTaskPrioritySet                         1
#define INCLUDE_uxTaskPriorityGet                        1
#define INCLUDE_vTaskDelete                              1
#define INCLUDE_vTaskCleanUpResources                    0
#define INCLUDE_vTaskSuspend                             1
#define INCLUDE_vTaskDelayUntil                          1
#define INCLUDE_vTaskDelay                               1
#define INCLUDE_uxTaskGetStackHighWaterMark              1
#define INCLUDE_uxTaskGetStackHighWaterMark2             1
#define INCLUDE_xTaskGetIdleTaskHandle                   0
#define INCLUDE_eTaskGetState                            1
#define INCLUDE_xTaskResumeFromISR                       1
#define INCLUDE_xTaskGetCurrentTaskHandle                1
#define INCLUDE_xTaskGetSchedulerState                   1
#define INCLUDE_xSemaphoreGetMutexHolder                 1
#define INCLUDE_xTimerPendFunctionCall                   1
#define INCLUDE_xTimerGetTimerDaemonTaskHandle           1
#define INCLUDE_xTaskGetHandle                           1
#define INCLUDE_xTaskAbortDelay                          1

/* This demo makes use of one or more example stats formatting functions.  These
 * format the raw data provided by the uxTaskGetSystemState() function in to
 * human readable ASCII form.  See the notes in the implementation of vTaskList()
 * within FreeRTOS/Source/tasks.c for limitations. */
#define configUSE_STATS_FORMATTING_FUNCTIONS             1

/* Dimensions a buffer that can be used by the FreeRTOS+CLI command interpreter.
 * See the FreeRTOS+CLI documentation for more information:
 * https://www.FreeRTOS.org/FreeRTOS-Plus/FreeRTOS_Plus_CLI/ */
#define configCOMMAND_INT_MAX_OUTPUT_SIZE                2048

/* Interrupt priority configuration follows...................... */

/* Interrupt priorities used by the kernel port layer itself.  These are generic
* to all Cortex-M ports, and do not rely on any particular library functions. */
#define configKERNEL_INTERRUPT_PRIORITY                 ( configUNIQUE_INTERRUPT_PRIORITIES - 1 )

/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
 * See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY            ( configMAX_API_CALL_INTERRUPT_PRIORITY )

/* Constants related to the generation of run time stats. */
#define configGENERATE_RUN_TIME_STATS                   0
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()
#define portGET_RUN_TIME_COUNTER_VALUE()    0

/* Adjust configTICK_RATE_HZ and pdMS_TO_TICKS to simulate a tick per ms on a fast model */
#define configTICK_RATE_HZ    ( ( TickType_t ) 1000 )
#define pdMS_TO_TICKS( xTimeInMs )    ( ( TickType_t ) ( xTimeInMs * 10 ) )

/* Enable dynamic allocation. */
#define configSUPPORT_DYNAMIC_ALLOCATION    1

/* Hardware specific configurations. */
#define configFPU_D32                       0
#define configUNIQUE_INTERRUPT_PRIORITIES   32
#define configINTERRUPT_CONTROLLER_BASE_ADDRESS 0xAF000000UL
#define configINTERRUPT_CONTROLLER_CPU_INTERFACE_OFFSET 0x1000

#define configNUMBER_OF_CORES 4

/* SMP specific configurations. */
#if ( configNUMBER_OF_CORES > 1 )
    #define configUSE_CORE_AFFINITY 0
    #define configRUN_MULTIPLE_PRIORITIES 1
    #define configUSE_PASSIVE_IDLE_HOOK 0
    #define configTIMER_SERVICE_TASK_CORE_AFFINITY 0
#endif

#if !defined(__ASSEMBLER__)
    #if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
        /*
        * The application must provide a function that configures a peripheral to
        * create the FreeRTOS tick interrupt, then define configSETUP_TICK_INTERRUPT()
        * in FreeRTOSConfig.h to call the function.  FreeRTOS_Tick_Handler() must
        * be installed as the peripheral's interrupt handler.
        */
        void vConfigureTickInterrupt( void );
        #define configSETUP_TICK_INTERRUPT() vConfigureTickInterrupt()
    #endif
#endif

/*
 * Interrupts that are assigned a priority at or below
 * configMAX_API_CALL_INTERRUPT_PRIORITY (which counter-intuitively in the ARM
 * generic interrupt controller [GIC] means a priority that has a numerical
 * value above configMAX_API_CALL_INTERRUPT_PRIORITY) can call FreeRTOS safe API
 * functions and will nest.
 *
 * Interrupts that are assigned a priority above
 * configMAX_API_CALL_INTERRUPT_PRIORITY (which in the GIC means a numerical
 * value below configMAX_API_CALL_INTERRUPT_PRIORITY) cannot call any FreeRTOS
 * API functions, will nest, and will not be masked by FreeRTOS critical
 * sections (although it is necessary for interrupts to be globally disabled
 * extremely briefly as the interrupt mask is updated in the GIC).
 *
 * FreeRTOS functions that can be called from an interrupt are those that end in
 * "FromISR".  FreeRTOS maintains a separate interrupt safe API to enable
 * interrupt entry to be shorter, faster, simpler and smaller.
 *
 * For the purpose of setting configMAX_API_CALL_INTERRUPT_PRIORITY 255
 * represents the lowest priority.
 */
#define configMAX_API_CALL_INTERRUPT_PRIORITY	18ULL

#ifndef __ASSEMBLER__
    void vClearTickInterrupt( void );
    #define configCLEAR_TICK_INTERRUPT() vClearTickInterrupt()
#endif /* __ASSEMBLER__ */

#endif /* FREERTOS_CONFIG_H */
