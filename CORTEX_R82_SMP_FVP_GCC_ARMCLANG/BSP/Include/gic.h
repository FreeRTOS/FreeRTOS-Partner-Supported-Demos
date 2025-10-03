/* Copyright 2025 Arm Limited and/or its affiliates <open-source-office@arm.com>
 * SPDX-License-Identifier: MIT
 */

#ifndef GIC_H
#define GIC_H

#include "FreeRTOSConfig.h"

#if ( configNUMBER_OF_CORES == 1 )
    #define ucPortGetCoreID()        ( 0 )                                          /* Single core system, always core 0 */
#endif

#define GICD_BASE                    ( 0xAF000000UL )                               /* Base of GIC Distributor on BaseR FVP */
#define GICR_BASE_PER_CORE( core )   ( 0xAF100000UL + ( 0x20000UL * ( core ) ) )    /* Base of GIC Redistributor per core on BaseR FVP */
#define SGI_BASE                     ( 0x10000UL )                                  /* SGI Base */
#define GICD_CTLR                    ( 0x000 )                                      /* Distributor Control Register */
#define GICR_WAKER                   ( 0x14  )                                      /* ReDistributor Wake Register */
#define GICR_PWRR                    ( 0x24  )                                      /* ReDistributor Power Register */
#define GICR_IGROUPR0                ( SGI_BASE + 0x80 )                            /* Interrupt Group Registers */
#define GICR_ISENABLER0              ( SGI_BASE + 0x100 )                           /* Interrupt Set-Enable Registers */
#define GICR_IPRIORITYR( n )         ( SGI_BASE + ( 0x400 + ( 4 * n ) ) )           /* Interrupt Priority Registers */
#define GICR_IGRPMODR0               ( SGI_BASE + 0xD00 )                           /* Distributor Interrupt group modifier Register */

#define GICD_CTLR_ENABLEGRP1NS_BIT   ( 1U )                                         /* GICD_CTRL.EnableGrp1NS bit */
#define GICD_CTLR_ENABLEGRP1S_BIT    ( 2U )                                         /* GICD_CTRL.EnableGrp1S bit */
#define GICD_CTLR_ARES_BIT           ( 4U )                                         /* GICD_CTRL.ARE_S bit */
#define GICD_CTLR_DS_BIT             ( 6U )                                         /* GICD_CTRL.DS bit */

#define GICR_PWRR_RDPD_BIT           ( 0U )                                         /* GICR_PWRR.RDPD bit */

#define GICR_WAKER_PS_BIT            ( 1U )                                         /* GICR_WAKER.PS bit */
#define GICR_WAKER_CA_BIT            ( 2U )                                         /* GICR_WAKER.CA bit */

#define GIC_MAX_INTERRUPT_ID         ( 31UL )                                       /* Maximum Interrupt ID for PPIs and SGIs */
#define GIC_WAIT_TIMEOUT             ( 1000000U )                                   /* Timeout for waiting on GIC operations */

/**
 * Assigns the specified interrupt to Group 1 and enables it
 * in the Redistributor for the local core.
 */
void vGIC_EnableIRQ( uint32_t ulInterruptID );

/**
 * Enables signaling of Group-1 interrupts at EL1 via ICC_IGRPEN1_EL1.
 */
void vGIC_EnableCPUInterface( void );

/**
 * Initializes the GIC Distributor:
 *   - Enables Group-1 Non-Secure and Group-1 Secure interrupts
 *   - Enables Affinity Routing (ARE_S) and Disable Security (DS) bits
 */
void vGIC_InitDist( void );

/**
 * Powers up and wakes the Redistributor for the current core:
 *   1. Clears the Redistributor power-down bit and waits for RDPD=0
 *   2. Clears the Processor-Sleep bit and waits for Children-Asleep=0
 */
void vGIC_PowerUpRedistributor( void );

/**
 * Sets the priority of the specified SGI/PPI (INTID 0‑31) in the local
 * Redistributor bank via GICR_IPRIORITYR.
 * For shared peripheral interrupts (SPI, INTID ≥ 32) use the GICD_IPRIORITYR path.
 *
 * @param ulInterruptID The ID of the interrupt to set the priority for.
 * @param ulPriority The priority value to set.
 */
void vGIC_SetPriority( uint32_t ulInterruptID, uint32_t ulPriority );

/**
 * Powers up the GIC Redistributor, Sets up the priority for SGI0,
 * sets SGI0 to be a Group 1 interrupt, and enables delivery of Group-1 IRQs to EL1.
 */
void vGIC_SetupSgi0( void );

#endif /* GIC_H */
