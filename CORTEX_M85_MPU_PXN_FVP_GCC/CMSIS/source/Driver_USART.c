/*
 * Copyright (c) 2013-2023 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Driver_USART_CMSDK.h"
#include "RTE_Components.h"
#include "cmsis_driver_config.h"

#if (defined(RTE_USART0) && (RTE_USART0 == 1)) || (defined(RTE_USART1) && (RTE_USART1 == 1))    \
    || (defined(RTE_USART2) && (RTE_USART2 == 1)) || (defined(RTE_USART3) && (RTE_USART3 == 1)) \
    || (defined(RTE_USART4) && (RTE_USART4 == 1))

#if (defined(RTE_USART0) && (RTE_USART0 == 1))

ARM_DRIVER_USART_CMSDK(UART0_CMSDK_DEV, Driver_USART0, UARTRX0_Handler, UARTTX0_Handler, UARTRX0_IRQn, UARTTX0_IRQn);

#endif /* RTE_USART0 */

#if (defined(RTE_USART1) && (RTE_USART1 == 1))

ARM_DRIVER_USART_CMSDK(UART1_CMSDK_DEV, Driver_USART1, UARTRX1_Handler, UARTTX1_Handler, UARTRX1_IRQn, UARTTX1_IRQn);

#endif /* RTE_USART1 */

#if (defined(RTE_USART2) && (RTE_USART2 == 1))

ARM_DRIVER_USART_CMSDK(UART2_CMSDK_DEV, Driver_USART2, UARTRX2_Handler, UARTTX2_Handler, UARTRX2_IRQn, UARTTX2_IRQn);

#endif /* RTE_USART2 */

#if (defined(RTE_USART3) && (RTE_USART3 == 1))

ARM_DRIVER_USART_CMSDK(UART3_CMSDK_DEV, Driver_USART3, UARTRX3_Handler, UARTTX3_Handler, UARTRX3_IRQn, UARTTX3_IRQn);

#endif /* RTE_USART3 */

#if (defined(RTE_USART4) && (RTE_USART4 == 1))

ARM_DRIVER_USART_CMSDK(UART4_CMSDK_DEV, Driver_USART4, UARTRX4_Handler, UARTTX4_Handler, UARTRX4_IRQn, UARTTX4_IRQn);
#endif /* RTE_USART4 */
#endif
