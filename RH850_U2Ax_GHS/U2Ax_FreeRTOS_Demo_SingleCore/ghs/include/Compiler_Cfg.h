/*============================================================================*/
/* Project      = AUTOSAR Renesas X2x MCAL Components                         */
/* Module       = Compiler_Cfg.h                                              */
/* SW-VERSION   = 2.1.1                                                       */
/*============================================================================*/
/*                                  COPYRIGHT                                 */
/*============================================================================*/
/* (c) 2017,2019-2021,2023, 2024                                              */ 
/*             Renesas Electronics Corporation. All rights reserved.          */
/*============================================================================*/
/* Purpose:                                                                   */
/* This file contains compiler macros                                         */
/*============================================================================*/
/*                                                                            */
/* Unless otherwise agreed upon in writing between your company and           */
/* Renesas Electronics Corporation the following shall apply!                 */
/*                                                                            */
/* Warranty Disclaimer                                                        */
/*                                                                            */
/* There is no warranty of any kind whatsoever granted by Renesas. Any        */
/* warranty is expressly disclaimed and excluded by Renesas, either expressed */
/* or implied, including but not limited to those for non-infringement of     */
/* intellectual property, merchantability and/or fitness for the particular   */
/* purpose.                                                                   */
/*                                                                            */
/* Renesas shall not have any obligation to maintain, service or provide bug  */
/* fixes for the supplied Product(s) and/or the Application.                  */
/*                                                                            */
/* Each User is solely responsible for determining the appropriateness of     */
/* using the Product(s) and assumes all risks associated with its exercise    */
/* of rights under this Agreement, including, but not limited to the risks    */
/* and costs of program errors, compliance with applicable laws, damage to    */
/* or loss of data, programs or equipment, and unavailability or              */
/* interruption of operations.                                                */
/*                                                                            */
/* Limitation of Liability                                                    */
/*                                                                            */
/* In no event shall Renesas be liable to the User for any incidental,        */
/* consequential, indirect, or punitive damage (including but not limited     */
/* to lost profits) regardless of whether such liability is based on breach   */
/* of contract, tort, strict liability, breach of warranties, failure of      */
/* essential purpose or otherwise and even if advised of the possibility of   */
/* such damages. Renesas shall not be liable for any services or products     */
/* provided by third party vendors, developers or consultants identified or   */
/* referred to the User by Renesas in connection with the Product(s) and/or   */
/* the Application.                                                           */
/*                                                                            */
/*============================================================================*/
/* Environment:                                                               */
/*              Devices:        X2x                                           */
/*============================================================================*/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*
 *  2.1.2:  15/08/2024  : Add FEE section
 *  2.1.1:  07/08/2024  : Add MEM section
 *          21/07/2024  : Change CAN_APPL_CODE to CAN_CALLOUT_CODE
 *  2.0.1:  19/09/2023  : Remove instance macro
 *  2.0.0:  28/04/2023  : Add OCU section
 *          24/03/2023  : Add define for CORTST: CORTST_PRIVATE_CODE
 *                        CORTST_CODE_FAST
 *  1.3.2:  06/09/2021  : Update memory section for ADC: 
 *                        seperate memory section from common instance 
 *                        to single instance and multiple instance.
 *          30/08/2021  : Update memory section for SPI: 
 *                        seperate memory section from common instance 
 *                        to single instance and multiple instance.
 *          30/08/2021  : Update memory section for ICU: 
 *                        seperate memory section from common instance 
 *                        to single instance and multiple instance.
 *          30/08/2021  : Update memory section for CAN: 
 *                        seperate memory section from common instance 
 *                        to single instance and multiple instance.
 *          26/08/2021  : Update memory section for GPT and PWM: 
 *                        seperate memory section from common instance 
 *                        to single instance and multiple instance.
 *  1.3.1:  30/06/2021  : Update memory section for WDG: 
 *                        seperate memory section from common instance 
 *                        to single instance and multiple instance.
 *          06/05/2021  : Modify the prefix of memory section from CAN_RSCAN
 *                        to CAN
 *          01/04/2021  : Remove vendor ID from the file name, API names
 *                        and parameters for ETH according to BSW00347 requirement
 *  1.2.0:  26/08/2020  : Release
 *  1.1.0:  19/06/2020  : Release
 *  1.0.4:  18/02/2020  : Remove FR_VAR_FAST_NO_INIT,FR_VAR_POWER_ON_INIT
 *                        FR_VAR_NOINIT, FR_VAR
 *          16/01/2020  : Remove AR Release version.
 *  1.0.3:  08/07/2019  : Corrected CAN_VAR to CAN_RSCAN_VAR, and
 *                        CAN_VAR_NOINIT to CAN_RSCAN_VAR_NOINIT
 *                        Update WDG part to support new code structure.
 *  1.0.2:  24/10/2017  : Add REGSPACE
 *
 *  1.0.1:  04/04/2017  : Removed redundant parameters
 *
 *  1.0.0:  05/01/2017  : Initial Version
 * 
 */
/******************************************************************************/

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/

#ifndef COMPILER_CFG_H
#define COMPILER_CFG_H

/*******************************************************************************
**                      Version Information                                  **
*******************************************************************************/

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/

/*******************************************************************************
 **                      Configuration data                                   **
*******************************************************************************/
/*
 * The following memory and pointer classes can be configured per module.
 * These #defines are passed to the compiler abstraction macros in Compiler.h
 *
 * Note:
 * module internal functions (statics) that get into one section
 * (together with API) shall fit into one page.
 */

/* ---------------------------------------------------------------------------*/
/*                   General                                                  */
/* ---------------------------------------------------------------------------*/
#define REGSPACE                       /* Registers                           */

/* ---------------------------------------------------------------------------*/
/*                   MCU                                                      */
/* ---------------------------------------------------------------------------*/

#define MCU_PUBLIC_CODE                /* API functions                       */
#define MCU_PUBLIC_CONST               /* API constants                       */

#define MCU_PRIVATE_CODE               /* Internal functions                  */

#define MCU_PRIVATE_CONST              /* Internal ROM Data                   */

#define MCU_APPL_CODE                  /* callbacks of the Application        */
#define MCU_APPL_CONST                 /* Applications' ROM Data              */
#define MCU_APPL_DATA                  /* Applications' RAM Data              */
#define MCU_VAR_FAST_NO_INIT           /* 'Near' RAM Data                     */

#define MCU_CODE_FAST                  /* API functions                       */
#define MCU_CONST                      /* API constants                       */

#define MCU_CONFIG_DATA                /* Desc. Tables -> Config-dependent    */
                                       /* Config. dependent (reg. size) data  */

#define MCU_VAR_INIT                   /* Data which is initialized during
                                          Startup                             */
#define MCU_VAR_NO_INIT                /* Data which is not initialized during
                                          Startup                             */
#define MCU_CONST                      /* Data Constants                      */
#define MCU_VAR                        /* Memory class for global variables   */
                                       /* which are initialized after every   */
                                       /* reset                               */
#define MCU_VAR_NOINIT                 /* Memory class for global variables   */
                                       /* which are initialized by driver     */

/* ---------------------------------------------------------------------------*/
/*                   GPT                                                      */
/* ---------------------------------------------------------------------------*/
#define GPT_PUBLIC_CODE                /* API functions                       */
#define GPT_PUBLIC_CONST               /* API constants                       */

#define GPT_PRIVATE_CODE               /* Internal functions                  */

#define GPT_PRIVATE_CONST              /* Internal ROM Data                   */

#define GPT_APPL_CODE                  /* callbacks of the Application        */
#define GPT_APPL_CONST                 /* Applications' ROM Data              */
#define GPT_APPL_DATA                  /* Applications' RAM Data              */
#define GPT_VAR_FAST_NO_INIT           /* 'Near' RAM Data                     */

#define GPT_CODE_FAST                  /* API functions                       */
#define GPT_CONST                      /* API constants                       */

#define GPT_CONFIG_DATA                /* Desc. Tables -> Config-dependent    */
                                       /* Config. dependent (reg. size) data  */

#define GPT_VAR_INIT                   /* Data which is initialized during
                                          Startup                             */
#define GPT_VAR_NO_INIT                /* Data which is not initialized during
                                          Startup                             */
#define GPT_CONST                      /* Data Constants                      */
#define GPT_VAR                        /* Memory class for global variables   */
                                       /* which are initialized after every   */
                                       /* reset                               */
#define GPT_VAR_NOINIT                 /* Memory class for global variables   */
                                       /* which are initialized by driver     */
/* ---------------------------------------------------------------------------*/
/*                   OCU                                                      */
/* ---------------------------------------------------------------------------*/
#define OCU_PUBLIC_CODE                /* API functions                       */
#define OCU_PUBLIC_CONST               /* API constants                       */

#define OCU_PRIVATE_CODE               /* Internal functions                  */

#define OCU_PRIVATE_CONST              /* Internal ROM Data                   */

#define OCU_APPL_CODE                  /* callbacks of the Application        */
#define OCU_APPL_CONST                 /* Applications' ROM Data              */
#define OCU_APPL_DATA                  /* Applications' RAM Data              */
#define OCU_VAR_FAST_NO_INIT           /* 'Near' RAM Data                     */

#define OCU_CODE_FAST                  /* API functions                       */
#define OCU_CONST                      /* API constants                       */

#define OCU_CONFIG_DATA                /* Desc. Tables -> Config-dependent    */
                                       /* Config. dependent (reg. size) data  */

#define OCU_VAR_INIT                   /* Data which is initialized during
                                          Startup                             */
#define OCU_VAR_NO_INIT                /* Data which is not initialized during
                                          Startup                             */
#define OCU_CONST                      /* Data Constants                      */
#define OCU_VAR                        /* Memory class for global variables   */
                                       /* which are initialized after every   */
                                       /* reset                               */
#define OCU_VAR_NOINIT                 /* Memory class for global variables   */
                                       /* which are initialized by driver     */
/* ---------------------------------------------------------------------------*/
/*                   WDG                                                      */
/* ---------------------------------------------------------------------------*/
#define WDG_PUBLIC_CODE                 /* API functions                      */
#define WDG_PUBLIC_CONST                /* API constants                      */

#define WDG_PRIVATE_CODE                /* Internal functions                 */

#define WDG_PRIVATE_DATA                /* Module internal data               */
#define WDG_PRIVATE_CONST               /* Internal ROM Data                  */

#define WDG_APPL_CODE                   /* callbacks of the Application       */
#define WDG_APPL_CONST                  /* Applications' ROM Data             */
#define WDG_APPL_DATA                   /* Applications' RAM Data             */
#define WDG_FAST_DATA                   /* 'Near' RAM Data                    */

#define WDG_CODE_FAST                   /* API functions                      */
#define WDG_FAST_CONST                  /* API constants                      */

#define WDG_CONFIG_CONST                /* Desc. Tables -> Config-dependent   */
#define WDG_CONFIG_DATA                 /* Config. dependent (reg. size) data */

#define WDG_INIT_DATA                   /* Data which is initialized during
                                           Startup                            */
#define WDG_NOINIT_DATA                 /* Data which is not initialized during
                                           Startup                            */
#define WDG_CONST                       /* Data Constants                     */
#define WDG_VAR                         /* Memory class for global variables  */
                                        /* which are initialized after every  */
                                        /* reset                              */
#define WDG_VAR_NOINIT                  /* Memory class for global variables  */
                                        /* which are initialized by driver    */
/* ---------------------------------------------------------------------------*/
/*                   PORT                                                     */
/* ---------------------------------------------------------------------------*/

#define PORT_PUBLIC_CODE                /* API functions                      */
#define PORT_PUBLIC_CONST               /* API constants                      */

#define PORT_PRIVATE_CODE               /* Internal functions                 */

#define PORT_PRIVATE_CONST              /* Internal ROM Data                  */

#define PORT_APPL_CODE                  /* callbacks of the Application       */
#define PORT_APPL_CONST                 /* Applications' ROM Data             */
#define PORT_APPL_DATA                  /* Applications' RAM Data             */
#define PORT_VAR_FAST_NO_INIT           /* 'Near' RAM Data                    */

#define PORT_CODE_FAST                  /* API functions                      */
#define PORT_CONST                      /* API constants                      */

#define PORT_CONFIG_DATA                /* Desc. Tables -> Config-dependent   */
                                        /* Config. dependent (reg. size) data */

#define PORT_VAR_INIT                   /* Data which is initialized during
                                           Startup                            */
#define PORT_VAR_NO_INIT                /* Data which is not initialized during
                                           Startup                            */
#define PORT_CONST                      /* Data Constants                     */
#define PORT_VAR                        /* Memory class for global variables  */
                                        /* which are initialized after every  */
                                        /* reset                              */
#define PORT_VAR_NOINIT                 /* Memory class for global variables  */
                                        /* which are initialized by driver    */

/* ---------------------------------------------------------------------------*/
/*                   DIO                                                      */
/* ---------------------------------------------------------------------------*/

#define DIO_PUBLIC_CODE                /* API functions                       */
#define DIO_PUBLIC_CONST               /* API constants                       */

#define DIO_PRIVATE_CODE               /* Internal functions                  */

#define DIO_PRIVATE_CONST              /* Internal ROM Data                   */

#define DIO_APPL_CODE                  /* callbacks of the Application        */
#define DIO_APPL_CONST                 /* Applications' ROM Data              */
#define DIO_APPL_DATA                  /* Applications' RAM Data              */
#define DIO_VAR_FAST_NO_INIT           /* 'Near' RAM Data                     */

#define DIO_CODE_FAST                  /* API functions                       */
#define DIO_CONST                      /* API constants                       */

#define DIO_CONFIG_DATA                /* Desc. Tables -> Config-dependent    */
                                       /* Config. dependent (reg. size) data  */

#define DIO_VAR_INIT                   /* Data which is initialized during
                                          Startup                             */
#define DIO_VAR_NO_INIT                /* Data which is not initialized during
                                          Startup                             */
#define DIO_CONST                      /* Data Constants                      */
#define DIO_VAR                        /* Memory class for global variables   */
                                       /* which are initialized after every   */
                                       /* reset                               */
#define DIO_VAR_NOINIT                 /* Memory class for global variables   */
                                       /* which are initialized by driver     */

/* ---------------------------------------------------------------------------*/
/*                   PWM                                                      */
/* ---------------------------------------------------------------------------*/
#define PWM_PUBLIC_CODE                /* API functions                       */
#define PWM_PUBLIC_CONST               /* API constants                       */

#define PWM_PRIVATE_CODE               /* Internal functions                  */

#define PWM_PRIVATE_CONST              /* Internal ROM Data                   */

#define PWM_APPL_CODE                  /* callbacks of the Application        */
#define PWM_APPL_CONST                 /* Applications' ROM Data              */
#define PWM_APPL_DATA                  /* Applications' RAM Data              */
#define PWM_VAR_FAST_NO_INIT           /* 'Near' RAM Data                     */

#define PWM_CODE_FAST                  /* API functions                       */
#define PWM_CONST                      /* API constants                       */

#define PWM_CONFIG_DATA                /* Desc. Tables -> Config-dependent    */
                                       /* Config. dependent (reg. size) data  */

#define PWM_VAR_INIT                   /* Data which is initialized during
                                          Startup                             */
#define PWM_VAR_NO_INIT                /* Data which is not initialized during
                                          Startup                             */
#define PWM_CONST                      /* Data Constants                      */
#define PWM_VAR                        /* Memory class for global variables   */
                                       /* which are initialized after every   */
                                       /* reset                               */
#define PWM_VAR_NOINIT                 /* Memory class for global variables   */
                                       /* which are initialized by driver     */
/* ---------------------------------------------------------------------------*/
/*                   SPI                                                      */
/* ---------------------------------------------------------------------------*/
#define SPI_PUBLIC_CODE                /* API functions                       */
#define SPI_PUBLIC_CONST               /* API constants                       */

#define SPI_PRIVATE_CODE               /* Internal functions                  */

#define SPI_PRIVATE_CONST              /* Internal ROM Data                   */

#define SPI_APPL_CODE                  /* callbacks of the Application        */
#define SPI_APPL_CONST                 /* Applications' ROM Data              */
#define SPI_APPL_DATA                  /* Applications' RAM Data              */
#define SPI_VAR_FAST_NO_INIT           /* 'Near' RAM Data                     */

#define SPI_CODE_FAST                  /* API functions                       */
#define SPI_CONST                      /* API constants                       */

#define SPI_CONFIG_DATA                /* Desc. Tables -> Config-dependent    */
                                       /* Config. dependent (reg. size) data  */

#define SPI_VAR_INIT                   /* Data which is initialized during
                                          Startup                             */
#define SPI_VAR_NO_INIT                /* Data which is not initialized during
                                          Startup                             */
#define SPI_CONST                      /* Data Constants                      */
#define SPI_VAR                        /* Memory class for global variables   */
                                       /* which are initialized after every   */
                                       /* reset                               */
#define SPI_VAR_NOINIT                 /* Memory class for global variables   */
                                       /* which are initialized by driver     */
/* ---------------------------------------------------------------------------*/
/*                   ADC                                                      */
/* ---------------------------------------------------------------------------*/
#define ADC_PUBLIC_CODE                /* API functions                       */
#define ADC_PUBLIC_CONST               /* API constants                       */

#define ADC_PRIVATE_CODE               /* Internal functions                  */

#define ADC_PRIVATE_CONST              /* Internal ROM Data                   */

#define ADC_APPL_CODE                  /* callbacks of the Application        */
#define ADC_APPL_CONST                 /* Applications' ROM Data              */
#define ADC_APPL_DATA                  /* Applications' RAM Data              */
#define ADC_VAR_FAST_NO_INIT           /* 'Near' RAM Data                     */

#define ADC_CODE_FAST                  /* API functions                       */
#define ADC_CONST                      /* API constants                       */

#define ADC_CONFIG_DATA                /* Desc. Tables -> Config-dependent    */
                                       /* Config. dependent (reg. size) data  */

#define ADC_VAR_INIT                   /* Data which is initialized during
                                          Startup                             */
#define ADC_VAR_NO_INIT                /* Data which is not initialized during
                                          Startup                             */
#define ADC_CONST                      /* Data Constants                      */
#define ADC_VAR                        /* Memory class for global variables   */
                                       /* which are initialized after every   */
                                       /* reset                               */
#define ADC_VAR_NOINIT                 /* Memory class for global variables   */
                                       /* which are initialized by driver     */

/* ---------------------------------------------------------------------------*/
/*                   ICU                                                      */
/* ---------------------------------------------------------------------------*/
#define ICU_PUBLIC_CODE                /* API functions                       */
#define ICU_PUBLIC_CONST               /* API constants                       */

#define ICU_PRIVATE_CODE               /* Internal functions                  */

#define ICU_PRIVATE_CONST              /* Internal ROM Data                   */

#define ICU_APPL_CODE                  /* callbacks of the Application        */
#define ICU_APPL_CONST                 /* Applications' ROM Data              */
#define ICU_APPL_DATA                  /* Applications' RAM Data              */
#define ICU_VAR_FAST_NO_INIT           /* 'Near' RAM Data                     */

#define ICU_CODE_FAST                  /* API functions                       */
#define ICU_CONST                      /* API constants                       */

#define ICU_CONFIG_DATA                /* Desc. Tables -> Config-dependent    */
                                       /* Config. dependent (reg. size) data  */

#define ICU_VAR_INIT                   /* Data which is initialized during
                                          Startup                             */
#define ICU_VAR_NO_INIT                /* Data which is not initialized during
                                          Startup                             */
#define ICU_CONST                      /* Data Constants                      */
#define ICU_VAR                        /* Memory class for global variables   */
                                       /* which are initialized after every   */
                                       /* reset                               */
#define ICU_VAR_NOINIT                 /* Memory class for global variables   */
                                       /* which are initialized by driver     */
/* -------------------------------------------------------------------------- */
/*             FlexRay Driver                                                 */
/* -------------------------------------------------------------------------- */
#define FR_PRIVATE_CODE                /* module internal functions           */

#define FR_PRIVATE_CONST               /* module internal consts              */

#define FR_PUBLIC_CODE                 /* API functions                       */
#define FR_PUBLIC_CONST                /* API constants                       */
#define FR_CODE
#define FR_APPL_CODE                   /* callbacks of the Application        */
#define FR_APPL_CONST                  /* Applications' ROM Data              */

#define FR_CODE_FAST                   /* API functions                       */
#define FR_CONST                       /* API constants                       */

#define FR_VAR_INIT                    /* module variables                    */
#define FR_CONST                       /* module constants                    */

#define FR_VAR_NO_INIT                 /* module variables that are           */
                                              /* not initialized              */
/*
 * the applications' data blocks must have the same classifier
 * like the EEPIF expects
 */
#define FR_APPL_DATA              /* Applications' Ram Data                   */
#define FR_CONFIG_DATA            /* Descriptor Tables -> Config-dependent    */
                                  /* Configuration dependent (reg. size) data */

/* ---------------------------------------------------------------------------*/
/*                   CAN                                                      */
/* ---------------------------------------------------------------------------*/
#define CAN_PUBLIC_CODE                /* API functions                       */
#define CAN_PUBLIC_CONST               /* API constants                       */
#define CAN_PRIVATE_CODE               /* Internal functions                  */
#define CAN_PRIVATE_CONST              /* Internal ROM Data                   */
#define CAN_CALLOUT_CODE               /* callbacks of the Application        */
#define CAN_APPL_CONST                 /* Applications' ROM Data              */
#define CAN_APPL_DATA                  /* Applications' RAM Data              */
#define CAN_CODE_FAST                  /* API functions                       */
#define CAN_CONST                      /* API constants                       */
                                       /* Data Constants                      */
#define CAN_CONFIG_DATA                /* Desc. Tables -> Config-dependent    */
                                       /* Config. dependent (reg. size) data  */
#define CAN_VAR_INIT                   /* Data which is initialized during
                                          Startup                             */
#define CAN_VAR_NO_INIT                /* Data which is not initialized during
                                          Startup                             */
#define CAN_VAR                        /* Memory class for global variables   */
                                       /* which are initialized after every   */
                                       /* reset                               */
#define CAN_VAR_NOINIT                 /* Memory class for global variables   */
                                       /* which are initialized by driver     */

/* ---------------------------------------------------------------------------*/
/*                   LIN                                                      */
/* ---------------------------------------------------------------------------*/

#define LIN_PUBLIC_CODE                /* API functions                       */
#define LIN_PUBLIC_CONST               /* API constants                       */

#define LIN_PRIVATE_CODE               /* Internal functions                  */

#define LIN_PRIVATE_CONST              /* Internal ROM Data                   */

#define LIN_APPL_CODE                  /* callbacks of the Application        */
#define LIN_APPL_CONST                 /* Applications' ROM Data              */
#define LIN_APPL_DATA                  /* Applications' RAM Data              */
#define LIN_VAR_FAST_NO_INIT           /* 'Near' RAM Data                     */

#define LIN_CODE_FAST                  /* API functions                       */
#define LIN_CONST                      /* API constants                       */

#define LIN_CONFIG_DATA                /* Desc. Tables -> Config-dependent    */
                                       /* Config. dependent (reg. size) data  */

#define LIN_VAR_INIT                   /* Data which is initialized during
                                          Startup                             */
#define LIN_VAR_NO_INIT                /* Data which is not initialized during
                                          Startup                             */
#define LIN_CONST                      /* Data Constants                      */
#define LIN_VAR                        /* Memory class for global variables   */
                                       /* which are initialized after every   */
                                       /* reset                               */
#define LIN_VAR_NOINIT                 /* Memory class for global variables   */
                                       /* which are initialized by driver     */

/* ---------------------------------------------------------------------------*/
/*                   FLS                                                      */
/* ---------------------------------------------------------------------------*/

#define FLS_PUBLIC_CODE                /* API functions                       */
#define FLS_PUBLIC_CONST               /* API constants                       */

#define FLS_PRIVATE_CODE               /* Internal functions                  */

#define FLS_PRIVATE_CONST              /* Internal ROM Data                   */

#define FLS_APPL_CODE                  /* callbacks of the Application        */
#define FLS_APPL_CONST                 /* Applications' ROM Data              */
#define FLS_APPL_DATA                  /* Applications' RAM Data              */
#define FLS_VAR_FAST_NO_INIT           /* 'Near' RAM Data                     */

#define FLS_CODE_FAST                  /* API functions                       */
#define FLS_CONST                      /* API constants                       */

#define FLS_CONFIG_DATA                /* Desc. Tables -> Config-dependent    */
                                       /* Config. dependent (reg. size) data  */

#define FLS_VAR_INIT                   /* Data which is initialized during
                                          Startup                             */
#define FLS_VAR_NO_INIT                /* Data which is not initialized during
                                          Startup                             */
#define FLS_CONST                      /* Data Constants                      */
#define FLS_VAR                        /* Memory class for global variables   */
                                       /* which are initialized after every   */
                                       /* reset                               */
#define FLS_VAR_NOINIT                 /* Memory class for global variables   */
                                       /* which are initialized by driver     */

/* ---------------------------------------------------------------------------*/
/*                   MEM                                                      */
/* ---------------------------------------------------------------------------*/

#define MEM_PUBLIC_CODE                /* API functions                       */
#define MEM_PUBLIC_CONST               /* API constants                       */

#define MEM_PRIVATE_CODE               /* Internal functions                  */

#define MEM_PRIVATE_CONST              /* Internal ROM Data                   */

#define MEM_APPL_CODE                  /* callbacks of the Application        */
#define MEM_APPL_CONST                 /* Applications' ROM Data              */
#define MEM_APPL_DATA                  /* Applications' RAM Data              */
#define MEM_VAR_FAST_NO_INIT           /* 'Near' RAM Data                     */

#define MEM_CODE_FAST                  /* API functions                       */
#define MEM_CONST                      /* API constants                       */

#define MEM_CONFIG_DATA                /* Desc. Tables -> Config-dependent    */
                                       /* Config. dependent (reg. size) data  */

#define MEM_VAR_INIT                   /* Data which is initialized during
                                          Startup                             */
#define MEM_VAR_NO_INIT                /* Data which is not initialized during
                                          Startup                             */
#define MEM_CONST                      /* Data Constants                      */
#define MEM_VAR                        /* Memory class for global variables   */
                                       /* which are initialized after every   */
                                       /* reset                               */
#define MEM_VAR_NOINIT                 /* Memory class for global variables   */
                                       /* which are initialized by driver     */
   
/* ---------------------------------------------------------------------------*/
/*                   MEMACC                                                   */
/* ---------------------------------------------------------------------------*/

#define MEMACC_PUBLIC_CODE                /* API functions                    */
#define MEMACC_PUBLIC_CONST               /* API constants                       */

#define MEMACC_PRIVATE_CODE               /* Internal functions                  */

#define MEMACC_PRIVATE_CONST              /* Internal ROM Data                   */

#define MEMACC_APPL_CODE                  /* callbacks of the Application        */
#define MEMACC_APPL_CONST                 /* Applications' ROM Data              */
#define MEMACC_APPL_DATA                  /* Applications' RAM Data              */
#define MEMACC_VAR_FAST_NO_INIT           /* 'Near' RAM Data                     */

#define MEMACC_CODE_FAST                  /* API functions                       */
#define MEMACC_CONST                      /* API constants                       */

#define MEMACC_CONFIG_DATA                /* Desc. Tables -> Config-dependent    */
                                       /* Config. dependent (reg. size) data  */

#define MEMACC_VAR_INIT                   /* Data which is initialized during
                                          Startup                             */
#define MEMACC_VAR_NO_INIT                /* Data which is not initialized during
                                          Startup                             */
#define MEMACC_CONST                      /* Data Constants                      */
#define MEMACC_VAR                        /* Memory class for global variables   */
                                       /* which are initialized after every   */
                                       /* reset                               */
#define MEMACC_VAR_NOINIT                 /* Memory class for global variables   */
                                       /* which are initialized by driver     */

/* ---------------------------------------------------------------------------*/
/*                   FEE                                                      */
/* ---------------------------------------------------------------------------*/

#define FEE_PUBLIC_CODE                /* API functions                       */
#define FEE_PUBLIC_CONST               /* API constants                       */

#define FEE_PRIVATE_CODE               /* Internal functions                  */

#define FEE_PRIVATE_CONST              /* Internal ROM Data                   */

#define FEE_APPL_CODE                  /* callbacks of the Application        */
#define FEE_APPL_CONST                 /* Applications' ROM Data              */
#define FEE_APPL_DATA                  /* Applications' RAM Data              */
#define FEE_VAR_FAST_NO_INIT           /* 'Near' RAM Data                     */

#define FEE_CODE_FAST                  /* API functions                       */
#define FEE_CONST                      /* API constants                       */

#define FEE_CONFIG_DATA                /* Desc. Tables -> Config-dependent    */
                                       /* Config. dependent (reg. size) data  */

#define FEE_VAR_INIT                   /* Data which is initialized during
                                          Startup                             */
#define FEE_VAR_NO_INIT                /* Data which is not initialized during
                                          Startup                             */
#define FEE_CONST                      /* Data Constants                      */
#define FEE_VAR                        /* Memory class for global variables   */
                                       /* which are initialized after every   */
                                       /* reset                               */
#define FEE_VAR_NOINIT                 /* Memory class for global variables   */
                                       /* which are initialized by driver     */

/* ---------------------------------------------------------------------------*/
/*                   FLSTST                                                   */
/* ---------------------------------------------------------------------------*/

#define FLSTST_PUBLIC_CODE             /* API functions                       */
#define FLSTST_PUBLIC_CONST            /* API constants                       */

#define FLSTST_APPL_CODE               /* callbacks of the Application        */
#define FLSTST_APPL_CONST              /* Applications' ROM Data              */
#define FLSTST_APPL_DATA               /* Applications' RAM Data              */
#define FLSTST_VAR_FAST_NO_INIT        /* 'Near' RAM Data                     */

#define FLSTST_CONFIG_DATA            /* Desc. Tables -> Config-dependen      */
                                      /* Config. dependent (reg. size) data   */

#define FLSTST_VAR_INIT               /* Data which is initialized during     */
                                      /*Startup                               */
#define FLSTST_VAR_NO_INIT            /* Data which is not initialized during */
                                      /*Startup                               */
#define FLSTST_CONST                  /* Data Constants                       */

/* ---------------------------------------------------------------------------*/
/*                   CORTST                                                   */
/* ---------------------------------------------------------------------------*/

#define CORTST_PUBLIC_CODE             /* API functions                       */

#define CORTST_PRIVATE_CODE            /* Internal functions                  */

#define CORTST_APPL_CODE               /* callbacks of the Application        */
#define CORTST_APPL_DATA               /* Applications' RAM Data              */

#define CORTST_CONFIG_DATA             /* Desc. Tables -> Config-dependent    */
                                       /* Config. dependent (reg. size) data  */

#define CORTST_VAR_INIT                /* Data which is initialized during    */
                                       /* Startup                             */
#define CORTST_VAR_NO_INIT             /* Data which is not initialized during*/
                                       /* Startup                             */
#define CORTST_CODE_FAST               /* API functions                       */
#define CORTST_CONST                   /* Data Constants                      */

/* ---------------------------------------------------------------------------*/
/*                   RAMTST                                                   */
/* ---------------------------------------------------------------------------*/

#define RAMTST_PUBLIC_CODE                /* API functions                    */
#define RAMTST_PUBLIC_CONST               /* API constants                    */

#define RAMTST_PRIVATE_CODE               /* Internal functions               */

#define RAMTST_PRIVATE_CONST              /* Internal ROM Data                */

#define RAMTST_APPL_CODE                  /* callbacks of the Application     */
#define RAMTST_APPL_CONST                 /* Applications' ROM Data           */
#define RAMTST_APPL_DATA                  /* Applications' RAM Data           */
#define RAMTST_VAR_FAST_NO_INIT           /* 'Near' RAM Data                  */


#define RAMTST_CONFIG_DATA                /* Desc. Tables -> Config-dependent */
                                          /* Config. dependent (reg. size)
                                                                        data  */

#define RAMTST_VAR_INIT                   /* Data which is initialized during
                                          Startup                             */
#define RAMTST_VAR_NO_INIT                /* Data which is not initialized
                                          during Startup                      */
#define RAMTST_CONST                      /* Data Constants                   */

#define RAMTST_CODE

/* ---------------------------------------------------------------------------*/
/*                   ETH                                                      */
/* ---------------------------------------------------------------------------*/

#define ETH_PUBLIC_CODE                /* API functions                       */
#define ETH_PUBLIC_CONST               /* API constants                       */

#define ETH_PRIVATE_CODE               /* Internal functions                  */

#define ETH_PRIVATE_CONST              /* Internal ROM Data                   */

#define ETH_APPL_CODE                  /* callbacks of the Application        */
#define ETH_APPL_CONST                 /* Applications' ROM Data              */
#define ETH_APPL_DATA                  /* Applications' RAM Data              */
#define ETH_VAR_FAST_NO_INIT           /* 'Near' RAM Data                     */

#define ETH_CODE_FAST                  /* API functions                       */
#define ETH_CONST                      /* API constants                       */

#define ETH_CONFIG_DATA                /* Desc. Tables -> Config-dependent    */
                                       /* Config. dependent (reg. size) data  */

#define ETH_VAR_INIT                   /* Data which is initialized during
                                          Startup                             */
#define ETH_VAR_NO_INIT                /* Data which is not initialized during
                                          Startup                             */
#define ETH_CONST                      /* Data Constants                      */
#define ETH_VAR                        /* Memory class for global variables   */
                                       /* which are initialized after every   */
                                       /* reset                               */
#define ETH_VAR_NOINIT                 /* Memory class for global variables   */
                                       /* which are initialized by driver     */
/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/


#endif /* COMPILER_CFG_H */

/*******************************************************************************
**                      End of File                                           **
*******************************************************************************/
