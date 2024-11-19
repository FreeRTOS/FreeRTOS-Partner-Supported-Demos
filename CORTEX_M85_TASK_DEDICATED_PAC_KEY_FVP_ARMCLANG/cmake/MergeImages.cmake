# Copyright 2023-2024, Arm Limited and/or its affiliates
# <open-source-office@arm.com>
# SPDX-License-Identifier: MIT

# To merge the secure image and non-secure user application image into
# one image.
# Order: <secure firmware>, <non-secure user app>.

function(merge_images target)
    find_program(srec_cat NAMES srec_cat REQUIRED)
    find_program(objcopy NAMES arm-none-eabi-objcopy objcopy REQUIRED)
    add_custom_command(
        TARGET
            ${target}
        POST_BUILD
        DEPENDS
            $<TARGET_FILE_DIR:${target}>/${target}_signed.bin
            COMMAND
            ${srec_cat}
                ${CMAKE_BINARY_DIR}/application_sectors/flash_s.bin -Binary -offset ${S_IMAGE_LOAD_ADDRESS}
                ${CMAKE_BINARY_DIR}/application_sectors/flash_ns.bin -Binary -offset ${NS_IMAGE_LOAD_ADDRESS}
                -o $<TARGET_FILE_DIR:${target}>/${target}_merged.hex
        COMMAND
            ${objcopy} -I ihex -O elf32-little
                $<TARGET_FILE_DIR:${target}>/${target}_merged.hex
                $<TARGET_FILE_DIR:${target}>/${target}_merged.elf
        COMMAND
            ${CMAKE_COMMAND} -E echo "-- merged: $<TARGET_FILE_DIR:${target}>/${target}_merged.elf"
        VERBATIM
    )
endfunction()
