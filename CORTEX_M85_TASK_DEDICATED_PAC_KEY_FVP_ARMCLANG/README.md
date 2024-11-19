# Task Dedicated PAC Key example on Armv8.1-M Cortex-M85 Fixed Virtual Platform

# Introduction

The Armv8.1-M architecture extension introduced **Pointer Authentication** to harden the security against Return-Oriented Programming (ROP) security exploit attacks. In return-oriented programming, instructions before a function return are used build gadgets in the executable code region. Please refer to the [document](https://developer.arm.com/documentation/102433/0200/Stack-smashing-and-execution-permissions) to find out more about stack smashing and return-oriented programming. The [blog](https://community.arm.com/arm-community-blogs/b/architectures-and-processors-blog/posts/armv8-1-m-pointer-authentication-and-branch-target-identification-extension) talks in depth about Armv8.1-M Pointer Authentication Code.

To harden the security, each task is assigned with a dedicated PAC key, so that attackers needs to get the all the PAC keys right to exploit the system using Return Oriented Programming. The kernel is updated to support the following:

* A PAC key set with a user defined random number generator is generated and is pushed onto the task's stack when a task is created.
* As part of scheduling, the task's PAC key is stacked/unstacked to/from the task's stack when a task is unscheduled/scheduled from/to run.

This example demonstrates the soundness of the procedure used to stack/unstack the tasks' dedicated PAC key. The example is based on Corstone-315 Ecosystem Fixed Virtual Platform (Arm Cortex-M85 CPU and Ethos-U65 NPU). Follow the [link](https://www.arm.com/products/development-tools/simulation/fixed-virtual-platforms) to learn more about Arm fixed virtual platforms.

Please refer to the Security related enhancements section in the [Introduction to Armv8.1-M architecture white paper document](https://www.arm.com/resources/white-paper/intro-armv8-1-m-architecture) for more information.

# Prerequisites

## Downloading and installing Corstone-315 Ecosystem Fixed Virtual Platform

Follow the instructions on the [page](https://developer.arm.com/downloads/-/arm-ecosystem-fvps) to download Corstone-315 Ecosystem FVP based on your operating system. Ensure that requirements mentioned in the [page](https://developer.arm.com/documentation/100966/1126/Getting-Started-with-Fixed-Virtual-Platforms/Requirements-for-FVPs?lang=en) are met.

Then, follow these instructions to install the Corstone-315 Ecosystem FVP
```bash
cd FVP_Corstone_SSE_315_11.xx_yy_<Linux/Win>64

./FVP_Corstone_SSE-315.sh

Do you want to proceed with the installation? [default: yes]
Yes.

Do you agree to the above terms and conditions?
Yes.

Where would you like to install to? [default: /home/<user>/FVP_Corstone_SSE-315]
Press Enter for the default installation location or specify the absolute path for the required location.

Installation completed successfully
```

Add the path to `FVP_Corstone_SSE-315` executable to the environment variable `PATH` (if the default installation location was used, the executable path would be something like `/home/<user>/FVP_Corstone_SSE-315/models/<Linux/Win>64_<armv8l>_GCC-9.3/`).

Execute the following command to ensure that the Fixed Virtual Platform for Corstone-315 was installed successfully
```bash
FVP_Corstone_SSE-315 --version

Fast Models [11.xx.yy (month day year)]
Copyright 2000-2024 ARM Limited.
All Rights Reserved.
```

## Build tools

* [CMake](https://cmake.org/download/)
  * The Task Dedicated Pac Key example uses `CMake` as the build system.
* [Arm Compiler for Embedded Toolchain](https://developer.arm.com/Tools%20and%20Software/Arm%20Compiler%20for%20Embedded)
  * To use Arm Compiler For Embedded Toolchain, login is required for the download, and you will need a license in order to
run the toolchain once installed.

# Supported toolchains

The example is supported and tested on the following toolchain:

  * Arm Compiler for Embedded v6.22 (armclang).

# Configuration

The FreeRTOS-Kernel provides a CMake variable `FREERTOS_ARM_V_8_1_M_PACBTI_CONFIG` to configure the Armv8.1-M PACBTI security feature. The following values are supported:

`ARM_V_8_1_M_PACBTI_CONFIG_STANDARD`: PACBTI Security Feature Standard Configuration (PAC enabled without leaf functions support, and BTI enabled ).

`ARM_V_8_1_M_PACBTI_CONFIG_PACRET_LEAF_BTI`:  PACBTI Security Feature Standard + Leaf Configuration (PAC with leaf functions support, and BTI enabled).

`ARM_V_8_1_M_PACBTI_CONFIG_PACRET`: PACBTI Security Feature with only PAC enabled.

`ARM_V_8_1_M_PACBTI_CONFIG_PACRET_LEAF`: PACBTI Security Feature with PAC and PAC for leaf functions support enabled.

`ARM_V_8_1_M_PACBTI_CONFIG_BTI`: PACBTI Security Feature with only BTI enabled.

`ARM_V_8_1_M_PACBTI_CONFIG_NONE`: PACBTI Security Feature disabled.

It is recommend to set the intended value of `FREERTOS_ARM_V_8_1_M_PACBTI_CONFIG` in the example's top level `CMakeLists.txt`. In this example, `FREERTOS_ARM_V_8_1_M_PACBTI_CONFIG` is set to `ARM_V_8_1_M_PACBTI_CONFIG_PACRET` to enable Pointer Authentication Code.

All the CMake targets defined to construct the example shall have the respective compiler options based on the configured value of the `FREERTOS_ARM_V_8_1_M_PACBTI_CONFIG` variable. This is handled as part of `CMSIS/CMakeLists.txt` where the respective compiler options are added for the `arm-corstone-platform-bsp` target which is later linked to the example's target (i.e `cortex_m85_task_dedicated_pac_key_fvp_example`). These options are added for the `arm-corstone-platform-bsp` target, rather than being directly added for the example's target, to avoid duplicating the compiler options and definitions handling code for multiple targets.

# Implementation

The example consists of two main tasks (**prvFirstTask** and **prvSecondTask**). These tasks' job is to make sure that the task's randomly generated PAC keys are stacked/unstacked robustly without being altered.

* These tasks perform the following sequence:
    1. Fetch the randomly generated dedicated PAC key for each task once they start running.
    2. Move the task to blocked state by calling `vTaskDelay()` function.
    3. As part of the context switching handler, the task's PAC key is unstacked after the task is unblocked, where the value of the PAC key before and after the blockage should be the same to prove the soundess of the task's dedicated PAC key stacking/unstacking procedure.
    4. If the task's PAC key before and after the blockage are not the same, a `configASSERT()` statement would be triggered.

## Building and running examples

First, run the following command to clone FreeRTOS repository:

```bash
git clone https://github.com/FreeRTOS/FreeRTOS.git --recurse-submodules
```

Run the following commands to build the Task Dedicated Pac Key example:

```bash
cd FreeRTOS/FreeRTOS/Demo/ThirdParty/Partner-Supported-Demos/CORTEX_M85_TASK_DEDICATED_PAC_KEY_FVP_ARMCLANG
rm -rf build && cmake -B build --toolchain=armclang_toolchain.cmake . && cmake --build build
```

### Running the example

Execute the following script to run the Task Dedicated Pac Key example:
```bash
./run.sh
```

### Expected output

```bash
$ ./run.sh
telnetterminal0: Listening for serial connection on port 5000
telnetterminal1: Listening for serial connection on port 5001
telnetterminal2: Listening for serial connection on port 5002
telnetterminal5: Listening for serial connection on port 5003
WARNING: ISAR5.PACBTI parameter is deprecated, use parameter CFGPACBTI
WARNING: ISAR5.PACBTI parameter is deprecated, use parameter CFGPACBTI

    Ethos-U rev 136b7d75 --- Apr 12 2023 13:47:17
    (C) COPYRIGHT 2019-2023 Arm Limited
    ALL RIGHTS RESERVED


Info: Corstone_SSE_315_Main: Loading MaliC55Model

Info: Corstone_SSE_315_Main: MaliC55Model build info:
***
Version: C55-F5M9R1
Git id: unknown
Host OS: Linux-4.11.0-44.el7a.aarch64 aarch64
Compiler: GNU 9.3.0 /arm/devsys-tools/collections/gcc/9.3.0/1/linux_4.11-redhat_10.7-armv8l_64/gcc-4.8.5-SYSTEM/bin/c++
Build tool: /usr/bin/gmake (+ CMake 3.14.3)
Build type: Release
BUILD_SHARED_LIBS: OFF
Build timestamp: Sep 15 2023 12:41:43
RTL Version: r0p0-00eac0
Copyright (C) 2023 Arm Limited or its affiliates. All rights reserved.
***
In secure world main ...
In non-secure world main ...

PAC_KEY_P_0 for first Task is = 0x37f41ae

PAC_KEY_P_1 for first Task is = 0x52e87011

PAC_KEY_P_2 for first Task is = 0x709a7ea6

PAC_KEY_P_3 for first Task is = 0x11c04236

PAC_KEY_P_0 for second Task is = 0x4b86ce0e

PAC_KEY_P_1 for second Task is = 0x6516132

PAC_KEY_P_2 for second Task is = 0xde4b81

PAC_KEY_P_3 for second Task is = 0x94a10b8

Restored PAC_KEY_P_0 for the first Task is = 0x37f41ae

Restored PAC_KEY_P_1 for the first Task is = 0x52e87011

Restored PAC_KEY_P_2 for the first Task is = 0x709a7ea6

Restored PAC_KEY_P_3 for the first Task is = 0x11c04236

First task's PAC keys matched successfully. Hence, deleteing the task.

Restored PAC_KEY_P_0 for the second Task is = 0x4b86ce0e

Restored PAC_KEY_P_1 for the second Task is = 0x6516132

Restored PAC_KEY_P_2 for the second Task is = 0xde4b81

Restored PAC_KEY_P_3 for the second Task is = 0x94a10b8

Second task's PAC keys matched successfully. Hence, deleteing the task.
^C
Stopping simulation...
```
