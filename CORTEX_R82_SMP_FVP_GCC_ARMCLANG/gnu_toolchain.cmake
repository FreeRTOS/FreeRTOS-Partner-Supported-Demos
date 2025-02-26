# Copyright 2023-2025 Arm Limited and/or its affiliates <open-source-office@arm.com>
#
# SPDX-License-Identifier: MIT

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR cortex-r82)

set(CMAKE_C_COMPILER "aarch64-none-elf-gcc")
set(CMAKE_CXX_COMPILER "aarch64-none-elf-g++")
set(CMAKE_ASM_COMPILER "aarch64-none-elf-gcc")

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "-mcpu=cortex-r82 -mabi=lp64 -mlittle-endian -Wall -Wextra -g -gdwarf-3 -mstrict-align")
set(CMAKE_ASM_FLAGS "-mcpu=cortex-r82 -mabi=lp64 -Wall -g -gdwarf-3")

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_EXE_LINKER_FLAGS_INIT
    "-specs=rdimon.specs -Wl,-e,_boot,--wrap=memset,--emit-relocs -lc -lrdimon")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS_INIT}")
