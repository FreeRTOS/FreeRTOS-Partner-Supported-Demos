/***********************************************************************************************************************
* File Name        : Config_UART0.c
* Description      : This file implements device driver for Config_UART0.
***********************************************************************************************************************/

/***********************************************************************************************************************
* Pragma directive
***********************************************************************************************************************/

/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Includes
***********************************************************************************************************************/
#include <stdio.h>
#include <stdarg.h>
#include "Config_UART0.h"

/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
static char s_tx_buff[ _UART_MAX_TRANSFER_SIZE ] = { 0 };

/***********************************************************************************************************************
* Global variables and functions
***********************************************************************************************************************/
volatile uint8_t * gp_uart0_tx_address; /* uart0 transmit data address */
volatile uint16_t g_uart0_tx_count;     /* uart0 transmit data number */
volatile uint32_t g_cg_sync_read;

void r_uart0_init_intc2_interrupt( void );

/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_UART0_Create
* Description  : This function initializes the UART0 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_UART0_Create( void )
{
    r_uart0_init_intc2_interrupt();

    /* Enable clock access */

    /* Disable write protect of clock controller */
    SYSCTRL.CLKKCPROT1.UINT32 = _WRITE_PROTECT_ENABLE;

    /* Change clock source for RLIN3 */
    SYSCTRL.CKSC_RLINC.BIT.RLINSCSID = _CLOCK_CLKC_HSB;

    while( ( SYSCTRL.CKSC_RLINS.BIT.RLINSACT ) != ( _CLOCK_CLKC_HSB ) )
    {
        NOP();
    }

    /* Enable write protect of clock controller */
    SYSCTRL.CLKKCPROT1.UINT32 = _WRITE_PROTECT_DISABLE;

    /* Disable module standby */
    SYSCTRL.MSRKCPROT.UINT32 = _WRITE_PROTECT_ENABLE;
    SYSCTRL.MSR_RLIN3.UINT32 = _MODULE_STANDBY_DISABLE;
    SYSCTRL.MSRKCPROT.UINT32 = _WRITE_PROTECT_DISABLE;

    /* Delay for clock supplied to module is stable */
    for( unsigned int i = 0; i < 10000; i++ )
    {
        NOP();
    }

    /* Set LIN reset mode */
    RLN30.LCUC.UINT8 = _UART_LIN_RESET_MODE_CAUSED;
    RLN30.LWBR.UINT8 = _UART_6_SAMPLING | _UART_PRESCALER_CLOCK_SELECT_1;
    RLN30.LBRP01.UINT16 = _UART0_BAUD_RATE_PRESCALER;
    RLN30.LMD.UINT8 = _UART_MODE_SELECT;
    RLN30.LSC.UINT8 = _UART_INTER_BYTE_SPACE_SELECT_0BIT;
    RLN30.LEDE.UINT8 = _UART_FRAMING_ERROR_DETECTED_ENABLE | _UART_OVERRUN_ERROR_DETECTED_ENABLE |
                       _UART_BIT_ERROR_DETECTED_ENABLE;
    RLN30.LBFC.UINT8 = _UART_TRANSMISSION_NORMAL | _UART_PARITY_PROHIBITED | _UART_LSB | _UART_LENGTH_8 |
                       _UART_STOP_BIT_1;
    RLN30.LCUC.UINT8 = _UART_LIN_RESET_MODE_CANCELED;
    g_cg_sync_read = RLN30.LCUC.UINT8;
    __syncp();

    /* Set RLIN30TX pin */
    PORT0.PKCPROT = _WRITE_PROTECT_ENABLE;
    PORT0.PWE = SETBIT( 10U );
    PORT0.PCR10_0.UINT32 = ( PORT0.PCR10_0.UINT32 & _PCR_DEFAULT_VALUE ) | _PCR_SET_PM;
    PORT0.PCR10_0.UINT32 |= _PCR_ALT_OUT4;
    PORT0.PCR10_0.UINT32 |= _PCR_SET_PMC;
    PORT0.PCR10_0.UINT32 &= _PCR_CLEAR_PM;
    PORT0.PCR10_0.UINT32 &= _PCR_ALT_OUT_SETTING;
    PORT0.PWE = _PORT_WRITE_PROTECT_DISABLE;
    PORT0.PKCPROT = _WRITE_PROTECT_DISABLE;
    R_Config_UART0_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: r_uart0_init_intc2_interrupt
* Description  : This function initializes interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_uart0_init_intc2_interrupt( void )
{
    /* Set INTRLIN30UR0 setting */
    INTC2.EIC417.BIT.EIMK417 = _INT_PROCESSING_DISABLED;
    INTC2.EIC417.BIT.EIRF417 = _INT_REQUEST_NOT_OCCUR;
    INTC2.EIC417.BIT.EITB417 = _INT_TABLE_VECTOR;
    INTC2.EIC417.UINT16 &= _INT_PRIORITY_LEVEL14;
    INTC2.EIBD417.BIT.PEID = _INT_CPU_PE0;
}

/***********************************************************************************************************************
* Function Name: r_uart0_enable_intc2_interrupt
* Description  : This function enables the UART0 channel interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_uart0_enable_intc2_interrupt( void )
{
    EI();

    /* Clear INTRLIN30UR0 request and enable operation */
    INTC2.EIC417.BIT.EIRF417 = _INT_REQUEST_NOT_OCCUR;
    INTC2.EIC417.BIT.EIMK417 = _INT_PROCESSING_ENABLED;
}

/***********************************************************************************************************************
* Function Name: r_uart0_disable_intc2_interrupt
* Description  : This function disables the UART0 channel interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_uart0_disable_intc2_interrupt( void )
{
    /* Disable INTRLIN30UR0 operation */
    INTC2.EIC417.BIT.EIMK417 = _INT_PROCESSING_DISABLED;

    /* Clear INTRLIN30UR0 request */
    INTC2.EIC417.BIT.EIRF417 = _INT_REQUEST_NOT_OCCUR;
}

/***********************************************************************************************************************
* Function Name: R_Config_UART0_Start
* Description  : This function starts the UART0 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_UART0_Start( void )
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
void R_Config_UART0_Stop( void )
{
    /* Disable UART0 operation */
    RLN30.LUOER.UINT8 &= ( uint8_t ) ~( _UART_TRANSMISSION_ENABLED );

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
MD_STATUS R_Config_UART0_Send( uint8_t * const tx_buf,
                               uint16_t tx_num )
{
    MD_STATUS status = MD_OK;

    if( tx_num < 1U )
    {
        status = MD_ARGERROR;
    }
    else
    {
        gp_uart0_tx_address = tx_buf;
        g_uart0_tx_count = tx_num;

        if( 0U == ( RLN30.LST.UINT8 & _UART_TRANSMISSION_OPERATED ) )
        {
            /* Clear INTRLIN30UR0 request and Disable operation */
            INTC2.EIC417.BIT.EIMK417 = _INT_PROCESSING_DISABLED;
            INTC2.EIC417.BIT.EIRF417 = _INT_REQUEST_NOT_OCCUR;
            RLN30.LUTDR.UINT16 = *gp_uart0_tx_address;
            gp_uart0_tx_address++;
            g_uart0_tx_count--;

            /* Clear INTRLIN30UR0 request and enable operation */
            INTC2.EIC417.BIT.EIRF417 = _INT_REQUEST_NOT_OCCUR;
            INTC2.EIC417.BIT.EIMK417 = _INT_PROCESSING_ENABLED;
        }
        else
        {
            status = MD_ERROR;
        }
    }

    return status;
}

/***********************************************************************************************************************
* Function Name: R_Config_UART0_Printf
* Description  : This function converts formatted string and transfers with UART.
* Arguments    : fmt -
*                    formatted string
* Return Value : status -
*                    MD_OK for transmission success
*                    MD_BUSY when another transmission is ongoing
*                    MD_OVF when the transmit buffer is overflowed
***********************************************************************************************************************/
MD_STATUS R_Config_UART0_Printf( const char * fmt,
                                 ... )
{
    if( g_uart0_tx_count > 0 )
    {
        return MD_BUSY;
    }

    int len = 0;
    va_list va;
    va_start( va, fmt );
    len = vsprintf( s_tx_buff, fmt, va );
    va_end( va );

    if( 0 < len )
    {
        if( ( _UART_MAX_TRANSFER_SIZE - 1 ) <= len )
        {
            return MD_OVF;
        }

        R_Config_UART0_Send( ( uint8_t * ) s_tx_buff, len );
    }

    while( 0U < g_uart0_tx_count )
    {
        NOP();
    }

    return MD_OK;
}

extern void r_Config_UART0_callback_sendend( void );

/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_UART0_Create_UserInit
* Description  : This function adds user code after initializing the UART0.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_UART0_Create_UserInit( void )
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
void r_Config_UART0_interrupt_send( void )
{
    if( ( g_uart0_tx_count > 0U ) && ( '\0' != *gp_uart0_tx_address ) )
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
