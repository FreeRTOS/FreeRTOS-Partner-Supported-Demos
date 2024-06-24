/*
 * Copyright (c) 2023 Arm Limited. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __DRIVER_USART_CMSDK_H__
#define __DRIVER_USART_CMSDK_H__

#include "Driver_USART_Common.h"
#include "uart_cmsdk_drv.h"

#ifndef CMSIS_device_header
/* CMSIS pack default header, containing the CMSIS_device_header definition */
#include "RTE_Components.h"
#endif
#include CMSIS_device_header

extern uint32_t PeripheralClock;

/* Driver Capabilities */
static const ARM_USART_CAPABILITIES DriverCapabilities = {
    1, /* supports UART (Asynchronous) mode */
    0, /* supports Synchronous Master mode */
    0, /* supports Synchronous Slave mode */
    0, /* supports UART Single-wire mode */
    0, /* supports UART IrDA mode */
    0, /* supports UART Smart Card mode */
    0, /* Smart Card Clock generator available */
    0, /* RTS Flow Control available */
    0, /* CTS Flow Control available */
    0, /* Transmit completed event: \ref ARM_USARTx_EVENT_TX_COMPLETE */
    0, /* Signal receive character timeout event: \ref ARM_USARTx_EVENT_RX_TIMEOUT */
    0, /* RTS Line: 0=not available, 1=available */
    0, /* CTS Line: 0=not available, 1=available */
    0, /* DTR Line: 0=not available, 1=available */
    0, /* DSR Line: 0=not available, 1=available */
    0, /* DCD Line: 0=not available, 1=available */
    0, /* RI Line: 0=not available, 1=available */
    0, /* Signal CTS change event: \ref ARM_USARTx_EVENT_CTS */
    0, /* Signal DSR change event: \ref ARM_USARTx_EVENT_DSR */
    0, /* Signal DCD change event: \ref ARM_USARTx_EVENT_DCD */
    0, /* Signal RI change event: \ref ARM_USARTx_EVENT_RI */
    0  /* Reserved */
};

typedef struct {
    struct uart_cmsdk_dev_t *dev;     /* UART device structure */
    ARM_USART_SignalEvent_t cb_event; /* Callback function for events */
    IRQn_Type rx_irq_number;          /* Num of the peripheral's rx IRQ line */
    IRQn_Type tx_irq_number;          /* Num of the peripheral's tx IRQ line */
    ARM_POWER_STATE state;            /* Active power state */
    uint8_t initialized;              /* Is the peripheral initialized */

    /* Databits can only be 8, so always use uint8_t for the buffers */
    const uint8_t *tx_buffer;   /* TX buffer, it's allocated by the caller */
    uint32_t tx_size;           /* Number of data items to transfer */
    uint32_t tx_nbr_bytes;      /* Number of data items transfered */
    uint8_t *rx_buffer;         /* RX buffer, it's allocated by the caller */
    uint32_t rx_size;           /* Number of data items to receive */
    uint32_t rx_nbr_bytes;      /* Number of data items recevied */
} UARTx_Resources;

static inline ARM_USART_CAPABILITIES ARM_USART_GetCapabilities(void);
static inline int32_t ARM_USARTx_Initialize(UARTx_Resources *uart_dev, ARM_USART_SignalEvent_t cb_event);
static inline int32_t ARM_USARTx_Uninitialize(UARTx_Resources *uart_dev);
static inline int32_t ARM_USARTx_PowerControl(UARTx_Resources *uart_dev, ARM_POWER_STATE state);
static inline int32_t ARM_USARTx_Send(UARTx_Resources *uart_dev, const void *data, uint32_t num);
static inline int32_t ARM_USARTx_Receive(UARTx_Resources *uart_dev, void *data, uint32_t num);
static inline uint32_t ARM_USARTx_GetTxCount(UARTx_Resources *uart_dev);
static inline uint32_t ARM_USARTx_GetRxCount(UARTx_Resources *uart_dev);
static ARM_USART_STATUS ARM_USARTx_GetStatus(UARTx_Resources *uart_dev);
static inline int32_t ARM_USARTx_Control(UARTx_Resources *uart_dev, uint32_t control, uint32_t arg);

static inline ARM_USART_CAPABILITIES ARM_USART_GetCapabilities(void)
{
    return DriverCapabilities;
}

static inline int32_t ARM_USARTx_Initialize(UARTx_Resources *uart_dev, ARM_USART_SignalEvent_t cb_event)
{
    uart_dev->cb_event = cb_event;
    uart_dev->initialized = 1;

    return ARM_DRIVER_OK;
}

static inline int32_t ARM_USARTx_Uninitialize(UARTx_Resources *uart_dev)
{
    ARM_USARTx_PowerControl(uart_dev, ARM_POWER_OFF);

    /* Remove the callback so it won't be called
     * if an interrupt happens */
    uart_dev->cb_event = NULL;
    uart_dev->initialized = 0;

    return ARM_DRIVER_OK;
}

static inline int32_t ARM_USARTx_PowerControl(UARTx_Resources *uart_dev, ARM_POWER_STATE state)
{
    if (uart_dev->initialized != 1) {
        return ARM_DRIVER_ERROR;
    }

    switch (state) {
        case ARM_POWER_OFF:
            uart_cmsdk_irq_rx_disable(uart_dev->dev);
            uart_cmsdk_irq_tx_disable(uart_dev->dev);

            uart_cmsdk_rx_disable(uart_dev->dev);
            uart_cmsdk_tx_disable(uart_dev->dev);

            NVIC_DisableIRQ(uart_dev->rx_irq_number);
            NVIC_DisableIRQ(uart_dev->tx_irq_number);

            uart_dev->state = ARM_POWER_OFF;
            return ARM_DRIVER_OK;
        case ARM_POWER_LOW:
            return ARM_DRIVER_ERROR_UNSUPPORTED;
        case ARM_POWER_FULL:
            if (uart_dev->state == ARM_POWER_FULL) {
                return ARM_DRIVER_OK;
            }

            /* Initializes generic UART driver */
            uart_cmsdk_init(uart_dev->dev, PeripheralClock);

            /* Clear pending IRQs, if they were set before */
            NVIC_ClearPendingIRQ(uart_dev->rx_irq_number);
            NVIC_ClearPendingIRQ(uart_dev->tx_irq_number);

            NVIC_EnableIRQ(uart_dev->rx_irq_number);
            NVIC_EnableIRQ(uart_dev->tx_irq_number);

            uart_cmsdk_irq_rx_enable(uart_dev->dev);
            uart_cmsdk_irq_tx_enable(uart_dev->dev);

            uart_dev->state = ARM_POWER_FULL;
            return ARM_DRIVER_OK;

            /* default:  The default is not defined intentionally to force the
             *           compiler to check that all the enumeration values are
             *           covered in the switch.*/
    }

    /* Invalid state requested */
    return ARM_DRIVER_ERROR;
}

static inline int32_t ARM_USARTx_Send(UARTx_Resources *uart_dev, const void *data, uint32_t num)
{
    if (uart_dev->initialized != 1) {
        return ARM_DRIVER_ERROR;
    }
    if (uart_dev->state != ARM_POWER_FULL) {
        return ARM_DRIVER_ERROR;
    }
    if ((data == NULL) || (num == 0U)) {
        /* Invalid parameters */
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    /* Data buffer must stay allocated during the transfer, it's the caller's
     * responsibility */
    uart_dev->tx_buffer = (const uint8_t *)data;
    uart_dev->tx_size = num;
    uart_dev->tx_nbr_bytes = 0;

    /* Only trigger the transaction. The rest of the logic will be in the IRQ handler:
     * the CMSDK UART generates an IRQ after each sent bytes */
    /* The caller has to make sure that the peripheral is ready */
    if (UART_CMSDK_ERR_NONE != uart_cmsdk_write(uart_dev->dev, *uart_dev->tx_buffer)) {
        return ARM_DRIVER_ERROR;
    }

    return ARM_DRIVER_OK;
}

static inline int32_t ARM_USARTx_Receive(UARTx_Resources *uart_dev, void *data, uint32_t num)
{
    uint8_t placeholder_data;

    if (uart_dev->initialized != 1) {
        return ARM_DRIVER_ERROR;
    }
    if (uart_dev->state != ARM_POWER_FULL) {
        return ARM_DRIVER_ERROR;
    }
    if ((data == NULL) || (num == 0U)) {
        /* Invalid parameters */
        return ARM_DRIVER_ERROR_PARAMETER;
    }

    /* Read out any unread data from fifo */
    placeholder_data = 0;
    while (uart_cmsdk_rx_ready(uart_dev->dev) == 1) {
        uart_cmsdk_read(uart_dev->dev, &placeholder_data);
    }

    /* Resets previous RX transaction values */
    /* Data buffer must stay allocated during the transfer, it's the caller's
     * responsibility */
    uart_dev->rx_buffer = (uint8_t *)data;
    uart_dev->rx_size = num;
    uart_dev->rx_nbr_bytes = 0;

    /* The rest of the logic is in the IRQ handler: the CMSDK UART
     * generates an IRQ after each received bytes */

    return ARM_DRIVER_OK;
}

static inline int32_t ARM_USARTx_Transfer(UARTx_Resources *uart_dev, const void *data_out, void *data_in, uint32_t num)
{
    ARG_UNUSED(data_out);
    ARG_UNUSED(data_in);
    ARG_UNUSED(num);

    if (uart_dev->initialized != 1) {
        return ARM_DRIVER_ERROR;
    }
    if (uart_dev->state != ARM_POWER_FULL) {
        return ARM_DRIVER_ERROR;
    }
    return ARM_DRIVER_ERROR_UNSUPPORTED;
}

static inline uint32_t ARM_USARTx_GetTxCount(UARTx_Resources *uart_dev)
{
    return uart_dev->tx_nbr_bytes;
}

static inline uint32_t ARM_USARTx_GetRxCount(UARTx_Resources *uart_dev)
{
    return uart_dev->rx_nbr_bytes;
}

static ARM_USART_STATUS ARM_USARTx_GetStatus(UARTx_Resources *uart_dev)
{
    ARM_USART_STATUS status = {0};

    /* The RX and TX buffers are only set when the operation is
     * ongoing */
    status.tx_busy = uart_dev->tx_buffer == NULL ? 0 : 1;
    status.rx_busy = uart_dev->rx_buffer == NULL ? 0 : 1;

    return status;
}

static inline int32_t ARM_USARTx_Control(UARTx_Resources *uart_dev, uint32_t control, uint32_t arg)
{
    if (uart_dev->initialized != 1) {
        return ARM_DRIVER_ERROR;
    }
    if (uart_dev->state != ARM_POWER_FULL) {
        return ARM_DRIVER_ERROR;
    }

    switch (control & ARM_USART_CONTROL_Msk) {
        case ARM_USART_CONTROL_TX:
            if (arg == 0) {
                uart_cmsdk_tx_disable(uart_dev->dev);
            } else if (arg == 1) {
                if (uart_cmsdk_tx_enable(uart_dev->dev) != UART_CMSDK_ERR_NONE) {
                    return ARM_DRIVER_ERROR;
                }
            } else {
                return ARM_DRIVER_ERROR_PARAMETER;
            }
            break;
        case ARM_USART_CONTROL_RX:
            if (arg == 0) {
                uart_cmsdk_rx_disable(uart_dev->dev);
            } else if (arg == 1) {
                if (uart_cmsdk_rx_enable(uart_dev->dev) != UART_CMSDK_ERR_NONE) {
                    return ARM_DRIVER_ERROR;
                }
            } else {
                return ARM_DRIVER_ERROR_PARAMETER;
            }
            break;
        case ARM_USART_MODE_ASYNCHRONOUS:
            if (uart_cmsdk_set_baudrate(uart_dev->dev, arg) != UART_CMSDK_ERR_NONE) {
                return ARM_USART_ERROR_BAUDRATE;
            }
            break;
        case ARM_USART_ABORT_SEND:
            uart_dev->tx_buffer = NULL;
            uart_dev->tx_nbr_bytes = 0;
            uart_dev->tx_size = 0;
            break;
        case ARM_USART_ABORT_RECEIVE:
            uart_dev->rx_buffer = NULL;
            uart_dev->rx_nbr_bytes = 0;
            uart_dev->rx_size = 0;
            break;
        /* Unsupported command */
        default:
            return ARM_DRIVER_ERROR_UNSUPPORTED;
    }

    /* UART Data bits */
    if (control & ARM_USART_DATA_BITS_Msk) {
        /* Data bit is not configurable */
        return ARM_DRIVER_ERROR_UNSUPPORTED;
    }

    /* UART Parity */
    if (control & ARM_USART_PARITY_Msk) {
        /* Parity is not configurable */
        return ARM_USART_ERROR_PARITY;
    }

    /* USART Stop bits */
    if (control & ARM_USART_STOP_BITS_Msk) {
        /* Stop bit is not configurable */
        return ARM_USART_ERROR_STOP_BITS;
    }

    return ARM_DRIVER_OK;
}

/*
 * \brief Macro for USART CMSDK Driver
 *
 * \param[in]  USART_DEV          Native driver device
 *                                \ref uart_cmsdk_dev_t
 * \param[out] USART_DRIVER_NAME  Resulting Driver name
 */
#define ARM_DRIVER_USART_CMSDK(USART_DEV, USART_DRIVER_NAME, RX_IRQ_HANDLER, TX_IRQ_HANDLER, RX_IRQ_NUM, TX_IRQ_NUM) \
                                                                                                                     \
    static UARTx_Resources USART_DRIVER_NAME##_DEV = {                                                               \
        .dev = &USART_DEV,                                                                                           \
        .tx_nbr_bytes = 0,                                                                                           \
        .rx_nbr_bytes = 0,                                                                                           \
        .cb_event = NULL,                                                                                            \
        .rx_irq_number = RX_IRQ_NUM,                                                                                 \
        .tx_irq_number = TX_IRQ_NUM,                                                                                 \
    };                                                                                                               \
                                                                                                                     \
    void RX_IRQ_HANDLER(void);                                                                                       \
    void TX_IRQ_HANDLER(void);                                                                                       \
                                                                                                                     \
    void RX_IRQ_HANDLER(void)                                                                                        \
    {                                                                                                                \
        uart_cmsdk_clear_interrupt(USART_DRIVER_NAME##_DEV.dev, UART_CMSDK_IRQ_RX);                                  \
                                                                                                                     \
        /* If RX buffer is NULL then there's no active Receive operation,                                            \
         * just flush the input fifo. */                                                                             \
        if (USART_DRIVER_NAME##_DEV.rx_buffer == NULL) {                                                             \
            uint8_t placeholder_data;                                                                                \
            uart_cmsdk_read(USART_DRIVER_NAME##_DEV.dev, &placeholder_data);                                         \
            return;                                                                                                  \
        }                                                                                                            \
                                                                                                                     \
        /* The return value is ignored because the RX FIFO has to be full if the interrupt happend */                \
        uart_cmsdk_read(USART_DRIVER_NAME##_DEV.dev,                                                                 \
                        &USART_DRIVER_NAME##_DEV.rx_buffer[USART_DRIVER_NAME##_DEV.rx_nbr_bytes]);                   \
        USART_DRIVER_NAME##_DEV.rx_nbr_bytes++;                                                                      \
                                                                                                                     \
        if (USART_DRIVER_NAME##_DEV.rx_nbr_bytes == USART_DRIVER_NAME##_DEV.rx_size) {                               \
            USART_DRIVER_NAME##_DEV.rx_buffer = NULL;                                                                \
            if (USART_DRIVER_NAME##_DEV.cb_event != NULL) {                                                          \
                USART_DRIVER_NAME##_DEV.cb_event(ARM_USART_EVENT_RECEIVE_COMPLETE);                                  \
            }                                                                                                        \
        }                                                                                                            \
    }                                                                                                                \
                                                                                                                     \
    void TX_IRQ_HANDLER(void)                                                                                        \
    {                                                                                                                \
        uart_cmsdk_clear_interrupt(USART_DRIVER_NAME##_DEV.dev, UART_CMSDK_IRQ_TX);                                  \
                                                                                                                     \
        /* If TX buffer is NULL then there's no active Send operation */                                             \
        if (USART_DRIVER_NAME##_DEV.tx_buffer == NULL) {                                                             \
            return;                                                                                                  \
        }                                                                                                            \
                                                                                                                     \
        USART_DRIVER_NAME##_DEV.tx_nbr_bytes++;                                                                      \
                                                                                                                     \
        if (USART_DRIVER_NAME##_DEV.tx_nbr_bytes == USART_DRIVER_NAME##_DEV.tx_size) {                               \
            USART_DRIVER_NAME##_DEV.tx_buffer = NULL;                                                                \
            if (USART_DRIVER_NAME##_DEV.cb_event != NULL) {                                                          \
                USART_DRIVER_NAME##_DEV.cb_event(ARM_USART_EVENT_SEND_COMPLETE);                                     \
            }                                                                                                        \
        } else {                                                                                                     \
            /* The return value is ignored because the TX FIFO has to be empty if the interrupt happend */           \
            uart_cmsdk_write(USART_DRIVER_NAME##_DEV.dev,                                                            \
                             USART_DRIVER_NAME##_DEV.tx_buffer[USART_DRIVER_NAME##_DEV.tx_nbr_bytes]);               \
        }                                                                                                            \
    }                                                                                                                \
                                                                                                                     \
    static int32_t USART_DRIVER_NAME##_Initialize(ARM_USART_SignalEvent_t cb_event)                                  \
    {                                                                                                                \
        return ARM_USARTx_Initialize(&USART_DRIVER_NAME##_DEV, cb_event);                                            \
    }                                                                                                                \
                                                                                                                     \
    static int32_t USART_DRIVER_NAME##_Uninitialize(void)                                                            \
    {                                                                                                                \
        return ARM_USARTx_Uninitialize(&USART_DRIVER_NAME##_DEV);                                                    \
    }                                                                                                                \
                                                                                                                     \
    static int32_t USART_DRIVER_NAME##_PowerControl(ARM_POWER_STATE state)                                           \
    {                                                                                                                \
        return ARM_USARTx_PowerControl(&USART_DRIVER_NAME##_DEV, state);                                             \
    }                                                                                                                \
                                                                                                                     \
    static int32_t USART_DRIVER_NAME##_Send(const void *data, uint32_t num)                                          \
    {                                                                                                                \
        return ARM_USARTx_Send(&USART_DRIVER_NAME##_DEV, data, num);                                                 \
    }                                                                                                                \
                                                                                                                     \
    static int32_t USART_DRIVER_NAME##_Receive(void *data, uint32_t num)                                             \
    {                                                                                                                \
        return ARM_USARTx_Receive(&USART_DRIVER_NAME##_DEV, data, num);                                              \
    }                                                                                                                \
                                                                                                                     \
    static int32_t USART_DRIVER_NAME##_Transfer(const void *data_out, void *data_in, uint32_t num)                   \
    {                                                                                                                \
        return ARM_USARTx_Transfer(&USART_DRIVER_NAME##_DEV, data_out, data_in, num);                                \
    }                                                                                                                \
                                                                                                                     \
    static uint32_t USART_DRIVER_NAME##_GetTxCount(void)                                                             \
    {                                                                                                                \
        return ARM_USARTx_GetTxCount(&USART_DRIVER_NAME##_DEV);                                                      \
    }                                                                                                                \
                                                                                                                     \
    static uint32_t USART_DRIVER_NAME##_GetRxCount(void)                                                             \
    {                                                                                                                \
        return ARM_USARTx_GetRxCount(&USART_DRIVER_NAME##_DEV);                                                      \
    }                                                                                                                \
    static int32_t USART_DRIVER_NAME##_Control(uint32_t control, uint32_t arg)                                       \
    {                                                                                                                \
        return ARM_USARTx_Control(&USART_DRIVER_NAME##_DEV, control, arg);                                           \
    }                                                                                                                \
                                                                                                                     \
    static ARM_USART_STATUS USART_DRIVER_NAME##_GetStatus(void)                                                      \
    {                                                                                                                \
        return ARM_USARTx_GetStatus(&USART_DRIVER_NAME##_DEV);                                                       \
    }                                                                                                                \
                                                                                                                     \
    static int32_t USART_DRIVER_NAME##_SetModemControl(ARM_USART_MODEM_CONTROL control)                              \
    {                                                                                                                \
        ARG_UNUSED(control);                                                                                         \
        return ARM_DRIVER_ERROR_UNSUPPORTED;                                                                         \
    }                                                                                                                \
                                                                                                                     \
    static ARM_USART_MODEM_STATUS USART_DRIVER_NAME##_GetModemStatus(void)                                           \
    {                                                                                                                \
        ARM_USART_MODEM_STATUS modem_status = {0, 0, 0, 0, 0};                                                       \
        return modem_status;                                                                                         \
    }                                                                                                                \
                                                                                                                     \
    extern ARM_DRIVER_USART USART_DRIVER_NAME;                                                                       \
    ARM_DRIVER_USART USART_DRIVER_NAME = {ARM_USART_GetVersion,                                                      \
                                          ARM_USART_GetCapabilities,                                                 \
                                          USART_DRIVER_NAME##_Initialize,                                            \
                                          USART_DRIVER_NAME##_Uninitialize,                                          \
                                          USART_DRIVER_NAME##_PowerControl,                                          \
                                          USART_DRIVER_NAME##_Send,                                                  \
                                          USART_DRIVER_NAME##_Receive,                                               \
                                          USART_DRIVER_NAME##_Transfer,                                              \
                                          USART_DRIVER_NAME##_GetTxCount,                                            \
                                          USART_DRIVER_NAME##_GetRxCount,                                            \
                                          USART_DRIVER_NAME##_Control,                                               \
                                          USART_DRIVER_NAME##_GetStatus,                                             \
                                          USART_DRIVER_NAME##_SetModemControl,                                       \
                                          USART_DRIVER_NAME##_GetModemStatus}

#endif /* __DRIVER_USART_CMSDK_H__ */
