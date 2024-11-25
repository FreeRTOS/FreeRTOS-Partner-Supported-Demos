# Copyright 2021-2024 Arm Limited and/or its affiliates
# <open-source-office@arm.com>
# SPDX-License-Identifier: MIT

function(target_axf_to_bin target output_binary_name)
    set(SECTORS_BIN_DIR ${CMAKE_BINARY_DIR}/application_sectors CACHE INTERNAL "Output sectors binaries directory")
    file(MAKE_DIRECTORY ${SECTORS_BIN_DIR})
    if(CMAKE_C_COMPILER_ID STREQUAL "IAR")
        find_program(objcopy NAMES arm-none-eabi-objcopy objcopy REQUIRED)
        set(elf_to_bin
            ${objcopy} -O binary
            $<TARGET_FILE:${target}>
            ${SECTORS_BIN_DIR}/${output_binary_name}.bin
        )
    elseif(CMAKE_C_COMPILER_ID STREQUAL "ARMClang")
        find_program(fromelf NAMES fromelf REQUIRED)
        set(elf_to_bin
            ${fromelf} --bin
            --output ${SECTORS_BIN_DIR}/${output_binary_name}.bin
            $<TARGET_FILE:${target}>
        )
    else()
        message(FATAL_ERROR "Unsupported compiler: ${CMAKE_C_COMPILER_ID}")
    endif()
    add_custom_command(
        TARGET
            ${target}
        POST_BUILD
        DEPENDS
            $<TARGET_FILE:${target}>
        COMMAND
            ${elf_to_bin}
        COMMAND
            ${CMAKE_COMMAND} -E echo "-- built: $<TARGET_FILE_DIR:${target}>/${output_binary_name}.bin"
        VERBATIM
    )
endfunction()
