/*
 * FreeRTOS V202212.01
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

#ifndef SEC_CORE_TYPES_H
#define SEC_CORE_TYPES_H

/**
 @ingroup   sec_coredriver
 @enum      SEC_CORE_SYSTEM_STATUS
 @brief     Defines the status enumeration for sec_core.
*/
enum SEC_CORE_SYSTEM_STATUS
{
    SEC_CORE_SYSTEM_STATUS_NOT_IN_LOW_POWER_MODE = 0,   /**< sec_core status is in Low power mode */
    SEC_CORE_SYSTEM_STATUS_SLEEP_MODE            = 1,   /**< sec_core status is in Sleep mode */
    SEC_CORE_SYSTEM_STATUS_IDLE_MODE             = 2,   /**< sec_core status is in Idle mode */
    SEC_CORE_SYSTEM_STATUS_RUNNING_STATE         = 3,   /**< sec_core status is in Running state */
    SEC_CORE_SYSTEM_STATUS_IN_RESET_STATE        = 4    /**< sec_core status is in Reset state */
};

/**
 @ingroup   sec_coredriver
 @enum      SEC_CORE_RESET_CAUSE
 @brief     Defines the sec_core reset cause enumeration
*/
enum SEC_CORE_RESET_CAUSE
{
    SEC_CORE_RESET_CAUSE_WATCHDOG    = 0,            /**< sec_core reset caused from Watchdog Timer */
    SEC_CORE_RESET_CAUSE_UNSPECIFIED = 1             /**< sec_core reset cause is unspecified */
};

/**
 @ingroup   sec_coredriver
 @enum      SEC_CORE_PROTOCOLS
 @brief     Defines the list of Protocols configured for SEC_CORE_driver. 
 @note      These Protocols are required to create the buffer by the application
*/
enum SEC_CORE_PROTOCOLS
{
    MSI1_ProtocolA,      /**< Custom-name of ProtocolA for write operation only */
    MSI1_ProtocolB,      /**< Custom-name of ProtocolB for read operation only */
};

/**
 @ingroup   sec_coredriver
 @enum      SEC_CORE_PROTOCOL_SIZE
 @brief     Defines the size for Protocols.
 @note      Size not required for the driver but will be required for application to create the buffers
*/
enum SEC_CORE_PROTOCOL_SIZE
{
    MSI1_ProtocolA_SIZE = 1,     /**< Size of ProtocolA in 16-bit word*/
    MSI1_ProtocolB_SIZE = 1,     /**< Size of ProtocolB in 16-bit word*/
};

#endif    /* SEC_CORE1_TYPES_H */