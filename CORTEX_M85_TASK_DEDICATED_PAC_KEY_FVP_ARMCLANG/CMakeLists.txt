# Copyright 2023-2024 Arm Limited and/or its affiliates <open-source-office@arm.com>
#
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.15)

project(
  dedicated-pac-key-example
  VERSION 0.1
  LANGUAGES C ASM)

set (CMAKE_BUILD_TYPE Release)

set(CMAKE_EXECUTABLE_SUFFIX ".axf")

add_executable(cortex_m85_task_dedicated_pac_key_fvp_example)

set(FREERTOS_ARM_V_8_1_M_PACBTI_CONFIG "ARM_V_8_1_M_PACBTI_CONFIG_PACRET" CACHE STRING "" FORCE)

# Select the native compile PORT
set(FREERTOS_PORT "GCC_ARM_CM85_NONSECURE" CACHE STRING "" FORCE)

set(FREERTOS_HEAP "4" CACHE STRING "" FORCE)

add_library(freertos_config INTERFACE)

target_include_directories(freertos_config
	INTERFACE
    	config
)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../../../../Source freertos_kernel)

target_sources(cortex_m85_task_dedicated_pac_key_fvp_example
	PRIVATE
    	${CMAKE_CURRENT_SOURCE_DIR}/main.c
		${CMAKE_CURRENT_SOURCE_DIR}/../Demos_Dependencies/arm_corstone_platform_bsp/corstone315/Device/Source/startup_SSE315.c
)

target_link_options(cortex_m85_task_dedicated_pac_key_fvp_example
	PRIVATE
		--scatter=${CMAKE_CURRENT_SOURCE_DIR}/armclang.sct
		--entry=Reset_Handler
		--map
)

# BSP serial library

add_library(cmsis_bsp STATIC)

target_sources(cmsis_bsp
    PUBLIC
        CMSIS/common/bsp_serial.c
)

target_include_directories(cmsis_bsp
    PUBLIC
		CMSIS/common
)

target_link_libraries(cmsis_bsp
    PUBLIC
        arm-corstone-platform-bsp
)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/secure)

add_dependencies(
	cortex_m85_task_dedicated_pac_key_fvp_example
	secure_cortex_m85_task_dedicated_pac_key_fvp_example
)

target_link_libraries(cortex_m85_task_dedicated_pac_key_fvp_example
	PRIVATE
		freertos_kernel
		freertos_config
		cmsis_bsp
		${CMAKE_BINARY_DIR}/veneers.o
)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
include(ConvertElfToBin)
include(MergeImages)

target_axf_to_bin(cortex_m85_task_dedicated_pac_key_fvp_example flash_ns)

merge_images(cortex_m85_task_dedicated_pac_key_fvp_example)
