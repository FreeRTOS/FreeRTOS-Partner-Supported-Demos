# MPU PXN, and PACBTI example on Armv8.1-M Cortex-M85 Fixed Virtual Platform

# Introduction

The Armv8.1-M architecture extension introduced **Pointer Authentication** and **Branch Target Identification (PACBTI)** to harden the security against Return-Oriented Programming (ROP) and Jump-Oriented Programming (JOP) security exploit attacks. In return-oriented programming, instructions before a function return are used build gadgets in the executable code region. Similarly, in jump-oriented programming, instructions before function calls or switch-case statements (indirect jumping) are used to build gadgets in the executable code region. Please refer to the [document](https://developer.arm.com/documentation/102433/0200/Stack-smashing-and-execution-permissions) to find out more about stack smashing, return-oriented programming and jump-oriented programming. The [blog](https://community.arm.com/arm-community-blogs/b/architectures-and-processors-blog/posts/armv8-1-m-pointer-authentication-and-branch-target-identification-extension) talks in depth about Armv8.1-M PACBTI.

Also, new MPU region attribute **Privileged eXecute Never (PXN)** is introduced in Armv8.1-M architecture extension, where if an MPU region has PXN attribute set and the processor attempts to execute the code inside with privileged level, the Memory Management Fault exception would be triggered, with IACCVIOL bit in MemManage Fault State Register set to 1. The PXN feature allows privileged software to ensure specific application tasks (threads) to execute in unprivileged level only. For example, a hacker cannot use stack corruption in a privileged peripheral handler to branch into unprivileged codes and execute them with privileged level.

Please refer to the Security related enhancements section in the [Introduction to Armv8.1-M architecture white paper document](https://www.arm.com/resources/white-paper/intro-armv8-1-m-architecture) for more information.

This example demonstrates how the new **Pointer Authentication** and **Branch Target Identification (PACBTI)** along with the new MPU region attribute **Privileged eXecute Never (PXN)** security features can be used on Cortex-M85 processor to harden the security. It encompasses [TrustedFirmware-M](https://www.trustedfirmware.org/projects/tf-m/) running on the secure side of the Armv8.1-M processor, and the FreeRTOS-Kernel along with the application's tasks running on the non-secure side of the Armv8.1-M processor. The example is based on Corstone-315 Ecosystem Fixed Virtual Platform (Arm Cortex-M85 CPU and Ethos-U65 NPU). Follow the [link](https://www.arm.com/products/development-tools/simulation/fixed-virtual-platforms) to learn more about Arm fixed virtual platforms.

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
  * The MPU PXN PACBTI example uses `CMake` as the build system.
* [Arm Compiler for Embedded Toolchain](https://developer.arm.com/Tools%20and%20Software/Arm%20Compiler%20for%20Embedded)
  * To use Arm Compiler For Embedded Toolchain, login is required for the download, and you will need a license in order to
run the toolchain once installed.

# Supported toolchains

The example is supported and tested on the following toolchains:

  * Arm Compiler for Embedded v6.21 (armclang).
  * IAR toolchain for Arm v9.60.2 (IARARM).

# Configuration

The FreeRTOS-Kernel provides a CMake variable `FREERTOS_ARM_V_8_1_M_PACBTI_CONFIG` to configure the Armv8.1-M PACBTI security feature. The following values are supported:

`ARM_V_8_1_M_PACBTI_CONFIG_STANDARD`: PACBTI Security Feature Standard Configuration (PAC enabled without leaf functions support, and BTI enabled ).

`ARM_V_8_1_M_PACBTI_CONFIG_PACRET_LEAF_BTI`:  PACBTI Security Feature Standard + Leaf Configuration (PAC with leaf functions support, and BTI enabled).

`ARM_V_8_1_M_PACBTI_CONFIG_PACRET`: PACBTI Security Feature with only PAC enabled.

`ARM_V_8_1_M_PACBTI_CONFIG_PACRET_LEAF`: PACBTI Security Feature with PAC and PAC for leaf functions support enabled.

`ARM_V_8_1_M_PACBTI_CONFIG_BTI`: PACBTI Security Feature with only BTI enabled.

`ARM_V_8_1_M_PACBTI_CONFIG_NONE`: PACBTI Security Feature disabled.

It is recommend to set the intended value of `FREERTOS_ARM_V_8_1_M_PACBTI_CONFIG` in the example's top level `CMakeLists.txt`. In this example, `FREERTOS_ARM_V_8_1_M_PACBTI_CONFIG` is set to `ARM_V_8_1_M_PACBTI_CONFIG_STANDARD`, to enable both PAC and BTI, to harden the security against **ROP (Return Oriented Programming)** and **JOP (Jump Oriented Programming)** security exploits.

All the CMake targets defined to construct the example shall have the respective compiler options and definitions based on the configured value of the `FREERTOS_ARM_V_8_1_M_PACBTI_CONFIG` variable. This is handled as part of `CMSIS/CMakeLists.txt` where the respective compiler options and definitions are added for the `arm-corstone-platform-bsp` target which is later linked to the example's target (i.e `cortex_m85_mpu_pxn_pacbti_fvp_example`). These options and defnitions are added for the `arm-corstone-platform-bsp` target, rather than being directly added for the example's target, to avoid duplicating the compiler options and definitions handling code for multiple targets.

# Implementation

The example consists of three main tasks (**prvPacTask** , **prvBtiTask**, and **prvPxnTask**). The **prvPacTask** and **prvBtiTask** tasks' job is to trigger the **UsageFault** exception by violating PAC and BTI rules. The privileged **prvPxnTask**'s job is to trigger the **MemoryFault** exception by executing an instruction from an MPU region with Privileged eXecute Never (PXN) attribute set.

## Pointer Authentication Test

The **prvPacTask** FreeRTOS task would call one of the application's functions (`prvCorruptLR`) which will change the stacked **Link Register (LR)** value before validating the Pointer Authentication Code at the end of the called function, this will lead to failure of **aut (authentication instruction)** resulting in **UsageFault** exception being triggered. The exception handler is designed in a way to recover gracefully by checking if the exception was triggered intentionally. After this, the task is killed.

## Branch Target Identification Test

The **prvBtiTask** FreeRTOS task will attempt to jump to the middle of one of the application's functions (`prvHandleUsageFault`) where this will result in **UsageFault** exception being triggered as the address jumped to is not a **BTI clearing** instruction. The exception handler is designed in a way to recover gracefully by checking if the exception was triggered intentionally. After this, the task is killed.

## Privileged Execute Never Test

The privileged FreeRTOS task (i.e. **prvPxnTask**) will attempt to execute an instruction from an MPU region with Privileged eXecute Never (PXN) attribute set, this will result in **MemoryFault** exception being triggered as the privileged tasks are not allowed to execute instructions located in Privileged eXecute Never MPU regions. The exception handler is designed in a way to recover gracefully by checking if the exception was triggered intentionally. After this, the task is killed.

## Building and running examples

First, run the following command to clone FreeRTOS repository:

```bash
git clone https://github.com/FreeRTOS/FreeRTOS.git --recurse-submodules
```

Run the following commands to build the MPU PXN PACBTI example:

```bash
cd FreeRTOS/FreeRTOS/Demo/ThirdParty/Partner-Supported-Demos/CORTEX_M85_MPU_PXN_PACBTI_FVP_ARMCLANG_IAR
rm -rf build && cmake -B build --toolchain=<armclang/iar>_toolchain.cmake . && cmake --build build
```

### Running the example

Execute the following script to run the MPU PXN PACBTI example:
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

    Ethos-U rev 136b7d75 --- Apr 12 2023 13:44:01
    (C) COPYRIGHT 2019-2023 Arm Limited
    ALL RIGHTS RESERVED


Info: Corstone_SSE_315_Main: Loading MaliC55Model

Info: Corstone_SSE_315_Main: MaliC55Model build info:
***
Version: C55-F5M9R1
Git id: unknown
Host OS: Linux-3.10.0-693.21.1.el7.x86_64 x86_64
Compiler: GNU 9.3.0 /arm/eda/tools/dsystool/gcc/9.3.0/1/linux_3.10-redhat_10.7-x86_64/gcc-4.8.5-SYSTEM/bin/c++
Build tool: /usr/bin/gmake (+ CMake 3.14.3)
Build type: Release
BUILD_SHARED_LIBS: OFF
FAST_CPU_MODE: ON
Build timestamp: Sep 15 2023 12:39:08
RTL Version: r0p0-00eac0
Copyright (C) 2023 Arm Limited or its affiliates. All rights reserved.
***
[INF] Starting bootloader
[INF] PSA Crypto init done, sig_type: EC-P256, using builtin keys
[INF] Image index: 1, Swap type: none
[INF] Image index: 0, Swap type: none
[INF] Bootloader chainload address offset: 0x0
[INF] Image version: v2.1.0
[INF] Jumping to the first image slot
Booting TF-M v2.1.0+deda2ede3
<NUL>[Sec Thread] Secure image initializing!
<NUL>PSA Framework version is: 257

Attempting to corrupt Link Register (LR).

Usage Fault Exception triggered due to incorrect computed PAC, was recovered gracefully.

Attempting to perform an indirect jump to middle of prvHandleUsageFault function.

Usage Fault Exception triggered due to middle of function jump, was recovered gracefully.

Attempting to execute a privileged execute never function.

Memory Fault Exception triggered due to unpermitted privleged execution, was recovered gracefully.

All tasks have successfully executed, deleting existing tasks.
^C
Stopping simulation...
```
