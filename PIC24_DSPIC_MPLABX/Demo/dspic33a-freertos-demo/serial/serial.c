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


/* BASIC INTERRUPT DRIVEN SERIAL PORT DRIVER. 

NOTE:  This driver is primarily to test the scheduler functionality.  It does
not effectively use the buffers or DMA and is therefore not intended to be
an example of an efficient driver. */

/* Standard include file. */
#include <stdlib.h>

/* Scheduler include files. */
#include "FreeRTOS.h"

#include "queue.h"
#include "task.h"

/* Demo app include files. */
#include "serial.h"

/* Hardware setup. */
#define serOUTPUT						0
#define serINPUT						1
#define serLOW_SPEED					0
#define serONE_STOP_BIT					0
#define serEIGHT_DATA_BITS_NO_PARITY	0
#define serNORMAL_IDLE_STATE			0
#define serAUTO_BAUD_OFF				0
#define serLOOPBACK_OFF					0
#define serWAKE_UP_DISABLE				0
#define serNO_HARDWARE_FLOW_CONTROL		0
#define serSTANDARD_IO					0
#define serNO_IRDA						0
#define serCONTINUE_IN_IDLE_MODE		0
#define serUART_ENABLED					1
#define serINTERRUPT_ON_SINGLE_CHAR		0
#define serTX_ENABLE					1
#define serRX_ENABLE					1
#define serINTERRUPT_ENABLE				1
#define serINTERRUPT_DISABLE			0
#define serCLEAR_FLAG					0
#define serSET_FLAG						1


/* The queues used to communicate between tasks and ISR's. */
static QueueHandle_t xRxedChars; 
static QueueHandle_t xCharsForTx; 

static portBASE_TYPE xTxHasEnded;
/*-----------------------------------------------------------*/

xComPortHandle xSerialPortInitMinimal( unsigned long ulWantedBaud, unsigned portBASE_TYPE uxQueueLength )
{
    char cChar;
    ANSELD = 0x00;
	/* Create the queues used by the com test task. */
	xRxedChars = xQueueCreate( uxQueueLength, ( portBASE_TYPE ) sizeof( signed char ) );
	xCharsForTx = xQueueCreate( uxQueueLength, ( portBASE_TYPE ) sizeof( signed char ) );
    
    //PPS Mapping
#if defined (__dsPIC33EP512MU810__)
    RPOR9bits.RP101R = 3;   //RF5 as U2TX
    _U2RXR = 100;           //RF4 as U2RX
#elif defined (__dsPIC33AK128MC106__)
    //RPINR9bits.U2RXR = 0x0017UL; //RB6->UART2:U2RX;
    //RPOR5bits.RP24R = 0x000BUL;  //RB7->UART2:U2TX;
    //Curiosity Board
    RPINR9bits.U2RXR = 0x003BUL; //RD10->UART2:U2RX;
    RPOR14bits.RP58R = 0x000BUL;  //RD9->UART2:U2TX;
#endif    

	/* Setup the UART. */
	U2CONbits.BRGH		= serLOW_SPEED;
	U2CONbits.STP	= serONE_STOP_BIT;
	U2CONbits.ABDEN	= serAUTO_BAUD_OFF;
	U2CONbits.WAKE		= serWAKE_UP_DISABLE;
	U2CONbits.FLO		= serNO_HARDWARE_FLOW_CONTROL;
	U2CONbits.MODE		= serNO_IRDA;
	U2CONbits.SIDL	= serCONTINUE_IN_IDLE_MODE;
	U2CONbits.ON	= serUART_ENABLED;

	U2BRG = (unsigned short)(( (float)configCPU_CLOCK_HZ / ( (float)16 * (float)ulWantedBaud ) ) - (float)0.5);

	U2STAbits.URXISEL	= serINTERRUPT_ON_SINGLE_CHAR;
    U2CONbits.RXEN		= serRX_ENABLE;
	U2CONbits.TXEN		= serTX_ENABLE;
	U2CONbits.UTXINV	= serNORMAL_IDLE_STATE;
	U2STAbits.TXWM	= serINTERRUPT_ON_SINGLE_CHAR;
	U2STAbits.RXWM	= serINTERRUPT_ON_SINGLE_CHAR;
    


	/* It is assumed that this function is called prior to the scheduler being
	started.  Therefore interrupts must not be allowed to occur yet as they
	may attempt to perform a context switch. */
	portDISABLE_INTERRUPTS();

	IFS2bits.U2RXIF = serCLEAR_FLAG;
	IFS2bits.U2TXIF = serCLEAR_FLAG;
	IPC11bits.U2RXIP = configKERNEL_INTERRUPT_PRIORITY;
	IPC11bits.U2TXIP = configKERNEL_INTERRUPT_PRIORITY;
	IEC2bits.U2TXIE = serINTERRUPT_ENABLE;
	IEC2bits.U2RXIE = serINTERRUPT_ENABLE;

	/* Clear the Rx buffer. */
	while( U2STAbits.RXBE == serCLEAR_FLAG )
	{
		cChar = U2RXB;
	}

	xTxHasEnded = pdTRUE;

	return NULL;
}
/*-----------------------------------------------------------*/

portBASE_TYPE xSerialGetChar( xComPortHandle pxPort, signed char *pcRxedChar, TickType_t xBlockTime )
{
	/* Only one port is supported. */
	( void ) pxPort;

	/* Get the next character from the buffer.  Return false if no characters
	are available or arrive before xBlockTime expires. */
	if( xQueueReceive( xRxedChars, pcRxedChar, xBlockTime ) )
	{
		return pdTRUE;
	}
	else
	{
		return pdFALSE;
	}
}
/*-----------------------------------------------------------*/

portBASE_TYPE xSerialPutChar( xComPortHandle pxPort, signed char cOutChar, TickType_t xBlockTime )
{
	/* Only one port is supported. */
	( void ) pxPort;

	/* Return false if after the block time there is no room on the Tx queue. */
	if( xQueueSend( xCharsForTx, &cOutChar, xBlockTime ) != pdPASS )
	{
		return pdFAIL;
	}

	/* A critical section should not be required as xTxHasEnded will not be
	written to by the ISR if it is already 0 (is this correct?). */
	if( xTxHasEnded )
	{
		xTxHasEnded = pdFALSE;
		IFS2bits.U2TXIF = serSET_FLAG;
	}

	return pdPASS;
}
/*-----------------------------------------------------------*/

void vSerialClose( xComPortHandle xPort )
{
}
/*-----------------------------------------------------------*/

void __attribute__((__interrupt__, auto_psv)) _U2RXInterrupt( void )
{
char cChar;
portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	/* Get the character and post it on the queue of Rxed characters.
	If the post causes a task to wake force a context switch as the woken task
	may have a higher priority than the task we have interrupted. */
	IFS2bits.U2RXIF = serCLEAR_FLAG;
	while( !U2STAbits.RXBE )
	{
		cChar = U2RXB;
		xQueueSendFromISR( xRxedChars, &cChar, &xHigherPriorityTaskWoken );
	}

	if( xHigherPriorityTaskWoken != pdFALSE )
	{
		taskYIELD();
	}
}
/*-----------------------------------------------------------*/

void __attribute__((__interrupt__, auto_psv)) _U2TXInterrupt( void )
{
signed char cChar;
portBASE_TYPE xTaskWoken = pdFALSE;

	/* If the transmit buffer is full we cannot get the next character.
	Another interrupt will occur the next time there is space so this does
	not matter. */
	IFS2bits.U2TXIF = serCLEAR_FLAG;
	while( !( U2STAbits.UTXBF ) )
	{
		if( xQueueReceiveFromISR( xCharsForTx, &cChar, &xTaskWoken ) == pdTRUE )
		{
			/* Send the next character queued for Tx. */
			U2TXB = cChar;
		}
		else
		{
			/* Queue empty, nothing to send. */
			xTxHasEnded = pdTRUE;
			break;
		}
	}

	if( xTaskWoken != pdFALSE )
	{
		taskYIELD();
	}
}


