# Copyright 2023 Arm Limited and/or its affiliates <open-source-office@arm.com>
#
# SPDX-License-Identifier: MIT

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR=cortex-m35p)

set(CMAKE_ASM_COMPILER "arm-none-eabi-gcc")
set(CMAKE_C_COMPILER "arm-none-eabi-gcc")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-march=armv8-m.main -mcpu=cortex-m35p+nodsp+nofp -mfloat-abi=soft -mthumb -Werror -Wall -Wextra")
set(CMAKE_ASM_FLAGS "-march=armv8-m.main -mcpu=cortex-m35p+nodsp+nofp -mfloat-abi=soft -mthumb")
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--no-warn-rwx-segment -specs=nano.specs -specs=nosys.specs -specs=rdimon.specs")
