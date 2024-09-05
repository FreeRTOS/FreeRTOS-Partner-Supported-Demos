# Copyright 2023-2024 Arm Limited and/or its affiliates
# <open-source-office@arm.com>
# SPDX-License-Identifier: Apache-2.0

set(arm_corstone_platform_bsp_SOURCE_DIR
    ${CMAKE_CURRENT_LIST_DIR}/../../Demos_Dependencies/arm_corstone_platform_bsp
    CACHE INTERNAL
    "Path to Arm Corstone-3xx Platform CMSIS-Driver Based Board Support Package source code"
)

set(ARM_CORSTONE_BSP_TARGET_PLATFORM "corstone315" CACHE STRING "")

add_subdirectory(${arm_corstone_platform_bsp_SOURCE_DIR} build)

if(FREERTOS_ARM_V_8_1_M_PACBTI_CONFIG STREQUAL "ARM_V_8_1_M_PACBTI_CONFIG_STANDARD")
    target_compile_options(arm-corstone-platform-bsp PUBLIC $<$<STREQUAL:${CMAKE_C_COMPILER_ID},ARMClang>:-mbranch-protection=standard>)
    target_compile_options(arm-corstone-platform-bsp PUBLIC $<$<STREQUAL:${CMAKE_C_COMPILER_ID},IAR>:$<$<COMPILE_LANGUAGE:C,CXX>:--branch_protection=bti+pac-ret>>)
elseif(FREERTOS_ARM_V_8_1_M_PACBTI_CONFIG STREQUAL "ARM_V_8_1_M_PACBTI_CONFIG_PACRET_LEAF_BTI")
	if(${CMAKE_C_COMPILER_ID} STREQUAL "ARMClang")
		target_compile_options(arm-corstone-platform-bsp
			PUBLIC
				-mbranch-protection=bti+pac-ret+leaf
		)
	elseif(${CMAKE_C_COMPILER_ID} STREQUAL "IAR")
		message(FATAL_ERROR "ARM_V_8_1_M_PACBTI_CONFIG_PACRET_LEAF_BTI PACBTI option is not supported on IAR Compiler.")
	endif()
elseif(FREERTOS_ARM_V_8_1_M_PACBTI_CONFIG STREQUAL "ARM_V_8_1_M_PACBTI_CONFIG_PACRET")
    target_compile_options(arm-corstone-platform-bsp PUBLIC $<$<STREQUAL:${CMAKE_C_COMPILER_ID},ARMClang>:-mbranch-protection=pac-ret>)
    target_compile_options(arm-corstone-platform-bsp PUBLIC $<$<STREQUAL:${CMAKE_C_COMPILER_ID},IAR>:$<$<COMPILE_LANGUAGE:C,CXX>:--branch_protection=pac-ret>>)
elseif(FREERTOS_ARM_V_8_1_M_PACBTI_CONFIG STREQUAL "ARM_V_8_1_M_PACBTI_CONFIG_PACRET_LEAF")
	if(${CMAKE_C_COMPILER_ID} STREQUAL "ARMClang")
		target_compile_options(arm-corstone-platform-bsp
			PUBLIC
				-mbranch-protection=pac-ret+leaf
		)
	elseif(${CMAKE_C_COMPILER_ID} STREQUAL "IAR")
		message(FATAL_ERROR "ARM_V_8_1_M_PACBTI_CONFIG_PACRET_LEAF PACBTI option is not supported on IAR Compiler.")
	endif()
elseif(FREERTOS_ARM_V_8_1_M_PACBTI_CONFIG STREQUAL "ARM_V_8_1_M_PACBTI_CONFIG_BTI")
    target_compile_options(arm-corstone-platform-bsp PUBLIC $<$<STREQUAL:${CMAKE_C_COMPILER_ID},ARMClang>:-mbranch-protection=bti>)
    target_compile_options(arm-corstone-platform-bsp PUBLIC $<$<STREQUAL:${CMAKE_C_COMPILER_ID},IAR>:$<$<COMPILE_LANGUAGE:C,CXX>:--branch_protection=bti>>)
elseif(FREERTOS_ARM_V_8_1_M_PACBTI_CONFIG STREQUAL "ARM_V_8_1_M_PACBTI_CONFIG_NONE")
	if(${CMAKE_C_COMPILER_ID} STREQUAL "ARMClang")
		target_compile_options(arm-corstone-platform-bsp
			PUBLIC
				-mbranch-protection=none
		)
	endif()
else()
	message(FATAL_ERROR "Invalid FREERTOS_ARM_V_8_1_M_PACBTI_CONFIG option chosen, the supported configurations are
	ARM_V_8_1_M_PACBTI_CONFIG_STANDARD,
	ARM_V_8_1_M_PACBTI_CONFIG_PACRET_LEAF_BTI,
	ARM_V_8_1_M_PACBTI_CONFIG_PACRET,
	ARM_V_8_1_M_PACBTI_CONFIG_PACRET_LEAF,
	ARM_V_8_1_M_PACBTI_CONFIG_BTI,
	ARM_V_8_1_M_PACBTI_CONFIG_NONE
	")
endif()

target_compile_definitions(arm-corstone-platform-bsp
    INTERFACE
        __DOMAIN_NS=1
)

set(BL1_IMAGE_LOAD_ADDRESS 0x11000000 CACHE STRING "Bootload stage 1 image loading address")
set(BL2_IMAGE_LOAD_ADDRESS 0x12031400 CACHE STRING "Bootload image loading address")
set(S_IMAGE_LOAD_ADDRESS 0x38000000 CACHE STRING "Secure TF-M firmware loading address")
set(NS_IMAGE_LOAD_ADDRESS 0x28040000 CACHE STRING "Non-secure user application loading address")
set(S_CM_PROVISIONING_BUNDLE_LOAD_ADDRESS 0x12024000 CACHE STRING "Secure CM provisioning bundle loading address")
set(S_DM_PROVISIONING_BUNDLE_LOAD_ADDRESS 0x1202aa00 CACHE STRING "Secure DM provisioning bundle loading address")

target_include_directories(arm-corstone-platform-bsp
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/corstone315/include
)

# BSP serial library

add_library(cmsis_bsp STATIC)

target_sources(cmsis_bsp
    PRIVATE
        common/bsp_serial.c
)

target_include_directories(cmsis_bsp
    PUBLIC
        common
)

target_link_libraries(cmsis_bsp
    PUBLIC
        arm-corstone-platform-bsp
        freertos_kernel
)
