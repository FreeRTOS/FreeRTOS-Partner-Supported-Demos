# Copyright 2023-2025 Arm Limited and/or its affiliates <open-source-office@arm.com>
#
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.15)

project(
  SMP-DEMO
  VERSION 0.1
  LANGUAGES C ASM)

set (CMAKE_BUILD_TYPE Release)

set(CMAKE_EXECUTABLE_SUFFIX ".axf")

get_filename_component(FREERTOS_DIR_PATH ${CMAKE_CURRENT_LIST_DIR}/../../../.. REALPATH)
message(DEBUG "FREERTOS_DIR_PATH is ${FREERTOS_DIR_PATH}")

set(KERNEL_DIR_PATH ${FREERTOS_DIR_PATH}/Source)
message(DEBUG "KERNEL_DIR_PATH is ${KERNEL_DIR_PATH}")

# Select the native compile PORT
if("${CMAKE_C_COMPILER_ID}" STREQUAL "ARMClang" OR "${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
	set(FREERTOS_PORT "GCC_ARM_CR82" CACHE STRING "" FORCE)
else()
	message(FATAL_ERROR "Unsupported compiler: "${CMAKE_C_COMPILER_ID}"")
endif()

set(FREERTOS_HEAP "4" CACHE STRING "" FORCE)

add_library(freertos_config INTERFACE)

target_include_directories(freertos_config SYSTEM
	INTERFACE
    	config
)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/BSP BSP)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../../../../Source freertos_kernel)

add_executable(cortex_r82_smp_fvp_example)

target_sources(cortex_r82_smp_fvp_example
	PRIVATE
    	${CMAKE_CURRENT_SOURCE_DIR}/main.c
    	${CMAKE_CURRENT_SOURCE_DIR}/crt_replacements.c
)

target_include_directories(cortex_r82_smp_fvp_example
	PRIVATE
  		${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_options(cortex_r82_smp_fvp_example
	PRIVATE
		$<$<C_COMPILER_ID:ARMClang>:--map $<$<VERSION_GREATER_EQUAL:${CMAKE_VERSION},4.0.0>:--list=cortex_r82_smp_fvp_example.map> --scatter=${CMAKE_CURRENT_SOURCE_DIR}/armclang_linker_script.sct>
		$<$<C_COMPILER_ID:GNU>:-T${CMAKE_CURRENT_SOURCE_DIR}/gnu_linker_script.ld -Wl,--gc-sections,-Map=cortex_r82_smp_fvp_example.map>
)

target_link_libraries(cortex_r82_smp_fvp_example
	PRIVATE
		bsp
)
