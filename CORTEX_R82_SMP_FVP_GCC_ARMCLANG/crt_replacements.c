/* Copyright 2025 Arm Limited and/or its affiliates <open-source-office@arm.com>
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#if defined( __GNUC__ ) && !defined( __clang__ )
    #include <reent.h>
    #include <sys/stat.h>
#endif /* if defined( __GNUC__ ) && !defined( __clang__ ) */

int fputc( int character, FILE *pxFile )
{
    ( void )pxFile;  // Unused parameter as required by the C standard

    register uint64_t ulSysWriteCode __asm__( "x0" ) = 0x03;        // SYS_WRITEC: semihosting write character command
    register const char *pcCharAddress __asm__( "x1" ) = ( const char * )&character; // Address of character to send
    __asm volatile (
        "hlt #0xF000\n"  // Issue semihosting call using HLT instruction
        : "+r"( ulSysWriteCode )
        : "r"( pcCharAddress )
        : "memory"
    );
    return character;
}

int puts( const char *pcString )
{
    const char *pcChar = pcString;
    while ( *pcChar ) {
        fputc( *pcChar++, stdout );
    }
    fputc( '\n', stdout );  // Append newline to the output string
    return 0;  // Standard puts() returns non-negative on success
}

#if defined( __GNUC__ ) && !defined( __clang__ )
    /* Non-re-entrant version provided by RDIMON. */
    extern int _fstat ( int /*fd*/, struct stat * /*buf*/ );

    /* ------------------------------------------------------------------------- */
    int _fstat_r ( struct _reent *pxReent, int fd, struct stat *pxStatBuffer )
    {
        // Create a 16-byte aligned temporary buffer for the stat structure needed by semihosting.
        struct stat xAlignedStat __attribute__( ( aligned ( 16 ) ) );

        int iFstatResult = _fstat( fd, &xAlignedStat );  // Perform semihosting call to get file status

        if ( iFstatResult == 0 ) {  // Success: copy stat data safely
            memcpy( pxStatBuffer, &xAlignedStat, sizeof( struct stat ) );
        }
        else {  // Failure: set error code from errno
            pxReent->_errno = errno;
        }
        return iFstatResult;
    }

    /* Helper: Returns true if the provided pointer is naturally 8-byte aligned. */
    static inline int prvIsEightByteAligned( const void *pvAddr )
    {
        return ( ( ( uintptr_t )pvAddr ) & 0x7U ) == 0U;  // Check alignment to an 8-byte boundary
    }

    void *memmove( void *pvDestination, const void *pvSource, size_t xCount )
    {
        unsigned char *pucDest = ( unsigned char * )pvDestination;
        const unsigned char *pucSrc = ( const unsigned char * )pvSource;

        if ( pucDest == pucSrc || xCount == 0 )
            return pvDestination;

        if ( pucDest < pucSrc ) {  // -------- forward copy --------
            if ( prvIsEightByteAligned( pucDest ) && prvIsEightByteAligned( pucSrc ) ) {
                while ( xCount >= 8 ) {
                    *( uint64_t * )pucDest = *( const uint64_t * )pucSrc;
                    pucDest += 8;
                    pucSrc += 8;
                    xCount -= 8;
                }
            }
            while ( xCount-- ) {  // Copy remaining bytes forward
                *pucDest++ = *pucSrc++;
            }
        } else {  // -------- backward copy --------
            pucDest += xCount;
            pucSrc += xCount;
            if ( prvIsEightByteAligned( pucDest ) && prvIsEightByteAligned( pucSrc ) ) {
                while ( xCount >= 8 ) {
                    pucDest -= 8;
                    pucSrc -= 8;
                    xCount -= 8;
                    *( uint64_t * )pucDest = *( const uint64_t * )pucSrc;
                }
            }
            while ( xCount-- ) {  // Copy remaining bytes in reverse order
                *--pucDest = *--pucSrc;
            }
        }
        return pvDestination;
    }

    /* memcpy() may assume no overlap, so alias to memmove() */
    void *memcpy( void *pvDestination, const void *pvSource, size_t xCount )
    {
        return memmove( pvDestination, pvSource, xCount );
    }

    /* Replacement for memset – linker will rename all calls to this symbol. */
    void *__wrap_memset( void *pvDestination, int value, size_t xCount )
    {
        unsigned char *pucDest   = ( unsigned char * )pvDestination;
        unsigned char ucValue  = ( unsigned char )value;
        volatile size_t xCountNumber = xCount;

        /* Fast 8-byte stores when destination is naturally aligned. */
        if ( prvIsEightByteAligned( pucDest ) )
        {
            uint64_t ullEightBytePattern = 0x0101010101010101ULL * ucValue;  // Prepare an 8-byte pattern with all bytes set to ucValue

            while ( xCountNumber >= 8 )
            {
                *( uint64_t * )pucDest = ullEightBytePattern;
                pucDest += 8;
                xCountNumber -= 8;
            }
        }

        /* Copy any remaining bytes (or if fully unaligned). */
        while ( xCountNumber-- > 0 )
            *pucDest++ = ucValue;

        return pvDestination;
    }
#endif /* if defined( __GNUC__ ) && !defined( __clang__ ) */
