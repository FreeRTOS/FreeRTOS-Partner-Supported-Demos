# FreeRTOS SMP Example on Raspberry Pi Pico 2 (RP2350)

This example demonstrates FreeRTOS SMP on the dual‑core Arm Cortex-M33 (Armv8-M) Raspberry Pi Pico 2 (RP2350) using the Raspberry Pi Pico SDK (pico‑sdk). Two tasks, pinned to different cores, exchange a shared flag and print Ping/Pong messages. Inter‑core preemption is exercised via a doorbell interrupt that yields from ISR.

## Dependencies

- pico‑sdk: Included as a git submodule. The build imports `pico_sdk_import.cmake` and calls `pico_sdk_init()`.
- Optional: picotool for UF2 flashing (fetched automatically if not found). Optional TinyUSB submodule if you need USB.
- picocom or any other alternative serial terminal for monitoring the logs.

## Build tools

* [CMake](https://cmake.org/download/)
  * The example uses `CMake` as the build system.

## Supported toolchains

The example is supported and tested on Arm GNU toolchain v14.2.

## Hardware

- Board: Raspberry Pi Pico 2 (`PICO_BOARD=pico2`).
- Platform: `rp2350-arm-s` (Cortex‑M33 secure only). See [CMakeLists.txt:9](CMakeLists.txt#L9).

## Build and Flash

### Build

First, run the following command to clone FreeRTOS repository:

```bash
git clone https://github.com/FreeRTOS/FreeRTOS.git --recurse-submodules
```

Run the following commands to build the example:

```bash
cd FreeRTOS/FreeRTOS/Demo/ThirdParty/Partner-Supported-Demos/RP2350_SMP_GCC
rm -rf build && cmake -B build . -DCMAKE_TOOLCHAIN_FILE=../Demos_Dependencies/pico-sdk/cmake/preload/toolchains/pico_arm_cortex_m33_gcc.cmake && cmake --build build
```

Artifacts are emitted with extra formats by the SDK:

- `build/rp2350_smp_example.elf`
- `build/rp2350_smp_example.uf2`

### Flash

Flashing can be done by either of the following approaches:

1.  Mounting the device then copying the output UF2 file to the mass-storage device
    1. Press and hold BOOTSEL button on the Pico 2 board
    2. Reset the board
    3. Release the BOOTSEL button
    4. Mount the device, for example on Linux, this would look like the following:
        ```bash
        udisksctl mount -b /dev/<device>
        ```
    5. Copy the output UF2 file to the mass‑storage device:
        ```bash
        cp build/rp2350_smp_example.uf2 <mounted_device_location>/RP2350
        ```

2. Via Picotool
    ```bash
    picotool load -f build/rp2350_smp_example.uf2
    ```

### Monitoring the application

This application overrides the pico-sdk default output configuration to route log output over USB Communication Device Class (CDC) instead of UART. The examples below use `picocom`, but any serial terminal (e.g., `minicom`) should work as well.

- If using USB CDC:
  ```bash
  picocom -b 115200 /dev/ttyACM0   # Linux
  picocom -b 115200 /dev/tty.usbmodem*  # macOS
  ```

- If using UART:
  ```bash
  picocom -b 115200 /dev/ttyUSB0   # Linux
  picocom -b 115200 /dev/tty.usbserial*  # macOS
  ```

#### Configuring stdio (USB vs UART)

Default for this demo: USB CDC enabled, UART disabled.

To enable UART and disable USB CDC:

- Edit `CMakeLists.txt`:
  ```cmake
  # Use UART instead of USB CDC
  pico_enable_stdio_usb(rp2350_smp_example 0)
  pico_enable_stdio_uart(rp2350_smp_example 1)
  ```

## What The Example Does

- Creates two tasks: `prvTaskCore0` (runs on core 0) prints “Ping”, `prvTaskCore1` (runs on core 1) prints “Pong”. See [main.c:63](main.c#L63) and [main.c:83](main.c#L83).
- Uses a shared flag `ulSharedFlag` to coordinate between the ping/pong tasks, each write is followed by Data Synchronisation Barrier (`DSB`) instruction for ordering.
- Pins ping/pong tasks to cores with `vTaskCoreAffinitySet`. See [main.c:158](main.c#L158).
- Also creates an unpinned task `prvBouncingTask` that bounces between both cores, and periodically prints the running core. This task is created with no affinity to validate that it can run on either core. See [main.c:103](main.c#L103).

## Boot Sequence (SMP)

The FreeRTOS Cortex‑M33 SMP port and pico‑sdk cooperate to bring both cores up deterministically. This demo follows the sequence below, aligning with the FreeRTOS Armv8‑M SMP guidance and the implementation in `xWakeSecondaryCore()` and `prvMakeSecondaryCoreReady()`:

- Primary core (core 0)
  - C runtime runs, execution enters `main()`. The application installs the FreeRTOS vectors (`SVC`, `PendSV`, and `SysTick`) via `prvSetupInterruptVectors()` before creating tasks. See [main.c:124](main.c#L124).
  - Application creates tasks, pins ping/pong tasks to cores, and initialises the inter‑core doorbell. Scheduler is started with `vTaskStartScheduler()` (see [main.c:170](main.c#L170)), which calls `xPortStartScheduler()`.
  - Inside `xPortStartScheduler()` the port:
    - Configures interrupt priorities.
    - Starts the tick timer and initialises per‑core critical nesting.
    - Sets `ucPrimaryCoreInitDoneFlag = 1` to signal shared init is complete.
    - Calls the application‑defined `configWAKE_SECONDARY_CORES` hook. In this demo it resolves to `xWakeSecondaryCore()`, which launches core 1 via `multicore_launch_core1(prvMakeSecondaryCoreReady)` once the flag is set. See [main.c:294](main.c#L294).
    - Waits until all entries in `ucSecondaryCoresReadyFlags[]` are set by the secondary core(s).
    - Calls `vStartFirstTask()` to start scheduling on core 0.

- Secondary core (core 1)
  - Secondary core is kept at bootrom until it is launched by the primary core using the pico‑sdk `multicore_launch_core1()` with the entry function set to  `prvMakeSecondaryCoreReady`. Launch is gated on `ucPrimaryCoreInitDoneFlag == 1` in `xWakeSecondaryCore()`, which matches the top‑level Armv8‑M SMP boot guidance that secondaries must not proceed until shared init completes.
  - In `prvMakeSecondaryCoreReady()` (see [main.c:310](main.c#L310)), the application:
    - Installs the FreeRTOS vectors via `prvSetupInterruptVectors()` and sets up the doorbell IRQ with `prvSetupDoorbellInterrupt()`.
    - Calls `vPortConfigureInterruptPriorities()` to apply per‑core priorities.
    - Signals readiness by setting `ucSecondaryCoresReadyFlags[portGET_CORE_ID()-1] = 1`.
    - Issues `SVC portSVC_START_SCHEDULER`, which in the SVC handler dispatches to `vRestoreContextOfFirstTask()` to begin scheduling on core 1. See [main.c:319](main.c#L319).

Notes
- On RP2350, the secondary core does not run full C runtime startup, it is explicitly launched from core 0. The demo relies on the pico‑sdk `multicore_*` APIs for this bring‑up.

## Notes on Coherency and Ordering

- RP2350 SRAM is not data‑cached, so inter‑core visibility is naturally coherent for SRAM. Use barriers for ordering across cores.
- The demo uses Data Synchronisation Barrier (`DSB`) instruction after updating the shared flag, FreeRTOS primitives also include required barriers.

## Expected Output

Over the selected stdio (USB/UART as configured in pico‑sdk):

```
FreeRTOS SMP Example starting on RP2350...
Core 0: Doorbell X claimed for inter-core interrupts.
Core 0: Waking up secondary core.
Core 1: Secondary core started.
Ping from Core 0
Bouncing task running on core X
Pong from Core 1
Bouncing task running on core Y
Ping from Core 0
Bouncing task running on core X
Pong from Core 1
Bouncing task running on core Y
Ping from Core 0
Bouncing task running on core X
Pong from Core 1
Bouncing task running on core Y
...
```

## License

This example is released under the **MIT License**. See the [LICENSE](LICENSE.md) and [Third Party Notices](THIRD_PARTY_NOTICES) files.
