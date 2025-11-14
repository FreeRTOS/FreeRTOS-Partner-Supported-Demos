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
#if ( configNUMBER_OF_CORES > 1 )
/* Base address of PEn EIC1. */
    #define PE0EIC1                   ( 0xFFFC4002 )
    #define PE1EIC1                   ( 0xFFFC8002 )
    #define PE2EIC1                   ( 0xFFFCC002 )

/* Set EICTn to Detection by level, EITBn to Table reference method. */
    #define PEn_EIC1_TAB_REF          ( 0x8000 )

/* Base address of IPIR registers (Channel 1). */
    #define portIPI1ENS_REGISTER      ( *( ( volatile uint32_t * ) 0xfffb9020UL ) )
    #define portIPI1FCLRS_REGISTER    ( *( ( volatile uint32_t * ) 0xfffb9028UL ) )

/* Clear interrupt requests received from other PEn. */
    #define IPI1FCLRS_CLEAR           ( 0x0F )

/* Macro value of PEn. */
    #define CORE_PE0                  ( BaseType_t ) ( 0 )
    #define CORE_PE1                  ( BaseType_t ) ( 1 )

/* IPIR notification values per Core. */
    #define IPIR_PE0_NOTIFY_IN_SMP    ( 0x02 )
    #define IPIR_PE1_NOTIFY_IN_SMP    ( 0x01 )
#endif /* End of #if (configNUMBER_OF_CORES > 1) */

#define DISABLE_WRITE_KEY_CODE        ( 0xA5A5A500UL )
#define ENABLE_WRITE_KEY_CODE         ( 0xA5A5A501UL )
#define MSRKCPROT                     ( *( ( volatile uint32 * ) 0xFF981710UL ) )
#define MSR_OSTM                      ( *( ( volatile uint32 * ) 0xFF981180UL ) )

/* Enable MSR for OSTM. */
#define MSR_OSTM_ENABLE               ( 0xFFFFFFC0UL )

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

#if ( configNUMBER_OF_CORES > 1 )
    void main_PE1( void );

    extern void vPortIPIHander( void );
    void vPortIPIRInit( void );
    void setup_PE0_EIC1();
    void setup_PE1_EIC1();
#endif /* End of #if (configNUMBER_OF_CORES > 1) */

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

#if ( configNUMBER_OF_CORES > 1 )
/* Initializes Inter-Processor Interrupt (IPI) settings based on the current Core ID. */
    void vPortIPIRInit( void )
    {
        /* Determine the IPI register based on the target core ID. */
        if( CORE_PE0 == xPortGET_CORE_ID() )
        {
            /* Clear Request Flag and Enable receive interrupt from Transmit PE1. */
            portIPI1FCLRS_REGISTER = IPI1FCLRS_CLEAR;
            portIPI1ENS_REGISTER = IPIR_PE0_NOTIFY_IN_SMP;
        }
        else if( CORE_PE1 == xPortGET_CORE_ID() )
        {
            /* Clear Request Flag and Enable receive interrupt from Transmit PE0. */
            portIPI1FCLRS_REGISTER = IPI1FCLRS_CLEAR;
            portIPI1ENS_REGISTER = IPIR_PE1_NOTIFY_IN_SMP;
        }
    }

/* Setup the Interrupt Vector EIC1 for PE0. */
    void setup_PE0_EIC1( void )
    {
        volatile uint16_t * pPE0EIC1RegAddr;

        pPE0EIC1RegAddr = ( volatile uint16_t * ) PE0EIC1;

        /* Enable Interrupt by setting Interrupt Mask. */
        /* Direct Vector Method is configured by EBASE.DV */
        *pPE0EIC1RegAddr = PEn_EIC1_TAB_REF;
    }

/* Setup the Interrupt Vector EIC1 for PE1. */
    void setup_PE1_EIC1( void )
    {
        volatile uint16_t * pPE1EIC1RegAddr;

        pPE1EIC1RegAddr = ( volatile uint16_t * ) PE1EIC1;

        /* Enable Interrupt by setting Interrupt Mask. */
        /* Direct Vector Method is configured by EBASE.DV */
        *pPE1EIC1RegAddr = PEn_EIC1_TAB_REF;
    }
#endif /* End of #if (configNUMBER_OF_CORES > 1) */

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

    #if ( configNUMBER_OF_CORES > 1 )
        /* Setup interrupt of EIC1 for PE0. */
        setup_PE0_EIC1();

        /* Configure interrupt for IPIR interrupt in secondary core to receive request from primary core.
         * Primary core is the core which start scheduler. */
        vPortIPIRInit();
    #endif /* End of #if (configNUMBER_OF_CORES > 1) */

    printf( "Start TESTING:\r\n" );
    asm ( "nop" );

    vStartTests();

    return 0;
}

#if ( configNUMBER_OF_CORES > 1 )
/* main function of PE1. */
    void main_PE1( void )
    {
        setup_PE1_EIC1();

        /* Configure interrupt for IPIR interrupt in secondary core to receive request from primary core
         * Primary core is the core which start scheduler. */
        vPortIPIRInit();

        /* The interrupt may be disabled by default. Just enable it. */
        __EI();

        /* Do nothing. Loop to keep PE2 run. */
        while( 1 )
        {
        }
    }
#endif /* End of #if (configNUMBER_OF_CORES > 1) */

/***********************************************************************************************************************
 * Vector Table Configuration
 **********************************************************************************************************************/
/* Interrupt vector table for PE0. Mapping into the memory section ".inttable_PE0". */
#pragma ghs section rodata=".inttable_PE0"
const int_vector_t g_vector_table_PE0[ 365 ] =
{
    [ 1 ] = vPortIPIHander, /* Internal processor interrupt 1 */
    [ 200 ] = vPortTickISR, /* INTOSTM1TINT (OSTM1 interrupt) */
};

#if ( configNUMBER_OF_CORES > 1 )
/* Interrupt vector table for PE1. Mapping into the memory section ".inttable_PE1". */
    #pragma ghs section rodata=".inttable_PE1"
    const int_vector_t g_vector_table_PE1[ 365 ] =
    {
        [ 1 ] = vPortIPIHander, /* Internal processor interrupt 1 */
    };
#endif /* End of #if (configNUMBER_OF_CORES > 1) */

#pragma ghs section text=default
