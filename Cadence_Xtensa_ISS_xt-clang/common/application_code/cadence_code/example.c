
//-----------------------------------------------------------------------------
// Copyright (c) 2003-2024 Cadence Design Systems, Inc.
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// This is a simple example to demonstrate the Xtensa port of FreeRTOS.
//-----------------------------------------------------------------------------

#include <stdint.h>
#include <string.h>

#include "FreeRTOS.h"
#include "semphr.h"
#include "event_groups.h"

#include "testcommon.h"

#if (XT_USE_THREAD_SAFE_CLIB > 0)
#include <stdio.h>
#endif

// If MPU support is enabled, these tasks will have to be created as
// privileged. xTaskCreate() does not support nonprivileged task creation.

#define INIT_TASK_PRIO          (4 | portPRIVILEGE_BIT)
#define COUNT_TASK_PRIO         (3 | portPRIVILEGE_BIT)
#define REPORT_TASK_PRIO        (2 | portPRIVILEGE_BIT)

#ifdef XT_BOARD
#include <xtensa/xtbsp.h>
// A low priority task puts something on the display for equipped boards.
#define DISPLAY_TASK_PRIO       (1 | portPRIVILEGE_BIT)
#endif

// Default task stack size.
#define TASK_STK_SIZE            ((XT_STACK_MIN_SIZE + 0x400) / sizeof(StackType_t))

// Flags set by each task on completion.
#define TASK_TERM_INIT          (1<<0)
#define TASK_TERM_REPORT        (1<<1)
#define TASK_TERM_COUNT         (1<<2)

// Message queue size.
#define QUEUE_SIZE              10

EventGroupHandle_t TaskTermFlags;
QueueHandle_t      Queue;
TaskHandle_t       Count_Task_TCB;
TaskHandle_t       Report_Task_TCB;

#if (XT_USE_THREAD_SAFE_CLIB > 0)
#define PRINTF(s)               printf(s);fflush(stdout)
#define PUTCHAR(c)              putchar(c);fflush(stdout)
#else
#define PRINTF(s)
#define PUTCHAR(c)
#endif


//-----------------------------------------------------------------------------
// Display Task. Display something on display-equipped boards to show the
// FreeRTOS Xtensa example is running.
//-----------------------------------------------------------------------------
#ifdef XT_BOARD
#define DISPLAY_TASK_STK_SIZE   TASK_STK_SIZE

void Display_Task( void * pdata )
{
    while( 1 )
    {
        xtbsp_display_string( "FreeRTOS Xtensa" );
        vTaskDelay(configTICK_RATE_HZ);
        xtbsp_display_string( "Counting Example" );
        vTaskDelay(configTICK_RATE_HZ);
    }
}
#endif


//-----------------------------------------------------------------------------
// Count Task. Counts at regular intervals and sends the count via queue to
// the Report Task.
//-----------------------------------------------------------------------------
void Count_Task( void * pdata )
{
    uint32_t count = 1;

    UNUSED(pdata);

    PRINTF( "[Count_Task] Started.\n" );

    // Count at regular intervals and place counter in queue.
    PRINTF( "[Count_Task] Counting.\n" );

    while ( 1 )
    {
        xQueueSend( Queue, (void *) &count, 2 );
        ++count;
        vTaskDelay( 1 );
#ifdef XT_SIMULATOR
        // On simulator, stop after a number of iterations.
        if ( count > 250 )
        {
            break;
        }
#endif
    }

    // Send a last message to terminate the Report Task.
    count = 0xFFFFFFFF;
    xQueueSend( Queue, (void *) &count, portMAX_DELAY );

    PRINTF( "\n[Count_Task] Terminating.\n" );

    // Set termination flag to alert Init_Task.
    xEventGroupSetBits( TaskTermFlags, TASK_TERM_COUNT );

    // Terminate this task. RTOS will continue to run other tasks.
    vTaskDelete( NULL );
}


//-----------------------------------------------------------------------------
// The Report Task waits for messages coming through the queue from the Count
// Task and reports progress. It terminates when it receives a magic value for
// the message.
//-----------------------------------------------------------------------------
void Report_Task( void * pdata )
{
    uint32_t count;
    int32_t  err = 0;

    UNUSED(pdata);

    PRINTF( "[Report_Task] Started.\n" );

    while ( 1 )
    {
        err = xQueueReceive( Queue, &count, 2 );
        if ( err == pdFAIL )
        {
            // Error
            break;
        }

        if ( count == 0xFFFFFFFF )
        {
            // End
            break;
        }

        PUTCHAR( '.' );
        if ( (count > 0) && (count % 10 == 0) )
        {
            PUTCHAR( ' ' );
        }
        if ( (count > 0) && (count % 50 == 0) )
        {
            PUTCHAR( '\n' );
        }
    }

    PRINTF( "\n[Report_Task] Terminating.\n" );

    // Set termination flag to alert Init_Task.
    xEventGroupSetBits( TaskTermFlags, TASK_TERM_REPORT );

    // Terminate this task. RTOS will continue to run other tasks.
    vTaskDelete( NULL );
}


//-----------------------------------------------------------------------------
// The Init Task creates the other tasks and waits for them to finish.
//-----------------------------------------------------------------------------
void Init_Task( void * pdata )
{
    int32_t err = 0;
    int32_t exit_code = 0;

    UNUSED(pdata);

    PRINTF( "[Init_Task] Started.\n" );

    // Create event flag group for task termination.
    PRINTF( "[Init_Task] Creating event flag group for task termination.\n" );
    TaskTermFlags = xEventGroupCreate();
    if ( TaskTermFlags == NULL )
    {
        PRINTF( "...FAILED .1!\n" );
        goto done;
    }

    // Create queue for sequence of counts.
    PRINTF( "[Init_Task] Creating queue for sequence of counts.\n" );
    Queue = xQueueCreate( QUEUE_SIZE, sizeof(uint32_t) );
    if ( Queue == NULL )
    {
        PRINTF( "...FAILED .2!\n" );
        err = 101;
        goto done;
    }

    // Create reporting task.
    PRINTF( "[Init_Task] Creating reporting task Report_Task.\n" );

    err = xTaskCreate( Report_Task, "Report_Task", TASK_STK_SIZE, NULL, REPORT_TASK_PRIO, &Report_Task_TCB );
    if ( err != pdPASS )
    {
        PRINTF( "...FAILED! .3!\n" );
        goto done;
    }

    // Create counting task.
    PRINTF( "[Init_Task] Creating counting task Count_Task.\n" );
    err = xTaskCreate( Count_Task, "Count_Task", TASK_STK_SIZE, NULL, COUNT_TASK_PRIO, &Count_Task_TCB );
    if ( err != pdPASS )
    {
        PRINTF( "...FAILED! .4!\n" );
        goto done;
    }

#ifdef XT_BOARD
    // Create display task, if board has a display.
    if ( xtbsp_display_exists() )
    {
        PRINTF( "[Init_Task] Creating display task Display_Task.\n" );
        err = xTaskCreate( Display_Task, "Display_Task", TASK_STK_SIZE, NULL, DISPLAY_TASK_PRIO, NULL );
        if ( err != pdPASS )
        {
            PRINTF( "...FAILED! .5!\n" );
            goto done;
        }
    }
#endif

    // Wait for counting and reporting tasks to finish.
    PRINTF( "[Init_Task] Waiting for counting and reporting tasks to finish.\n" );
    xEventGroupWaitBits( TaskTermFlags, TASK_TERM_REPORT | TASK_TERM_COUNT, 0, pdTRUE, portMAX_DELAY );

done:
    // Clean up and shut down.
    exit_code = ( err != pdPASS );
    PRINTF( "[Init_Task] Cleaning up resources and terminating.\n" );

    vQueueDelete( Queue );
    vEventGroupDelete( TaskTermFlags );

#ifdef XT_SIMULATOR
    // This string indicates test passed.
    if ( exit_code == 0 )
    {
        PRINTF( "PASS\n" );
    }
    // Shut down simulator and report error code as exit code to host (0 = OK).
    exit( exit_code );
#endif

    // Terminate this task. RTOS will continue to run other tasks.
    vTaskDelete( NULL );
}


#if configUSE_TICK_HOOK
void vApplicationTickHook( void )
{
}
#endif


void vApplicationStackOverflowHook( TaskHandle_t xTask, char * pcTaskName )
{
    UNUSED(xTask);
    UNUSED(pcTaskName);

    puts( "\nStack overflow, stopping." );
    exit( 0 );
}

int main( int argc, char * argv[] )
{
    int err = 0;

    UNUSED(argc);
    UNUSED(argv);

#ifdef XT_BOARD
    // Display waypoint for debugging.
    xtbsp_display_string( "main()" );
#endif

    PRINTF( "\nFreeRTOS example on Xtensa.\n" );
    PRINTF( "Xtensa port version " XTENSA_PORT_VERSION_STRING "\n" );

    // Create only the Init_Task here. It will create the others.
    PRINTF( "[main] Creating first task Init_Task.\n" );
    err = xTaskCreate( Init_Task, "Init_Task", TASK_STK_SIZE, NULL, INIT_TASK_PRIO, NULL );

    if ( err != pdPASS )
    {
        PRINTF( "FAILED! main\n" );
        return 1;
    }

    // Start task scheduler.
    PRINTF( "[main] Starting multi-tasking.\n" );
    vTaskStartScheduler();

    // If we got here then scheduler failed.
    PRINTF( "vTaskStartScheduler FAILED!\n" );
    return 1;
}

