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


int main( void );
void Guard_Init( void );

/*typedef void (* int_vector_t)(void); */
typedef unsigned long uint32; /*           0 .. 4294967295     */
void vDummyISR( void )
{
    while( 1 )
    {
    }
}

#if (configNUMBER_OF_CORES > 1)
#define PE0EIC1              (0xfffc4002)
#define PE1EIC1              (0xfffc8002)
#define PE2EIC1              (0xfffcc002)

extern void setup_PE0_EIC1 (void)
{
    volatile uint16_t * pPE0EIC1RegAddr;
    pPE0EIC1RegAddr = (volatile uint16_t *) PE0EIC1;
    /* Initial Interrupt Vector Method and Enable Interrupt */
    *pPE0EIC1RegAddr = 0x8000;
}

extern void setup_PE1_EIC1 (void)
{
    volatile uint16_t * pPE1EIC1RegAddr;
    pPE1EIC1RegAddr = (volatile uint16_t *) PE1EIC1;
    /* Initial Interrupt Vector Method and Enable Interrupt */
    *pPE1EIC1RegAddr = 0x8005;
}
#endif

#define portIPI1ENS_REGISTER              (*((volatile uint32_t *) 0xfffb9020UL))
#define portIPI1FCLRS_REGISTER            (*((volatile uint32_t *) 0xfffb9028UL))

#define DISABLE_WRITE_KEY_CODE    ( 0xA5A5A500UL )
#define ENABLE_WRITE_KEY_CODE     ( 0xA5A5A501UL )
#define MSRKCPROT                 ( *( ( volatile uint32 * ) 0xFF981710UL ) )
#define MSR_OSTM                  ( *( ( volatile uint32 * ) 0xFF981180UL ) )

/*    Setup peripheral guard to enable Read/write                             */
void Guard_Init( void )
{
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
        if (200 == irq)
        {
            vPortTickISR();
        }
        else
        {
            vDummyISR();
        }
    }
}

/*void vPortTickISR(void); */
#if (configNUMBER_OF_CORES > 1)
void vPortIPIRInit (void)
{
    /* Determine the IPI register based on the target core ID */
    if (0 == xPortGET_CORE_ID())
    {
        /* Clear Request Flag and Enable receive interrupt from Transmit PE1 */
        portIPI1FCLRS_REGISTER = 0x0F;
        portIPI1ENS_REGISTER = 0x02;
    }
    else if (1 == xPortGET_CORE_ID())
    {
        /* Clear Request Flag and Enable receive interrupt from Transmit PE0 */
        portIPI1FCLRS_REGISTER = 0x0F;
        portIPI1ENS_REGISTER = 0x01;
    }
}
#endif


int main( void )
{
    Guard_Init();

    #if (configNUMBER_OF_CORES > 1)
    vPortIPIRInit();
    #endif

    printf("Start TESTING");
    asm("nop");
    vStartTests();

    return 0;
}



#if (configNUMBER_OF_CORES >= 2)
void main_PE1(void)
{
    setup_PE1_EIC1();
    /* Configure interrupt for IPIR interrupt in secondary core to receive request from primary core
     * Primary core is the core which start scheduler.
     */
    vPortIPIRInit();

    /* The interrupt may be disabled by default. Just enable it */
    __EI();

    /* Do nothing. Loop to keep PE2 run */
    while(1);
}
#endif //#if (configNUMBER_OF_CORES >= 2)

