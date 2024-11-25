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

#if (configNUMBER_OF_CORES > 1)
void main_PE1(void);
extern void vPortIPIHander(void);
extern void setup_PE0_EIC0();
extern void setup_PE1_EIC0();
#endif

#if (configNUMBER_OF_CORES > 2)
void main_PE2(void);
#endif

#define PE0EIC0              (0xfffc4002)
#define PE1EIC0              (0xfffc8002)

#if (configNUMBER_OF_CORES > 1)
void setup_PE0_EIC0 (void)
{
    volatile uint16_t * pPE0EIC0RegAddr;

    pPE0EIC0RegAddr = (volatile uint16_t *) PE0EIC0;
    /* Initial Interrupt Vector Method and Enable Interrupt */
    *pPE0EIC0RegAddr = 0x8000;
}


void setup_PE1_EIC0 (void)
{
    volatile uint16_t * pPE1EIC0RegAddr;
    
    pPE1EIC0RegAddr = (volatile uint16_t *) PE1EIC0;
    /* Initial Interrupt Vector Method and Enable Interrupt */
    *pPE1EIC0RegAddr = 0x8005;
}

#endif

extern void vPortTickISR(void);

void Guard_Init(void);

typedef void (* int_vector_t)(void);
typedef unsigned long       uint32;         /*           0 .. 4294967295     */

const int_vector_t g_vector_table_PE0[RTOS_VECTOR_TABLE_MAX_ENTRIES] =
{
#if (configNUMBER_OF_CORES > 1)
    [1]   = vPortIPIHander,            /* Internal processor interrupt 0 */
#endif
    [200] = vPortTickISR,               /* INTOSTM1TINT (OSTM1 interrupt) */
};

const int_vector_t g_vector_table_PE1[RTOS_VECTOR_TABLE_MAX_ENTRIES] =
{
#if (configNUMBER_OF_CORES > 1)
    [1]  = vPortIPIHander,                 /* Internal processor interrupt 0 */
#else
    [0]  = vDummyISR,
#endif
};
#define portIPI1ENS_REGISTER              (*((volatile uint32_t *) 0xfffb9020UL))
#define portIPI1FCLRS_REGISTER            (*((volatile uint32_t *) 0xfffb9028UL))

#define PBG11H1                        0xFF8B1600UL
#define PBG51                          0xFFC7A400UL

/* PBGnPROT0_m */
#define PBG11PROT0(m)   (*(volatile uint32*)(PBG11H1 + m * 0x08UL))
#define PBG51PROT0(m)   (*(volatile uint32*)(PBG51 + m * 0x08UL))
#define DISABLE_WRITE_KEY_CODE  (0xA5A5A500UL)
#define ENABLE_WRITE_KEY_CODE   (0xA5A5A501UL)
#define MSRKCPROT           (*((volatile uint32 *)0xFF981710UL))
#define MSR_OSTM            (*((volatile uint32 *)0xFF981180UL))
/* Protection key codes */
#define GUARD_PE0CL0             0xffc6c000UL
#define GUARD_PE1CL0             0xffc6c100UL
#define GUARD_PE2CL1             0xffc6c200UL

#define PEGKCPROT_PE0           (*(volatile uint32 *)(GUARD_PE0CL0))
#define PEGKCPROT_PE1           (*((volatile uint32 *)GUARD_PE1CL0))
#define PEGKCPROT_PE2           (*((volatile uint32 *)GUARD_PE2CL1))

#define PEGPROT0(m)             (*(volatile uint32 *)(GUARD_PE0CL0 + 0x40UL + m * 0x10UL))
#define PEGPROT1(m)             (*(volatile uint32 *)(GUARD_PE1CL0 + 0x40UL + m * 0x10UL))
#define PEGPROT2(m)             (*(volatile uint32 *)(GUARD_PE2CL1 + 0x40UL + m * 0x10UL))
/*    Setup peripheral guard to enable Read/write                             */
void Guard_Init(void)
{
  const uint32 RSLVXX_base[]=
  {
  (uint32) 0xFFC6B000UL, //PBGERRSLV00_base
  (uint32) 0xFFC63100UL, //PBGERRSLV10_base
  (uint32) 0xFFDE1000UL, //PBGERRSLV20_base
  (uint32) 0xFFC73200UL, //PBGERRSLV30_base
  (uint32) 0xFFC75400UL, //PBGERRSLV40_base
  (uint32) 0xFFC7B000UL, //PBGERRSLV50_base
  (uint32) 0xFFC83000UL, //PBGERRSLV60_base
  (uint32) 0xFFC83040UL, //PBGERRSLV62_base
  (uint32) 0xFFF4A000UL, //PBGERRSLV70_base
  (uint32) 0xFFF2A000UL, //PBGERRSLV8H0_base
  (uint32) 0xFF97A200UL, //PBGERRSLV8L0_base
  (uint32) 0xFF0A1400UL, //PBGERRSLV90_base
  (uint32) 0xFF0A1600UL, //PBGERRSLV91_base
  (uint32) 0xFF87A000UL, //PBGERRSLV100_base
  (uint32) 0xFF8B2000UL, //PBGERRSLV11H0_base
  (uint32) 0xFF8F8100UL,  //PBGERRSLV11L0_base
  };

  const uint32 PBGXX_base[]=
  {
  (uint32) 0xFFC6B080UL, //PBG00_base
  (uint32) 0xFFC6B100UL, //PBG01_base
  (uint32) 0xFFC63000UL, //PBG10_base
  (uint32) 0xFFDE0B00UL, //PBG20_base
  (uint32) 0xFFDE0C00UL, //PBG21_base
  (uint32) 0xFFDE0D00UL, //PBG22_base
  (uint32) 0xFFDE0E00UL, //PBG23_base
  (uint32) 0xFFDE1200UL, //PBG24_base
  (uint32) 0xFFC72B00UL, //PBG30_base
  (uint32) 0xFFC72C00UL, //PBG31_base
  (uint32) 0xFFC72D00UL, //PBG32_base
  (uint32) 0xFFC75300UL, //PBG40_base
  (uint32) 0xFFC75380UL, //PBG41_base
  (uint32) 0xFFC7A300UL, //PBG50_base
  (uint32) 0xFFC7A400UL, //PBG51_base
  (uint32) 0xFFC7A500UL, //PBG52_base
  (uint32) 0xFFC81000UL, //PBG60_base
  (uint32) 0xFFC81200UL, //PBG61_base
  (uint32) 0xFFC82000UL, //PBG62_base
  (uint32) 0xFFF49400UL, //PBG70_base
  (uint32) 0xFFF49600UL, //PBG71_base
  (uint32) 0xFFF29300UL, //PBG8H0_base
  (uint32) 0xFF97A000UL, //PBG8L0_base
  (uint32) 0xFF0A1300UL, //PBG90_base
  (uint32) 0xFF0A1500UL, //PBG91_base
  (uint32) 0xFF0A1700UL, //PBG92_base
  (uint32) 0xFF879400UL, //PBG100_base
  (uint32) 0xFF879500UL, //PBG101_base
  (uint32) 0xFF8B1400UL, //PBG11H0_base
  (uint32) 0xFF8B1600UL, //PBG11H1_base
  (uint32) 0xFF8B1800UL, //PBG11H2_base
  (uint32) 0xFF8F8000UL, //PBG11L0_base    
  (uint32) 0xFF860000UL, //PBG_CANFD0_base
  (uint32) 0xFF860800UL, //PBG_CANFD1_base
  (uint32) 0xFF88FA00UL, //PBG_ATU_base
  (uint32) 0xFFF60000UL  //PBG_GTM_base
  };
  
  /* Release the write protection of Stndby controller register. */
  MSRKCPROT = ENABLE_WRITE_KEY_CODE;
  /* Enable OSTM */
  MSR_OSTM &= 0xFFFFFFC0UL;
  /* Set the write protection of Stndby controller registers. */
  MSRKCPROT = DISABLE_WRITE_KEY_CODE;
  
//  GuardFuncInitialized = pdPASS;
}

void vDummyISR(void)
{
    while(1);
}

uint32_t uart0_tx_completed_flag = 0; 

void r_Config_UART0_callback_sendend(void);

void r_Config_UART0_callback_sendend(void)
{
    /* Start user code for r_Config_UART0_callback_sendend. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
    uart0_tx_completed_flag  = 1;
}
void vPortIPIRInit (void)
{
    /* Determine the IPI register based on the target core ID */
    if (0 == xPortGET_CORE_ID())
    {
        /* Clear Request Flag and Enable receive interrupt from Transmit PE1, PE2 */
        portIPI1FCLRS_REGISTER = 0x0F;
        portIPI1ENS_REGISTER = 0x02;
    }
    else if (1 == xPortGET_CORE_ID())
    {
        /* Clear Request Flag and Enable receive interrupt from Transmit PE0, PE2 */
        portIPI1FCLRS_REGISTER = 0x0F;
        portIPI1ENS_REGISTER = 0x01;
    }
}


void main(void)
{
    Guard_Init();

    /*setup interrupt for PE1*/
    setup_PE0_EIC0();
    
    /* Configure interrupt for IPIR interrupt in secondary core to receive request from primary core
     * Primary core is the core which start scheduler.
     */
    vPortIPIRInit();
    
    /* Test runner */
    vStartTests();
    
}

#if (configNUMBER_OF_CORES >= 2)
void main_PE1(void)
{
    setup_PE1_EIC0();
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