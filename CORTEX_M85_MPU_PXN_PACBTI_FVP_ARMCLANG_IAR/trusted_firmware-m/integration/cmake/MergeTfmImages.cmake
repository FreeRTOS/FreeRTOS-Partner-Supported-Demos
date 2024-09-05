# Copyright 2023-2024, Arm Limited and/or its affiliates
# <open-source-office@arm.com>
# SPDX-License-Identifier: MIT

include(ExternalProject)

ExternalProject_Get_Property(trusted_firmware-m-build BINARY_DIR)

# To merge the bootloader image, TF-M secure image, non-secure user application image,
# and provsioning bundle images into one image, their addresses are
# needed. As the addresses are defined in their respective linker scripts, there is no
# simple way to programmatically get them, so they need to be specified by the user project.
# Order: <bootloader>, <signed secure TF-M firmware>, <signed non-secure user app>, <provisioning bundle address>.

function(tf_m_merge_images target)
    find_program(srec_cat NAMES srec_cat REQUIRED)
    find_program(objcopy NAMES arm-none-eabi-objcopy objcopy REQUIRED)
    add_custom_command(
        TARGET
            ${target}
        POST_BUILD
        DEPENDS
            $<TARGET_FILE_DIR:${target}>/${target}_signed.bin
        COMMAND
            ${srec_cat} ${BINARY_DIR}/api_ns/bin/bl1_1.bin -Binary -offset ${BL1_IMAGE_LOAD_ADDRESS}
                ${BINARY_DIR}/api_ns/bin/cm_provisioning_bundle.bin -Binary -offset ${S_CM_PROVISIONING_BUNDLE_LOAD_ADDRESS}
                ${BINARY_DIR}/api_ns/bin/dm_provisioning_bundle.bin -Binary -offset ${S_DM_PROVISIONING_BUNDLE_LOAD_ADDRESS}
                ${BINARY_DIR}/api_ns/bin/bl2_signed.bin -Binary -offset ${BL2_IMAGE_LOAD_ADDRESS}
                ${BINARY_DIR}/api_ns/bin/tfm_s_signed.bin -Binary -offset ${S_IMAGE_LOAD_ADDRESS}
                $<TARGET_FILE_DIR:${target}>/${target}_signed.bin -Binary -offset ${NS_IMAGE_LOAD_ADDRESS}
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
