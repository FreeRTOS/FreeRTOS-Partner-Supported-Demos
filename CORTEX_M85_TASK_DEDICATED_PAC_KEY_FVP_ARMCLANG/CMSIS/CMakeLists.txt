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
    target_compile_options(arm-corstone-platform-bsp
        PUBLIC
            -mbranch-protection=standard
    )
elseif(FREERTOS_ARM_V_8_1_M_PACBTI_CONFIG STREQUAL "ARM_V_8_1_M_PACBTI_CONFIG_PACRET_LEAF_BTI")
    target_compile_options(arm-corstone-platform-bsp
        PUBLIC
            -mbranch-protection=bti+pac-ret+leaf
    )
elseif(FREERTOS_ARM_V_8_1_M_PACBTI_CONFIG STREQUAL "ARM_V_8_1_M_PACBTI_CONFIG_PACRET")
    target_compile_options(arm-corstone-platform-bsp
        PUBLIC
            -mbranch-protection=pac-ret
    )
elseif(FREERTOS_ARM_V_8_1_M_PACBTI_CONFIG STREQUAL "ARM_V_8_1_M_PACBTI_CONFIG_PACRET_LEAF")
    target_compile_options(arm-corstone-platform-bsp
        PUBLIC
            -mbranch-protection=pac-ret+leaf
    )
elseif(FREERTOS_ARM_V_8_1_M_PACBTI_CONFIG STREQUAL "ARM_V_8_1_M_PACBTI_CONFIG_BTI")
    target_compile_options(arm-corstone-platform-bsp
        PUBLIC
            -mbranch-protection=bti
    )
elseif(FREERTOS_ARM_V_8_1_M_PACBTI_CONFIG STREQUAL "ARM_V_8_1_M_PACBTI_CONFIG_NONE")
    target_compile_options(arm-corstone-platform-bsp
        PUBLIC
            -mbranch-protection=none
    )
else()
	message(FATAL_ERROR "Invalid FREERTOS_ARM_V_8_1_M_PACBTI_CONFIG option chosen, the supported configurations are
	ARM_V_8_1_M_PACBTI_CONFIG_STANDARD,
	ARM_V_8_1_M_PACBTI_CONFIG_PACRET_LEAF_BTI,
	ARM_V_8_1_M_PACBTI_CONFIG_PACRET,
	ARM_V_8_1_M_PACBTI_CONFIG_PACRET_LEAF,
	ARM_V_8_1_M_PACBTI_CONFIG_BTI,
	ARM_V_8_1_M_PACBTI_CONFIG_NONE"
    )
endif()

target_compile_definitions(arm-corstone-platform-bsp
    INTERFACE
        __DOMAIN_NS=1
)

set(S_IMAGE_LOAD_ADDRESS  0x11000000 CACHE STRING "Secure firmware loading address")
set(NS_IMAGE_LOAD_ADDRESS 0x01020000 CACHE STRING "Non-secure user application loading address")

target_include_directories(arm-corstone-platform-bsp
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/corstone315/include
)
