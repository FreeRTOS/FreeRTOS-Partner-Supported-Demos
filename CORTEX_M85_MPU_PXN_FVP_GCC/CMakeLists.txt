# Copyright 2023-2024 Arm Limited and/or its affiliates <open-source-office@arm.com>
#
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.15)

project(
  example
  VERSION 0.1
  LANGUAGES C ASM)

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_EXECUTABLE_SUFFIX ".axf")

get_filename_component(FREERTOS_DIR_PATH ${CMAKE_CURRENT_LIST_DIR}/../../../.. REALPATH)
message(DEBUG "FREERTOS_DIR_PATH is ${FREERTOS_DIR_PATH}")

set(KERNEL_DIR_PATH ${FREERTOS_DIR_PATH}/Source)
set(DEMO_COMMON_PATH ${FREERTOS_DIR_PATH}/Demo/Common)
message(DEBUG "KERNEL_DIR_PATH is ${KERNEL_DIR_PATH}")
message(DEBUG "DEMO_COMMON_PATH is ${DEMO_COMMON_PATH}")

add_executable(cortex_m85_mpu_pxn_fvp_example)

target_sources(cortex_m85_mpu_pxn_fvp_example
	PRIVATE
    	${CMAKE_CURRENT_SOURCE_DIR}/main.c
)

add_subdirectory(CMSIS)

target_link_libraries(cortex_m85_mpu_pxn_fvp_example
	PRIVATE
  		cmsis_bsp
)

add_library(freertos_config INTERFACE)

target_include_directories(freertos_config SYSTEM
	INTERFACE
    	config
)

set( FREERTOS_HEAP "4" CACHE STRING "" FORCE)

# Select the native compile PORT
set( FREERTOS_PORT "GCC_ARM_CM85_NTZ_NONSECURE" CACHE STRING "" FORCE )

add_subdirectory(../../../../Source freertos_kernel)

target_link_options(cortex_m85_mpu_pxn_fvp_example
	PRIVATE
		-T ${CMAKE_CURRENT_SOURCE_DIR}/gcc_arm.ld
		-Wl,--gc-sections,-Map=cortex_m85_mpu_pxn_fvp_example.map
)

target_link_libraries(cortex_m85_mpu_pxn_fvp_example
	PRIVATE
  		freertos_kernel
)
