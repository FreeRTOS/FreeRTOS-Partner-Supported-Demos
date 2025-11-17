# RH850 U2Ax FreeRTOS Partner Supported Demo with GHS Compiler

## Introduction
This directory contains demo project for Renesas RH850 U2A8 using the GHS compiler.<br />
This example implements the standard test demos detailed in following link: [RTOS Third Party Demo](https://github.com/FreeRTOS/FreeRTOS/blob/main/FreeRTOS/Demo/ThirdParty/Template/README.md).

## How to build and run the demo project
   1. The `FreeRTOS.bat` script is used to generate the output file.<br />
      Make sure the Green Hills compiler (`ghs`) is available in the system PATH.<br />
      If GHS is installed at `C:\ghs\comp_<version_number>`, update the **COMPILER_INSTALL_DIR** field in the `FreeRTOS.bat` file accordingly before running the script.
      Use the output file **obj\ghs\App_FreeRTOS.out** to start debugging in the GHS MULTI IDE.

   2. Select test case you want to run by changing definition `configSTART_<Test_Name>_TESTS` in `freertos_config\FreeRTOSConfig.h` macros to `0` or ` 1` as needed.<br />
      Add breakpoint at line `vTaskSuspend(NULL);` in `prvCheckTask` function of file `TestRunner.c` to check the result.<br />
      Please check the results in the "I/O" tab.<br />
      ![IO Result](../Image/IO_Result.png)

## Note
   1. The configuration `configRUN_MULTIPLE_PRIORITIES` must be set to `0` to PASS the following test cases:<br />
    - `configSTART_GENERIC_QUEUE_TESTS` = `1`<br />
    - `configSTART_PEEK_QUEUE_TESTS` = `1`<br />
    - `configSTART_RECURSIVE_MUTEX_TESTS` = `1`<br />
    - `configSTART_EVENT_GROUP_TESTS` = `1`<br />
    - `configSTART_INTERRUPT_SEMAPHORE_TESTS` = `1`<br />
    - `configSTART_BLOCK_TIME_TESTS` = `1`<br />
    - `configSTART_TIMER_TESTS` = `1`

   2. For more information on setting up the vector table and configuring the interrupt vector method, disabling the FPU Unit when not in use, and configuring interrupts, please refer to the [README.md](https://github.com/FreeRTOS/FreeRTOS-Kernel-Partner-Supported-Ports/blob/main/GHS/U2x/README.md) file included with the Port files.<br />
      It provides helpful background information on system initialization and hardware-specific settings.
