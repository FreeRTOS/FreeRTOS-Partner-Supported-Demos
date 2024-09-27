# Copyright 2023-2024 Arm Limited and/or its affiliates <open-source-office@arm.com>
#
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.15)

project(
  pacbti-example
  VERSION 0.1
  LANGUAGES C ASM)

set (CMAKE_BUILD_TYPE Release)

set(CMAKE_EXECUTABLE_SUFFIX ".axf")

get_filename_component(FREERTOS_DIR_PATH ${CMAKE_CURRENT_LIST_DIR}/../../../.. REALPATH)
message(DEBUG "FREERTOS_DIR_PATH is ${FREERTOS_DIR_PATH}")

set(KERNEL_DIR_PATH ${FREERTOS_DIR_PATH}/Source)
set(DEMO_COMMON_PATH ${FREERTOS_DIR_PATH}/Demo/Common)
message(DEBUG "KERNEL_DIR_PATH is ${KERNEL_DIR_PATH}")
message(DEBUG "DEMO_COMMON_PATH is ${DEMO_COMMON_PATH}")

set(FREERTOS_ARM_V_8_1_M_PACBTI_CONFIG "ARM_V_8_1_M_PACBTI_CONFIG_STANDARD" CACHE STRING "" FORCE)
set(TFM_PACBTI_CONFIGURATION "BRANCH_PROTECTION_STANDARD" CACHE STRING "" FORCE)

# Select the native compile PORT
if(${CMAKE_C_COMPILER_ID} STREQUAL "ARMClang")
	set(FREERTOS_PORT "GCC_ARM_CM85_TFM" CACHE STRING "" FORCE)
elseif(${CMAKE_C_COMPILER_ID} STREQUAL "IAR")
	set(FREERTOS_PORT "IAR_ARM_CM85_TFM" CACHE STRING "" FORCE)
else()
	message(FATAL_ERROR "Unsupported compiler: ${CMAKE_C_COMPILER_ID}")
endif()

set(FREERTOS_HEAP "4" CACHE STRING "" FORCE)

add_subdirectory(CMSIS)

add_library(freertos_config INTERFACE)

target_include_directories(freertos_config SYSTEM
	INTERFACE
    	config
)

target_link_libraries(freertos_config
    INTERFACE
        tfm-ns-interface
)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../../../../Source freertos_kernel)

add_subdirectory(trusted_firmware-m)

add_executable(cortex_m85_pacbti_fvp_example)

# Trusted Firmware-M must be built before the application, because
# the application depends on the NS interface and the BL2 signing scripts,
# both of which are generated as parts of the Trusted Firmware-M build process.
add_dependencies(cortex_m85_pacbti_fvp_example trusted_firmware-m-build)

target_sources(cortex_m85_pacbti_fvp_example
	PRIVATE
    	${CMAKE_CURRENT_SOURCE_DIR}/main.c
)

target_include_directories(cortex_m85_pacbti_fvp_example
	PUBLIC
  		${CMAKE_CURRENT_SOURCE_DIR}
)

if(${CMAKE_C_COMPILER_ID} STREQUAL "IAR")
	target_link_options(cortex_m85_pacbti_fvp_example
		PRIVATE
			--config ${CMAKE_CURRENT_SOURCE_DIR}/iar_arm.icf
	)
elseif(${CMAKE_C_COMPILER_ID} STREQUAL "ARMClang")
	target_link_options(cortex_m85_pacbti_fvp_example
		PRIVATE
			--scatter=${CMAKE_CURRENT_SOURCE_DIR}/armclang.sct
			--map
	)
endif()

target_link_libraries(cortex_m85_pacbti_fvp_example
	freertos_kernel
	tfm-ns-interface
	cmsis_bsp
)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/trusted_firmware-m/integration/cmake)
include(MergeTfmImages)
include(SignTfmImage)

# The non-secure application image should be padded while being signed
# Hence, passing "TRUE" as the input parameter to the pad option of sign function.
tf_m_sign_image(cortex_m85_pacbti_fvp_example cortex_m85_pacbti_fvp_example_signed 0.0.1 TRUE)

tf_m_merge_images(cortex_m85_pacbti_fvp_example)
