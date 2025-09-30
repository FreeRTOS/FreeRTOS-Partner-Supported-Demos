# Arm Cortex-R82 SMP Example on FVP_BaseR_AEMv8R

# Introduction

This example shows how to bring-up **FreeRTOS-SMP** on an **Arm Cortex-R82** multiprocessor system using the **BaseR AEMv8R** Architecture Envelope Model (AEM) Fixed Virtual Platform (FVP).

Two FreeRTOS tasks, each pinned to a different core, exchange a shared flag and print **Ping / Pong** messages to the console to prove that:

	• Both cores are utilised by the scheduler.
	• Inter-core communication through a fully cache-coherent RAM works.

The application is intentionally minimal so that it can serve as a starting point for larger SMP applications on Cortex-R82.

# Prerequisites

## Downloading and installing AEMv8R Architecture Envelope Model (AEM) Fixed Virtual Platform

Follow the instructions on the [page](https://developer.arm.com/Tools%20and%20Software/Fixed%20Virtual%20Platforms/Arm%20Architecture%20FVPs) to download FVP_Base_AEMv8R based on your operating system.

Ensure that requirements mentioned in the [page](https://developer.arm.com/documentation/100966/1126/Getting-Started-with-Fixed-Virtual-Platforms/Requirements-for-FVPs?lang=en) are met.

Then, add the path to `FVP_BaseR_AEMv8R` executable to the environment variable `PATH` (the executable path would be something like `/home/<user>/AEMv8R_base_pkg/models/<Linux/Win>64_<armv8l>_GCC-9.3/`).

Execute the following command to ensure that the FVP was installed successfully
```bash
FVP_BaseR_AEMv8R --version

Fast Models [11.xx.yy (MMM DD YYYY)]
Copyright 2000-2025 ARM Limited.
All Rights Reserved.
```

## Build tools

* [CMake](https://cmake.org/download/)
  * The Arm Cortex-R82 SMP example uses `CMake` as the build system.
* [Arm GNU Toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads) or [Arm Compiler for Embedded](https://developer.arm.com/Tools%20and%20Software/Arm%20Compiler%20for%20Embedded)
  * To use Arm Compiler For Embedded, login is required for the download, and you will need a license in order to run the toolchain once installed.

# Supported toolchains

The example is supported and tested on the following toolchains:

  * Arm Compiler for Embedded v6.23 (armclang).
  * Arm GNU toolchain v14.2.

# Supported FVPs

The example is supported and tested on FVP_BaseR_AEMv8R Fast Models [11.28.23 (Feb 17 2025)]

## Cache Coherency

This example relies on the `FVP_BaseR_AEMv8R` implementing fully coherent caches. Shared-memory communication (for example, the `ulSharedFlag`) does not require explicit cache clean/invalidate operations; only barrier instructions are used to enforce ordering and visibility. If run on hardware or models without full cache coherency, additional software cache maintenance or non-cacheable mappings would be required for correctness.

# Implementation

| Item | Description |
|------|-------------|
| **Shared flag** | `ulSharedFlag` (64-bit) is cache-coherent across cores. |
| **Tasks** | `prvTaskCore0` (core 0) prints **Ping**, sets flag to 1; `prvTaskCore1` (core 1) prints **Pong**, sets flag to 0. Each task delays for 1 s (`vTaskDelay( pdMS_TO_TICKS(1000) )`). |
| **Core affinity** | After creation, tasks are pinned via `vTaskCoreAffinitySet()` to ensure deterministic execution. |
| **Scheduler bring-up** | Only primary Core (i.e., Core 0) jumps to main, creates the user tasks, and calls `vTaskStartScheduler()`. Each secondary core starts and does all the its core specific initialisation and spin in `wfe` until `ucPrimaryCoreInitDoneFlag` is set to `1`, initialize the GIC redistributor and enable SGIs so interrupts from the primary core are receivable, signal the primary core that this secondary core is online and ready by setting the its flag in the `ucSecondaryCoresReadyFlags` array, finally issues an SVC with immediate value `106` to enter `FreeRTOS_SWI_Handler`, which will call `vPortRestoreContext()` based on the SVC number to start scheduling on the secondary core. |
| **Tick timer** | `vConfigureTickInterrupt()` programs `CNTP_EL0` for the FreeRTOS tick and routes `IRQ 30` through the GIC to all cores. |
| **Cache maintenance** | Each write to `ulSharedFlag` is followed by a `DSB SY` to guarantee visibility before the other core wakes. |

## Building and running examples

First, run the following command to clone FreeRTOS repository:

```bash
git clone https://github.com/FreeRTOS/FreeRTOS.git --recurse-submodules
```

Run the following commands to build the example:

```bash
cd FreeRTOS/FreeRTOS/Demo/ThirdParty/Partner-Supported-Demos/CORTEX_R82_SMP_FVP_GCC_ARMCLANG
rm -rf build && cmake -B build --toolchain=<armclang/gnu>_toolchain.cmake . && cmake --build build
```

### Running the example

Execute the following script to run the example:
```bash
./run.sh
```

### Expected output

```bash
$ ./run.sh

Info: FVP_BaseR_AEMv8R: terminal_0: Listening for serial connection on port 5000


Info: FVP_BaseR_AEMv8R: terminal_1: Listening for serial connection on port 5001


Info: FVP_BaseR_AEMv8R: terminal_2: Listening for serial connection on port 5002


Info: FVP_BaseR_AEMv8R: terminal_3: Listening for serial connection on port 5003

Ping from Core 0
Pong from Core 1
Ping from Core 0
Pong from Core 1
Ping from Core 0
Pong from Core 1
```

## Configuration — Running on up to 4 Cores

FreeRTOS is built for SMP and the FVP model can start **1 – 4 Cortex-R82 cores**.
To change the core count you must keep the firmware and the model in sync by editing **two** settings:

| File | Setting to change | Example for 4 cores |
|------|-------------------|---------------------|
| `FreeRTOSConfig.h` | `#define configNUMBER_OF_CORES …` | `#define configNUMBER_OF_CORES 4` |
| `fvp_config.txt` | `cluster0.NUM_CORES=…` | `cluster0.NUM_CORES=4` |

1. **Edit `FreeRTOSConfig.h`**
  Set `configNUMBER_OF_CORES` to the number of logical CPUs you want FreeRTOS to schedule (maximum = 4).

2. **Edit `fvp_config.txt`**
  Set `cluster0.NUM_CORES` to the **same** number so the Arm FVP instantiates the requested CPUs.

> ⚠️ **Both values must match.**

3. **Edit `main.c`**
  Create new tasks along with their functions as per the number of cores desired (optionally pin the new tasks to the required core).

Rebuild the example and launch the model as usual.

## License

This example is released under the **MIT License**.
