/*===========================================================================*/

/* File name: vector_data.h                                                  */
/*===========================================================================*/
/* Environment:                                                              */
/*              IDE:            GHS Multi V7.1.4 or later                    */
/*===========================================================================*/

#ifndef BSP_IRQ_VECTOR_DATA_H
#define BSP_IRQ_VECTOR_DATA_H

#define RTOS_VECTOR_TABLE_MAX_ENTRIES    ( 1024 )

typedef void (* int_vector_t)( void );

extern void vPortTickISR( void );

extern void vDummyISR( void );

#endif /* BSP_IRQ_VECTOR_DATA_H */
