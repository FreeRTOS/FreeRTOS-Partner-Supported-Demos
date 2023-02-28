
# Blinky example on Armv8-M Cortex-M35P Fast Model
This blinky example validates the porting of FreeRTOS kernel to Armv8-M Cortex-M35P MCU. The example is based on Armv8-M Cortex-M35P Fast Model. Follow the [link](https://www.arm.com/products/development-tools/simulation/fast-models) to learn more about Arm fast models.

The example supports two configurations, namely blinky demo `BLINKY_DEMO` and full demo `FULL_DEMO`. The blinky demo prints a message to the console when a receiver task receives an expected message from the sender task. The full demo implements soak testing of FreeRTOS kernel on Cortex-M35P fast-model.

# Prerequisites

## Fast models
### Downloading and installing Fast models
Follow the instructions on the [page](https://developer.arm.com/Tools%20and%20Software/Fast%20Models) to download the fast models based on your operating system.

Ensure that requirements mentioned in the [page](https://developer.arm.com/documentation/100965/1120/Installing-Fast-Models/Requirements-for-Fast-Models) are met. Then, follow the instructions on the [page](https://developer.arm.com/documentation/100965/1120/Installing-Fast-Models) to install the fast models.

### Building Armv8-M Cortex-M35P Fast Model
Run the following commands to build Armv8-M Cortex-M35P fast model:
```bash
git clone https://github.com/ARM-software/Tool-Solutions.git
cd Tool-Solutions/fast-models-examples/cortex-m35/system
./build_linux.sh
```

If the build is successful, then Cortex-M35P fast model `Cortex-M35-example.x` will be created in `Tool-Solutions/fast-models-examples/cortex-m35/system` folder.

**NOTE**  
Make sure that the path to fast model executable is available in the `PATH` variable.

## Build tools
* [CMake](https://cmake.org/download/)
* [GNU Arm Embedded Toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)

# The blinky example
The blinky example uses `CMake` as the build system and supports two configurations, blinky demo `BLINKY_DEMO` and full demo `FULL_DEMO`. Run the following command to clone FreeRTOS repository:

```bash
git clone https://github.com/FreeRTOS/FreeRTOS.git --recurse-submodules
```

## Building and running the blinky demo
### Building the blinky demo
Run the following commands to build the blinky demo:

```bash
cd FreeRTOS/FreeRTOS/Demo/ThirdParty/Partner-Supported-Demos/CORTEX_M35P_FAST_MODEL_BLINKY_GCC
rm -rf build && cmake -B build -DBLINKY_DEMO=1 --toolchain=gcc_toolchain.cmake . && cmake --build build
```

### Running the demo
Run the following command to execute the blinky demo:
```bash
Cortex-M35-example.x --stat -a build/cortex_m35p_mpu_fast_model_example
```

### Expected output
```bash
Fast Models [11.19.14 (Sep  8 2022)]
Copyright 2000-2022 ARM Limited.
All Rights Reserved.

Received something
blinking
Received something
blinking
Received something
blinking
Received something
blinking
Received something
blinking
Received something
blinking
^C
Stopping simulation...


Info: /OSCI/SystemC: Simulation stopped by user.

--- m35 statistics: -----------------------------------------------------------
Simulated time                          : 64.648100s
User time                               : 4.001537s
System time                             : 0.000000s
Wall time                               : 4.034367s
Performance index                       : 16.02
cortex-m35-subsystem.armcortexm35pct    : 403.89 MIPS (  1616198523 Inst)
-------------------------------------------------------------------------------

```

### Building the full demo
Run the following commands to build the full demo:

```bash
cd FreeRTOS/FreeRTOS/Demo/ThirdParty/Partner-Supported-Demos/CORTEX_M35P_FAST_MODEL_BLINKY_GCC
rm -rf build && cmake -B build -DFULL_DEMO=1 --toolchain=gcc_toolchain.cmake . && cmake --build build
```

### Running the full demo
Run the following command to execute the full demo:
```bash
Cortex-M35-example.x --stat -a build/cortex_m35p_mpu_fast_model_example
```

### Expected output
```bash
Fast Models [11.19.14 (Sep  8 2022)]
Copyright 2000-2022 ARM Limited.
All Rights Reserved.

OK: No errors - tick count 10000
OK: No errors - tick count 20000
^C
Stopping simulation...


Info: /OSCI/SystemC: Simulation stopped by user.

--- m35 statistics: -----------------------------------------------------------
Simulated time                          : 230.107494s
User time                               : 37.090885s
System time                             : 0.271216s
Wall time                               : 37.826357s
Performance index                       : 6.08
cortex-m35-subsystem.armcortexm35pct    : 153.97 MIPS (  5752613410 Inst)
-------------------------------------------------------------------------------
```
