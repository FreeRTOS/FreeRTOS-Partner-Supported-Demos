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
#include "integer.h"
#include "comtest2.h"
#include "partest.h"

/* Demo task priorities. */
#define mainBLOCK_Q_PRIORITY				( tskIDLE_PRIORITY + 2 )
#define mainCHECK_TASK_PRIORITY				( tskIDLE_PRIORITY + 3 )
#define mainCOM_TEST_PRIORITY				( 2 )

/* The check task may require a bit more stack as it calls sprintf(). */
#define mainCHECK_TASK_STACK_SIZE			( configMINIMAL_STACK_SIZE )

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

/*-----------------------------------------------------------*/

/*
 * The check task as described at the top of this file.
 */
static void vCheckTask( void *pvParameters );

/*
 * Setup the processor ready for the demo.
 */
static void prvSetupHardware( void );
static void vClockInitialise( void );
/*-----------------------------------------------------------*/

/*
 * Create the demo tasks then start the scheduler.
 */
int main( void )
{
	/* Configure any hardware required for this demo. */
	prvSetupHardware();

	/* Create the standard demo tasks. */
	vStartBlockingQueueTasks( mainBLOCK_Q_PRIORITY );
	vStartIntegerMathTasks( tskIDLE_PRIORITY );
	vStartFlashCoRoutines( mainNUM_FLASH_COROUTINES );
	vAltStartComTestTasks( mainCOM_TEST_PRIORITY, mainCOM_TEST_BAUD_RATE, mainCOM_TEST_LED );
	vCreateBlockTimeTasks();

	/* Create the test tasks defined within this file. */
	xTaskCreate( vCheckTask, "Check", mainCHECK_TASK_STACK_SIZE, NULL, mainCHECK_TASK_PRIORITY, NULL );

	/* Finally start the scheduler. */
	vTaskStartScheduler();

	/* Will only reach here if there is insufficient heap available to start
	the scheduler. */
	return 0;
}
/*-----------------------------------------------------------*/

static void prvSetupHardware( void )
{
    vClockInitialise();
	vParTestInitialise();
}

/*-----------------------------------------------------------
 * Clock Initialization.
 *-----------------------------------------------------------*/

void vClockInitialise()
{
       /*  
        System Clock Source                             :  PLL1 Out output
        System/Generator 1 frequency (Fosc)             :  50 MHz
        
        Clock Generator 2 frequency                     : 8 MHz
        Clock Generator 3 frequency                     : 8 MHz
        Clock Generator 8 frequency                     : 8 MHz
        
        PLL 1 frequency                                 : 200 MHz
        PLL 1 VCO Out frequency                         : 1000 MHz

    */
    // TUN 0x0; 
    FRCTUN = 0x0UL;
    // TUN 0x0; 
    BFRCTUN = 0x0UL;
    
    OSCCTRLbits.FRCEN = 1U;
    while(OSCCTRLbits.FRCRDY == 0U){}; 
    OSCCTRLbits.BFRCEN = 1U;
    while(OSCCTRLbits.BFRCRDY == 0U){};
    
    // NOSC FRC Oscillator; OE enabled; SIDL disabled; ON enabled; BOSC Serial Test Mode clock (PGC); FSCMEN disabled; DIVSWEN disabled; OSWEN disabled; EXTCFSEL disabled; EXTCFEN disabled; FOUTSWEN disabled; RIS disabled; PLLSWEN disabled; 
    PLL1CON = 0x9100UL;
    // POSTDIV2 1x divide; POSTDIV1 5x divide; PLLFBDIV 125; PLLPRE 1; 
    PLL1DIV = 0x1007D29UL;
    //Enable PLL Input and Feedback Divider update
    PLL1CONbits.PLLSWEN = 1U;
    while (PLL1CONbits.PLLSWEN == 1){};
    PLL1CONbits.FOUTSWEN = 1U;
    while (PLL1CONbits.FOUTSWEN == 1U){};
    //enable clock switching
    PLL1CONbits.OSWEN = 1U; 
    //wait for switching
    while(PLL1CONbits.OSWEN == 1U){}; 
    //wait for clock to be ready
    while(OSCCTRLbits.PLL1RDY == 0U){};    
    
    //Configure VCO Divider
    // INTDIV 0; 
    VCO1DIV = 0x0UL;
    //enable PLL VCO divider
    PLL1CONbits.DIVSWEN = 1U; 
    //wait for setup complete
    while(PLL1CONbits.DIVSWEN == 1U){}; 
    
    // NOSC PLL1 Out output; OE enabled; SIDL disabled; ON enabled; BOSC FRC Oscillator; FSCMEN disabled; DIVSWEN disabled; OSWEN disabled; EXTCFSEL External clock fail detection module #1; EXTCFEN disabled; RIS disabled; 
    CLK1CON = 0x19500UL;
    // FRACDIV 0; INTDIV 2; 
    CLK1DIV = 0x20000UL;
    //enable divide factors
    CLK1CONbits.DIVSWEN = 1U; 
    //wait for divide factors to get updated
    while(CLK1CONbits.DIVSWEN == 1U){};
    //enable clock switching
    CLK1CONbits.OSWEN = 1U; 
    //wait for clock switching complete
    while(CLK1CONbits.OSWEN == 1U){};
    
    // NOSC FRC Oscillator; OE enabled; SIDL disabled; ON enabled; BOSC FRC Oscillator; FSCMEN disabled; DIVSWEN disabled; OSWEN disabled; EXTCFSEL External clock fail detection module #1; EXTCFEN disabled; RIS disabled; 
    CLK2CON = 0x19101UL;
    //enable clock switching
    CLK2CONbits.OSWEN = 1U; 
    //wait for clock switching complete
    while(CLK2CONbits.OSWEN == 1U){};
    
    // NOSC Backup FRC Oscillator; OE enabled; SIDL disabled; ON enabled; BOSC Backup FRC Oscillator; FSCMEN disabled; DIVSWEN disabled; OSWEN disabled; EXTCFSEL External clock fail detection module #1; EXTCFEN disabled; RIS disabled; 
    CLK3CON = 0x29202UL;
    //enable clock switching
    CLK3CONbits.OSWEN = 1U; 
    //wait for clock switching complete
    while(CLK3CONbits.OSWEN == 1U){};
    
    // NOSC FRC Oscillator; OE enabled; SIDL disabled; ON enabled; BOSC Serial Test Mode clock (PGC); FSCMEN disabled; DIVSWEN disabled; OSWEN disabled; EXTCFSEL External clock fail detection module #1; EXTCFEN disabled; RIS disabled; 
    CLK8CON = 0x9100UL;
    // FRACDIV 0x0; INTDIV 0x0; 
    CLK8DIV = 0x0UL;
    //enable clock switching
    CLK8CONbits.OSWEN = 1U; 
    //wait for clock switching complete
    while(CLK8CONbits.OSWEN == 1U){};
     
}
/*-----------------------------------------------------------*/

static void vCheckTask( void *pvParameters )
{
    /* Used to wake the task at the correct frequency. */
    TickType_t xLastExecutionTime;

	/* Initialise xLastExecutionTime so the first call to vTaskDelayUntil()
	works correctly. */
	xLastExecutionTime = xTaskGetTickCount();

	for( ;; )
	{
		/* Wait until it is time for the next cycle. */
		vTaskDelayUntil( &xLastExecutionTime, mainCHECK_TASK_PERIOD );

		/* Has an error been found in any of the standard demo tasks? */

		if( xAreIntegerMathsTaskStillRunning() != pdTRUE )
		{
            PORTDbits.RD0 = 1; //Green
		}

		if( xAreComTestTasksStillRunning() != pdTRUE )
		{
            PORTDbits.RD2 = 1; //Blue
		}

		if( xAreBlockTimeTestTasksStillRunning() != pdTRUE )
		{
            PORTCbits.RC2 = 1; //Red
		}

		if( xAreBlockingQueuesStillRunning() != pdTRUE )
		{
            PORTDbits.RD0 = 1;
            Nop();
            Nop();
            Nop();
            Nop();
            PORTDbits.RD2 = 1;
            Nop();
            Nop();
            Nop();
            Nop();
		}
	}
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
	/* Schedule the co-routines from within the idle task hook. */
	vCoRoutineSchedule();
}
/*-----------------------------------------------------------*/

