/*
 * Copyright (c) 2019-2024, Arm Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "SSE315.h"
#include "system_SSE315.h"
#include "platform_irq.h"
#include "core_cm85.h"
#include "secure_config.h"
#include "Driver_MPC.h"
#include "Driver_PPC.h"
#include "cmsis_armclang.h"
#include "device_cfg.h"
#include "region_defs.h"
#include "platform_base_address.h"
#include "platform_regs.h"

#define REGION(a, b, c) a##b##c
#define REGION_NAME(a, b, c) REGION(a, b, c)
#define REGION_DECLARE(a, b, c) extern uint32_t REGION_NAME(a, b, c)

REGION_DECLARE(Image$$, ER_CODE_CMSE_VENEER, $$Base);
REGION_DECLARE(Image$$, ER_CODE_CMSE_VENEER, $$Limit);

#define CMSE_VENEER_REGION_BASE  ((uint32_t)&REGION_NAME(Image$$, ER_CODE_CMSE_VENEER, $$Base))
#define CMSE_VENEER_REGION_LIMIT ((uint32_t)&REGION_NAME(Image$$, ER_CODE_CMSE_VENEER, $$Limit) - 1)

extern ARM_DRIVER_MPC Driver_SRAM_MPC;
extern ARM_DRIVER_MPC Driver_ISRAM0_MPC;
extern ARM_DRIVER_PPC_CORSTONE315 Driver_PERIPH_EXP2_PPC_CORSTONE315;

#define CHECK_ERROR(error_code)              \
    do {                                     \
        if ((error_code) != ARM_DRIVER_OK) { \
            __BKPT(0);                       \
            return error_code;               \
        }                                    \
    } while (0)

static int32_t MPC_config(void)
{
    /* Initialize the MPC */
    CHECK_ERROR(Driver_SRAM_MPC.Initialize());
    CHECK_ERROR(Driver_ISRAM0_MPC.Initialize());

    /* Configure the MPCs */
    /* SRAM holds NS Code */
    CHECK_ERROR(Driver_SRAM_MPC.ConfigRegion(NS_CODE_START, NS_CODE_LIMIT, ARM_MPC_ATTR_NONSECURE));

    /* ISRAM0 holds NS Data */
    CHECK_ERROR(Driver_ISRAM0_MPC.ConfigRegion(NS_DATA_START, NS_DATA_LIMIT, ARM_MPC_ATTR_NONSECURE));

    /* Lock down the MPC configurations */
    CHECK_ERROR(Driver_SRAM_MPC.LockDown());
    CHECK_ERROR(Driver_ISRAM0_MPC.LockDown());

    return ARM_DRIVER_OK;
}

static int32_t PPC_config(void)
{
    struct corstone315_sacfg_t *sactrl = (struct corstone315_sacfg_t *)CORSTONE315_SACFG_BASE_S;

    /* Set UART0 to Non-Secure
     * After these settings, UART0 can be used only through
     * the non-secure alias
     */
    CHECK_ERROR(Driver_PERIPH_EXP2_PPC_CORSTONE315.Initialize());
    CHECK_ERROR(Driver_PERIPH_EXP2_PPC_CORSTONE315.ConfigSecurity(UART0_PERIPH_PPCEXP2_POS_MASK,
                                                                  ARM_PPC_CORSTONE315_NONSECURE_CONFIG));

    CHECK_ERROR(Driver_PERIPH_EXP2_PPC_CORSTONE315.ConfigPrivilege(UART0_PERIPH_PPCEXP2_POS_MASK,
                                                                   ARM_PPC_CORSTONE315_NONSECURE_CONFIG,
                                                                   ARM_PPC_CORSTONE315_PRIV_AND_NONPRIV_CONFIG));
    /*
     * Configure the response to a security violation as a
     * bus error instead of RAZ/WI
     */
    sactrl->secrespcfg |= 1;

    return ARM_DRIVER_OK;
}

static void SAU_config(void)
{
    uint32_t sau_idx = 0;
    struct corstone315_sacfg_t *sactrl = (struct corstone315_sacfg_t *)CORSTONE315_SACFG_BASE_S;
    /* Enable SAU */
    TZ_SAU_Enable();

    SAU->RNR = sau_idx++;
    SAU->RBAR = (NS_CODE_START & SAU_RBAR_BADDR_Msk);
    SAU->RLAR = ((NS_CODE_LIMIT)&SAU_RLAR_LADDR_Msk) | SAU_RLAR_ENABLE_Msk;

    SAU->RNR = sau_idx++;
    SAU->RBAR = ((NS_DATA_START)&SAU_RBAR_BADDR_Msk);
    SAU->RLAR = ((NS_DATA_LIMIT)&SAU_RLAR_LADDR_Msk) | SAU_RLAR_ENABLE_Msk;

    SAU->RNR = sau_idx++;
    SAU->RBAR = (CMSE_VENEER_REGION_BASE & SAU_RBAR_BADDR_Msk);
    SAU->RLAR = ((CMSE_VENEER_REGION_LIMIT)&SAU_RLAR_LADDR_Msk) | SAU_RLAR_ENABLE_Msk | SAU_RLAR_NSC_Msk;

    /* Set UART0 to Non-Secure */
    SAU->RNR = sau_idx;
    SAU->RBAR = (UART0_BASE_NS & SAU_RBAR_BADDR_Msk);
    SAU->RLAR = ((UART1_BASE_NS - 1) & SAU_RLAR_LADDR_Msk) | SAU_RLAR_ENABLE_Msk;

#ifdef SYSWDOG_ARMV8_M_DEV
    sau_idx++;
    /* Enable NS System Watchdog access in Non-Secure */
    SAU->RNR = sau_idx;
    SAU->RBAR = (SYSWDOG_ARMV8_M_CNTRL_BASE_NS & SAU_RLAR_LADDR_Msk);
    SAU->RLAR = ((SYSWDOG_ARMV8_M_CNTRL_BASE_NS + 0x1FFF) & SAU_RLAR_LADDR_Msk) | SAU_RLAR_ENABLE_Msk;
#endif

    /* Configures CODE region (0x1000_0000 to 0x1FFF_FFFF)
     * to Non-secure Callable. */
    sactrl->nsccfg |= 1;
}

/*
 * \brief Configures the necessary security components
 */
void secure_platform_init(void)
{
    bool error = false;

    SAU_config();
    error = error || MPC_config();
    error = error || PPC_config();

    if (error) {
        while (1)
            ;
    }

    NVIC_EnableIRQ(MPC_IRQn);
    NVIC_EnableIRQ(PPC_IRQn);

    /* Enables BUS, MEM, USG and Secure faults */
    SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_MEMFAULTENA_Msk
                  | SCB_SHCSR_SECUREFAULTENA_Msk;

    SCB->NSACR |= SCB_NSACR_CP10_Msk | SCB_NSACR_CP11_Msk;

    NVIC_SetTargetState(UARTTX0_IRQn);
    NVIC_SetTargetState(UARTRX0_IRQn);
}
