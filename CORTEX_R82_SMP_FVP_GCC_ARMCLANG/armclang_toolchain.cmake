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
set(CMAKE_C_FLAGS "--target=aarch64-arm-none-eabi -mcpu=cortex-r82 -mlittle-endian -Werror -g -gdwarf-3 -mstrict-align" )
set(CMAKE_ASM_FLAGS "--target=aarch64-arm-none-eabi -mcpu=cortex-r82 -Werror -g -gdwarf-3 -mstrict-align" )

set(CMAKE_EXE_LINKER_FLAGS_INIT    "--entry=_boot" )
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS_INIT}")
