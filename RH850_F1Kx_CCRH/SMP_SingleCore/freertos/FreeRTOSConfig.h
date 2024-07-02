/*
 * FreeRTOSConfig.h
 *
 *  Created on: Jun 25, 2024
 *      Author: tin.nguyen-van
 */

#ifndef FREERTOSCONFIG_H_
#define FREERTOSCONFIG_H_

#include "FreeRTOSConfigSingleCore.h"
/*#include "FreeRTOSConfigMultiCore.h" */


#define configTASK_NOTIFICATION_ARRAY_ENTRIES    3
#ifdef configUSE_TICK_HOOK
    #undef configUSE_TICK_HOOK
#endif
#define configUSE_TICK_HOOK                          1
#define configPRINTF( X )    printf X

#define configSTART_TASK_NOTIFY_TESTS                0
#define configSTART_TASK_NOTIFY_ARRAY_TESTS          0
#define configSTART_BLOCKING_QUEUE_TESTS             0
#define configSTART_SEMAPHORE_TESTS                  0
#define configSTART_POLLED_QUEUE_TESTS               0
#define configSTART_GENERIC_QUEUE_TESTS              0
#define configSTART_PEEK_QUEUE_TESTS                 0
#define configSTART_INTEGER_MATH_TESTS               0
#define configSTART_MATH_TESTS                       0
#define configSTART_RECURSIVE_MUTEX_TESTS            0
#define configSTART_COUNTING_SEMAPHORE_TESTS         0
#define configSTART_QUEUE_SET_TESTS                  0
#define configSTART_QUEUE_OVERWRITE_TESTS            0
#define configSTART_EVENT_GROUP_TESTS                0
#define configSTART_INTERRUPT_SEMAPHORE_TESTS        0
#define configSTART_QUEUE_SET_POLLING_TESTS          0
#define configSTART_BLOCK_TIME_TESTS                 0
#define configSTART_ABORT_DELAY_TESTS                0
#define configSTART_MESSAGE_BUFFER_TESTS             0
#define configSTART_STREAM_BUFFER_TESTS              0
#define configSTART_STREAM_BUFFER_INTERRUPT_TESTS    0
#define configSTART_TIMER_TESTS                      1
#define configSTART_REGISTER_TESTS                   1
#define configSTART_DELETE_SELF_TESTS                1

#endif /* FREERTOSCONFIG_H_ */
