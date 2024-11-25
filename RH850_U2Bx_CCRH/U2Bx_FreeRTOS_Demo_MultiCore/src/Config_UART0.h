/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2018, 2022 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name        : Config_UART0.h
* Component Version: 1.5.0
* Device(s)        : R7F70254*EABB -C
* Description      : This file implements device driver for Config_UART0.
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include <stdint.h>
#include "iodefine.h"

#ifndef CFG_Config_UART0_H
#define CFG_Config_UART0_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define _UART0_BAUD_RATE_PRESCALER                 (136U)
#ifndef __TYPEDEF__
#define DI()      __DI()
#define EI()      __EI()
#define HALT()    __halt()
#define NOP()     __nop()
#define SETBIT(x)     (1U << x)
#endif
/* Status list definition */
#define MD_STATUSBASE        (0x00U)
#define MD_OK                (MD_STATUSBASE + 0x00U) /* register setting OK */
/* Error list definition */
#define MD_ERRORBASE         (0x80U)
#define MD_ERROR             (MD_ERRORBASE + 0x00U)  /* error */
#define MD_ARGERROR          (MD_ERRORBASE + 0x01U)  /* error argument input error */
/* Interrupt request flag (RFxxx) */
#define _INT_REQUEST_NOT_OCCUR              (0x0U)  /* No interrupt request is made */
/* Interrupt mask (MKxxx) */
#define _INT_PROCESSING_ENABLED             (0x0U)  /* Enables interrupt processing */
#define _INT_PROCESSING_DISABLED            (0x1U)  /* Disables interrupt processing */
/* Interrupt vector method select (TBxxx) */
#define _INT_DIRECT_VECTOR                  (0x0U)  /* Direct jumping to an address by the level of priority */
#define _INT_TABLE_VECTOR                   (0x1U)  /* Table reference */
/* Specify 16 interrupt priority levels (P3xxx,P2xxx,P1xxx,P0xxx) */
#define _INT_PRIORITY_HIGHEST               (0x00C0U)  /* Level 0 (highest) */
#define _INT_PRIORITY_LEVEL1                (0x00C1U)  /* Level 1 */
#define _INT_PRIORITY_LEVEL2                (0x00C2U)  /* Level 2 */
#define _INT_PRIORITY_LEVEL3                (0x00C3U)  /* Level 3 */
#define _INT_PRIORITY_LEVEL4                (0x00C4U)  /* Level 4 */
#define _INT_PRIORITY_LEVEL5                (0x00C5U)  /* Level 5 */
#define _INT_PRIORITY_LEVEL6                (0x00C6U)  /* Level 6 */
#define _INT_PRIORITY_LEVEL7                (0x00C7U)  /* Level 7 */
#define _INT_PRIORITY_LEVEL8                (0x00C8U)  /* Level 8 */
#define _INT_PRIORITY_LEVEL9                (0x00C9U)  /* Level 9 */
#define _INT_PRIORITY_LEVEL10               (0x00CAU)  /* Level 10 */
#define _INT_PRIORITY_LEVEL11               (0x00CBU)  /* Level 11 */
#define _INT_PRIORITY_LEVEL12               (0x00CCU)  /* Level 12 */
#define _INT_PRIORITY_LEVEL13               (0x00CDU)  /* Level 13 */
#define _INT_PRIORITY_LEVEL14               (0x00CEU)  /* Level 14 */
#define _INT_PRIORITY_LOWEST                (0x00CFU)  /* Level 15 (lowest) */

/*
    Port Control Register (PCRn_m)
*/
/* Port Control Register Default Value (PCR_DEFAULT) */
#define _PCR_DEFAULT_VALUE                  (0x5B5CE000UL) /* Default Value */
/* Analog Input Mode Selection (PCR_ANALOG) */
#define _PCR_ANALOG_INPUT                   (0x00000010UL) /* Analog input */
/* Alternative Mode Selection (ALT_MODE) */
#define _PCR_ALT_IN1                        (0x00000050UL) /* Alternative input mode 1 */
#define _PCR_ALT_IN2                        (0x00000051UL) /* Alternative input mode 2 */
#define _PCR_ALT_IN3                        (0x00000052UL) /* Alternative input mode 3 */
#define _PCR_ALT_IN4                        (0x00000053UL) /* Alternative input mode 4 */
#define _PCR_ALT_IN5                        (0x00000054UL) /* Alternative input mode 5 */
#define _PCR_ALT_IN6                        (0x00000055UL) /* Alternative input mode 6 */
#define _PCR_ALT_IN7                        (0x00000056UL) /* Alternative input mode 7 */
#define _PCR_ALT_IN8                        (0x00000057UL) /* Alternative input mode 8 */
#define _PCR_ALT_IN9                        (0x00000058UL) /* Alternative input mode 9 */
#define _PCR_ALT_IN10                       (0x00000059UL) /* Alternative input mode 10 */
#define _PCR_ALT_IN11                       (0x0000005AUL) /* Alternative input mode 11 */
#define _PCR_ALT_IN12                       (0x0000005BUL) /* Alternative input mode 12 */
#define _PCR_ALT_IN13                       (0x0000005CUL) /* Alternative input mode 13 */
#define _PCR_ALT_IN14                       (0x0000005DUL) /* Alternative input mode 14 */
#define _PCR_ALT_IN15                       (0x0000005EUL) /* Alternative input mode 15 */
#define _PCR_ALT_IN16                       (0x0000005FUL) /* Alternative input mode 16 */
#define _PCR_ALT_OUT1                       (0x00000040UL) /* Alternative output mode 1 */
#define _PCR_ALT_OUT2                       (0x00000041UL) /* Alternative output mode 2 */
#define _PCR_ALT_OUT3                       (0x00000042UL) /* Alternative output mode 3 */
#define _PCR_ALT_OUT4                       (0x00000043UL) /* Alternative output mode 4 */
#define _PCR_ALT_OUT5                       (0x00000044UL) /* Alternative output mode 5 */
#define _PCR_ALT_OUT6                       (0x00000045UL) /* Alternative output mode 6 */
#define _PCR_ALT_OUT7                       (0x00000046UL) /* Alternative output mode 7 */
#define _PCR_ALT_OUT8                       (0x00000047UL) /* Alternative output mode 8 */
#define _PCR_ALT_OUT9                       (0x00000048UL) /* Alternative output mode 9 */
#define _PCR_ALT_OUT10                      (0x00000049UL) /* Alternative output mode 10 */
#define _PCR_ALT_OUT11                      (0x0000004AUL) /* Alternative output mode 11 */
#define _PCR_ALT_OUT12                      (0x0000004BUL) /* Alternative output mode 12 */
#define _PCR_ALT_OUT13                      (0x0000004CUL) /* Alternative output mode 13 */
#define _PCR_ALT_OUT14                      (0x0000004DUL) /* Alternative output mode 14 */
#define _PCR_ALT_OUT15                      (0x0000004EUL) /* Alternative output mode 15 */
#define _PCR_ALT_OUT16                      (0x0000004FUL) /* Alternative output mode 16 */

#define _PCR_SET_PM                         (0x00000010UL) /* Input mode */
/* Specify input/output mode of the corresponding pin (CLEAR_PM) */
#define _PCR_CLEAR_PM                       (0xFFFFFFEFUL) /* Input mode */
/* Specify the operation mode of the corresponding pin: (SET_PMC) */
#define _PCR_SET_PMC                        (0x00000040UL) /* Alternative mode */
/* Specify input/output mode of the corresponding pin (SET_PBDC) */
#define _PCR_SET_PBDC                       (0x00020000UL) /* Bi-Direction mode */
/* Specify input/output mode of the corresponding pin (SET_PODC) */
#define _PCR_SET_PODC                       (0x10000000UL) /* Open-drain mode */
/* For I/O Alternative Mode, PBDC/PIBC setting (DIRECT_ALT_SETTING) */
#define _PCR_ALT_OUT_SETTING                (0xFFFCFFFFUL) /* ALT Output setting */
#define _PCR_DIRECT_ALT_MODE_SETTING        (0xFFFCFFFFUL) /* Direct ALT mode setting */
/* Write protected macro definition */
#define _WRITE_PROTECT_ENABLE               (0xA5A5A501UL)  /* Write protected */
#define _WRITE_PROTECT_DISABLE              (0xA5A5A500UL)  /* Write protected */
#define _PORT_WRITE_PROTECT_DISABLE         (0x00000000UL)  /* Write protected */
/* Specify the interrupt bind (request) destination */
#define _INT_CPU_PE0                        (0x00000000UL)  /* Bound to PE0 */
#define _INT_CPU_PE1                        (0x00000001UL)  /* Bound to PE1 */
#define _INT_CPU_PE2                        (0x00000002UL)  /* Bound to PE2 */
#define _INT_CPU_PE3                        (0x00000003UL)  /* Bound to PE3 */


/* RLIN3 MACRO */
/*
    LIN Wake-up Baud Rate Select Register (RLN3n.LWBR)
*/
/* Bit sampling count select (NSPB[3:0]) */
#define _UART_4_SAMPLING                            (0x30U) /* 4 samplings */
#define _UART_6_SAMPLING                            (0x50U) /* 6 samplings */
#define _UART_7_SAMPLING                            (0x60U) /* 7 samplings */
#define _UART_8_SAMPLING                            (0x70U) /* 8 samplings */
#define _UART_9_SAMPLING                            (0x80U) /* 9 samplings */
#define _UART_10_SAMPLING                           (0x90U) /* 10 samplings */
#define _UART_11_SAMPLING                           (0xA0U) /* 11 samplings */
#define _UART_12_SAMPLING                           (0xB0U) /* 12 samplings */
#define _UART_13_SAMPLING                           (0xC0U) /* 13 samplings */
#define _UART_14_SAMPLING                           (0xD0U) /* 14 samplings */
#define _UART_15_SAMPLING                           (0xE0U) /* 15 samplings */
#define _UART_16_SAMPLING                           (0xF0U) /* 16 samplings */
/* Prescaler clock select (LPRS[2:0]) */
#define _UART_PRESCALER_CLOCK_SELECT_1              (0x00U) /* Prescaler clock select 1/1 */
#define _UART_PRESCALER_CLOCK_SELECT_2              (0x02U) /* Prescaler clock select 1/2 */
#define _UART_PRESCALER_CLOCK_SELECT_4              (0x04U) /* Prescaler clock select 1/4 */
#define _UART_PRESCALER_CLOCK_SELECT_8              (0x06U) /* Prescaler clock select 1/8 */
#define _UART_PRESCALER_CLOCK_SELECT_16             (0x08U) /* Prescaler clock select 1/16 */
#define _UART_PRESCALER_CLOCK_SELECT_32             (0x0AU) /* Prescaler clock select 1/32 */
#define _UART_PRESCALER_CLOCK_SELECT_64             (0x0CU) /* Prescaler clock select 1/64 */
#define _UART_PRESCALER_CLOCK_SELECT_128            (0x0EU) /* Prescaler clock select 1/128 */

/*
    UART Mode Register (RLN3n.LMD)
*/
/* UART reception data noise filtering disable (LRDNFS) */
#define _UART_NOISE_FILTER_ENABLED                  (0x00U) /* The noise filter is enabled */
#define _UART_NOISE_FILTER_DISABLED                 (0x20U) /* The noise filter is disabled */
/* LIN or UART mode select (LMD[1:0]) */
#define _UART_MODE_SELECT                           (0x01U) /* UART mode */

/*
    UART Configuration Register (RLN3n.LBFC)
*/
/* UART output polarity switch (UTPS) */
#define _UART_TRANSMISSION_NORMAL                   (0x00U) /* Transmit data normal output */
#define _UART_TRANSMISSION_INVERTED                 (0x40U) /* Transmit data with inversion output */
/* UART input polarity switch (URPS) */
#define _UART_RECEPTION_NORMAL                      (0x00U) /* Reception data normal output */
#define _UART_RECEPTION_INVERTED                    (0x20U) /* Reception data with inversion output */
/* UART parity select (UPS[1:0]) */
#define _UART_PARITY_PROHIBITED                     (0x00U) /* Parity prohibited */
#define _UART_PARITY_EVEN                           (0x08U) /* Even parity */
#define _UART_PARITY_ZERO                           (0x10U) /* 0 parity */
#define _UART_PARITY_ODD                            (0x18U) /* Odd parity */
/* UART stop bit length select (USBLS) */
#define _UART_STOP_BIT_1                            (0x00U) /* Stop bit: 1 bit */
#define _UART_STOP_BIT_2                            (0x04U) /* Stop bit: 2 bits */
/* UART transfer format order select (UBOS) */
#define _UART_LSB                                   (0x00U) /* LSB first */
#define _UART_MSB                                   (0x02U) /* MSB first */
/* UART character length select (UBLS) */
#define _UART_LENGTH_8                              (0x00U) /* UART 8 bits communication */
#define _UART_LENGTH_7                              (0x01U) /* UART 7 bits communication */

/*
    UART Space Configuration Register (RLN3nLSC)
*/
/* Inter-Byte space select (IBS1,IBS0) */
#define _UART_INTER_BYTE_SPACE_SELECT_0BIT          (0x00U) /* 0 tbit */
#define _UART_INTER_BYTE_SPACE_SELECT_1BIT          (0x01U) /* 1 tbit */
#define _UART_INTER_BYTE_SPACE_SELECT_2BIT          (0x02U) /* 2 tbit */
#define _UART_INTER_BYTE_SPACE_SELECT_3BIT          (0x03U) /* 3 tbit */

/*
    UART Error Detection Enable Register (RLN3n.LEDE)
*/
/* Framing error detection enable (FERE) */
#define _UART_FRAMING_ERROR_DETECTED_DISABLE        (0xF7U) /* Disable framing error detection */
#define _UART_FRAMING_ERROR_DETECTED_ENABLE         (0x08U) /* Enable framing error detection */
/* Overrun error detection enable (OERE) */
#define _UART_OVERRUN_ERROR_DETECTED_DISABLE        (0xFBU) /* Disables overrun error detection */
#define _UART_OVERRUN_ERROR_DETECTED_ENABLE         (0x04U) /* Enables overrun error detection */
/* Bit error detection enable (BERE) */
#define _UART_BIT_ERROR_DETECTED_DISABLE            (0xFEU) /* Disables bit error detection */
#define _UART_BIT_ERROR_DETECTED_ENABLE             (0x01U) /* Enables bit error detection */

/*
    UART Control Register (RLN3n.LCUC)
*/
/* LIN reset (OM0) */
#define _UART_LIN_RESET_MODE_CAUSED                 (0x00U) /* LIN reset mode is caused */
#define _UART_LIN_RESET_MODE_CANCELED               (0x01U) /* LIN reset mode is canceled. */

/*
    UART Transmission Control Register (RLN3n.LTRC)
*/
/* UART buffer transmission start (RTS) */
#define _UART_BUFFER_TRANSMISSION_IS_STOPPED        (0x00U) /* UART buffer transmission is stopped */
#define _UART_BUFFER_TRANSMISSION_IS_STARTED        (0x02U) /* UART buffer transmission is started */

/*
    UART Status Register (RLN3n.LST)
*/
/* Reception status flag (URS) */
#define _UART_RECEPTION_ISNOT_OPERATED              (0x00U) /* Reception is not operated */
#define _UART_RECEPTION_OPERATED                    (0x20U) /* Reception is operated */
/* Transmission status flag (UTS) */
#define _UART_TRANSMISSION_ISNOT_OPERATED           (0x00U) /* Transmission is not operated */
#define _UART_TRANSMISSION_OPERATED                 (0x10U) /* Transmission is operated */
/* Error detection flag (ERR) */
#define _UART_REC_NO_ERROR_DETECTED                 (0x00U) /* No error has been detected */
#define _UART_RECEPTION_ERROR_DETECTED              (0x08U) /* Error has been detected */
/* Successful UART buffer transmission flag (FTC)*/ 
#define _UART_BUFFER_TRANSMIT_NOT_COMPLETED         (0x00U) /* UART buffer transmission has not been completed */
#define _UART_TRANSMISSION_COMPLETED                (0x01U) /* UART buffer transmission has been completed */
#define _UART_CLEAR_BUFFER_TRANSMISSION_FLAG        (0xFEU) /* Clear UART buffer transmission flag */

/*
    UART Error Status Register (RLN3n.LEST)
*/
/* Parity error flag (UPER) */
#define _UART_PARITY_ERROR_NOT_DETECTED             (0x00U) /* Parity error has not been detected */
#define _UART_PARITY_ERROR_FLAG                     (0x40U) /* Parity error has been detected */
#define _UART_CLEAR_PARITY_ERROR_FLAG               (0xBFU) /* Clear parity error flag */
/* ID matching flag (IDMT) */
#define _UART_MATCH_ID_NOT_DETECTED                 (0x00U) /* The receive data does not match with the ID value */
#define _UART_MATCH_ID_DETECTED                     (0x20U) /* The receive data match with the ID value */
#define _UART_CLEAR_MATCH_ID_FLAG                   (0xDFU) /* Clear the ID match flag */
/* Expanded bit detection flag (BER) */
#define _UART_BIT_EXPAND_NOT_DETECTED               (0x00U) /* Expanded bit has not been detected */
#define _UART_BIT_EXPAND_DETECTED                   (0x10U) /* Expanded bit has been detected */
#define _UART_CLEAR_BIT_EXPAND_DETECTION_FLAG       (0xEFU) /* Clear the expanded bit detection flag */
/* Framing error flag (FER) */
#define _UART_FRAMING_ERROR_NOT_DETECTED            (0x00U) /* Framing error has not been detected */
#define _UART_FRAMING_ERROR_FLAG                    (0x08U) /* Framing error has been detected */
#define _UART_CLEAR_FRAMING_ERROR_FLAG              (0xF7U) /* Clear framing error flag */
/* Overrun error flag (OER) */
#define _UART_OVERRUN_ERROR_NOT_DETECTED            (0x00U) /* Overrun error has not been detected */
#define _UART_OVERRUN_ERROR_FLAG                    (0x04U) /* Overrun error has been detected */
#define _UART_CLEAR_OVERRUN_ERROR_FLAG              (0xFBU) /* Clear overrun error flag */
/* Bit error flag (BER) */
#define _UART_BIT_ERROR_NOT_DETECTED                (0x00U) /* Bit error has not been detected */
#define _UART_BIT_ERROR_FLAG                        (0x01U) /* Bit error has been detected */
#define _UART_CLEAR_BIT_ERROR_FLAG                  (0xFEU) /* Clear bit error flag */

/*
    UART Data Field Configuration Register (RLN3nLDFC)
*/
/* Transmission start wait (UTSW) */
#define _UART_TRANSFER_START_IMMEDIATELY            (0x00U) /* Transmission starts immediately */
#define _UART_TRANSFER_ONCE                         (0x20U) /* Transmission starts after the stop bit is completed */
/* UART buffer data length select (MDL2,MDL1,MDL0) */
#define _UART_BUFFER_DATA_1BYTE                     (0x01U) /* 1 data */
#define _UART_BUFFER_DATA_2BYTE                     (0x02U) /* 2 data */
#define _UART_BUFFER_DATA_3BYTE                     (0x03U) /* 3 data */
#define _UART_BUFFER_DATA_4BYTE                     (0x04U) /* 4 data */
#define _UART_BUFFER_DATA_5BYTE                     (0x05U) /* 5 data */
#define _UART_BUFFER_DATA_6BYTE                     (0x06U) /* 6 data */
#define _UART_BUFFER_DATA_7BYTE                     (0x07U) /* 7 data */
#define _UART_BUFFER_DATA_8BYTE                     (0x08U) /* 8 data */
#define _UART_BUFFER_DATA_9BYTE                     (0x09U) /* 9 data */

/*
    UART Operation Enable Register (RLN3n.LUOER)
*/
/* Reception enable (UROE) */
#define _UART_RECEPTION_DISABLED                    (0x00U) /* Disables reception */
#define _UART_RECEPTION_ENABLED                     (0x02U) /* Enables reception */
/* Transmission enable (UTOE) */
#define _UART_TRANSMISSION_DISABLED                 (0x00U) /* Disables transmission */
#define _UART_TRANSMISSION_ENABLED                  (0x01U) /* Enables transmission */

/*
    UART Option Register 1 (RLN3n.LUOR1)
*/
/* Transmission interrupt generation timing select (UTIGTS) */
#define _UART_INT_TRANSMISSION_START                (0x00U) /* INT is generated at the start of transmission */
#define _UART_INT_TRANSMISSION_END                  (0x08U) /* INT is generated at the completion of transmission */

/*
    UART Error Clear Flag Macro Definition
*/
/* Transmission interrupt generation timing select (UTIGTS) */
#define _UART_CLEAR_ERROR_FLAG                      (0x7D)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef unsigned short        MD_STATUS;
/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_Config_UART0_Create(void);
void R_Config_UART0_Start(void);
void R_Config_UART0_Stop(void);
MD_STATUS R_Config_UART0_Send(uint8_t * const tx_buf, uint16_t tx_num);
void R_Config_UART0_Create_UserInit(void);
/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

#endif
