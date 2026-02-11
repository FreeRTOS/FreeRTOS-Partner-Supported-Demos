# Copyright 2023-2024, 2026, Arm Limited and/or its affiliates
# <open-source-office@arm.com>
# SPDX-License-Identifier: MIT

set(trusted_firmware-m_SOURCE_DIR
    ${CMAKE_CURRENT_LIST_DIR}/../../Demos_Dependencies/trusted_firmware-m/
    CACHE INTERNAL
    "Path to Trusted Firmware-M source code"
)

set(PATCH_FILES_DIRECTORY "${trusted_firmware-m_SOURCE_DIR}/../integration/trusted_firmware-m/patches")
set(PATCH_FILE
    "${PATCH_FILES_DIRECTORY}/0001-ethos-u-core-driver-Fix-invalid-URL.patch"
)

# Check if the patch is already applied by reverse dry-run.
execute_process(
    COMMAND patch -R --dry-run -s -p1 -d "${trusted_firmware-m_SOURCE_DIR}" -i "${PATCH_FILE}"
    RESULT_VARIABLE _rev_rc
)

if(_rev_rc EQUAL 0)
    message(STATUS "ApplyPatch: Patch already applied; continuing: ${PATCH_FILE}")
else()
    set(TFM_PATCH_COMMAND patch -p1 -d ${trusted_firmware-m_SOURCE_DIR} -i "${PATCH_FILE}")
endif()

add_subdirectory(integration)
