/*
 * FreeRTOS Kernel V11.2.0
 * Copyright (C) 2024 Amazon.com, Inc. or its affiliates. All Rights Reserved.
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
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

#ifndef FREERTOSCONFIG_H
#define FREERTOSCONFIG_H

#include "FreeRTOSConfigMultiCore.h"

#define configTASK_NOTIFICATION_ARRAY_ENTRIES    3
#ifdef configUSE_TICK_HOOK
    #undef configUSE_TICK_HOOK
#endif
#define configUSE_TICK_HOOK                          1
#define configPRINTF( X )    printf X

#define configSTART_TASK_NOTIFY_TESTS                1
#define configSTART_TASK_NOTIFY_ARRAY_TESTS          1
#define configSTART_BLOCKING_QUEUE_TESTS             1
#define configSTART_SEMAPHORE_TESTS                  0
#define configSTART_POLLED_QUEUE_TESTS               0
#define configSTART_GENERIC_QUEUE_TESTS              0 /* configRUN_MULTIPLE_PRIORITIES = 0 */
#define configSTART_PEEK_QUEUE_TESTS                 0 /* configRUN_MULTIPLE_PRIORITIES = 0 */
#define configSTART_INTEGER_MATH_TESTS               0
#define configSTART_MATH_TESTS                       0
#define configSTART_RECURSIVE_MUTEX_TESTS            0 /* configRUN_MULTIPLE_PRIORITIES = 0 */
#define configSTART_COUNTING_SEMAPHORE_TESTS         0
#define configSTART_QUEUE_SET_TESTS                  0
#define configSTART_QUEUE_OVERWRITE_TESTS            0
#define configSTART_EVENT_GROUP_TESTS                0 /* configRUN_MULTIPLE_PRIORITIES = 0 */
#define configSTART_INTERRUPT_SEMAPHORE_TESTS        0 /* configRUN_MULTIPLE_PRIORITIES = 0 */
#define configSTART_QUEUE_SET_POLLING_TESTS          0
#define configSTART_BLOCK_TIME_TESTS                 0 /* configRUN_MULTIPLE_PRIORITIES = 0 */
#define configSTART_ABORT_DELAY_TESTS                0
#define configSTART_MESSAGE_BUFFER_TESTS             0
#define configSTART_STREAM_BUFFER_TESTS              0
#define configSTART_STREAM_BUFFER_INTERRUPT_TESTS    0
#define configSTART_TIMER_TESTS                      0 /* configRUN_MULTIPLE_PRIORITIES = 0 */
#define configSTART_INTERRUPT_QUEUE_TESTS            0 /* Not supported */
#define configSTART_REGISTER_TESTS                   0
#define configSTART_DELETE_SELF_TESTS                1

#endif /* FREERTOSCONFIG_H */
