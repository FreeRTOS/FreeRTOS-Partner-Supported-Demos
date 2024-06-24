# Copyright 2023-2024 Arm Limited and/or its affiliates <open-source-office@arm.com>
#
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.15)

add_library(cmsis_bsp INTERFACE)

target_include_directories(cmsis_bsp
    INTERFACE
        include
)

target_compile_definitions(cmsis_bsp
    INTERFACE
        CORSTONE310_FVP
        CMSIS_device_header="SSE310MPS3.h"
)

target_sources(cmsis_bsp
INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/source/startup_SSE310MPS3.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/system_SSE310MPS3.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/bsp_serial.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/Driver_USART.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/uart_cmsdk_drv.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/device_definition.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/syscalls_stub.c
)
