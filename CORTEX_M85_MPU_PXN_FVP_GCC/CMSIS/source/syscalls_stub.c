/*
 * Copyright (c) 2023-2024, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Based on: https://git.trustedfirmware.org/TF-M/trusted-firmware-m.git/tree/platform/ext/common/syscalls_stub.c?id=9ca8a5eb3c85eecee1303dffa262800ea0385584
 *
 */

/*
 * Note: Arm GNU toolchain version 12 and above require user defined definitions of the functions below.
 * However, in prior versions those definitions were provided as weakly linked definitions by the toolchain.
 * Provide them here as weakly linked functions to allow applications that consume the FRI to provide
 * their own definitions as intended by the toolchain.
 */

#ifdef __GNUC__
#if defined( __ARM_ARCH )
    #include <stddef.h>
    #include <stdint.h>

    __attribute__( ( weak ) )
    void _close( void )
    {
    }

    __attribute__( ( weak ) )
    void _fstat( void )
    {
    }

    __attribute__( ( weak ) )
    void _getpid( void )
    {
    }

    __attribute__( ( weak ) )
    void _isatty( void )
    {
    }

    __attribute__( ( weak ) )
    void _kill( void )
    {
    }

    __attribute__( ( weak ) )
    void _lseek( void )
    {
    }

    __attribute__( ( weak ) )
    void _read( void )
    {
    }

    __attribute__( ( weak ) )
    void _write( void )
    {
    }
#endif /* if defined( __ARM_ARCH ) */
#endif /* ifdef __GNUC__ */
