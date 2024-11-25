# Copyright 2023-2024 Arm Limited and/or its affiliates <open-source-office@arm.com>
#
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.15)

project(
  s-dedicated-pac-key-example
  VERSION 0.1
  LANGUAGES C ASM
)

set (CMAKE_BUILD_TYPE Release)

set(CMAKE_EXECUTABLE_SUFFIX ".axf")

get_filename_component(FREERTOS_DIR_PATH ${CMAKE_CURRENT_LIST_DIR}/../../../../.. REALPATH)
message(DEBUG "FREERTOS_DIR_PATH is ${FREERTOS_DIR_PATH}")

add_compile_options("-mcmse")

function(CreateVeneer app_name target_name)
    set(veneers_name "veneers")
    add_library(${app_name}_${veneers_name} STATIC)

    target_sources(${app_name}_${veneers_name}
        PRIVATE
            ${CMAKE_BINARY_DIR}/${veneers_name}.o
    )

    # Since s_veneers.o doesn't exist when this is evaluated by cmake we need to
    # explicity specify what language it will use.
    set_target_properties(${app_name}_${veneers_name}
        PROPERTIES
            LINKER_LANGUAGE C
    )

    add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/${veneers_name}.o
        COMMAND
        DEPENDS ${app_name}
    )

    target_link_options(${app_name}
        PRIVATE
			--import_cmse_lib_out=${CMAKE_BINARY_DIR}/${veneers_name}.o
    )
    set(${target_name} "${app_name}_${veneers_name}" CACHE PATH "Veneers target name" FORCE)
endfunction()

add_subdirectory(../CMSIS CMSIS)

add_executable(secure_cortex_m85_task_dedicated_pac_key_fvp_example)

target_sources(secure_cortex_m85_task_dedicated_pac_key_fvp_example
	PUBLIC
    	${CMAKE_CURRENT_SOURCE_DIR}/main_s.c
		${CMAKE_CURRENT_SOURCE_DIR}/secure_config.c
		${CMAKE_CURRENT_SOURCE_DIR}/../CMSIS/common/bsp_serial.c
)

add_library(secure_kernel_lib)

target_sources(secure_kernel_lib
	PUBLIC
		${FREERTOS_DIR_PATH}/Source/portable/GCC/ARM_CM85/secure/secure_context.c
		${FREERTOS_DIR_PATH}/Source/portable/GCC/ARM_CM85/secure/secure_context_port.c
		${FREERTOS_DIR_PATH}/Source/portable/GCC/ARM_CM85/secure/secure_init.c
		${FREERTOS_DIR_PATH}/Source/portable/GCC/ARM_CM85/secure/secure_heap.c
		${CMAKE_CURRENT_SOURCE_DIR}/../../Demos_Dependencies/arm_corstone_platform_bsp/corstone315/Device/Source/startup_SSE315.c
)

target_include_directories(secure_kernel_lib
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/../../Demos_Dependencies/arm_corstone_platform_bsp/corstone315/Device/Include
		${CMAKE_CURRENT_SOURCE_DIR}/../../Demos_Dependencies/arm_corstone_platform_bsp/corstone315/Board/Platform
		${CMAKE_CURRENT_SOURCE_DIR}/../../Demos_Dependencies/arm_corstone_platform_bsp/CMSIS/CMSIS_6/CMSIS/Core/Include/
)

target_link_libraries(secure_kernel_lib
	freertos_config
)

target_include_directories(freertos_kernel_port_headers
	INTERFACE
		${FREERTOS_DIR_PATH}/Source/portable/GCC/ARM_CM85/secure
)

target_include_directories(secure_cortex_m85_task_dedicated_pac_key_fvp_example
    PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/../CMSIS/common
		${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(secure_cortex_m85_task_dedicated_pac_key_fvp_example
	PRIVATE
		arm-corstone-platform-bsp
		secure_kernel_lib
)

target_link_options(secure_cortex_m85_task_dedicated_pac_key_fvp_example
	PRIVATE
		--scatter=${CMAKE_CURRENT_SOURCE_DIR}/armclang_s.sct
		--map
		--entry=Reset_Handler
)

CreateVeneer(secure_cortex_m85_task_dedicated_pac_key_fvp_example VENEERS_TARGET_NAME)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../cmake)
include(ConvertElfToBin)
target_axf_to_bin(secure_cortex_m85_task_dedicated_pac_key_fvp_example flash_s)
