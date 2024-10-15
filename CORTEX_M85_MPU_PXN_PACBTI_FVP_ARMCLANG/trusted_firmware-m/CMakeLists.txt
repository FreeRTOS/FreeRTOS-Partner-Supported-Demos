# Copyright 2023-2024, Arm Limited and/or its affiliates
# <open-source-office@arm.com>
# SPDX-License-Identifier: MIT

set(trusted_firmware-m_SOURCE_DIR
    ${CMAKE_CURRENT_LIST_DIR}/../../Demos_Dependencies/trusted_firmware-m/
    CACHE INTERNAL
    "Path to Trusted Firmware-M source code"
)

add_subdirectory(integration)
