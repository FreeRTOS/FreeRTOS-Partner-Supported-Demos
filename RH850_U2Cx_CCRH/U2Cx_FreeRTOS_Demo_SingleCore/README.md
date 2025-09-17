# RH850 U2Cx FreeRTOS Community Supported Demo

## Introduction
This directory contains demo project for Renesas RH850 U2C8

This example implements the standard test demos detailed in following link: [RTOS Third Party Demo](https://github.com/FreeRTOS/FreeRTOS/blob/main/FreeRTOS/Demo/ThirdParty/Template/README.md)

## IDE
- [CS+ support RH850 family](https://www.renesas.com/en/software-tool/cs?srsltid=AfmBOoqK5LDXK_CY45rHXBlWg4XojnYUopLwSC9DjUXsDYs4pa0oTZke#downloads)

## How to use
Open CS+ and import this project File->Open.

Select test case you want to run by changing definition `configSTART_<Test_Name>_TESTS` in `freertos\FreeRTOSConfig.h` macros to `0` or ` 1` as needed.

Add breakpoint at line `configPRINTF(("%s \r\n", pcStatusMessage));` in `prvCheckTask` function in file `TestRunner.c` to check the result.

## Linker

This project also support other single core devices by changing the linker.

## Linker Script
Add below linker script to your linker section
```c
RESET,STARTUP_CODE_PE0,STARTUP_CODE_PEn,STARTUP_CODE/00000000,ex_entry_PE0/00000800,ex_entry_PE1,.const,.INIT_BSEC.const,.INIT_DSEC.const,.data,.text/00005000,ex_entry_PE2/00035000,.stack_pe2.bss/FD800000,.stack_pe1.bss/FDA00000,.stack.bss,.data.R,.bss/FDC00000,.mev_address.bss/FE000000
```
![Linker section](../Image//Linker.png)


## Note
The test case for `configSTART_INTERRUPT_QUEUE_TESTS` is out of scope.