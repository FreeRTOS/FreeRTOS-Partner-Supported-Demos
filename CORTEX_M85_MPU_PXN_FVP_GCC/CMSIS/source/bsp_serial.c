/* Copyright 2017-2024 Arm Limited and/or its affiliates
 * <open-source-office@arm.com>
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <string.h>
#include "device_cfg.h"
#include "Driver_USART.h"
#include "bsp_serial.h"

#include "FreeRTOS.h"
#include "semphr.h"

#define STDIN_FILENO     0
#define STDOUT_FILENO    1
#define STDERR_FILENO    2

typedef enum
{
    WRITE_ERROR_SEND_FAIL = -3,
    WRITE_ERROR_SYNC_FAILED = -2,
    WRITE_ERROR_INVALID_ARGS = -1,
    WRITE_ERROR_NONE = 0
} WriteError_t;

typedef struct
{
    WriteError_t error;
    unsigned int charsWritten;
} WriteResult_t;

extern ARM_DRIVER_USART Driver_USART0;

static SemaphoreHandle_t xLoggingMutex = NULL;

static bool prvValidFdHandle( int fd );
static void prvWriteChars( int fd,
                           const unsigned char * str,
                           unsigned int len,
                           WriteResult_t * result );

void bsp_serial_init( void )
{
    Driver_USART0.Initialize( NULL );
    Driver_USART0.PowerControl( ARM_POWER_FULL );
    Driver_USART0.Control( ARM_USART_MODE_ASYNCHRONOUS, DEFAULT_UART_BAUDRATE );
    Driver_USART0.Control( ARM_USART_CONTROL_TX, 1 );
    Driver_USART0.Control( ARM_USART_CONTROL_RX, 1 );

    if( xLoggingMutex == NULL )
    {
        xLoggingMutex = xSemaphoreCreateMutex();
        configASSERT( xLoggingMutex );
    }
}

void bsp_serial_print( char * str )
{
    ( void ) Driver_USART0.Send( str, strlen( str ) );

    while( Driver_USART0.GetTxCount() != strlen( str ) )
    {
    }
}

#if defined( __ARMCOMPILER_VERSION )

/* Retarget armclang, which requires all IO system calls to be overridden together. */

    #include <rt_sys.h>

    FILEHANDLE _sys_open( const char * name,
                          int openmode )
    {
        if( name == NULL )
        {
            return -1;
        }

        /* By default, the Arm Compiler uses the special file path ":tt" for stdin, */
        /* stdout and stderr and distinguishes between them using openmode. For details, */
        /* see https://github.com/ARM-software/abi-aa/blob/2022Q1/semihosting/semihosting.rst#sys-open-0x01 */
        if( strcmp( name, ":tt" ) == 0 )
        {
            if( openmode & OPEN_W )
            {
                return STDOUT_FILENO;
            }

            if( openmode & OPEN_A )
            {
                return STDERR_FILENO;
            }

            return STDIN_FILENO;
        }

        return -1;
    }

    int _sys_close( FILEHANDLE fh )
    {
        /* Not implemented */
        ( void ) fh;
        return -1;
    }

    int _sys_write( FILEHANDLE fd,
                    const unsigned char * str,
                    unsigned int len,
                    int mode )
    {
        /* From <rt_sys.h>: `mode` exists for historical reasons and must be ignored. */
        ( void ) mode;

        WriteResult_t result = { .error = WRITE_ERROR_NONE, .charsWritten = 0 };
        prvWriteChars( ( int ) fd, str, len, &result );

        if( ( result.error == WRITE_ERROR_NONE ) && ( result.charsWritten == len ) )
        {
            return 0;
        }
        else if( result.error == WRITE_ERROR_SEND_FAIL )
        {
            return len - result.charsWritten;
        }
        else
        {
            return ( int ) result.error;
        }
    }

    int _sys_read( FILEHANDLE fd,
                   unsigned char * str,
                   unsigned int len,
                   int mode )
    {
        /* From <rt_sys.h>: `mode' exists for historical reasons and must be ignored. */
        ( void ) mode;

        /* Not implemented */
        ( void ) str;
        ( void ) len;
        return -1;
    }

    int _sys_istty( FILEHANDLE fh )
    {
        /* Not implemented */
        ( void ) fh;
        return 0;
    }

    long _sys_flen( FILEHANDLE fh )
    {
        /* Not implemented */
        ( void ) fh;
        return -1;
    }

    int _sys_seek( FILEHANDLE fh,
                   long offset )
    {
        /* Not implemented */
        ( void ) fh;
        ( void ) offset;
        return -1;
    }

#else /* !defined(__ARMCOMPILER_VERSION) */

/* Redirects gcc printf to UART0 */
    int _write( int fd,
                const unsigned char * str,
                int len )
    {
        WriteResult_t result = { .error = WRITE_ERROR_NONE, .charsWritten = 0 };

        prvWriteChars( fd, str, len, &result );

        return ( ( result.error == WRITE_ERROR_NONE ) && ( result.charsWritten == len ) ) ? result.charsWritten : -1;
    }

#endif /* if defined( __ARMCOMPILER_VERSION ) */

static bool prvValidFdHandle( int fd )
{
    return ( bool ) ( ( fd == STDOUT_FILENO ) || ( fd == STDERR_FILENO ) );
}

static void prvWriteChars( int fd,
                           const unsigned char * str,
                           unsigned int len,
                           WriteResult_t * result )
{
    result->charsWritten = 0;

    if( prvValidFdHandle( fd ) == false )
    {
        result->error = WRITE_ERROR_INVALID_ARGS;
        return;
    }

    if( xSemaphoreTake( xLoggingMutex, portMAX_DELAY ) != pdTRUE )
    {
        result->error = WRITE_ERROR_SYNC_FAILED;
        return;
    }

    bool allCharsWritten = ( bool ) ( Driver_USART0.Send( str, len ) == ARM_DRIVER_OK );

    while( Driver_USART0.GetTxCount() != len )
    {
    }

    ( void ) xSemaphoreGive( xLoggingMutex );

    if( allCharsWritten == true )
    {
        result->charsWritten = len;
        result->error = WRITE_ERROR_NONE;
    }
    else
    {
        result->error = WRITE_ERROR_SEND_FAIL;
    }
}
