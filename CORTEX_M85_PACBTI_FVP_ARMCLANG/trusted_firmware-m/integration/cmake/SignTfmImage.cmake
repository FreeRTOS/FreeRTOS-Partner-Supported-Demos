# Copyright 2023-2024 Arm Limited and/or its affiliates
# <open-source-office@arm.com>
# SPDX-License-Identifier: MIT

include(ConvertElfToBin)
include(ExternalProject)

ExternalProject_Get_Property(trusted_firmware-m-build BINARY_DIR)

function(tf_m_sign_image target signed_target_name version pad)
    if(${pad})
        set(pad_option "--pad")
    else()
        set(pad_option "")
    endif()

    target_axf_to_bin(
        ${target}
        "flash"
    )

    add_custom_command(
        TARGET
            ${target}
        POST_BUILD
        DEPENDS
            $<TARGET_FILE_DIR:${target}>/${target}.bin
        COMMAND
            # Sign the non-secure (application) image for TF-M bootloader (BL2)
            python3 ${BINARY_DIR}/api_ns/image_signing/scripts/wrapper/wrapper.py
                -v ${version}
                --layout ${BINARY_DIR}/api_ns/image_signing/layout_files/signing_layout_ns.o
                -k ${BINARY_DIR}/api_ns/image_signing/keys/image_ns_signing_private_key.pem
                --public-key-format full
                --align 1 --pad-header ${pad_option} -H 0x400 -s auto
                --measured-boot-record
                --confirm
                ${SECTORS_BIN_DIR}/flash.bin
                $<TARGET_FILE_DIR:${target}>/${signed_target_name}.bin
        COMMAND
            ${CMAKE_COMMAND} -E echo "-- signed: $<TARGET_FILE_DIR:${target}>/${signed_target_name}.bin"
        VERBATIM
    )
endfunction()
