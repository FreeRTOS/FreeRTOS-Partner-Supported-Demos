/**********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
 * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
 * applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 *
 * Copyright (C) 2020, 2021 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/***********************************************************************/
/*                                                                     */
/*  FILE        :Main.c                                                */
/*  DATE        :Wed, Oct 16, 2024                                     */
/*  DESCRIPTION :Main Program                                          */
/*  CPU TYPE    :                                                      */
/*                                                                     */
/*  NOTE:THIS IS A TYPICAL EXAMPLE.                                    */
/*                                                                     */
/***********************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "vector_data.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "TestRunner.h"


typedef unsigned long uint32; /*           0 .. 4294967295     */
int main( void );
void Guard_Init( void );
void vDummyISR( void )
{
    while( 1 )
    {
    }
}

#define portIPI1ENS_REGISTER              (*((volatile uint32_t *) 0xfffb9020UL))
#define portIPI1FCLRS_REGISTER            (*((volatile uint32_t *) 0xfffb9028UL))

#define PBG11H1    0xFF8B1600UL
#define PBG51      0xFFC7A400UL

/* PBGnPROT0_m */
#define PBG11PROT0( m )    ( *( volatile uint32 * ) ( PBG11H1 + m * 0x08UL ) )
#define PBG51PROT0( m )    ( *( volatile uint32 * ) ( PBG51 + m * 0x08UL ) )
#define DISABLE_WRITE_KEY_CODE    ( 0xA5A5A500UL )
#define ENABLE_WRITE_KEY_CODE     ( 0xA5A5A501UL )
#define MSRKCPROT                 ( *( ( volatile uint32 * ) 0xFF981710UL ) )
#define MSR_OSTM                  ( *( ( volatile uint32 * ) 0xFF981180UL ) )

/*    Setup peripheral guard to enable Read/write                             */
void Guard_Init( void )
{
    const uint32 RSLVXX_base[] =
    {
        ( uint32 ) 0xFFC6B000UL, /*PBGERRSLV00_base */
        ( uint32 ) 0xFFC63100UL, /*PBGERRSLV10_base */
        ( uint32 ) 0xFFDE1000UL, /*PBGERRSLV20_base */
        ( uint32 ) 0xFFC73200UL, /*PBGERRSLV30_base */
        ( uint32 ) 0xFFC75400UL, /*PBGERRSLV40_base */
        ( uint32 ) 0xFFC7B000UL, /*PBGERRSLV50_base */
        ( uint32 ) 0xFFC83000UL, /*PBGERRSLV60_base */
        ( uint32 ) 0xFFC83040UL, /*PBGERRSLV62_base */
        ( uint32 ) 0xFFF4A000UL, /*PBGERRSLV70_base */
        ( uint32 ) 0xFFF2A000UL, /*PBGERRSLV8H0_base */
        ( uint32 ) 0xFF97A200UL, /*PBGERRSLV8L0_base */
        ( uint32 ) 0xFF0A1400UL, /*PBGERRSLV90_base */
        ( uint32 ) 0xFF0A1600UL, /*PBGERRSLV91_base */
        ( uint32 ) 0xFF87A000UL, /*PBGERRSLV100_base */
        ( uint32 ) 0xFF8B2000UL, /*PBGERRSLV11H0_base */
        ( uint32 ) 0xFF8F8100UL, /*PBGERRSLV11L0_base */
    };

    const uint32 PBGXX_base[] =
    {
        ( uint32 ) 0xFFC6B080UL, /*PBG00_base */
        ( uint32 ) 0xFFC6B100UL, /*PBG01_base */
        ( uint32 ) 0xFFC63000UL, /*PBG10_base */
        ( uint32 ) 0xFFDE0B00UL, /*PBG20_base */
        ( uint32 ) 0xFFDE0C00UL, /*PBG21_base */
        ( uint32 ) 0xFFDE0D00UL, /*PBG22_base */
        ( uint32 ) 0xFFDE0E00UL, /*PBG23_base */
        ( uint32 ) 0xFFDE1200UL, /*PBG24_base */
        ( uint32 ) 0xFFC72B00UL, /*PBG30_base */
        ( uint32 ) 0xFFC72C00UL, /*PBG31_base */
        ( uint32 ) 0xFFC72D00UL, /*PBG32_base */
        ( uint32 ) 0xFFC75300UL, /*PBG40_base */
        ( uint32 ) 0xFFC75380UL, /*PBG41_base */
        ( uint32 ) 0xFFC7A300UL, /*PBG50_base */
        ( uint32 ) 0xFFC7A400UL, /*PBG51_base */
        ( uint32 ) 0xFFC7A500UL, /*PBG52_base */
        ( uint32 ) 0xFFC81000UL, /*PBG60_base */
        ( uint32 ) 0xFFC81200UL, /*PBG61_base */
        ( uint32 ) 0xFFC82000UL, /*PBG62_base */
        ( uint32 ) 0xFFF49400UL, /*PBG70_base */
        ( uint32 ) 0xFFF49600UL, /*PBG71_base */
        ( uint32 ) 0xFFF29300UL, /*PBG8H0_base */
        ( uint32 ) 0xFF97A000UL, /*PBG8L0_base */
        ( uint32 ) 0xFF0A1300UL, /*PBG90_base */
        ( uint32 ) 0xFF0A1500UL, /*PBG91_base */
        ( uint32 ) 0xFF0A1700UL, /*PBG92_base */
        ( uint32 ) 0xFF879400UL, /*PBG100_base */
        ( uint32 ) 0xFF879500UL, /*PBG101_base */
        ( uint32 ) 0xFF8B1400UL, /*PBG11H0_base */
        ( uint32 ) 0xFF8B1600UL, /*PBG11H1_base */
        ( uint32 ) 0xFF8B1800UL, /*PBG11H2_base */
        ( uint32 ) 0xFF8F8000UL, /*PBG11L0_base */
        ( uint32 ) 0xFF860000UL, /*PBG_CANFD0_base */
        ( uint32 ) 0xFF860800UL, /*PBG_CANFD1_base */
        ( uint32 ) 0xFF88FA00UL, /*PBG_ATU_base */
        ( uint32 ) 0xFFF60000UL  /*PBG_GTM_base */
    };

    /*Enable write */
    for( int i = 0; i < 16; i++ )
    {
        volatile uint32 * ptr;
        ptr = ( volatile uint32 * ) ( RSLVXX_base[ i ] + ( uint32 ) 0x18UL );
        *ptr = ( uint32 ) 0xA5A5A501UL;
    }

    /*Write PBG0 */
    for( int i = 0; i < 36; i++ )
    {
        for( int ii = 0; ii < 16; ii++ )
        {
            volatile uint32 * ptr;
            ptr = ( volatile uint32 * ) ( PBGXX_base[ i ] + ( ( uint32 ) ii * 0x8UL ) );
            *ptr |= 0x00000143UL;
        }
    }

    /*Write PBG1 */
    for( int i = 0; i < 36; i++ )
    {
        for( int ii = 0; ii < 16; ii++ )
        {
            volatile uint32 * ptr;
            ptr = ( volatile uint32 * ) ( PBGXX_base[ i ] + 0x4UL + ( ( uint32 ) ii * 0x8UL ) );
            *ptr |= 0xFFFFUL;
        }
    }

    PBG51PROT0( 6 );

    /*Disable write */
    for( int i = 0; i < 16; i++ )
    {
        volatile uint32 * ptr;
        ptr = ( volatile uint32 * ) ( RSLVXX_base[ i ] + 0x18UL );
        *ptr = 0xA5A5A500UL;
    }

    /* Release the write protection of Stndby controller register. */
    MSRKCPROT = ENABLE_WRITE_KEY_CODE;
    /* Enable OSTM */
    MSR_OSTM &= 0xFFFFFFC0UL;
    /* Set the write protection of Stndby controller registers. */
    MSRKCPROT = DISABLE_WRITE_KEY_CODE;
}

void vCommonISRHandler (int irq)
{
    if (xPortGET_CORE_ID() == 0)
    {

        if (360 == irq)
        {
            vPortTickISR();
        }
        else
        {
            vDummyISR();
        }
    }
}

int main( void )
{
    Guard_Init();
    vStartTests();

    return 0;
}
