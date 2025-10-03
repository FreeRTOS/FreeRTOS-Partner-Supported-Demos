/* Copyright 2025 Arm Limited and/or its affiliates <open-source-office@arm.com>
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "gic.h"

void vGIC_EnableCPUInterface( void )
{
    /* Enable delivery of Group-1 IRQs to EL1 via ICC_IGRPEN1_EL1 */
    __asm volatile(
        "msr ICC_IGRPEN1_EL1, %0\n"
        "dsb sy\n"
        "isb sy\n"
        :: "r"(1UL)
        : "memory"
    );
}

void vGIC_PowerUpRedistributor( void )
{
    volatile uint32_t *pulPwrr = ( volatile uint32_t * )( ( portPOINTER_SIZE_TYPE ) GICR_BASE_PER_CORE( ucPortGetCoreID() ) +
                                                          ( portPOINTER_SIZE_TYPE ) GICR_PWRR );

    volatile uint32_t *pulWaker = ( volatile uint32_t * )( ( portPOINTER_SIZE_TYPE ) GICR_BASE_PER_CORE( ucPortGetCoreID() ) +
                                                           ( portPOINTER_SIZE_TYPE ) GICR_WAKER );

    uint32_t ulTimeout = GIC_WAIT_TIMEOUT;

    /* Clear RDPD (Redistributor Power-Down) to 0 → power on the redistributor */
    *pulPwrr &= ~(1U << GICR_PWRR_RDPD_BIT);
    /* Wait until RDPD reads 0 (powered up) */
    while( (*pulPwrr & (1U << GICR_PWRR_RDPD_BIT)) != 0U )
    {
        if( --ulTimeout == 0U )
        {
            configASSERT( "GICR_PWRR_RDPD_BIT did not clear in time" );
            return;
        }
    }

    /* Clear ProcessorSleep to 0 → wake hardware threads */
    *pulWaker &= ~(1U << GICR_WAKER_PS_BIT);
    ulTimeout = GIC_WAIT_TIMEOUT;
    /* Wait until ChildrenAsleep reads 0 (all subcomponents awake) */
    while( (*pulWaker & (1U << GICR_WAKER_CA_BIT)) != 0U )
    {
        if( --ulTimeout == 0U )
        {
            configASSERT( "GICR_WAKER_CA_BIT did not clear in time" );
            return;
        }
    }
    __asm volatile ( "dsb sy" ::: "memory" );
    __asm volatile ( "isb sy" ::: "memory" );
}

void vGIC_InitDist( void )
{
    /* Enable Group-1 Non-Secure (NS) and Secure (S) interrupts, and turn on Affinity-routing (ARE_S)
     *plus Disable-Security (DS) for full GICv3 operation
     */
    *( volatile uint32_t * )( ( portPOINTER_SIZE_TYPE ) GICD_BASE + ( portPOINTER_SIZE_TYPE ) GICD_CTLR )  = ( 1 << GICD_CTLR_ENABLEGRP1NS_BIT ) |
                                                                                                             ( 1 << GICD_CTLR_ENABLEGRP1S_BIT )  |
                                                                                                             ( 1 << GICD_CTLR_ARES_BIT )         |
                                                                                                             ( 1 << GICD_CTLR_DS_BIT );

    /* Ensure distributor configuration is visible before continuing */
    __asm volatile ( "dsb sy" ::: "memory" );
    __asm volatile ( "isb sy" ::: "memory" );
}

void vGIC_SetPriority( uint32_t ulInterruptID, uint32_t ulPriority )
{
    if( ( ulInterruptID <= GIC_MAX_INTERRUPT_ID ) && ( ulPriority <= 0xFF ) )
    {
        /* Each GICR_IPRIORITYR<n> holds 4 one‑byte priority fields.         */
        uint32_t ulIndex  = ulInterruptID / 4U;          /* Register number  */
        uint32_t ulShift  = ( ulInterruptID % 4U ) * 8U; /* Byte lane offset */
        uint32_t ulMask   = 0xFFUL << ulShift;           /* Field mask       */

        volatile uint32_t * pulPriorityReg = ( volatile uint32_t * ) ( ( portPOINTER_SIZE_TYPE ) GICR_BASE_PER_CORE( ucPortGetCoreID() ) +
                                                                       ( portPOINTER_SIZE_TYPE ) GICR_IPRIORITYR( ulIndex ) );
        uint32_t ulRegValue = *pulPriorityReg;
        ulRegValue &= ~( ulMask );
        ulRegValue |= ( ( uint32_t ) ulPriority << ulShift );
        *pulPriorityReg = ulRegValue;
        /* Ensure priority write is observed */
        __asm volatile ( "dsb ish" ::: "memory" );
    }
    else
    {
        configASSERT( "Invalid ulInterruptID or ulPriority passed to vGIC_SetPriority" );
    }
}

void vGIC_EnableIRQ( uint32_t ulInterruptID )
{
    if( ulInterruptID <= GIC_MAX_INTERRUPT_ID )
    {
        uint32_t ulBitMask   = 1U << ulInterruptID;

        /* 1. Assign the interrupt to group 1 */
        *( volatile uint32_t * )( ( portPOINTER_SIZE_TYPE ) GICR_BASE_PER_CORE( ucPortGetCoreID() ) + ( portPOINTER_SIZE_TYPE ) GICR_IGROUPR0 )  |= ( 1U << ulInterruptID );

        /* 2. Enable the interrupt in GIC */
        *( volatile uint32_t* )( ( portPOINTER_SIZE_TYPE ) GICR_BASE_PER_CORE( ucPortGetCoreID() ) + ( portPOINTER_SIZE_TYPE ) GICR_ISENABLER0 ) |= ulBitMask;

        /* Ensure interrupt enable is visible */
        __asm volatile ( "dsb sy" ::: "memory" );
        __asm volatile ( "isb sy" ::: "memory" );
    }
    else
    {
        configASSERT( "Invalid ulInterruptID for vGIC_EnableIRQ: must be < 32" );
    }
}


void vGIC_SetupSgi0( void )
{
    vGIC_PowerUpRedistributor();
    vGIC_SetPriority( portYIELD_CORE_INT_ID, ( portLOWEST_USABLE_INTERRUPT_PRIORITY << portPRIORITY_SHIFT ) );
    vGIC_EnableIRQ( portYIELD_CORE_INT_ID );
    vGIC_EnableCPUInterface();
}
