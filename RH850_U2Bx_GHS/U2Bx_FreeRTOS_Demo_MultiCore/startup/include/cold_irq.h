/*===========================================================================*/
/* Module      = cold_irq.h                                                  */
/*               manually created                                            */
/*===========================================================================*/
/*                                  COPYRIGHT                                */
/*===========================================================================*/
/* Copyright (c) 2015 by Renesas Electronics Europe GmbH,                    */
/*               a company of the Renesas Electronics Corporation            */
/*===========================================================================*/
/* Purpose:      Declarations of Interrupt Vector Table                      */
/*                                                                           */
/*===========================================================================*/
/*                                                                           */
/* Warranty Disclaimer                                                       */
/*                                                                           */
/* Because the Product(s) is licensed free of charge, there is no warranty   */
/* of any kind whatsoever and expressly disclaimed and excluded by Renesas,  */
/* either expressed or implied, including but not limited to those for       */
/* non-infringement of intellectual property, merchantability and/or         */
/* fitness for the particular purpose.                                       */
/* Renesas shall not have any obligation to maintain, service or provide bug */
/* fixes for the supplied Product(s) and/or the Application.                 */
/*                                                                           */
/* Each User is solely responsible for determining the appropriateness of    */
/* using the Product(s) and assumes all risks associated with its exercise   */
/* of rights under this Agreement, including, but not limited to the risks   */
/* and costs of program errors, compliance with applicable laws, damage to   */
/* or loss of data, programs or equipment, and unavailability or             */
/* interruption of operations.                                               */
/*                                                                           */
/* Limitation of Liability                                                   */
/*                                                                           */
/* In no event shall Renesas be liable to the User for any incidental,       */
/* consequential, indirect, or punitive damage (including but not limited    */
/* to lost profits) regardless of whether such liability is based on breach  */
/* of contract, tort, strict liability, breach of warranties, failure of     */
/* essential purpose or otherwise and even if advised of the possibility of  */
/* such damages. Renesas shall not be liable for any services or products    */
/* provided by third party vendors, developers or consultants identified or  */
/* referred to the User by Renesas in connection with the Product(s) and/or  */
/* the Application.                                                          */
/*                                                                           */
/*===========================================================================*/
/* Environment:                                                              */
/*              Device:         All RH850 Mutlicore                          */
/*              IDE:            GHS Multi for V800  V6.xx or later           */
/*===========================================================================*/

#ifndef __COLD_IRQ_H
#define __COLD_IRQ_H

#define IRQ_TABLE_START                 0x00000200u

#define RESET_ENABLE                    0x00000001u

// #define SYSERR_ENABLE                   0x00000010u

// #define FETRAPEX_ENABLE                 0x00000030u

// #define EITRAP0_ENABLE                  0x00000040u

// #define EITRAP1_ENABLE                  0x00000050u

// #define RIEX_ENABLE                     0x00000060u

// #define UCPOP_ENABLE                    0x00000080u

// #define MIP_MDP_ENABLE                  0x00000090u

// #define PIEX_ENABLE                     0x000000A0u

// #define MAEX_ENABLE                     0x000000C0u

// #define MAE_ENABLE                      0x000000C0u

// #define FENMI_ENABLE                    0x000000E0u

// #define FEINT_ENABLE                    0x000000F0u

// #define EINTPRIO_0_ENABLE               0x00000100u

// #define EINTPRIO_1_ENABLE               0x00000110u

// #define EINTPRIO_2_ENABLE               0x00000120u

// #define EINTPRIO_3_ENABLE               0x00000130u

// #define EINTPRIO_4_ENABLE               0x00000140u

// #define EINTPRIO_5_ENABLE               0x00000150u

// #define EINTPRIO_6_ENABLE               0x00000160u

// #define EINTPRIO_7_ENABLE               0x00000170u

// #define EINTPRIO_8_ENABLE               0x00000180u

// #define EINTPRIO_9_ENABLE               0x00000190u

// #define EINTPRIO_10_ENABLE              0x000001A0u

// #define EINTPRIO_11_ENABLE              0x000001B0u

// #define EINTPRIO_12_ENABLE              0x000001C0u

// #define EINTPRIO_13_ENABLE              0x000001D0u

// #define EINTPRIO_14_ENABLE              0x000001E0u

// #define EINTPRIO_15_ENABLE              0x000001F0u

// #define INTECM1MI_ENABLE                (IRQ_TABLE_START + 0x00000000u)

// #define INTECM0MI_ENABLE                (IRQ_TABLE_START + 0x00000000u)

#endif /* __COLD_IRQ_H */
