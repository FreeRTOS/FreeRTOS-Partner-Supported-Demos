
# MPU PXN example on Armv8.1-M Cortex-M85 Fixed Virtual Platform
# Introduction
A new MPU region attribute Privileged eXecute Never (PXN) is introduced in Armv8.1-M architecture, where if an MPU region has PXN attribute set and the processor attempts to execute the code inside with privileged level, the Memory Management Fault exception would be triggered, with IACCVIOL bit in MemManage Fault State Register set to 1. The PXN feature allows privileged software to ensure specific application tasks (threads) to execute in unprivileged level only. For example, a hacker cannot use stack corruption in a privileged peripheral handler to branch into unprivileged codes and execute them with privileged level.
Please refer to the Security related enhancements section in Introduction to Armv8.1-M architecture white paper document for more information.

This example demonstrates how the new MPU region attribute Privileged eXecute Never (PXN) introduced in Armv8.1-M architecture can be used on Cortex-M85 MCU. The example is based on Corstone-310 Ecosystem Fixed Virtual Platform (Arm Cortex-M85 CPU and Ethos-U55 NPU). Follow the [link](https://www.arm.com/products/development-tools/simulation/fixed-virtual-platforms) to learn more about Arm fixed virtual platforms.

The example simulates a privileged task trying to execute an instruction from an MPU region with Privileged eXecute Never (PXN) attribute set, this result in a memory fault exception which recovers gracefully after clearing an entry in a fault tracker buffer indicating that the exception was triggered and recovered.

# Prerequisites

## Fixed Virtual Platform
### Downloading and installing Corstone-310 Ecosystem Fixed Virtual Platform
Follow the instructions on the [page](https://developer.arm.com/downloads/-/arm-ecosystem-fvps) to download Corstone-310 Ecosystem FVP based on your operating system.

Ensure that requirements mentioned in the [page](https://developer.arm.com/documentation/100966/1126/Getting-Started-with-Fixed-Virtual-Platforms/Requirements-for-FVPs?lang=en) are met.

Then, follow these instructions to install the Corstone-310 Ecosystem FVP
```bash
cd FVP_Corstone_SSE_310_11.xx_yy_<Linux/Win>64

./FVP_Corstone_SSE-310.sh

Do you want to proceed with the installation? [default: yes]
Yes.

Do you agree to the above terms and conditions?
Yes.

Where would you like to install to? [default: /home/<user>/FVP_Corstone_SSE-310]
Press Enter for the default installation location or specify the absolute path for the required location.

Installation is now completed.
```

Add the path to FVP_Corstone_SSE-310 executable to the environment variable `PATH` (if the default installation location was used, the executable path would be something like `/home/<user>/FVP_Corstone_SSE-310/v11.xx.yy/models/<Linux/Win>64_<armv8l/x86>_GCC-9.3/`).

Execute the following command to ensure that the Fixed Virtual Platform for Corstone-310 was installed successfully
```bash
FVP_Corstone_SSE-310 --version

Fast Models [11.xx.yy (month day year)]
Copyright 2000-2024 ARM Limited.
All Rights Reserved.
```

## Build tools
* [CMake](https://cmake.org/download/)
* [GNU Arm Embedded Toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)

# The MPU PXN example
The MPU PXN example uses `CMake` as the build system.

Run the following command to clone FreeRTOS repository:

```bash
git clone https://github.com/FreeRTOS/FreeRTOS.git --recurse-submodules
```

## Building and running the MPU PXN example
### Building the MPU PXN example
Run the following commands to build the MPU PXN example:

```bash
cd FreeRTOS/FreeRTOS/Demo/ThirdParty/Partner-Supported-Demos/CORTEX_M85_MPU_PXN_FVP_GCC
rm -rf build && cmake -B build --toolchain=gcc_toolchain.cmake . && cmake --build build
```

### Running the example
Execute the following script to run the MPU PXN example:
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

    Ethos-U rev 136b7d75 --- Apr 12 2023 13:47:17
    (C) COPYRIGHT 2019-2023 Arm Limited
    ALL RIGHTS RESERVED

 Exception recovered gracefully
 Exception recovered gracefully
 Exception recovered gracefully
^C
Stopping simulation...
```
