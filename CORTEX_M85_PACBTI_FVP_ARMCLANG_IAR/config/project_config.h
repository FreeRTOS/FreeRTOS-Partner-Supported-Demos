/* Copyright 2023-2024 Arm Limited and/or its affiliates
 * <open-source-office@arm.com>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#define PLATFORM_SERVICE_INPUT_BUFFER_SIZE      64

#define PLATFORM_SERVICE_OUTPUT_BUFFER_SIZE     64

#define PLATFORM_SP_STACK_SIZE                  0x500

#define PLATFORM_NV_COUNTER_MODULE_DISABLED     0

#define CRYPTO_ENGINE_BUF_SIZE                  0x8000

#define CRYPTO_CONC_OPER_NUM                    8

#define CRYPTO_RNG_MODULE_ENABLED               1

#define CRYPTO_KEY_MODULE_ENABLED               1

#define CRYPTO_AEAD_MODULE_ENABLED              1

#define CRYPTO_MAC_MODULE_ENABLED               1

#define CRYPTO_HASH_MODULE_ENABLED              1

#define CRYPTO_CIPHER_MODULE_ENABLED            1

#define CRYPTO_ASYM_SIGN_MODULE_ENABLED         1

#define CRYPTO_ASYM_ENCRYPT_MODULE_ENABLED      1

#define CRYPTO_KEY_DERIVATION_MODULE_ENABLED    1

#define CRYPTO_IOVEC_BUFFER_SIZE                5120

#define CRYPTO_NV_SEED                          1

#define CRYPTO_SINGLE_PART_FUNCS_DISABLED       0

#define CRYPTO_STACK_SIZE                       0x1B00

#define TFM_FWU_BUF_SIZE                        PSA_FWU_MAX_WRITE_SIZE

#define FWU_STACK_SIZE                          0x600

#define ATTEST_INCLUDE_OPTIONAL_CLAIMS          1

#define ATTEST_INCLUDE_COSE_KEY_ID              0

#define ATTEST_STACK_SIZE                       0x700

#define ATTEST_TOKEN_PROFILE_PSA_IOT_1          1

#define ITS_CREATE_FLASH_LAYOUT                 1

#define ITS_RAM_FS                              0

#define ITS_VALIDATE_METADATA_FROM_FLASH        1

#define ITS_MAX_ASSET_SIZE                      1300

#define ITS_BUF_SIZE                            ITS_MAX_ASSET_SIZE

#define ITS_NUM_ASSETS                          10

#define ITS_STACK_SIZE                          0x720

#define PS_CREATE_FLASH_LAYOUT                  1

#define PS_RAM_FS                               0

#define PS_ROLLBACK_PROTECTION                  1

#define PS_VALIDATE_METADATA_FROM_FLASH         1

#define PS_MAX_ASSET_SIZE                       2048

#define PS_NUM_ASSETS                           10

#define PS_STACK_SIZE                           0x700

#define CONFIG_TFM_CONN_HANDLE_MAX_NUM          8

#define CONFIG_TFM_DOORBELL_API                 0
