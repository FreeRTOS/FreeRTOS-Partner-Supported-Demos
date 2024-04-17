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

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Demo includes. */
#include "lcd.h"

#define LCD_COMMAND_CLEAR_SCREEN        0x01
#define LCD_COMMAND_RETURN_HOME         0x02
#define LCD_COMMAND_ENTER_DATA_MODE     0x06
#define LCD_COMMAND_CURSOR_OFF          0x0C
#define LCD_COMMAND_CURSOR_ON           0x0F
#define LCD_COMMAND_MOVE_CURSOR_LEFT    0x10
#define LCD_COMMAND_MOVE_CURSOR_RIGHT   0x14
#define LCD_COMMAND_SET_MODE_4_BIT      0x28
#define LCD_COMMAND_SET_MODE_8_BIT      0x38
#define LCD_COMMAND_ROW_0_HOME          0x80
#define LCD_COMMAND_ROW_1_HOME          0xC0
#define LCD_START_UP_COMMAND_1          0x33    
#define LCD_START_UP_COMMAND_2          0x32   

/*
 * The LCD is written to by more than one task so is controlled by this
 * 'gatekeeper' task.  This is the only task that is actually permitted to
 * access the LCD directly.  Other tasks wanting to display a message send
 * the message to the gatekeeper.
 */
static void vLCDTask( void *pvParameters );

/*
 * Setup the peripherals required to communicate with the LCD.
 */
static void prvSetupLCD( void );

/*
 * Move to the first (0) or second (1) row of the LCD.
 */
static void prvLCDGotoRow( unsigned short usRow );

/*
 * Write a string of text to the LCD.
 */
static void prvLCDPutString( char *pcString );

/*
 * Clear the LCD.
 */
static void prvLCDClear( void );

/*-----------------------------------------------------------*/

/* Brief delay to permit the LCD to catch up with commands. */
#define lcdVERY_SHORT_DELAY	( 1 )
#define lcdSHORT_DELAY		( 30 / portTICK_PERIOD_MS )
#define lcdLONG_DELAY		( 50 / portTICK_PERIOD_MS )
#define lcdVERYLONG_DELAY	(1600 / portTICK_PERIOD_MS )

/* LCD commands. */
#define lcdCLEAR			( 0x01 )
#define lcdHOME				( 0x02 )
#define lcdLINE2			( 0xc0 )

/* SFR that seems to be missing from the standard header files. */
#define PMAEN				*( ( unsigned short * ) 0x60c )

/* LCD R/W signal. */

/* LCD lcdRS signal. */
#define  lcdRS  LATEbits.LATE11

/* LCD lcdE signal . */
#define  lcdE   LATEbits.LATE10

/* Control signal pin direction. */
#define  RS_TRIS	TRISEbits.TRISE11
#define  E_TRIS		TRISEbits.TRISE10

/* Port for LCD data */
#define  lcdDATA      LATE
#define  lcdDATAPORT  PORTE

/* I/O setup for data Port. */
#define  TRISDATA  TRISE

/* The length of the queue used to send messages to the LCD gatekeeper task. */
#define lcdQUEUE_SIZE		3
/*-----------------------------------------------------------*/

/* The queue used to send messages to the LCD task. */
QueueHandle_t xLCDQueue;

static void prvLCDCommand( char cCommand );
static void prvLCDData( char cChar );

unsigned short runCount;
/*-----------------------------------------------------------*/

QueueHandle_t xStartLCDTask( void )
{
	/* Create the queue used by the LCD task.  Messages for display on the LCD
	are received via this queue. */
	xLCDQueue = xQueueCreate( lcdQUEUE_SIZE, sizeof( xLCDMessage ) );

	/* Start the task that will write to the LCD.  The LCD hardware is
	initialised from within the task itself so delays can be used. */
	xTaskCreate( vLCDTask, "LCD", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL );

	return xLCDQueue;
}
/*-----------------------------------------------------------*/

static void prvLCDGotoRow( unsigned short usRow )
{
	if( usRow == 0 )
	{
		prvLCDCommand( lcdHOME );
	}
	else
	{
		prvLCDCommand( lcdLINE2 );
	}
}
/*-----------------------------------------------------------*/

static void prvLCDCommand( char cCommand )
{
    int cmd_shift = cCommand << 8;
    int upper_nibble = cmd_shift & 0xF000;
    int lower_nibble = cmd_shift & 0x0F00;
    lower_nibble = lower_nibble <<4;
	
    lcdRS = 0;
    LATE &= 0x0FFF ;
    LATE |= upper_nibble ;
    
    lcdE = 1;
    vTaskDelay( lcdVERY_SHORT_DELAY );
    lcdE = 0;
    
    lcdRS = 0;
    LATE &= 0x0FFF ;
    LATE |= lower_nibble ;
    
    lcdE = 1;
     vTaskDelay( lcdVERY_SHORT_DELAY );
    lcdE = 0;
    
    vTaskDelay(lcdLONG_DELAY);
    
}
/*-----------------------------------------------------------*/

static void prvLCDData( char cChar )
{
    int data_shift = cChar << 8;
    int upper_nibble = data_shift & 0xF000;
    int lower_nibble = data_shift & 0x0F00;
    lower_nibble = lower_nibble <<4;
	
    Nop();
    lcdRS = 1;
    LATE &= 0x0FFF ;
    LATE |= upper_nibble ;
    Nop();
    Nop();
    Nop();

    lcdE = 1;
    Nop();
    Nop();
    Nop();
    
    lcdE = 0;
    LATE &= 0x0FFF ;
    LATE |= lower_nibble ;
    Nop();
    Nop();
    Nop();
    
    lcdE = 1;
    Nop();
    Nop();
    Nop();

    lcdE = 0;
    lcdRS = 0;
    vTaskDelay( lcdLONG_DELAY );
}
/*-----------------------------------------------------------*/

static void prvLCDPutString( char *pcString )
{
	/* Write out each character with appropriate delay between each. */
	while( *pcString )
	{
		prvLCDData( *pcString );
		pcString++;
		vTaskDelay( lcdVERY_SHORT_DELAY );
	}
    vTaskDelay( lcdVERYLONG_DELAY );
}
/*-----------------------------------------------------------*/

static void prvLCDClear( void )
{
	prvLCDCommand( lcdCLEAR );
}
/*-----------------------------------------------------------*/

static void prvSetupLCD( void )
{
	/* Wait for proper power up. */
	vTaskDelay( lcdLONG_DELAY );

	/* Set initial states for the data and control pins */
	LATE &= 0x00FF;
    
    /* Set data and control pins to outputs */
	TRISE &= 0x00FF;

	/* lcdRS state set low. */
	lcdRS = 0;

	/* lcdE state set low. */
	lcdE = 0;

	/* lcdRS pin set as output. */
	RS_TRIS = 0;

	/* lcdE pin set as output. */
	E_TRIS = 0;

	/* Toggle lcdE signal. */
    lcdE = 1;

	vTaskDelay( lcdLONG_DELAY );

	/* Function set. */
    prvLCDCommand( 0x28 );

	/* Display on/off control, cursor blink off (0x0C). */
    prvLCDCommand( 0x0C );

	/* Entry mode set (0x06). */
    prvLCDCommand( 0x06 );

	prvLCDCommand( lcdCLEAR );
}
/*-----------------------------------------------------------*/

static void vLCDTask( void *pvParameters )
{
xLCDMessage xMessage;
unsigned short usRow = 0;

	/* Initialise the hardware.  This uses delays so must not be called prior
	to the scheduler being started. */
	prvSetupLCD();

	/* Welcome message. */
	prvLCDPutString( "www.FreeRTOS.org" );

	for( ;; )
	{
        runCount++;
		/* Wait for a message to arrive that requires displaying. */
		while( xQueueReceive( xLCDQueue, &xMessage, portMAX_DELAY ) != pdPASS );

		/* Clear the current display value. */
		prvLCDClear();

		/* Switch rows each time so we can see that the display is still being
		updated. */
		prvLCDGotoRow( usRow & 0x01 );
		usRow++;
		prvLCDPutString( xMessage.pcMessage );

		/* Delay the requested amount of time to ensure the text just written
		to the LCD is not overwritten. */
		vTaskDelay( xMessage.xMinDisplayTime);
	}
}




