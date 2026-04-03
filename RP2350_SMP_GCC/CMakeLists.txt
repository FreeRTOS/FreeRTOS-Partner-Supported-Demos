# Copyright 2026 Arm Limited and/or its affiliates <open-source-office@arm.com>
#
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.15)

# Ensure correct Pico platform and board for RP2350
if(NOT DEFINED PICO_PLATFORM)
    set(PICO_PLATFORM "rp2350-arm-s" CACHE STRING "Pico platform" FORCE)
endif()
if(NOT DEFINED PICO_BOARD)
    set(PICO_BOARD "pico2" CACHE STRING "Pico board" FORCE)
endif()

# Point to the bundled Pico SDK and import it BEFORE project()
set(PICO_SDK_PATH "${CMAKE_CURRENT_LIST_DIR}/../Demos_Dependencies/pico-sdk" CACHE PATH "Path to the Raspberry Pi Pico SDK")
include("${PICO_SDK_PATH}/external/pico_sdk_import.cmake")

project(
  RP2350-SMP-EXAMPLE
  VERSION 0.1
  LANGUAGES C CXX ASM
)

set (CMAKE_BUILD_TYPE Release)

# Initialize the Pico SDK
pico_sdk_init()

get_filename_component(FREERTOS_DIR_PATH ${CMAKE_CURRENT_LIST_DIR}/../../../.. REALPATH)
message(DEBUG "FREERTOS_DIR_PATH is ${FREERTOS_DIR_PATH}")

set(KERNEL_DIR_PATH ${FREERTOS_DIR_PATH}/Source)
message(DEBUG "KERNEL_DIR_PATH is ${KERNEL_DIR_PATH}")

# Select the native compile PORT
if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
	set(FREERTOS_PORT "GCC_ARM_CM33_NTZ_NONSECURE" CACHE STRING "" FORCE)
else()
	message(FATAL_ERROR "Unsupported compiler: ${CMAKE_C_COMPILER_ID}")
endif()

set(FREERTOS_HEAP "4" CACHE STRING "" FORCE)

add_library(freertos_config INTERFACE)

target_include_directories(freertos_config SYSTEM
	INTERFACE
        config
        ${PICO_SDK_PATH}/src/rp2350/hardware_regs/include
)

add_subdirectory(${KERNEL_DIR_PATH} freertos_kernel)

add_executable(rp2350_smp_example)

target_sources(rp2350_smp_example
	PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/main.c
        ${PICO_SDK_PATH}/src/rp2_common/cmsis/stub/CMSIS/Device/RP2350/Source/system_RP2350.c
)

target_link_libraries(rp2350_smp_example
	PRIVATE
		pico_stdlib
        freertos_kernel
        pico_multicore
)

target_include_directories(rp2350_smp_example
    PRIVATE
        ${PICO_SDK_PATH}/src/rp2_common/cmsis/stub/CMSIS/Device/RP2350/Include
        ${PICO_SDK_PATH}/src/rp2_common/cmsis/stub/CMSIS/Core/Include
        ${PICO_SDK_PATH}/src/rp2_common/hardware_irq/include/hardware
)

# Generates output (.uf2) file, map file, and other useful files
pico_add_extra_outputs(rp2350_smp_example)

# Enable USB output for stdio, disable UART output
pico_enable_stdio_usb(rp2350_smp_example 1)
pico_enable_stdio_uart(rp2350_smp_example 0)
