//###########################################################################
// //
//	Copyright: Copyright (C) Texas Instruments Incorporated
//	All rights reserved not granted herein.
//
//  Redistribution and use in source and binary forms, with or without 
//  modification, are permitted provided that the following conditions 
//  are met:
//
//  Redistributions of source code must retain the above copyright 
//  notice, this list of conditions and the following disclaimer.
//
//  Redistributions in binary form must reproduce the above copyright
//  notice, this list of conditions and the following disclaimer in the 
//  documentation and/or other materials provided with the   
//  distribution.
//
//  Neither the name of Texas Instruments Incorporated nor the names of
//  its contributors may be used to endorse or promote products derived
//  from this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
//  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
//  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
//  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
//  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
//  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

//###########################################################################

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
 *
 * See http://www.freertos.org/a00110.html.
 *----------------------------------------------------------*/
/* Constants related to the behaviour or the scheduler. */
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 1
#define configTICK_RATE_HZ                      ( ( portTickType ) 1000 )
#define configUSE_PREEMPTION                    1
#define configUSE_TIME_SLICING                  1
#define configMAX_PRIORITIES                    16
#define configIDLE_SHOULD_YIELD                 1
#define configUSE_16_BIT_TICKS                  0 /* Only for 8 and 16-bit hardware. */

/* Constants used to specify if only static allocation is to be supported (in
which case a heap_n.c file is not required), only dynamic allocation is to be
supported, or if both static and dynamic allocation are supported. */
#define configSUPPORT_STATIC_ALLOCATION         1
#define configSUPPORT_DYNAMIC_ALLOCATION        1
#define configAPPLICATION_ALLOCATED_HEAP        1

/* Constants that describe the hardware and memory usage. */
#define configCPU_CLOCK_HZ                      ( ( unsigned long ) 200000000 )
#define configMINIMAL_STACK_SIZE                ( ( uint16_t ) 300 )
#define configMAX_TASK_NAME_LEN                 ( 12 )


#define configTOTAL_HEAP_SIZE                   ( ( size_t ) ( 180000 ) )
#define configHEAP_CLEAR_MEMORY_ON_FREE         1

/* configUSE_MINI_LIST_ITEM nust be defined as 0 for C29x port*/
#define configUSE_MINI_LIST_ITEM                0

/* Constants that build features in or out. */
#define configUSE_MUTEXES                       1
#define configUSE_TICKLESS_IDLE                 0
#define configUSE_APPLICATION_TASK_TAG          0
#define configUSE_NEWLIB_REENTRANT              0
#define configUSE_CO_ROUTINES                   0
#define configMAX_CO_ROUTINE_PRIORITIES     ( 2 )
#define configUSE_COUNTING_SEMAPHORES           1
#define configUSE_RECURSIVE_MUTEXES             1
#define configUSE_QUEUE_SETS                    1
#define configUSE_TASK_NOTIFICATIONS            1

/* Constants that define which hook (callback) functions should be used. */
#define configUSE_IDLE_HOOK                     1
#define configUSE_TICK_HOOK                     1
#define configUSE_MALLOC_FAILED_HOOK            1

/* Constants provided for debugging and optimisation assistance. */
#define configCHECK_FOR_STACK_OVERFLOW          2
void vMainAssertCalled( const char *pcFileName, uint32_t ulLineNumber );
#define configASSERT( x ) if( ( x ) == 0 ) { vMainAssertCalled( __FILE__, __LINE__ ); }
#define configQUEUE_REGISTRY_SIZE               10

/* Software timer definitions. */
#define configUSE_TIMERS                        1
#define configTIMER_TASK_PRIORITY               ( 3 )
#define configTIMER_QUEUE_LENGTH                5
#define configTIMER_TASK_STACK_DEPTH            ( configMINIMAL_STACK_SIZE  )

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function.  NOTE:  Setting an INCLUDE_ parameter to 0 is only
necessary if the linker does not automatically remove functions that are not
referenced anyway. */
#define INCLUDE_vTaskPrioritySet                1
#define INCLUDE_uxTaskPriorityGet               1
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_vTaskCleanUpResources           0
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_vTaskDelayUntil                 1
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_uxTaskGetStackHighWaterMark     0
#define INCLUDE_xTaskGetIdleTaskHandle          0
#define INCLUDE_xTaskGetHandle                  1
#define INCLUDE_eTaskGetState                   1
#define INCLUDE_xTaskResumeFromISR              0
#define INCLUDE_xTaskGetCurrentTaskHandle       1
#define INCLUDE_xTaskGetSchedulerState          0
#define INCLUDE_xSemaphoreGetMutexHolder        1
#define INCLUDE_xTimerPendFunctionCall          1


//
// Adding for test project
//
#define INCLUDE_xTaskAbortDelay                 1

/* This demo makes use of one or more example stats formatting functions.  These
format the raw data provided by the uxTaskGetSystemState() function in to human
readable ASCII form.  See the notes in the implementation of vTaskList() within
FreeRTOS/Source/tasks.c for limitations. */
#define configUSE_STATS_FORMATTING_FUNCTIONS    1

/* Dimensions a buffer that can be used by the FreeRTOS+CLI command
interpreter.  See the FreeRTOS+CLI documentation for more information:
http://www.FreeRTOS.org/FreeRTOS-Plus/FreeRTOS_Plus_CLI/ */
#define configCOMMAND_INT_MAX_OUTPUT_SIZE       2048

/* The trace facility is turned on to make some functions available for use in
CLI commands. */
#define configUSE_TRACE_FACILITY            1

/* Constants related to the generation of run time stats. */
#define configGENERATE_RUN_TIME_STATS           0
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()
#define portGET_RUN_TIME_COUNTER_VALUE()        0

/* The test that checks the trigger level on stream buffers requires an
allowable margin of error on slower processors (slower than the Win32
machine on which the test is developed). */
#define configSTREAM_BUFFER_TRIGGER_LEVEL_TEST_MARGIN   4

#define configTASK_NOTIFICATION_ARRAY_ENTRIES 3
#define configPRINTF( X ) myprintf( X )

#define TESTALL 1
#define TEST 100

// Minimal Test related configuration
#define configSTART_TASK_NOTIFY_TESTS             ((TESTALL==1) ||(TEST==1))
#define configSTART_TASK_NOTIFY_ARRAY_TESTS       ((TESTALL==1) ||(TEST==2))
#define configSTART_BLOCKING_QUEUE_TESTS          ((TESTALL==1) ||(TEST==3))
#define configSTART_SEMAPHORE_TESTS               ((TESTALL==1) ||(TEST==4))
#define configSTART_POLLED_QUEUE_TESTS            ((TESTALL==1) ||(TEST==5))
#define configSTART_INTEGER_MATH_TESTS            ((TESTALL==1) ||(TEST==6))
#define configSTART_GENERIC_QUEUE_TESTS           ((TESTALL==1) ||(TEST==7))
#define configSTART_PEEK_QUEUE_TESTS              ((TESTALL==1) ||(TEST==8))
#define configSTART_MATH_TESTS                    ((TESTALL==1) ||(TEST==9))
#define configSTART_RECURSIVE_MUTEX_TESTS         ((TESTALL==1) ||(TEST==10))
#define configSTART_COUNTING_SEMAPHORE_TESTS      ((TESTALL==1) ||(TEST==11))
#define configSTART_QUEUE_SET_TESTS               ((TESTALL==1) ||(TEST==12))
#define configSTART_QUEUE_OVERWRITE_TESTS         ((TESTALL==1) ||(TEST==13))
#define configSTART_EVENT_GROUP_TESTS             ((TESTALL==1) ||(TEST==14))
#define configSTART_INTERRUPT_SEMAPHORE_TESTS     ((TESTALL==1) ||(TEST==15))
#define configSTART_QUEUE_SET_POLLING_TESTS       ((TESTALL==1) ||(TEST==16))
#define configSTART_BLOCK_TIME_TESTS              ((TESTALL==1) ||(TEST==17))
#define configSTART_ABORT_DELAY_TESTS             ((TESTALL==1) ||(TEST==18))
#define configSTART_MESSAGE_BUFFER_TESTS          ((TESTALL==1) ||(TEST==19))
#define configSTART_STREAM_BUFFER_TESTS           ((TESTALL==1) ||(TEST==20))
#define configSTART_STREAM_BUFFER_INTERRUPT_TESTS ((TESTALL==1) ||(TEST==21))
#define configSTART_TIMER_TESTS                   ((TESTALL==1) ||(TEST==22))
#define configSTART_INTERRUPT_QUEUE_TESTS         ((TESTALL==1) ||(TEST==23))
#define configSTART_REGISTER_TESTS                ((TESTALL==1) ||(TEST==24))
#define configSTART_DELETE_SELF_TESTS             ((TESTALL==1) ||(TEST==25))


#endif /* FREERTOS_CONFIG_H */

