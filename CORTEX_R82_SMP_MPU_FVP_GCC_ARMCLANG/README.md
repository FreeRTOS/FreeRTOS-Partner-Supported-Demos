# Arm Cortex‑R82 FreeRTOS SMP MPU Demo (FVP_BaseR_AEMv8R)

**Overview**

This demo brings up FreeRTOS‑SMP on Arm Cortex‑R82 `FVP_BaseR_AEMv8R` and demonstrates MPU‑backed isolation for unprivileged application tasks that communicate via queues while a privileged logger task prints activity.

What actually runs in this test:

- Sender task (unprivileged): periodically enqueues a counter value to a queue which is shared with the receiver task and logs a status message.
- Receiver task (unprivileged): dequeues messages from the same shared queue and logs what it received.
- Logger task (privileged): receives log messages from both unprivileged tasks and prints them to the console.

The unprivileged tasks are created with `xTaskCreateRestricted()` and granted access only to a specific shared memory window that contains queue handles. The logger task runs privileged (via `portPRIVILEGE_BIT`). The demo validates correct MPU configuration for normal message‑passing between isolated tasks.

# Prerequisites

## Downloading and installing AEMv8R Architecture Envelope Model (AEM) Fixed Virtual Platform

Follow the instructions on the [page](https://developer.arm.com/Tools%20and%20Software/Fixed%20Virtual%20Platforms/Arm%20Architecture%20FVPs) to download `FVP_Base_AEMv8R` based on your operating system.

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
  * The Arm Cortex-R82 SMP MPU example uses `CMake` as the build system.
* [Arm GNU Toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads) or [Arm Compiler for Embedded](https://developer.arm.com/Tools%20and%20Software/Arm%20Compiler%20for%20Embedded)
  * To use Arm Compiler For Embedded (ArmClang), login is required for the download, and you will need a license in order to run the toolchain once installed.

## Supported toolchains

The example is supported and tested on the following toolchains:

  * Arm Compiler for Embedded v6.23 (armclang).
  * Arm GNU toolchain v14.2.

## Supported FVPs

The example is supported and tested on FVP_BaseR_AEMv8R Fast Models [11.28.23 (Feb 17 2025)]

## Cache Coherency

This example relies on the `FVP_BaseR_AEMv8R` implementing fully coherent caches. If run on hardware or models without full cache coherency, additional cache maintenance or non‑cacheable mappings would be required for correctness.

## What It Demonstrates

- Privilege separation between kernel privileged code and unprivileged tasks using the Armv8‑R Profile MPU.
- Enforced read‑only flash regions for both privileged and unprivileged code, with a separate region for FreeRTOS system calls.
- Privileged‑only RAM for kernel data (for example, TCBs) versus unprivileged task stacks/data.
- Controlled data sharing via a user‑defined MPU region that exposes only queue handle storage to unprivileged tasks.

## How It Works

- MPU setup happens in `Source/portable/GCC/ARM_CR82/port.c` during `vSetupMPU()`, using linker‑defined region boundaries from `gnu_linker_script.ld` (GNU) or `armclang_linker_script.sct` (ArmClang).
- Regions configured include:
  - `__privileged_functions_*__`: privileged flash, read‑only to privileged code.
  - `__syscalls_flash_*__`: unprivileged‑accessible flash containing FreeRTOS system calls.
  - `__unprivileged_flash_*__`: unprivileged flash, read‑only by all.
  - `__privileged_sram_*__`: RAM reserved for kernel data, privileged‑only access.
- Unprivileged tasks are given one user‑defined MPU region for a small shared array that holds queue handles (`xQueueHandlesArray`). Stacks and the shared array are 64‑byte aligned.

Important: Any user‑defined MPU region must be aligned to 64 bytes and shouldn't be less than 64 bytes in size. This demo explicitly aligns unprivileged task stacks and the shared queue handle array to 64 bytes to satisfy the MPU granularity and alignment requirements.

## Building and Running

First, run the following command to clone FreeRTOS repository:

```bash
git clone https://github.com/FreeRTOS/FreeRTOS.git --recurse-submodules
```

Run the following commands to build the example:

```bash
cd FreeRTOS/FreeRTOS/Demo/ThirdParty/Partner-Supported-Demos/CORTEX_R82_SMP_MPU_FVP_GCC_ARMCLANG
rm -rf build && cmake -B build --toolchain=<armclang/gnu>_toolchain.cmake . && cmake --build build
```

### Running

Execute the following script to run the example:
```bash
./run.sh
```

### Expected Output

Your console will show FVP startup messages followed by periodic sender/receiver activity logged by the privileged logger task, for example:

```
[Core: x] Sender: Sent message 0
[Core: y] Receiver: Received message 0
[Core: x] Sender: Sent message 1
[Core: z] Receiver: Received message 1
... (continues) ...
```

## Configuration — Up To 4 Cores

FreeRTOS is built for SMP and the FVP model can start 1–4 Cortex‑R82 cores. Keep firmware and model in sync:

- `config/FreeRTOSConfig.h`: set `#define configNUMBER_OF_CORES <N>`
- `fvp_config.txt`: set `cluster0.NUM_CORES=<N>`

Both values must match. Rebuild and run as usual.

## Notes on MPU Regions

- Minimum MPU granularity and alignment: 64 bytes. Ensure any user‑defined region base and size are 64‑byte aligned. This demo aligns unprivileged stacks and the shared handle array accordingly.

- Keep user regions minimal. Only map the specific buffers that must be shared between unprivileged tasks.

## License

This example is released under the **MIT License**. See the [LICENSE](LICENSE.md) file.
