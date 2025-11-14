/***********************************************************************/
/*  FILE        : main0.c                                              */
/*  DATE        : Wed, Oct 16, 2024                                    */
/*  DESCRIPTION : Main Program                                         */
/*                                                                     */
/*  NOTE: THIS IS A TYPICAL EXAMPLE.                                   */
/***********************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "bsp_compiler_support.h"
#include "vector_data.h"
#include "FreeRTOS.h"
#include "task.h"
#include "TestRunner.h"
#include "Config_UART0.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#if ( configNUMBER_OF_CORES > 1 )
/* Base address of PEn EIC1. */
    #define PE0EIC1                   ( 0xFFFC4002 )
    #define PE1EIC1                   ( 0xFFFC8002 )

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

#if defined( PBG11H1 )
    #undef  PBG11H1
    #define PBG11H1    ( 0xFF8B1600UL )
#endif

#if defined( PBG51 )
    #undef  PBG51
    #define PBG51    ( 0xFFC7A400UL )
#endif

/* PBGnPROT0_m */
#define PBG11PROT0( m )    ( *( volatile uint32 * ) ( PBG11H1 + m * 0x08UL ) )
#define PBG51PROT0( m )    ( *( volatile uint32 * ) ( PBG51 + m * 0x08UL ) )

#define DISABLE_WRITE_KEY_CODE    ( 0xA5A5A500UL )
#define ENABLE_WRITE_KEY_CODE     ( 0xA5A5A501UL )
#define MSRKCPROT                 ( *( ( volatile uint32 * ) 0xFF981710UL ) )
#define MSR_OSTM                  ( *( ( volatile uint32 * ) 0xFF981180UL ) )

/* Protection key codes */
#if defined( GUARD_PE0CL0 )
    #undef  GUARD_PE0CL0
    #define GUARD_PE0CL0    ( 0xFFC6C000UL )
#endif

#if defined( GUARD_PE1CL0 )
    #undef  GUARD_PE1CL0
    #define GUARD_PE1CL0    ( 0xFFC6C100UL )
#endif

#if defined( GUARD_PE2CL1 )
    #undef  GUARD_PE2CL1
    #define GUARD_PE2CL1    ( 0xFFC6C200UL )
#endif

#define PEGKCPROT_PE0       ( *( ( volatile uint32 * ) GUARD_PE0CL0 ) )
#define PEGKCPROT_PE1       ( *( ( volatile uint32 * ) GUARD_PE1CL0 ) )
#define PEGKCPROT_PE2       ( *( ( volatile uint32 * ) GUARD_PE2CL1 ) )

#define PEGPROT0( m )    ( *( volatile uint32 * ) ( GUARD_PE0CL0 + 0x40UL + m * 0x10UL ) )
#define PEGPROT1( m )    ( *( volatile uint32 * ) ( GUARD_PE1CL0 + 0x40UL + m * 0x10UL ) )
#define PEGPROT2( m )    ( *( volatile uint32 * ) ( GUARD_PE2CL1 + 0x40UL + m * 0x10UL ) )

/* Enable MSR for OSTM. */
#define MSR_OSTM_ENABLE    ( 0xFFFFFFC0UL )

/* Transmission completion flag for UART0. */
uint32_t uart0_tx_completed_flag = 0U;

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
typedef void (* int_vector_t)( void );
typedef unsigned long uint32; /* 0 .. 4294967295 */

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
void main( void );
void Guard_Init( void );
extern void vPortTickISR( void );
extern void r_Config_UART0_interrupt_send( void );
void r_Config_UART0_callback_sendend( void );

#if ( configNUMBER_OF_CORES > 1 )
    void main_PE1( void );

    extern void vPortIPIHander( void );
    void vPortIPIRInit( void );
    void setup_PE0_EIC1();
    void setup_PE1_EIC1();
#endif /* End of #if (configNUMBER_OF_CORES > 1) */

/***********************************************************************************************************************
 * Vector Table Configuration
 **********************************************************************************************************************/
/* Interrupt vector table for PE0. Mapping into the memory section ".inttable_PE0". */
#pragma section const ".inttable_PE0"
const int_vector_t g_vector_table_PE0[ RTOS_VECTOR_TABLE_MAX_ENTRIES ] =
{
    #if ( configNUMBER_OF_CORES > 1 )
        [ 1 ] = vPortIPIHander,              /* Internal processor interrupt 1 */
    #endif /* End of #if (configNUMBER_OF_CORES > 1) */
    [ 200 ] = vPortTickISR,                  /* INTOSTM1TINT (OSTM1 interrupt) */
    [ 417 ] = r_Config_UART0_interrupt_send, /* EIC417: INTRLIN30UR0 (RLIN30 transmit interrupt) */
};

#if ( configNUMBER_OF_CORES > 1 )
    /* Interrupt vector table for PE1. Mapping into the memory section ".inttable_PE1". */
    #pragma section const ".inttable_PE1"
    const int_vector_t g_vector_table_PE1[ RTOS_VECTOR_TABLE_MAX_ENTRIES ] =
    {
        [ 1 ] = vPortIPIHander,            /* Internal processor interrupt 1 */
    };
#endif /* End of #if (configNUMBER_OF_CORES > 1) */

#pragma section default

/***********************************************************************************************************************
 * Callback function
 **********************************************************************************************************************/
/* UART0 transmit complete callback function. */
void r_Config_UART0_callback_sendend( void )
{
    /* Start user code for r_Config_UART0_callback_sendend. Do not edit comment generated here. */
    /* End user code. Do not edit comment generated here. */
    uart0_tx_completed_flag = 1U;
}

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
        else
        {
            /* Do nothing. */
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

/***********************************************************************************************************************
 * Main functions
 **********************************************************************************************************************/
/* main function of PE0. */
void main( void )
{
    Guard_Init();

    #if ( configNUMBER_OF_CORES > 1 )
        /* Setup interrupt of EIC1 for PE0. */
        setup_PE0_EIC1();

        /* Configure interrupt for IPIR interrupt in secondary core to receive request from primary core.
         * Primary core is the core which start scheduler. */
        vPortIPIRInit();
    #endif /* End of #if (configNUMBER_OF_CORES > 1) */

    /* Init uart peripheral */
    R_Config_UART0_Create();

    /* Start uart peripheral */
    R_Config_UART0_Start();

    /* Test runner */
    vStartTests();
}

#if ( configNUMBER_OF_CORES > 1 )
/* main function of PE1. */
    void main_PE1( void )
    {
        setup_PE1_EIC1();

        /* Configure interrupt for IPIR interrupt in secondary core to receive request from primary core.
         * Primary core is the core which start scheduler. */
        vPortIPIRInit();

        /* The interrupt may be disabled by default. Just enable it. */
        __EI();

        /* Do nothing. Loop to keep PE1 run. */
        while( 1 )
        {
        }
    }
#endif /* End of #if (configNUMBER_OF_CORES > 1) */
