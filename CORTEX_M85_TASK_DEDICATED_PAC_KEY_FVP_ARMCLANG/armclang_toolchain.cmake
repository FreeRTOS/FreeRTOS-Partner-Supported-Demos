# Copyright 2023-2024 Arm Limited and/or its affiliates <open-source-office@arm.com>
#
# SPDX-License-Identifier: MIT

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-m85)

set(CMAKE_C_COMPILER armclang)
set(CMAKE_CXX_COMPILER armclang)
set(CMAKE_ASM_COMPILER armclang)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "-march=armv8.1-m.main+pacbti+fp.dp+mve.fp -mfloat-abi=hard -mthumb -fdata-sections -g")
set(CMAKE_ASM_FLAGS "-march=armv8.1-m.main+pacbti+fp.dp+mve.fp+ -mfloat-abi=hard -mthumb -masm=auto --target=arm-arm-none-eabi -g")
