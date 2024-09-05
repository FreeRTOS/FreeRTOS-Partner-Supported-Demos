/* Copyright 2017-2024 Arm Limited and/or its affiliates
 * <open-source-office@arm.com>
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __SERIAL_H__
#define __SERIAL_H__


#include <stddef.h>

/**
 * \brief Initializes default UART device
 */
void bsp_serial_init( void );

/**
 * \brief Prints a string through the default UART device
 */
void bsp_serial_print( char * str );

#endif /* __SERIAL_H__ */
