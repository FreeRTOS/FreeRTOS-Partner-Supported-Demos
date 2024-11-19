/* Copyright 2017-2024 Arm Limited and/or its affiliates
 * <open-source-office@arm.com>
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <string.h>
#include "device_cfg.h"
#include "Driver_USART.h"
#include "bsp_serial.h"

extern ARM_DRIVER_USART Driver_USART0;

void bsp_serial_init( void )
{
    Driver_USART0.Initialize( NULL );

    Driver_USART0.PowerControl( ARM_POWER_FULL );

    Driver_USART0.Control( ARM_USART_MODE_ASYNCHRONOUS, DEFAULT_UART_BAUDRATE );

    Driver_USART0.Control( ARM_USART_CONTROL_TX, 1 );

    Driver_USART0.Control( ARM_USART_CONTROL_RX, 1 );
}

#if defined( __ARMCOMPILER_VERSION )

    int stdio_output_string(const unsigned char *str, uint32_t len)
    {
        int32_t ret;

        /* Add a busy wait before sending. */
        while (Driver_USART0.GetStatus().tx_busy);
        ret = Driver_USART0.Send(str, len);
        if (ret != ARM_DRIVER_OK) {
            return 0;
        }
        /* Add a busy wait after sending. */
        while (Driver_USART0.GetStatus().tx_busy);

        return Driver_USART0.GetTxCount();
    }

    int fputc(int ch, FILE *f)
    {
        (void)f;

        /* Send byte to USART */
        (void)stdio_output_string((const unsigned char *)&ch, 1);

        /* Return character written */
        return ch;
    }

#endif /* if defined( __ARMCOMPILER_VERSION ) */
