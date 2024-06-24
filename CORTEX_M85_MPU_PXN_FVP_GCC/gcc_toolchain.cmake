# Copyright 2023-2024 Arm Limited and/or its affiliates <open-source-office@arm.com>
#
# SPDX-License-Identifier: MIT

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR=cortex-m85)

set(CMAKE_ASM_COMPILER "arm-none-eabi-gcc")
set(CMAKE_C_COMPILER "arm-none-eabi-gcc")

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "-march=armv8.1-m.main+fp.dp+mve.fp -mfloat-abi=hard -mthumb -Wall")
set(CMAKE_ASM_FLAGS "-march=armv8.1-m.main+fp.dp+mve.fp -mfloat-abi=hard -mthumb")
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--no-warn-rwx-segment -specs=nano.specs -specs=nosys.specs")
