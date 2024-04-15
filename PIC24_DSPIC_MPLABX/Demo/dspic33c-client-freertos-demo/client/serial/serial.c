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
#include <xc.h>

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

	/* Create the queues used by the com test task. */
	xRxedChars = xQueueCreate( uxQueueLength, ( unsigned portBASE_TYPE ) sizeof( signed char ) );
	xCharsForTx = xQueueCreate( uxQueueLength, ( unsigned portBASE_TYPE ) sizeof( signed char ) );
    
    TRISCbits.TRISC12 = 1;
    TRISCbits.TRISC13 = 0;
    //PPS Mapping
    RPINR18bits.U1RXR = 0x003C;    //;   //RC12->UART1:S1U1RX
    RPOR14bits.RP61R = 0x0001;    //;   //RC13->UART1:S1U1TX   

	/* Setup the UART. */
	U1MODEbits.BRGH		= serLOW_SPEED;
    U1MODEbits.MOD = serEIGHT_DATA_BITS_NO_PARITY;
	U1MODEHbits.STSEL	= serONE_STOP_BIT;
	U1MODEbits.ABAUD	= serAUTO_BAUD_OFF;
	U1MODEbits.WAKE		= serWAKE_UP_DISABLE;
	U1MODEHbits.FLO		= serNO_HARDWARE_FLOW_CONTROL;
	U1MODEbits.USIDL	= serCONTINUE_IN_IDLE_MODE;
	U1MODEbits.UARTEN	= serUART_ENABLED;

	U1BRG = (unsigned short)(( (float)configCPU_CLOCK_HZ / ( (float)16 * (float)ulWantedBaud ) ) - (float)0.5);

	U1STAHbits.URXISEL	= serINTERRUPT_ON_SINGLE_CHAR;
	U1MODEbits.UTXEN		= serTX_ENABLE;
    U1MODEbits.URXEN		= serRX_ENABLE;
	U1MODEHbits.UTXINV	= serNORMAL_IDLE_STATE;
	U1STAHbits.UTXISEL0	= serINTERRUPT_ON_SINGLE_CHAR;
	U1STAHbits.UTXISEL1	= serINTERRUPT_ON_SINGLE_CHAR;

	/* It is assumed that this function is called prior to the scheduler being
	started.  Therefore interrupts must not be allowed to occur yet as they
	may attempt to perform a context switch. */
	portDISABLE_INTERRUPTS();

	IFS0bits.U1RXIF = serCLEAR_FLAG;
	IFS0bits.U1TXIF = serCLEAR_FLAG;
	IPC2bits.U1RXIP = configKERNEL_INTERRUPT_PRIORITY;
	IPC3bits.U1TXIP = configKERNEL_INTERRUPT_PRIORITY;
	IEC0bits.U1TXIE = serINTERRUPT_ENABLE;
	IEC0bits.U1RXIE = serINTERRUPT_ENABLE;

	/* Clear the Rx buffer. */
	while( !U1STAHbits.URXBE )
	{
		cChar = U1RXREG;
	}

	xTxHasEnded = pdTRUE;

	return NULL;
}
/*-----------------------------------------------------------*/

signed portBASE_TYPE xSerialGetChar( xComPortHandle pxPort, signed char *pcRxedChar, TickType_t xBlockTime )
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

signed portBASE_TYPE xSerialPutChar( xComPortHandle pxPort, signed char cOutChar, TickType_t xBlockTime )
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
		IFS0bits.U1TXIF = serSET_FLAG;
	}

	return pdPASS;
}
/*-----------------------------------------------------------*/

void vSerialClose( xComPortHandle xPort )
{
}
/*-----------------------------------------------------------*/

void __attribute__((__interrupt__, auto_psv)) _U1RXInterrupt( void )
{
char cChar;
portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	/* Get the character and post it on the queue of Rxed characters.
	If the post causes a task to wake force a context switch as the woken task
	may have a higher priority than the task we have interrupted. */
	IFS0bits.U1RXIF = serCLEAR_FLAG;
    while( !U1STAHbits.URXBE )
	{
		cChar = U1RXREG;
        Nop();
		xQueueSendFromISR( xRxedChars, &cChar, &xHigherPriorityTaskWoken );
	}

	if( xHigherPriorityTaskWoken != pdFALSE )
	{
		taskYIELD();
	}
}
/*-----------------------------------------------------------*/

void __attribute__((__interrupt__, auto_psv)) _U1TXInterrupt( void )
{
signed char cChar;
portBASE_TYPE xTaskWoken = pdFALSE;

	/* If the transmit buffer is full we cannot get the next character.
	Another interrupt will occur the next time there is space so this does
	not matter. */
	IFS0bits.U1TXIF = serCLEAR_FLAG;
	while( !( U1STAHbits.UTXBF ) )
	{
		if( xQueueReceiveFromISR( xCharsForTx, &cChar, &xTaskWoken ) == pdTRUE )
		{
			/* Send the next character queued for Tx. */
			U1TXREG = cChar;
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


