# Copyright 2023-2024 Arm Limited and/or its affiliates <open-source-office@arm.com>
#
# SPDX-License-Identifier: MIT

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR  cortex-m85)

set(CMAKE_ASM_COMPILER iasmarm)
set(CMAKE_C_COMPILER   iccarm)
set(CMAKE_CXX_COMPILER iccarm)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS_INIT "--cpu ${CMAKE_SYSTEM_PROCESSOR}")
set(CMAKE_ASM_FLAGS_INIT "--cpu ${CMAKE_SYSTEM_PROCESSOR}")
set(CMAKE_C_LINK_FLAGS "--cpu ${CMAKE_SYSTEM_PROCESSOR}")
set(CMAKE_ASM_LINK_FLAGS "--cpu ${CMAKE_SYSTEM_PROCESSOR}")

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS_INIT})
set(CMAKE_ASM_FLAGS ${CMAKE_ASM_FLAGS_INIT})

add_compile_options(
    $<$<COMPILE_LANGUAGE:C,CXX>:-e>
    $<$<COMPILE_LANGUAGE:C,CXX>:--dlib_config=full>
    $<$<COMPILE_LANGUAGE:C,CXX>:--silent>
    $<$<COMPILE_LANGUAGE:C,CXX>:-DNO_TYPEOF>
    $<$<COMPILE_LANGUAGE:C,CXX>:-D_NO_DEFINITIONS_IN_HEADER_FILES>
    $<$<COMPILE_LANGUAGE:C,CXX>:--diag_suppress=Pe546,Pe940,Pa082,Pa084>
    $<$<COMPILE_LANGUAGE:C,CXX>:--no_path_in_file_macros>
    $<$<COMPILE_LANGUAGE:C,ASM>:--fpu=FPv5_D16>
)

add_link_options(
    --silent
    --semihosting
    --redirect __write=__write_buffered
    --diag_suppress=lp005,Lp023
)

add_compile_definitions(
    __ARM_ARCH_8_1M_MAIN__=1
)
