# RH850 U2Bx FreeRTOS Community Supported Demo

## Introduction
This directory contains demo project for Renesas RH850 U2B10

This example implements the standard test demos detailed in following link: [RTOS Third Party Demo](https://github.com/FreeRTOS/FreeRTOS/blob/main/FreeRTOS/Demo/ThirdParty/Template/README.md)

## Note
1. The configuration `configRUN_MULTIPLE_PRIORITIES` must be set to `0` to PASS the following test cases:
- `configSTART_GENERIC_QUEUE_TESTS` = `1`
- `configSTART_PEEK_QUEUE_TESTS` = `1`
- `configSTART_RECURSIVE_MUTEX_TESTS` = `1`
- `configSTART_EVENT_GROUP_TESTS` = `1`
- `configSTART_INTERRUPT_SEMAPHORE_TESTS` = `1`
- `configSTART_TIMER_TESTS` = `1`
2. The test case `configSTART_INTERRUPT_QUEUE_TESTS` are out of scope.
3. For other test cases, please use default configuraton (`configRUN_MULTIPLE_PRIORITIES` = `1`)