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
* File Name        : Config_UART0.c
* Component Version: 1.5.0
* Device(s)        : R7F70254*EABB -C
* Description      : This file implements device driver for Config_UART0.
***********************************************************************************************************************/
/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "Config_UART0.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
volatile uint8_t *  gp_uart0_tx_address;    /* uart0 transmit data address */
volatile uint16_t   g_uart0_tx_count;       /* uart0 transmit data number */
volatile uint32_t g_cg_sync_read;
void r_uart0_init_intc2_interrupt(void);
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_UART0_Create
* Description  : This function initializes the UART0 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_UART0_Create(void)
{
    r_uart0_init_intc2_interrupt();

    /* Set LIN reset mode */
    RLN30.LCUC.UINT8 = _UART_LIN_RESET_MODE_CAUSED;
    RLN30.LWBR.UINT8 = _UART_7_SAMPLING | _UART_PRESCALER_CLOCK_SELECT_1;
    RLN30.LBRP01.UINT16 = _UART0_BAUD_RATE_PRESCALER;
    RLN30.LMD.UINT8 = _UART_MODE_SELECT;
    RLN30.LSC.UINT8 = _UART_INTER_BYTE_SPACE_SELECT_0BIT;
    RLN30.LEDE.UINT8 = _UART_FRAMING_ERROR_DETECTED_ENABLE | _UART_OVERRUN_ERROR_DETECTED_ENABLE | _UART_BIT_ERROR_DETECTED_ENABLE;
    RLN30.LBFC.UINT8 = _UART_TRANSMISSION_NORMAL | _UART_PARITY_PROHIBITED | _UART_LSB | _UART_LENGTH_8 | 
                       _UART_STOP_BIT_1;
    RLN30.LCUC.UINT8 = _UART_LIN_RESET_MODE_CANCELED;
    g_cg_sync_read = RLN30.LCUC.UINT8;
    __syncp();

    /* Set RLIN30TX pin */
    PORT0.PKCPROT.UINT32 = _WRITE_PROTECT_ENABLE;
    PORT0.PWE.UINT32 = SETBIT(2U);
    PORT0.PCR02_6.UINT32 = (PORT0.PCR02_6.UINT32 & _PCR_DEFAULT_VALUE) | _PCR_SET_PM;
    PORT0.PCR02_6.UINT32 |= _PCR_ALT_OUT8;
    PORT0.PCR02_6.UINT32 |= _PCR_SET_PMC;
    PORT0.PCR02_6.UINT32 &= _PCR_CLEAR_PM;
    PORT0.PCR02_6.UINT32 &= _PCR_ALT_OUT_SETTING;
    PORT0.PWE.UINT32 = _PORT_WRITE_PROTECT_DISABLE;
    PORT0.PKCPROT.UINT32 = _WRITE_PROTECT_DISABLE;

    R_Config_UART0_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: r_uart0_init_intc2_interrupt
* Description  : This function initializes interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_uart0_init_intc2_interrupt(void)
{
    /* Set INTRLIN30UR0 setting */
    INTC2.EIC621.BIT.EIMK621 = _INT_PROCESSING_DISABLED;
    INTC2.EIC621.BIT.EIRF621 = _INT_REQUEST_NOT_OCCUR;
    INTC2.EIC621.BIT.EITB621 = _INT_TABLE_VECTOR;
    INTC2.EIC621.UINT16 &= _INT_PRIORITY_LEVEL14;
    INTC2.EIBD621.BIT.PEID = _INT_CPU_PE0;
}

/***********************************************************************************************************************
* Function Name: r_uart0_enable_intc2_interrupt
* Description  : This function enables the UART0 channel interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_uart0_enable_intc2_interrupt(void)
{
    /* Clear INTRLIN30UR0 request and enable operation */
    INTC2.EIC621.BIT.EIRF621 = _INT_REQUEST_NOT_OCCUR;
    INTC2.EIC621.BIT.EIMK621 = _INT_PROCESSING_ENABLED;
}

/***********************************************************************************************************************
* Function Name: r_uart0_disable_intc2_interrupt
* Description  : This function disables the UART0 channel interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_uart0_disable_intc2_interrupt(void)
{
    /* Disable INTRLIN30UR0 operation */
    INTC2.EIC621.BIT.EIMK621 = _INT_PROCESSING_DISABLED;
    /* Clear INTRLIN30UR0 request */
    INTC2.EIC621.BIT.EIRF621 = _INT_REQUEST_NOT_OCCUR;
}

/***********************************************************************************************************************
* Function Name: R_Config_UART0_Start
* Description  : This function starts the UART0 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_UART0_Start(void)
{
    /* Clear UART0 interrupt request and enable operation */
    r_uart0_enable_intc2_interrupt();
    /* Enable UART0 operation */
    RLN30.LUOER.UINT8 |= _UART_TRANSMISSION_ENABLED;
}

/***********************************************************************************************************************
* Function Name: R_Config_UART0_Stop
* Description  : This function stops the UART0 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_UART0_Stop(void)
{
    /* Disable UART0 operation */
    RLN30.LUOER.UINT8 &= (uint8_t) ~(_UART_TRANSMISSION_ENABLED);
    /* Disable UART0 interrupt operation and clear request */
    r_uart0_disable_intc2_interrupt();
    /* Synchronization processing */
    g_cg_sync_read = RLN30.LCUC.UINT8;
    __syncp();
}

/***********************************************************************************************************************
* Function Name: R_Config_UART0_Send
* Description  : This function sends UART0 data.
* Arguments    : tx_buf -
*                    send buffer pointer
*                tx_num -
*                    buffer size
* Return Value : status -
*                    MD_OK or MD_ARGERROR
***********************************************************************************************************************/
MD_STATUS R_Config_UART0_Send(uint8_t * const tx_buf, uint16_t tx_num)
{
    MD_STATUS status = MD_OK;
    if (tx_num < 1U)
    {
        status = MD_ARGERROR;
    }
    else
    {
        gp_uart0_tx_address = tx_buf;
        g_uart0_tx_count = tx_num;
        if (0U == (RLN30.LST.UINT8 & _UART_TRANSMISSION_OPERATED))
        {
            /* Clear INTRLIN30UR0 request and Disable operation */
            INTC2.EIC621.BIT.EIMK621 = _INT_PROCESSING_DISABLED;
            INTC2.EIC621.BIT.EIRF621 = _INT_REQUEST_NOT_OCCUR;
            RLN30.LUTDR.UINT16 = *gp_uart0_tx_address;
            gp_uart0_tx_address++;
            g_uart0_tx_count--;
            /* Clear INTRLIN30UR0 request and enable operation */
            INTC2.EIC621.BIT.EIRF621 = _INT_REQUEST_NOT_OCCUR;
            INTC2.EIC621.BIT.EIMK621 = _INT_PROCESSING_ENABLED;
        }
        else
        {
            status = MD_ERROR;
        }
    }

    return (status);
}

extern void r_Config_UART0_callback_sendend(void);
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_UART0_Create_UserInit
* Description  : This function adds user code after initializing the UART0.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_UART0_Create_UserInit(void)
{
    /* Start user code for user init. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_UART0_interrupt_send
* Description  : This function is UART0 send interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_Config_UART0_interrupt_send(void)
{
   if (g_uart0_tx_count > 0U)
   {
      RLN30.LUTDR.UINT16 = *gp_uart0_tx_address;
      gp_uart0_tx_address++;
      g_uart0_tx_count--;
   }
   else
   {
      r_Config_UART0_callback_sendend();
   }
}
