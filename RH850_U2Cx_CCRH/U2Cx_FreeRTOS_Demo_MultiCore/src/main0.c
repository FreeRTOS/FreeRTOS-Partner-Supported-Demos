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
 * Copyright (C) 2025 Renesas Electronics Corporation. All rights reserved.
 *********************************************************************************************************************/
/***********************************************************************/
/*                                                                     */
/*  FILE        :main0.c                                               */
/*  DATE        :Tue, Aug 19, 2025                                     */
/*  DESCRIPTION :Main Program                                          */
/*  CPU TYPE    :                                                      */
/*                                                                     */
/*  NOTE:THIS IS A TYPICAL EXAMPLE.                                    */
/*                                                                     */
/***********************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "bsp_common.h"
#include "FreeRTOS.h"
#include "TestRunner.h"

void main(void);

void vTaskAssert(char file[], uint32_t line)
{
	(void)file;
	(void)line;
	DI();
	while(1)
	{
		__nop();
	}
}

#define PE0EIC0              (0xfffc4002)
extern void setup_PE0_EIC0();
extern void vPortIPIHander(void);

#if (configNUMBER_OF_CORES > 1)
extern void setup_PE1_EIC0();
void main_PE1(void);
#endif //#if (configNUMBER_OF_CORES > 1)

#define PE1EIC0              (0xfffc8002)

void setup_PE0_EIC0 (void)
{
    volatile uint16_t * pPE0EIC0RegAddr;

    pPE0EIC0RegAddr = (volatile uint16_t *) PE0EIC0;

    /* Initial Interrupt Vector Method and Enable Interrupt. */
    *pPE0EIC0RegAddr = 0x8000;
}

#if (configNUMBER_OF_CORES > 1)
void setup_PE1_EIC0 (void)
{
    volatile uint16_t * pPE1EIC0RegAddr;
    
    pPE1EIC0RegAddr = (volatile uint16_t *) PE1EIC0;

    /* Initial Interrupt Vector Method and Enable Interrupt. */
    *pPE1EIC0RegAddr = 0x8005;
}
#endif //#if (configNUMBER_OF_CORES > 1)

extern void vPortTickISR(void);

void Guard_Init(void);

typedef void (* int_vector_t)(void);
typedef unsigned long        uint32;   /* 0 .. 4294967295 */

const int_vector_t g_vector_table_PE0[RTOS_VECTOR_TABLE_MAX_ENTRIES] =
{
#if (configNUMBER_OF_CORES > 1)
    [1]   = vPortIPIHander,            /* Internal processor interrupt 0 */
#endif
    [63]  = vPortTickISR,              /* INTOSTM0TINT (OSTM0 interrupt) */
};

const int_vector_t g_vector_table_PE1[RTOS_VECTOR_TABLE_MAX_ENTRIES] =
{
#if (configNUMBER_OF_CORES > 1)
    [1]  = vPortIPIHander,             /* Internal processor interrupt 0 */
#else
    [0]  = vDummyISR,
#endif
};

#define portIPI1ENS_REGISTER           (*((volatile uint32_t *) 0xfffb9020UL))
#define portIPI1FCLRS_REGISTER         (*((volatile uint32_t *) 0xfffb9028UL))

#ifdef PBG11H1
  #undef PBG11H1
  #define PBG11H1                      (uint32) 0xFF8B1600UL
#endif

#ifdef PBG51
  #undef PBG51
  #define PBG51                        (uint32) 0xFFC7A400UL
#endif

/* PBGnPROT0_m */
#define PBG11PROT0(m)                  (*(volatile uint32*)(PBG11H1 + m * 0x08UL))
#define PBG51PROT0(m)                  (*(volatile uint32*)(PBG51 + m * 0x08UL))
#define DISABLE_WRITE_KEY_CODE         (0xA5A5A500UL)
#define ENABLE_WRITE_KEY_CODE          (0xA5A5A501UL)
#define MSRKCPROT                      (*((volatile uint32 *)0xFF981710UL))
#define MSR_OSTM                       (*((volatile uint32 *)0xFF981180UL))

/* Protection key codes */
#ifdef GUARD_PE0CL0
  #undef GUARD_PE0CL0
  #define GUARD_PE0CL0                 (uint32) 0xffc6c000UL
#endif

#ifdef GUARD_PE1CL0
  #undef GUARD_PE1CL0
  #define GUARD_PE1CL0                 (uint32) 0xffc6c100UL
#endif

#define PEGKCPROT_PE0                  (*(volatile uint32 *)(GUARD_PE0CL0))
#define PEGKCPROT_PE1                  (*(volatile uint32 *)(GUARD_PE1CL0))

#define PEGPROT0(m)                    (*(volatile uint32 *)(GUARD_PE0CL0 + 0x40UL + m * 0x10UL))
#define PEGPROT1(m)                    (*(volatile uint32 *)(GUARD_PE1CL0 + 0x40UL + m * 0x10UL))


/* Setup peripheral guard to enable Read/write. */
void Guard_Init(void)
{ 
  /* Release the write protection of Stndby controller register. */
  MSRKCPROT = ENABLE_WRITE_KEY_CODE;

  /* Enable OSTM. */
  MSR_OSTM &= 0xFFFFFFC0UL;

  /* Set the write protection of Stndby controller registers. */
  MSRKCPROT = DISABLE_WRITE_KEY_CODE;
}

void vDummyISR(void)
{
    while(1);
}

uint32_t uart0_tx_completed_flag = 0; 

void r_Config_UART0_callback_sendend(void);

void r_Config_UART0_callback_sendend(void)
{
    /* Start user code for r_Config_UART0_callback_sendend. Do not edit comment generated here.
     * End user code. Do not edit comment generated here. */
    uart0_tx_completed_flag  = 1;
}

void vPortIPIRInit (void)
{
    /* Determine the IPI register based on the target core ID. */
    if (0 == xPortGET_CORE_ID())
    {
        /* Clear Request Flag and Enable receive interrupt from Transmit PE1. */
        portIPI1FCLRS_REGISTER = 0x0F;
        portIPI1ENS_REGISTER   = 0x02;
    }
    else if (1 == xPortGET_CORE_ID())
    {
        /* Clear Request Flag and Enable receive interrupt from Transmit PE0. */
        portIPI1FCLRS_REGISTER = 0x0F;
        portIPI1ENS_REGISTER   = 0x01;
    }
}

void main(void)
{
    Guard_Init();

    /* Setup interrupt for PE0. */
    setup_PE0_EIC0();
    
    /* Configure interrupt for IPIR interrupt in secondary core to receive request from primary core
     * Primary core is the core which start scheduler. */
    vPortIPIRInit();
    
    /* Test runner. */
    vStartTests();
}

#if (configNUMBER_OF_CORES > 1)
void main_PE1(void)
{
    setup_PE1_EIC0();
    /* Configure interrupt for IPIR interrupt in secondary core to receive request from primary core
     * Primary core is the core which start scheduler. */
    vPortIPIRInit();

    /* The interrupt may be disabled by default. Just enable it. */
    __EI();

    /* Do nothing. Loop to keep running. */
    while(1);
}
#endif //#if (configNUMBER_OF_CORES > 1)

void vApplicationIdleHook(void)
{

}

void vCommonISRHandler (int irq)
{
    if (xPortGET_CORE_ID() == 0)
    {
        g_vector_table_PE0[irq]();
    }
    else if (xPortGET_CORE_ID() == 1)
    {
        g_vector_table_PE1[irq]();
    }
}
