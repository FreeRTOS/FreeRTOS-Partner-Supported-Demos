# RH850 U2Ax FreeRTOS Partner Supported Demo

## Introduction
This directory contains demo project for Renesas RH850 U2A8

This example implements the standard test demos detailed in following link: [RTOS Third Party Demo](https://github.com/FreeRTOS/FreeRTOS/blob/main/FreeRTOS/Demo/ThirdParty/Template/README.md)

## IDE
- [CS+ suport RH850 family](https://www.renesas.com/en/software-tool/cs?srsltid=AfmBOoqK5LDXK_CY45rHXBlWg4XojnYUopLwSC9DjUXsDYs4pa0oTZke#downloads)

## How to use
Open CS+ and import this project File->Open

Select test case you want to run by changing definition `configSTART_<Test_Name>_TESTS` in `freertos\FreeRTOSConfig.h` macros to `0` or ` 1` as needed

Add breakpoint at line `configPRINTF(("%s \r\n", pcStatusMessage));` in `prvCheckTask` function in file `TestRunner.c` to check the result.

## Linker Script
Add below linker script to your linker section
```c
RESET,STARTUP_CODE_PE0,STARTUP_CODE_PEn,STARTUP_CODE/00000000,ex_entry_PE0/00000800,ex_entry_PE1,.const,.INIT_BSEC.const,.INIT_DSEC.const,.data,.text/00005000,ex_entry_PE2/00035000,.stack_pe2.bss/FD800000,.stack_pe1.bss/FDA00000,.stack.bss,.data.R,.bss/FDC00000,.mev_address.bss/FE000000
```
![Linker section](../Image//Linker.png)

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