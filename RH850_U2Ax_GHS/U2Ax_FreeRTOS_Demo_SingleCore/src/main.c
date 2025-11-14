/***********************************************************************/
/*  FILE        : main.c                                               */
/*  DATE        : Wed, Oct 16, 2024                                    */
/*  DESCRIPTION : Main Program                                         */
/*                                                                     */
/*  NOTE: THIS IS A TYPICAL EXAMPLE.                                   */
/***********************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "vector_data.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "TestRunner.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define DISABLE_WRITE_KEY_CODE    ( 0xA5A5A500UL )
#define ENABLE_WRITE_KEY_CODE     ( 0xA5A5A501UL )
#define MSRKCPROT                 ( *( ( volatile uint32 * ) 0xFF981710UL ) )
#define MSR_OSTM                  ( *( ( volatile uint32 * ) 0xFF981180UL ) )

/* Enable MSR for OSTM. */
#define MSR_OSTM_ENABLE           ( 0xFFFFFFC0UL )

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
typedef unsigned long uint32; /* 0 .. 4294967295 */

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
int main( void );
void Guard_Init( void );
extern void vPortTickISR( void );

#if ( configCHECK_FOR_STACK_OVERFLOW > 0 )
    void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                        char * pcTaskName );
#endif /* End of #if (configCHECK_FOR_STACK_OVERFLOW > 0) */

/***********************************************************************************************************************
 * Internal functions
 **********************************************************************************************************************/
/* Setup peripheral guard to enable Read/Write. */
void Guard_Init( void )
{
    /* Release the write protection of Stndby controller register. */
    MSRKCPROT = ENABLE_WRITE_KEY_CODE;

    /* Enable OSTM. */
    MSR_OSTM &= MSR_OSTM_ENABLE;

    /* Set the write protection of Stndby controller registers. */
    MSRKCPROT = DISABLE_WRITE_KEY_CODE;
}

#if ( configCHECK_FOR_STACK_OVERFLOW > 0 )
    void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                        char * pcTaskName )
    {
        /* Check pcTaskName for the name of the offending task,
         * or pxCurrentTCB if pcTaskName has itself been corrupted. */
        ( void ) xTask;
        ( void ) pcTaskName;

        while( 1 )
        {
        }
    }
#endif /* End of #if (configCHECK_FOR_STACK_OVERFLOW > 0) */

/***********************************************************************************************************************
 * Main functions
 **********************************************************************************************************************/
/* main function of PE0. */
int main( void )
{
    Guard_Init();

    printf( "Start TESTING:\r\n" );
    asm ( "nop" );

    vStartTests();

    return 0;
}

/***********************************************************************************************************************
 * Vector Table Configuration
 **********************************************************************************************************************/
/* Interrupt vector table for PE0. Mapping into the memory section ".inttable_PE0". */
#pragma ghs section rodata=".inttable_PE0"
const int_vector_t g_vector_table_PE0[ 365 ] =
{
    [ 200 ] = vPortTickISR, /* INTOSTM1TINT (OSTM1 interrupt) */
};
