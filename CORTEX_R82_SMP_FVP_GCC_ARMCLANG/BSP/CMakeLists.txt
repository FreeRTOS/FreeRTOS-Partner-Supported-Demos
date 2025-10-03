# Copyright 2023-2025 Arm Limited and/or its affiliates <open-source-office@arm.com>
#
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.15)

add_library(bsp INTERFACE)

target_sources(bsp
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/Source/port_asm_vectors.S
        ${CMAKE_CURRENT_SOURCE_DIR}/Source/boot.S
        ${CMAKE_CURRENT_SOURCE_DIR}/Source/startup.S
        ${CMAKE_CURRENT_SOURCE_DIR}/Source/gic.c
)

target_include_directories(bsp
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/Include
)

target_link_libraries(bsp
    INTERFACE
	    freertos_kernel
)
