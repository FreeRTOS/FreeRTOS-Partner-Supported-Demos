#include "FreeRTOSConfig.h"
#include "vector_data.h"

// typedef void (* int_vector_t)( void );

const int_vector_t g_vector_table_PE0[ RTOS_VECTOR_TABLE_MAX_ENTRIES ] =
{
    #if ( configNUMBER_OF_CORES > 1 )
        [ 1 ] = vPortIPIHander, /* Internal processor interrupt 0 */
        [ 10 ] = vCH0_TAUD0_Handler0,
    #endif
    #if ( configDEVICE_NAME == U2Bx_DEVICES )
        [ 360 ] = vPortTickISR, /* INTOSTM0TINT (OSTM0 interrupt) */
    #elif ( configDEVICE_NAME == U2Ax_DEVICES )
        [ 200 ] = vPortTickISR, /* INTOSTM1TINT (OSTM1 interrupt) */
    #elif ( configDEVICE_NAME == U2Cx_DEVICES )
        [ 63 ] = vPortTickISR, /* INTOSTM0TINT (OSTM0 interrupt) */
    #endif
};

const int_vector_t g_vector_table_PEn[ RTOS_VECTOR_TABLE_MAX_ENTRIES ] =
{
    #if ( configNUMBER_OF_CORES > 1 )
        [ 1 ] = vPortIPIHander, /* Internal processor interrupt 0 */
        [ 11 ] = vCH2_TAUD0_Handler1
    #else
        [ 0 ] = vDummyISR,
                 #endif
};
