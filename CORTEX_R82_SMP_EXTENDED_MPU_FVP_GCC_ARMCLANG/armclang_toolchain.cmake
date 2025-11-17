# Copyright 2023-2025 Arm Limited and/or its affiliates <open-source-office@arm.com>
#
# SPDX-License-Identifier: MIT

set(CMAKE_SYSTEM_NAME        Generic)
set(CMAKE_SYSTEM_ARCH        armv8-r)

# Use the ARM Compiler 6 front-end
set(CMAKE_C_COMPILER         armclang)
set(CMAKE_CXX_COMPILER       armclang++)
set(CMAKE_ASM_COMPILER       armclang)

set(CMAKE_C_STANDARD         11)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Common flags for compilation
# -Wno-pointer-to-int-cast and -Wno-int-to-pointer-cast are added to suppress warnings in FreeRTOS MPU code (i.e., mpu_wrappers_v2.c) not the Arm Cortex-R82 port code.
set(CMAKE_C_FLAGS "--target=aarch64-arm-none-eabi -mcpu=cortex-r82 -mlittle-endian -mstrict-align -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast" )
set(CMAKE_ASM_FLAGS "--target=aarch64-arm-none-eabi -mcpu=cortex-r82 -mstrict-align" )

set(CMAKE_EXE_LINKER_FLAGS_INIT    "--entry=_boot" )
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS_INIT}")
