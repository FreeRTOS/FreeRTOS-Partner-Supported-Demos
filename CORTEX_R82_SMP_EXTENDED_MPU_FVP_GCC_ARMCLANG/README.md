# Arm Cortex‑R82 FreeRTOS SMP Extended MPU Demo (FVP_BaseR_AEMv8R)

**Overview**

This demo brings up FreeRTOS‑SMP on Arm Cortex‑R82 `FVP_BaseR_AEMv8R` and focuses on Memory Protection Unit (MPU) configuration and privilege separation between kernel/privileged code and unprivileged tasks.

The example creates three tasks that intentionally exercise MPU protection violations:

- First Task: Attempts to elevate its privilege by modifying its own TCB (in privileged RAM) and triggers an MPU fault.
- Second Task: Successfully accesses its own unprivileged stack, then attempts to write into another task’s stack and triggers an MPU fault.
- Third Task: Attempts to write to read‑only unprivileged flash and triggers an MPU fault.

All MPU violations are handled gracefully: the exception handler redirects to a recovery function that prints a message and deletes the offending task, allowing the system to continue running.

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
  * The Arm Cortex-R82 SMP Extended MPU example uses `CMake` as the build system.
* [Arm GNU Toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads) or [Arm Compiler for Embedded](https://developer.arm.com/Tools%20and%20Software/Arm%20Compiler%20for%20Embedded)
  * To use Arm Compiler For Embedded (ArmClang), login is required for the download, and you will need a license in order to run the toolchain once installed.

# Supported toolchains

The example is supported and tested on the following toolchains:

  * Arm Compiler for Embedded v6.23 (armclang).
  * Arm GNU toolchain v14.2.

# Supported FVPs

The example is supported and tested on FVP_BaseR_AEMv8R Fast Models [11.28.23 (Feb 17 2025)]

## Cache Coherency

This example relies on the `FVP_BaseR_AEMv8R` implementing fully coherent caches. If run on hardware or models without full cache coherency, additional cache maintenance or non‑cacheable mappings would be required for correctness.

## What It Demonstrates

- Privilege separation between kernel privileged code and unprivileged tasks using the Armv8-R‑Profile MPU.
- Enforced read‑only flash regions for both privileged and unprivileged code, with a separate region for FreeRTOS system calls.
- Privileged‑only RAM for kernel data (e.g., TCBs) versus unprivileged user task stacks.

## How It Works

- MPU setup happens in `Source/portable/GCC/ARM_CR82/port.c` during `vSetupMPU()`, using linker‑defined region boundaries from `gnu_linker_script.ld` (GNU) or `armclang_linker_script.sct` (ArmClang).
- Regions configured include:
  - `__privileged_functions_*__`: privileged flash, read‑only to privileged code.
  - `__syscalls_flash_*__`: unprivileged‑accessible flash containing FreeRTOS system calls.
  - `__unprivileged_flash_*__`: unprivileged flash, read‑only by all.
  - `__privileged_sram_*__`: RAM reserved for kernel data, privileged‑only access.
- The exception path (`App_Fault_Handler`) detects a data abort from a lower exception level and redirects to `vRecoverFromException()`, which prints a message and issues an SVC to call `vTaskDelete()` for the offending task.

Important: Any user‑defined MPU region must be aligned to 64 bytes and shouldn't be less than 64 bytes in size. This demo explicitly aligns unprivileged task stacks and the shared queue handle array to 64 bytes to satisfy the MPU granularity and alignment requirements.

## Building and Running

First, run the following command to clone FreeRTOS repository:

```bash
git clone https://github.com/FreeRTOS/FreeRTOS.git --recurse-submodules
```

Run the following commands to build the example:

```bash
cd FreeRTOS/FreeRTOS/Demo/ThirdParty/Partner-Supported-Demos/CORTEX_R82_SMP_EXTENDED_MPU_FVP_GCC_ARMCLANG
rm -rf build && cmake -B build --toolchain=<armclang/gnu>_toolchain.cmake . && cmake --build build
```

### Running

Execute the following script to run the example:
```bash
./run.sh
```

### Expected Output (Excerpt)

Your console will show FVP startup messages followed by task activity and recovery messages similar to:

```
Starting MPU demo with exception recovery.

[Core: x] Second Task: Attempting to access its own unprivileged stack.
[Core: y] First Task: Attempting to raise its privilege level by accessing its privileged data.
[Core: x] Third Task: Attempting to write to a read-only region.
[Core: z] Second Task: Successfully accessed its own unprivileged stack.
[Core: x] First Task: Recovered from the data abortion exception gracefully.
[Core: y] Third Task: Recovered from the data abortion exception gracefully.
[Core: x] Second Task: Attempting to access first task unprivileged stack.
[Core: z] Second Task: Recovered from the data abortion exception gracefully.
```

Each offending task is deleted after recovery while the system remains alive.

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
