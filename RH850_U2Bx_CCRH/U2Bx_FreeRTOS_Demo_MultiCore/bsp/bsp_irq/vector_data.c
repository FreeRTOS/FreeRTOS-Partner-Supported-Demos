#include "bsp_common.h"
#include "FreeRTOSConfig.h"

typedef void (* int_vector_t)(void);

// const int_vector_t g_vector_table_PE0[RTOS_VECTOR_TABLE_MAX_ENTRIES] =
// {
//     [360] = vPortTickISR,               /* INTOSTM0TINT (OSTM0 interrupt) */
//     [621] = r_Config_UART0_interrupt_send,
// };

// const int_vector_t g_vector_table_PEn[RTOS_VECTOR_TABLE_MAX_ENTRIES] =
// {
//     [360] = vPortTickISR,               /* INTOSTM0TINT (OSTM0 interrupt) */
//     [621] = r_Config_UART0_interrupt_send,
// };