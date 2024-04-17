/*
 * FreeRTOS V202212.01
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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

/*
 * Creates all the demo application tasks, then starts the scheduler.  The WEB
 * documentation provides more details of the standard demo application tasks.
 * In addition to the standard demo tasks, the following tasks and tests are
 * defined and/or created within this file:
 *
 * "Fast Interrupt Test" - A high frequency periodic interrupt is generated
 * using a free running timer to demonstrate the use of the
 * configKERNEL_INTERRUPT_PRIORITY configuration constant.  The interrupt
 * service routine measures the number of processor clocks that occur between
 * each interrupt - and in so doing measures the jitter in the interrupt
 * timing.  The maximum measured jitter time is latched in the usMaxJitter
 * variable, and displayed on the LCD by the 'Check' as described below.
 * The fast interrupt is configured and handled in the timer_test.c source
 * file.
 *
 * "LCD" task - the LCD task is a 'gatekeeper' task.  It is the only task that
 * is permitted to access the LCD directly.  Other tasks wishing to write a
 * message to the LCD send the message on a queue to the LCD task instead of
 * accessing the LCD themselves.  The LCD task just blocks on the queue waiting
 * for messages - waking and displaying the messages as they arrive.  The LCD
 * task is defined in lcd.c.
 *
 * "Check" task -  This only executes every three seconds but has the highest
 * priority so is guaranteed to get processor time.  Its main function is to
 * check that all the standard demo tasks are still operational.  Should any
 * unexpected behaviour within a demo task be discovered the 'check' task will
 * write "FAIL #n" to the LCD (via the LCD task).  If all the demo tasks are
 * executing with their expected behaviour then the check task writes the max
 * jitter time to the LCD (again via the LCD task), as described above.
 */
/* Standard includes. */
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"

/* Demo application includes. */
#include "BlockQ.h"
#include "crflash.h"
#include "blocktim.h"
#include "comtest2.h"
#include "partest.h"
#include "lcd.h"
#include "timertest.h"
#include "main_core.h"

/* Demo task priorities. */
#define mainBLOCK_Q_PRIORITY				( tskIDLE_PRIORITY + 2 )
#define mainCHECK_TASK_PRIORITY				( tskIDLE_PRIORITY + 3 )
#define mainCOM_TEST_PRIORITY				( 2 )

/* The check task may require a bit more stack as it calls sprintf(). */
#define mainCHECK_TASK_STACK_SIZE			( configMINIMAL_STACK_SIZE * 4 )

/* The execution period of the check task. */
#define mainCHECK_TASK_PERIOD				( ( TickType_t ) 3000 / portTICK_PERIOD_MS )

/* The number of flash co-routines to create. */
#define mainNUM_FLASH_COROUTINES			( 5 )

/* Baud rate used by the comtest tasks. */
#define mainCOM_TEST_BAUD_RATE				( 19200 )

/* The LED used by the comtest tasks.  mainCOM_TEST_LED + 1 is also used.
See the comtest.c file for more information. */
#define mainCOM_TEST_LED					( 6 )

/* The frequency at which the "fast interrupt test" interrupt will occur. */
#define mainTEST_INTERRUPT_FREQUENCY		( 20000 )

/* The number of processor clocks we expect to occur between each "fast
interrupt test" interrupt. */
#define mainEXPECTED_CLOCKS_BETWEEN_INTERRUPTS ( configCPU_CLOCK_HZ / mainTEST_INTERRUPT_FREQUENCY )

/* The number of nano seconds between each processor clock. */
#define mainNS_PER_CLOCK ( ( unsigned short ) ( ( 1.0 / ( double ) configCPU_CLOCK_HZ ) * 1000000000.0 ) )

/* Dimension the buffer used to hold the value of the maximum jitter time when
it is converted to a string. */
#define mainMAX_STRING_LENGTH				( 20 )
/*Data send from Host to Client*/
#define MSI_DATA1 0xAAAA
#define MSI_DATA2 0xBBBB
#define MSI_DATA3 0xCCCC
#define MSI_DATA4 0xDDDD
#define MSI_DATA5 0xEEEE

/*-----------------------------------------------------------*/

/* Buffer to receive the MSI data sent from the Main core */
uint16_t msiDataReceive[MSI1_ProtocolA_SIZE];

/* Set to pdTRUE should an error be detected in any of the standard demo tasks. */
unsigned short usErrorDetected = pdFALSE;
/*
 * The check task as described at the top of this file.
 */
static void vCheckTask( void *pvParameters );

/*
 * The MSI task receives the message sent by the main core through MSI interface .
 */
static void vMsiRxTask( void *pvParameters );
/*
 * Setup the processor ready for the demo.
 */

static void prvSetupHardware( void );
/*
 * Function checks whether MSI task is still running
 */
BaseType_t xIsMsiTaskStillRunning( void );

/*
 * Setup the Clock.
 */
void prvSetupClock(void);

/*-----------------------------------------------------------*/

/* The queue used to send messages to the LCD task. */
static QueueHandle_t xLCDQueue;

/*-----------------------------------------------------------*/

/*
 * Create the demo tasks then start the scheduler.
 */
int main( void )
{
	/* Configure any hardware required for this demo. */
	prvSetupHardware();
	/* Create the standard demo tasks. */
	vStartFlashCoRoutines( mainNUM_FLASH_COROUTINES );
	vAltStartComTestTasks( mainCOM_TEST_PRIORITY, mainCOM_TEST_BAUD_RATE, mainCOM_TEST_LED );
    
    /* Create a task for reading the messages received from main core through MSI*/
    xTaskCreate( vMsiRxTask, "MSI",  configMINIMAL_STACK_SIZE*2 , NULL, mainCHECK_TASK_PRIORITY-1, NULL );
	/* Create the test tasks defined within this file. */
	xTaskCreate( vCheckTask, "Check", mainCHECK_TASK_STACK_SIZE, NULL, mainCHECK_TASK_PRIORITY, NULL );
    
	/* Start the task that will control the LCD.  This returns the handle
	to the queue used to write text out to the task. */
	xLCDQueue = xStartLCDTask();

	/* Finally start the scheduler. */
	vTaskStartScheduler();

	/* Will only reach here if there is insufficient heap available to start
	the scheduler. */
	return 0;
}

/*-----------------------------------------------------------*/

static void prvSetupHardware( void )
{
    prvSetupClock();
	vParTestInitialise();
    MAIN_CORE_Initialize();
}

void prvSetupClock(void)
{
    // FRCDIV FRC/1; PLLPRE 1; DOZE 1:8; DOZEN disabled; ROI disabled; 
    CLKDIV = 0x3001;
    // PLLFBDIV 50; 
    PLLFBD = 0x32;
    // POST1DIV 1:4; VCODIV FVCO/4; POST2DIV 1:1; 
    PLLDIV = 0x41;
    // CF no clock failure; NOSC FRCPLL; CLKLOCK unlocked; OSWEN Switch is Complete; 
    __builtin_write_OSCCONH((uint8_t) ((0x100 >> _OSCCON_NOSC_POSITION) & 0x00FF));
    __builtin_write_OSCCONL((uint8_t) ((0x100 | _OSCCON_OSWEN_MASK) & 0xFF));
    // Wait for Clock switch to occur
    while (OSCCONbits.OSWEN != 0);
    while (OSCCONbits.LOCK != 1);
}
/*-----------------------------------------------------------*/

static void vCheckTask( void *pvParameters )
{
/* Used to wake the task at the correct frequency. */
TickType_t xLastExecutionTime;

extern unsigned short runCount ;

/* Buffer into which the maximum jitter time is written as a string. */
static char cStringBuffer[ mainMAX_STRING_LENGTH ];

/* The message that is sent on the queue to the LCD task.  The first
parameter is the minimum time (in ticks) that the message should be
left on the LCD without being overwritten.  The second parameter is a pointer
to the message to display itself. */
xLCDMessage xMessage = { 0, cStringBuffer };

	/* Initialise xLastExecutionTime so the first call to vTaskDelayUntil()
	works correctly. */
	xLastExecutionTime = xTaskGetTickCount();

	for( ;; )
	{
		/* Wait until it is time for the next cycle. */
		vTaskDelayUntil( &xLastExecutionTime, mainCHECK_TASK_PERIOD );

		/* Has an error been found in any of the standard demo tasks? */
        if( xIsMsiTaskStillRunning() != pdTRUE )
		{
			usErrorDetected = pdTRUE;
			sprintf( cStringBuffer, "FAIL #1" );
		}

		if( xAreComTestTasksStillRunning() != pdTRUE )
		{
			usErrorDetected = pdTRUE;
			sprintf( cStringBuffer, "FAIL #2" );
		}

		if( usErrorDetected != pdTRUE )
		{
			/* No errors have been discovered, so display LCD task run count". */
			sprintf( cStringBuffer, "No Errors:%d", ( short ) runCount );
		}

		/* Send the message to the LCD gatekeeper for display. */
		xQueueSend( xLCDQueue, &xMessage, portMAX_DELAY );
	}
}
/*-----------------------------------------------------------*/
static portTASK_FUNCTION( vMsiRxTask, pvParameters )
{
    static char cStringBuffer[ mainMAX_STRING_LENGTH ];
    xLCDMessage xMessage = { 0, cStringBuffer };
    const TickType_t xDelay50ms = pdMS_TO_TICKS( 50 );
    for (;;) {
        //Wait for interrupt from Main core    
        while (!MAIN_CORE_IsInterruptRequested());
        MAIN_CORE_InterruptRequestAcknowledge();
        while (MAIN_CORE_IsInterruptRequested());
        MAIN_CORE_InterruptRequestAcknowledgeComplete();

        msiDataReceive[0] = 0x00;
        //Mailbox read 
        MAIN_CORE_ProtocolRead(MSI1_ProtocolA, (uint16_t*) msiDataReceive);
        if(usErrorDetected != pdTRUE )
        {
           sprintf( cStringBuffer, "DATA S<-M:0x%X", ( short ) msiDataReceive[0] );
           xQueueSend( xLCDQueue, &xMessage, portMAX_DELAY );
        }
        vTaskDelay(xDelay50ms);
    }
}

/*-----------------------------------------------------------*/
BaseType_t xIsMsiTaskStillRunning(void) {
    BaseType_t xReturn = pdFALSE;

    /* Check data received is the expected ones */
    if (msiDataReceive[0] == MSI_DATA1 || msiDataReceive[0] == MSI_DATA2 || msiDataReceive[0] == MSI_DATA3
        || msiDataReceive[0] == MSI_DATA4 || msiDataReceive[0] == MSI_DATA5) {
        xReturn = pdTRUE;
    }

    return xReturn;
}
/*-----------------------------------------------------------*/
void vApplicationIdleHook( void )
{
	/* Schedule the co-routines from within the idle task hook. */
	vCoRoutineSchedule();
}
/*-----------------------------------------------------------*/

