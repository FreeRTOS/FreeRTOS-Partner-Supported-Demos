/*
    FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdbool.h>
#include <libpic30.h>

/* Demo includes. */
#include "lcd.h"

#define lcdVERY_SHORT_DELAY	( 1 )
#define lcdSHORT_DELAY		( 4 / portTICK_PERIOD_MS )
#define lcdLONG_DELAY		( 15 / portTICK_PERIOD_MS )
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

/* The length of the queue used to send messages to the LCD gatekeeper task. */
#define lcdQUEUE_SIZE		3
/*-----------------------------------------------------------*/

/* The queue used to send messages to the LCD task. */
QueueHandle_t xLCDQueue;

static void prvLCDCommand( char cCommand );
static void prvLCDData( char cChar );

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

//static void prvLCDGotoRow( unsigned short usRow )
//{
//	if( usRow == 0 )
//	{
//		prvLCDCommand( lcdHOME );
//	}
//	else
//	{
//		prvLCDCommand( lcdLINE2 );
//	}
//}
///*-----------------------------------------------------------*/
//
//static void prvLCDCommand( char cCommand )
//{
//	/* Prepare RD0 - RD7. */
//	lcdDATA &= 0xFFFF;
//
//	/* Command byte to lcd. */
//    lcdDATA |= (cCommand << 8);
//
//	/* Ensure lcdRW is 0. */
//	//lcdRW = 0;
//    lcdRS = 0;
//
//	/* Toggle lcdE line. */
//    lcdE = 1;
//    vTaskDelay( lcdVERY_SHORT_DELAY );
//    lcdE = 0;
//
//   	vTaskDelay( lcdSHORT_DELAY );
//}
///*-----------------------------------------------------------*/
//
//static void prvLCDData( char cChar )
//{
//	/* ensure lcdRW is 0. */
//	//lcdRW = 0;
//
//	/* Assert register select to 1. */
//    lcdRS = 1;
//
//	/* Prepare RD0 - RD7. */
//	lcdDATA &= 0xFFFF;
//
//	/* Data byte to lcd. */
//    lcdDATA |= (cChar << 8);
//    lcdE = 1;
// 	Nop();
//    Nop();
//    Nop();
//
//	/* Toggle lcdE signal. */
//    lcdE = 0;
//
//	/* Negate register select to 0. */
//    lcdRS = 0;
//
//	vTaskDelay( lcdVERY_SHORT_DELAY );
//}
///*-----------------------------------------------------------*/
//
//static void prvLCDPutString( char *pcString )
//{
//	/* Write out each character with appropriate delay between each. */
//	while( *pcString )
//	{
//		prvLCDData( *pcString );
//		pcString++;
//		vTaskDelay( lcdSHORT_DELAY );
//	}
//}
///*-----------------------------------------------------------*/
//
//static void prvLCDClear( void )
//{
//	prvLCDCommand( lcdCLEAR );
//}
///*-----------------------------------------------------------*/
//
//static void prvSetupLCD( void )
//{
//	/* Wait for proper power up. */
//	vTaskDelay( lcdLONG_DELAY );
//
//	/* Set initial states for the data and control pins */
//	LATE &= 0xFFFF;
//
//	/* R/W state set low. */
//    //lcdRW = 0;
//
//	/* lcdRS state set low. */
//	lcdRS = 0;
//
//	/* lcdE state set low. */
//	lcdE = 0;
//
//	/* Set data and control pins to outputs */
//	TRISE &= 0xFFFF;
//
//	/* lcdRW pin set as output. */
// 	//RW_TRIS = 0;
//
//	/* lcdRS pin set as output. */
//	RS_TRIS = 0;
//
//	/* lcdE pin set as output. */
//	E_TRIS = 0;
//
//	/* 1st LCD initialization sequence */
//	lcdDATA &= 0xFFFF;
//    lcdDATA |= (0x0038 << 8);
//    lcdE = 1;
//    Nop();
//    Nop();
//    Nop();
//
//	/* Toggle lcdE signal. */
//    lcdE = 0;
//
//	vTaskDelay( lcdSHORT_DELAY );
//	vTaskDelay( lcdSHORT_DELAY );
//	vTaskDelay( lcdSHORT_DELAY );
//
//	/* 2nd LCD initialization sequence */
//	lcdDATA &= 0xFFFF;
//    lcdDATA |= (0x0038 << 8);
//    lcdE = 1;
//    Nop();
//    Nop();
//    Nop();
//
//	/* Toggle lcdE signal. */
//    lcdE = 0;
//
//    vTaskDelay( lcdSHORT_DELAY );
//
//	/* 3rd LCD initialization sequence */
//	lcdDATA &= 0xFFFF;
//    lcdDATA |= (0x0038 << 8); 
//    lcdE = 1;
//    Nop();
//    Nop();
//    Nop();
//
//	/* Toggle lcdE signal. */
//    lcdE = 0;
//
//	vTaskDelay( lcdSHORT_DELAY );
//
//
//	/* Function set. */
//    prvLCDCommand( 0x38 );
//
//	/* Display on/off control, cursor blink off (0x0C). */
//    prvLCDCommand( 0x0C );
//
//	/* Entry mode set (0x06). */
//    prvLCDCommand( 0x06 );
//
//	prvLCDCommand( lcdCLEAR );
//}
///*-----------------------------------------------------------*/

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
		vTaskDelay( xMessage.xMinDisplayTime );
	}
}


#include <libpic30.h>

/* Private Definitions ***********************************************/
#define LCD_FAST_INSTRUCTION_TIME_US    50
#define LCD_SLOW_INSTRUCTION_TIME_US    1600
#define LCD_STARTUP_TIME_MS             30
#define LCD_SIGNAL_TIMING_US            1

#define LCD_MAX_COLUMN      16

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

#define LCD_DATA_LAT                   LATE
#define LCD_DATA_TRIS                  TRISE

#define LCD_RSSignal_Set()        LATEbits.LATE11 = 1 //set Register Select bit
#define LCD_RSSignal_Clear()      LATEbits.LATE11 = 0 //clear Register Select bit
#define LCD_RSSignal_Input()      TRISEbits.TRISE11 = 1 //set Register Select bit
#define LCD_RSSignal_Output()     TRISEbits.TRISE11 = 0 //clear Register Select bit

#define LCD_RWSignal_Set()        
#define LCD_RWSignal_Clear()      
#define LCD_RWSignal_Input()      
#define LCD_RWSignal_Output()     

#define LCD_EnableSignal_Set()    LATEbits.LATE10 = 1  //set Enable bit
#define LCD_EnableSignal_Clear()  LATEbits.LATE10 = 0  //clear Enable bit
#define LCD_EnableSignal_Input()  TRISEbits.TRISE10 = 1  //set Enable bit
#define LCD_EnableSignal_Output() TRISEbits.TRISE10 = 0  //clear Enable bit

/* Private Functions *************************************************/
static void LCD_CarriageReturn ( void ) ;
static void LCD_ShiftCursorLeft ( void ) ;
static void LCD_ShiftCursorRight ( void ) ;
static void LCD_ShiftCursorUp ( void ) ;
static void LCD_ShiftCursorDown ( void ) ;
static void LCD_SendData ( char ) ;
static void LCD_SendCommand ( unsigned int ) ;

/* Private variables ************************************************/
static uint8_t row ;
static uint8_t column ;
/*********************************************************************
 * Function: void LCD_Initialize(void);
 *
 * Overview: Initializes the LCD screen.  Can take several hundred
 *           milliseconds.
 *
 * PreCondition: none
 *
 * Input: None
 *
 * Output: true if initialized, false otherwise
 *
 ********************************************************************/
static void prvSetupLCD ( void )
{
    //clear data bus
    LATE &= 0x00FF;
    //drive to output
    TRISE &= 0x00FF;

    // Control signal data pins
    LCD_RWSignal_Clear ( ) ; // LCD R/W signal
    LCD_RSSignal_Clear ( ) ; // LCD RS signal
    LCD_EnableSignal_Clear ( ) ;     // LCD E signal

    // Control signal pin direction
    LCD_RSSignal_Output ( )  ;
    LCD_RWSignal_Output ( )  ;
    LCD_EnableSignal_Output ( ) ;

    LCD_EnableSignal_Set ( )  ;
    
    vTaskDelay(LCD_STARTUP_TIME_MS);

    
    LCD_SendCommand ( LCD_COMMAND_SET_MODE_4_BIT  ) ;
    LCD_SendCommand ( LCD_COMMAND_CURSOR_OFF  ) ;
    LCD_SendCommand ( LCD_COMMAND_ENTER_DATA_MODE ) ;

    prvLCDClear ( ) ;
}

/*********************************************************************
 * Function: void LCD_PutString(char* inputString, uint16_t length);
 *
 * Overview: Puts a string on the LCD screen.  Unsupported characters will be
 *           discarded.  May block or throw away characters is LCD is not ready
 *           or buffer space is not available.  Will terminate when either a
 *           null terminator character (0x00) is reached or the length number
 *           of characters is printed, which ever comes first.
 *
 * PreCondition: already initialized via LCD_Initialize()
 *
 * Input: char* - string to print
 *        uint16_t - length of string to print
 *
 * Output: None
 *
 ********************************************************************/
void prvLCDPutString(char* inputString) {
    while (*inputString) {
        LCD_PutChar(*inputString++);
        inputString++;
         vTaskDelay( lcdSHORT_DELAY );
        
    }
}
/*********************************************************************
 * Function: void LCD_PutChar(char);
 *
 * Overview: Puts a character on the LCD screen.  Unsupported characters will be
 *           discarded.  May block or throw away characters is LCD is not ready
 *           or buffer space is not available.
 *
 * PreCondition: already initialized via LCD_Initialize()
 *
 * Input: char - character to print
 *
 * Output: None
 *
 ********************************************************************/
void LCD_PutChar ( char inputCharacter )
{
    static char lastCharacter = 0;
        
    switch (inputCharacter)
    {
        case '\r':
            if(lastCharacter != '\n')
            {
                LCD_CarriageReturn ( ) ;
            }
            break ;

        case '\n': 
            if(lastCharacter != '\r')
            {
                LCD_CarriageReturn ( ) ;
            }
            
            if (row == 0)
            {
                LCD_ShiftCursorDown ( ) ;
            }
            else
            {
                LCD_ShiftCursorUp ( ) ;
            }
            break ;

        case '\b':
            LCD_ShiftCursorLeft ( ) ;
            LCD_PutChar ( ' ' ) ;
            LCD_ShiftCursorLeft ( ) ;
            break ;
            
        case '\f':
            prvLCDClear();
            break;

        default:
            if (column == LCD_MAX_COLUMN)
            {
                column = 0 ;
                if (row == 0)
                {
                    LCD_SendCommand ( LCD_COMMAND_ROW_1_HOME ) ;
                    row = 1 ;
                }
                else
                {
                    LCD_SendCommand ( LCD_COMMAND_ROW_0_HOME ) ;
                    row = 0 ;
                }
            }
            
            LCD_SendData ( inputCharacter ) ;
            column++ ;
            break ;
    }
    
    lastCharacter = inputCharacter;
}
/*********************************************************************
 * Function: void prvLCDClear(void);
 *
 * Overview: Clears the screen, if possible.
 *
 * PreCondition: already initialized via LCD_Initialize()
 *
 * Input: None
 *
 * Output: None
 *
 ********************************************************************/
void prvLCDClear ( void )
{
    LCD_SendCommand ( LCD_COMMAND_CLEAR_SCREEN ) ;
    LCD_SendCommand ( LCD_COMMAND_RETURN_HOME ) ;

    row = 0 ;
    column = 0 ;
}


/*******************************************************************/
/*******************************************************************/
/* Private Functions ***********************************************/
/*******************************************************************/
/*******************************************************************/
/*********************************************************************
 * Function: static void LCD_CarriageReturn(void)
 *
 * Overview: Handles a carriage return
 *
 * PreCondition: already initialized via LCD_Initialize()
 *
 * Input: None
 *
 * Output: None
 *
 ********************************************************************/
static void LCD_CarriageReturn ( void )
{
    if (row == 0)
    {
        LCD_SendCommand ( LCD_COMMAND_ROW_0_HOME ) ;
    }
    else
    {
        LCD_SendCommand ( LCD_COMMAND_ROW_1_HOME ) ;
    }
    column = 0 ;
}

static void prvLCDGotoRow( unsigned short usRow )
{
	if( usRow == 0 )
	{
		LCD_SendCommand( LCD_COMMAND_ROW_0_HOME );
	}
	else
	{
		LCD_SendCommand( LCD_COMMAND_ROW_1_HOME );
	}
}
/*********************************************************************
 * Function: static void LCD_ShiftCursorLeft(void)
 *
 * Overview: Shifts cursor left one spot (wrapping if required)
 *
 * PreCondition: already initialized via LCD_Initialize()
 *
 * Input: None
 *
 * Output: None
 *
 ********************************************************************/
static void LCD_ShiftCursorLeft ( void )
{
    uint8_t i ;

    if (column == 0)
    {
        if (row == 0)
        {
            LCD_SendCommand ( LCD_COMMAND_ROW_1_HOME ) ;
            row = 1 ;
        }
        else
        {
            LCD_SendCommand ( LCD_COMMAND_ROW_0_HOME ) ;
            row = 0 ;
        }

        //Now shift to the end of the row
        for (i = 0 ; i < ( LCD_MAX_COLUMN - 1 ) ; i++)
        {
            LCD_ShiftCursorRight ( ) ;
        }
    }
    else
    {
        column-- ;
        LCD_SendCommand ( LCD_COMMAND_MOVE_CURSOR_LEFT ) ;
    }
}
/*********************************************************************
 * Function: static void LCD_ShiftCursorRight(void)
 *
 * Overview: Shifts cursor right one spot (wrapping if required)
 *
 * PreCondition: already initialized via LCD_Initialize()
 *
 * Input: None
 *
 * Output: None
 *
 ********************************************************************/
static void LCD_ShiftCursorRight ( void )
{
    LCD_SendCommand ( LCD_COMMAND_MOVE_CURSOR_RIGHT ) ;
    column++ ;

    if (column == LCD_MAX_COLUMN)
    {
        column = 0 ;
        if (row == 0)
        {
            LCD_SendCommand ( LCD_COMMAND_ROW_1_HOME ) ;
            row = 1 ;
        }
        else
        {
            LCD_SendCommand ( LCD_COMMAND_ROW_0_HOME ) ;
            row = 0 ;
        }
    }
}
/*********************************************************************
 * Function: static void LCD_ShiftCursorUp(void)
 *
 * Overview: Shifts cursor up one spot (wrapping if required)
 *
 * PreCondition: already initialized via LCD_Initialize()
 *
 * Input: None
 *
 * Output: None
 *
 ********************************************************************/
static void LCD_ShiftCursorUp ( void )
{
    uint8_t i ;

    for (i = 0 ; i < LCD_MAX_COLUMN ; i++)
    {
        LCD_ShiftCursorLeft ( ) ;
    }
}
/*********************************************************************
 * Function: static void LCD_ShiftCursorDown(void)
 *
 * Overview: Shifts cursor down one spot (wrapping if required)
 *
 * PreCondition: already initialized via LCD_Initialize()
 *
 * Input: None
 *
 * Output: None
 *
 ********************************************************************/
static void LCD_ShiftCursorDown ( void )
{
    uint8_t i ;

    for (i = 0 ; i < LCD_MAX_COLUMN ; i++)
    {
        LCD_ShiftCursorRight ( ) ;
    }
}

/*********************************************************************
 * Function: void LCD_CursorEnable(bool enable)
 *
 * Overview: Enables/disables the cursor
 *
 * PreCondition: None
 *
 * Input: bool - specifies if the cursor should be on or off
 *
 * Output: None
 *
 ********************************************************************/
void LCD_CursorEnable ( bool enable )
{
    if (enable == true)
    {
        LCD_SendCommand ( LCD_COMMAND_CURSOR_ON ) ;
    }
    else
    {
        LCD_SendCommand ( LCD_COMMAND_CURSOR_OFF ) ;
    }
}

/*********************************************************************
 * Function: static void LCD_SendData(char data)
 *
 * Overview: Sends data to LCD
 *
 * PreCondition: None
 *
 * Input: char - contains the data to be sent to the LCD
 *
 * Output: None
 *
 ********************************************************************/
static void LCD_SendData ( char data )
{
    //Split data into nibbles 
    int data_shift = data << 8;
    int upper_nibble = data_shift & 0xF000;
    int lower_nibble = data_shift & 0x0F00;
    lower_nibble = lower_nibble <<4;
    
    LCD_RWSignal_Clear ( ) ;
    LCD_RSSignal_Set ( ) ;
    LATE &= 0x0FFF ;
    LATE |= upper_nibble ;
    vTaskDelay(LCD_SIGNAL_TIMING_US);
    LCD_EnableSignal_Set ( ) ;
    vTaskDelay(LCD_SIGNAL_TIMING_US);
    
    LCD_EnableSignal_Clear ( ) ;
    LATE &= 0x0FFF ;
    LATE |= lower_nibble ;
    vTaskDelay(LCD_SIGNAL_TIMING_US);
    LCD_EnableSignal_Set ( ) ;
    vTaskDelay(LCD_SIGNAL_TIMING_US);
    LCD_EnableSignal_Clear ( ) ;
    LCD_RSSignal_Clear ( ) ;
    vTaskDelay(LCD_FAST_INSTRUCTION_TIME_US);
}
/*********************************************************************
 * Function: static void LCD_SendCommand(char data)
 *
 * Overview: Sends command to LCD
 *
 * PreCondition: None
 *
 * Input: char - contains the command to be sent to the LCD
 *        unsigned int - has the specific delay for the command
 *
 * Output: None
 *
 ********************************************************************/
static void LCD_SendCommand ( unsigned int command )
{
    int cmd_shift = command << 8;
    int upper_nibble = cmd_shift & 0xF000;
    int lower_nibble = cmd_shift & 0x0F00;
    lower_nibble = lower_nibble <<4;
    
    LCD_RWSignal_Clear ( ) ;
    LCD_RSSignal_Clear ( ) ;
    LATE &= 0x0FFF ;
    LATE |= upper_nibble ;
    
    LCD_EnableSignal_Set ( ) ;
    vTaskDelay(LCD_SIGNAL_TIMING_US);
    LCD_EnableSignal_Clear ( ) ;
    
    LCD_RWSignal_Clear ( ) ;
    LCD_RSSignal_Clear ( ) ;
    LATE &= 0x0FFF ;
    LATE |= lower_nibble ;
   
    LCD_EnableSignal_Set ( ) ;
    vTaskDelay(LCD_SIGNAL_TIMING_US);
    LCD_EnableSignal_Clear ( ) ;  
    
    switch(command)
    {
        case LCD_COMMAND_MOVE_CURSOR_LEFT:
        case LCD_COMMAND_MOVE_CURSOR_RIGHT:
        case LCD_COMMAND_SET_MODE_8_BIT:
		case LCD_COMMAND_SET_MODE_4_BIT:
        case LCD_COMMAND_CURSOR_OFF:
            __delay_us(LCD_FAST_INSTRUCTION_TIME_US);
            break;
            
        case LCD_COMMAND_ENTER_DATA_MODE:
        case LCD_COMMAND_CLEAR_SCREEN:
        case LCD_COMMAND_RETURN_HOME:
        case LCD_COMMAND_CURSOR_ON:
        case LCD_COMMAND_ROW_0_HOME:
        case LCD_COMMAND_ROW_1_HOME:
        default:
            __delay_us(LCD_SLOW_INSTRUCTION_TIME_US);
            break;
    }
    
}

void lcd()
{
    prvSetupLCD();
    prvLCDClear();
    prvLCDPutString( "www.FreeRTOS.org" );
    prvLCDGotoRow( 0x01 );
    prvLCDPutString( "www.FreeRTOS.org" );
}





