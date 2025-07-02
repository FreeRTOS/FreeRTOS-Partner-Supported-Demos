//#############################################################################
//
// FILE:   main.c
//
// TITLE:  FreeRTOS C29x Port Validation Tests
//
//! This example validates the FreeRTOS C29x port and is based on the project
//! template detailed in the below shown link:
//! https://github.com/FreeRTOS/FreeRTOS/blob/main/FreeRTOS/Demo/ThirdParty/Template/README.md
//! 
//! The project configures following tests for the C29x port
//!
//! #define configSTART_TASK_NOTIFY_TESTS             1
//! #define configSTART_TASK_NOTIFY_ARRAY_TESTS       1
//! #define configSTART_BLOCKING_QUEUE_TESTS          1
//! #define configSTART_SEMAPHORE_TESTS               1
//! #define configSTART_POLLED_QUEUE_TESTS            1
//! #define configSTART_INTEGER_MATH_TESTS            1
//! #define configSTART_GENERIC_QUEUE_TESTS           1
//! #define configSTART_PEEK_QUEUE_TESTS              1
//! #define configSTART_MATH_TESTS                    1
//! #define configSTART_RECURSIVE_MUTEX_TESTS         1
//! #define configSTART_COUNTING_SEMAPHORE_TESTS      1
//! #define configSTART_QUEUE_SET_TESTS               1
//! #define configSTART_QUEUE_OVERWRITE_TESTS         1
//! #define configSTART_EVENT_GROUP_TESTS             1
//! #define configSTART_INTERRUPT_SEMAPHORE_TESTS     1
//! #define configSTART_QUEUE_SET_POLLING_TESTS       1
//! #define configSTART_BLOCK_TIME_TESTS              1
//! #define configSTART_ABORT_DELAY_TESTS             1
//! #define configSTART_MESSAGE_BUFFER_TESTS          1
//! #define configSTART_STREAM_BUFFER_TESTS           1
//! #define configSTART_STREAM_BUFFER_INTERRUPT_TESTS 1
//! #define configSTART_TIMER_TESTS                   1
//! #define configSTART_INTERRUPT_QUEUE_TESTS         1
//! #define configSTART_REGISTER_TESTS                1
//! #define configSTART_DELETE_SELF_TESTS             1
//! 
//! For running the application open the COM port with the following settings
//! using a terminal:
//!  -  Find the correct COM port
//!  -  Bits per second = 9600
//!  -  Data Bits = 8
//!  -  Parity = None
//!  -  Stop Bits = 1
//!  -  Hardware Control = None
//!  The program will print out the test results on COM port
//! 
//! External Connections:
//!  -  None : The configured UART pins are routed to the UART on the
//!            debugger.
//! 
//! Watch Variables:
//!  - pcStatusMessage  : Captures the validation result. The same result
//!                       can also be checked over COM port.
//!  - loopCount        : Number of interations of Check task completed.
//
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

//
// Included Files
//
#include "device.h"
#include "driverlib.h"

#include "FreeRTOS.h"
#include "task.h"
#include "TestRunner.h"

//
// Globals
//
static StaticTask_t xIdleTaskTCB;
__attribute__((aligned(portBYTE_ALIGNMENT))) static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

static StaticTask_t xTimerTaskTCB;
__attribute__((aligned(portBYTE_ALIGNMENT))) static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];


#if(configAPPLICATION_ALLOCATED_HEAP == 1)
__attribute__((aligned(portBYTE_ALIGNMENT))) uint8_t ucHeap[ configTOTAL_HEAP_SIZE ];
#endif

//	
// UARTA  - GPIO Settings
//
#define myUART0_UARTTX_GPIO         DEVICE_GPIO_PIN_UARTA_TX
#define myUART0_UARTTX_PIN_CONFIG   DEVICE_GPIO_CFG_UARTA_TX
#define myUART0_UARTRX_GPIO         DEVICE_GPIO_PIN_UARTA_RX
#define myUART0_UARTRX_PIN_CONFIG   DEVICE_GPIO_CFG_UARTA_RX

//
// Function Declarations
//
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName);
void vApplicationMallocFailedHook( void );
void vApplicationIdleHook( void );
void vApplicationTickHook(void);
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer,
                                     StackType_t **ppxTimerTaskStackBuffer,
                                     configSTACK_DEPTH_TYPE *pulTimerTaskStackSize );
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    configSTACK_DEPTH_TYPE *pulIdleTaskStackSize );

void vMainAssertCalled( const char *pcFileName, uint32_t ulLineNumber );

__attribute__((weak)) void *malloc( size_t xSize );

extern int checkPrintf();
int myprintf(const char *format, ...);

void UART_init(){
    GPIO_setPinConfig(myUART0_UARTTX_PIN_CONFIG);
	GPIO_setPadConfig(myUART0_UARTTX_GPIO, GPIO_PIN_TYPE_STD);
	GPIO_setQualificationMode(myUART0_UARTTX_GPIO, GPIO_QUAL_ASYNC);

	GPIO_setPinConfig(myUART0_UARTRX_PIN_CONFIG);
	GPIO_setPadConfig(myUART0_UARTRX_GPIO, GPIO_PIN_TYPE_STD | GPIO_PIN_TYPE_PULLUP);
	GPIO_setQualificationMode(myUART0_UARTRX_GPIO, GPIO_QUAL_ASYNC);

    UART_setConfig(
        UARTA_BASE,                 // Base address
        DEVICE_SYSCLK_FREQ,         // UART source clock
        9600,                       // Baud rate
        ( UART_CONFIG_WLEN_8        // Word length
          | UART_CONFIG_STOP_ONE    // Stop bits
        )
    );
    
    //
    // Enable RX, TX, and the UART.
    //
    UART_enableModuleNonFIFO(UARTA_BASE);
}

int main(void)
{
    Device_init();

    UART_init();

    //
    // Uncomment to check printf() functionality
    // if no output is seen on COM port.
    // checkPrintf();
    myprintf(" Test start\n");

    //
    // Enable PIPE Global Interrupt (for INTs and RTINTs) and INT enable in CPU.
    //
    ENINT;
    Interrupt_enableGlobal();

    //
    // Start tests
    //
    vStartTests();

    /* Should never reach here. */
    for( ; ; );
}

//
// Application must provide an implementation of vApplicationGetIdleTaskMemory()
// to provide the memory that is used by the Idle task if configUSE_STATIC_ALLOCATION
// is set to 1.
//
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    configSTACK_DEPTH_TYPE *pulIdleTaskStackSize )
{
    /* If the buffers to be provided to the Idle task are declared inside this
    function then they must be declared static - otherwise they will be allocated on
    the stack and so not exists after this function exits. */

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
    state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

//
// Application must provide an implementation of vApplicationGetTimerTaskMemory()
// to provide the memory that is used by the Timer service task if both
// configUSE_STATIC_ALLOCATION and configUSE_TIMERS are set to 1.
//
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer,
                                     StackType_t **ppxTimerTaskStackBuffer,
                                     configSTACK_DEPTH_TYPE *pulTimerTaskStackSize )
{
    /* If the buffers to be provided to the Timer task are declared inside this
    function then they must be declared static - otherwise they will be allocated on
    the stack and so not exists after this function exits. */

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
    task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

//
// Hook function for catching pvPortMalloc() failures
//
void vApplicationMallocFailedHook( void )
{
    /* vApplicationMallocFailedHook() will only be called if
    configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h. It is a hook
    function that will get called if a call to pvPortMalloc() fails.
    pvPortMalloc() is called internally by the kernel whenever a task, queue,
    timer or semaphore is created.  It is also called by various parts of the
    demo application.  If heap_1.c or heap_2.c are used, then the size of the
    heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
    FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
    to query the size of free heap space that remains (although it does not
    provide information on how the remaining heap might be fragmented). */
    taskDISABLE_INTERRUPTS();
    for( ;; );
}

//
// Hook function for idle task
//
void vApplicationIdleHook( void )
{
    /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
    to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
    task.  It is essential that code added to this hook function never attempts
    to block in any way (for example, call xQueueReceive() with a block time
    specified, or call vTaskDelay()).  If the application makes use of the
    vTaskDelete() API function (as this demo application does) then it is also
    important that vApplicationIdleHook() is permitted to return to its calling
    function, because it is the responsibility of the idle task to clean up
    memory allocated by the kernel to any task that has since been deleted. */
}

//
// Checks run time stack overflow
//
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
    ( void ) pcTaskName;
    ( void ) pxTask;

    /* Run time stack overflow checking is performed if
    configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
    function is called if a stack overflow is detected. */
    taskDISABLE_INTERRUPTS();
    for( ;; );
}

//
// Catch asserts so the file and line number of the assert can be viewed.
//
void vMainAssertCalled( const char *pcFileName, uint32_t ulLineNumber )
{
    volatile BaseType_t xSetToNonZeroToStepOutOfLoop = 0;

    taskENTER_CRITICAL();
    while( xSetToNonZeroToStepOutOfLoop == 0 )
    {
        /* Use the variables to prevent compiler warnings and in an attempt to
        ensure they can be viewed in the debugger.  If the variables get
        optimised away then set copy their values to file scope or globals then
        view the variables they are copied to. */
        ( void ) pcFileName;
        ( void ) ulLineNumber;
    }
}

//
// Traps malloc calls
//
void *malloc( size_t xSize )
{
    /* There should not be a heap defined, so trap any attempts to call
    malloc. */
    taskDISABLE_INTERRUPTS();
    for( ;; );
}

//
// End of File
//
