# Overview

This directory contains a demo project for AM64x running in A53 (GCC compiler)
using FreeRTOS SMP kernel (running on dual core A53).


# Initial Setup
## Compiler installation
- This port uses GCC AArch64 compiler for building binaries. The download link for
the same is below.
    - [Linux](https://developer.arm.com/-/media/Files/downloads/gnu-a/9.2-2019.12/binrel/gcc-arm-9.2-2019.12-x86_64-aarch64-none-elf.tar.xz)
    - [Windows](https://developer.arm.com/-/media/Files/downloads/gnu-rm/7-2017q4/gcc-arm-none-eabi-7-2017-q4-major-win32.zip)
- The compiler installation path needs to be set correctly in `imports.mak` file

## CCS (Code composer studio installation)
- CCS can be used for loading binaries and debugging the program. The download link
for CCS is below.
    - [Linux](https://dr-download.ti.com/software-development/ide-configuration-compiler-or-debugger/MD-J1VdearkvK/11.2.0.00007/CCS11.2.0.00007_linux-x64.tar.gz)
    - [Windows](https://dr-download.ti.com/software-development/ide-configuration-compiler-or-debugger/MD-J1VdearkvK/11.2.0.00007/CCS11.2.0.00007_win64.zip)
- CCS installation will also be used for consuming cygwin for building in windows environment
- The CCS install path needs to set correctly in `imports.mak` file

## NodeJs
- NodeJS needs to be installed as it is used for appimage generation.
- NodeJS can be downloaded from the link below
    - [Linux](https://nodejs.org/dist/latest-v18.x/node-v12.18.4-linux-x64.tar.xz)
    - [Windows](https://nodejs.org/dist/latest-v18.x/node-v18.12.1-x64.msi)
- The NodeJs path needs to be set correctly in `imports.mak` file

# Test Demo

## Folder structure
- ``` bash
  |--- drivers         -> Drivers required to run the example (UART, pinmux, sciclient)
  |--- examples
  |     |--- test_demo     -> Demo example source code
  |--- kernel          -> Kernel source (startup code and driver porting layer)
  |     |--- config
  |           |--- FreeRTOSConfig.h     --> FreeRTOS configuration file
  |--- tools
  ```

## test_demo
- This demo is a demonstration of various freeRTOS features in AM64x Armv8 dual core A53
  using the FreeRTOS SMP (symmetric multiprocessing) kernel.
- The demo creates various tasks in the file **TestRunner.c** and a monitoring task to
  check if the tasks are running as expected.
- The test tasks are created as per the macros set in file **TestIncludes.h**

## Building the test Demo
- Before building the demo binary the dependent libraries needs to be build.
    - The makefile for kernel library is in the kernel folder (**makefile.am64x.a53.freertos-smp**)
    - The makefile for drivers library is in the drivers folder (**makefile.am64x.a53.gcc-aarch64**)
- After building the libraries the demo can be build.
    - The makefile for building teh example is at examples/test_demo.

## Running the test Demo
- For setting up the board and flashing and running the example refer to
  https://software-dl.ti.com/mcu-plus-sdk/esd/AM64X/08_04_00_17/exports/docs/api_guide_am64x/EVM_SETUP_PAGE.html
- For more information on debugging SMP mode from CCS refer to
    - https://software-dl.ti.com/mcu-plus-sdk/esd/AM64X/08_04_00_17/exports/docs/api_guide_am64x/SMP_FREERTOS_GUIDE.html
    - https://software-dl.ti.com/ccs/esd/documents/ccs_smp-debug.html

# References

- Product link for AM64x - https://www.ti.com/product/AM6442
- AM64x Data sheet - https://www.ti.com/lit/gpn/am6421
- AM64x Technical Reference manual - https://www.ti.com/lit/gpn/am6421
- Code Composer studio - https://www.ti.com/tool/CCSTUDIO
- MCU+SDK (TI SDK with Baremetal and RTOS support) - https://www.ti.com/tool/download/MCU-PLUS-SDK-AM64X/08.04.00.17
    - User needs to install this SDK to build and run bootloader for the demo applications