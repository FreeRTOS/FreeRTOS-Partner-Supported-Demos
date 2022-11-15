/*
 *  Copyright (C) 2022 Texas Instruments Incorporated
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SOC_AM64X_H_
#define SOC_AM64X_H_

#ifdef __cplusplus
extern "C"
{
#endif

/**
 *  \defgroup DRV_SOC_MODULE APIs for SOC Specific Functions
 *  \ingroup DRV_MODULE
 *
 * For more details and example usage, see \ref DRIVERS_SOC_PAGE
 *
 *  @{
 */

#include <drivers/sciclient.h>
#include <kernel/dpl/CpuIdP.h>

/**
 *  \anchor SOC_DomainId_t
 *  \name SOC Domain ID
 *  @{
 */
#define SOC_DOMAIN_ID_MAIN     (0U)
#define SOC_DOMAIN_ID_MCU      (1U)
/** @} */

/**
 * \brief Switch value for SD card boot mode
 */
#define SOC_BOOTMODE_MMCSD      (0X36C3)

/**
 * \brief Enable clock to specified module
 *
 * \param moduleId [in] see \ref tisci_devices for list of device ID's
 * \param enable [in] 1: enable clock to the module, 0: disable clock to the module
 *
 * \return SystemP_SUCCESS Module clock is enabled
 * \return SystemP_FAILURE Module clock could not be enabled
 */
int32_t SOC_moduleClockEnable(uint32_t moduleId, uint32_t enable);

/**
 * \brief Set module clock to specified frequency
 *
 * \param moduleId [in] see \ref tisci_devices for list of module ID's
 * \param clkId [in] see \ref tisci_clocks for list of clocks associated with the specified module ID
 * \param clkRate [in] Frequency to set in Hz
 *
 * \return SystemP_SUCCESS Module clock is enabled
 * \return SystemP_FAILURE Module clock could not be enabled
 */
int32_t SOC_moduleSetClockFrequency(uint32_t moduleId, uint32_t clkId, uint64_t clkRate);

/**
 * \brief Get module clock frequency
 *
 * \param moduleId [in] see \ref tisci_devices for list of module ID's
 * \param clkId [in] see \ref tisci_clocks for list of clocks associated with the specified module ID
 * \param clkRate [out] Frequency of the clock
 *
 * \return SystemP_SUCCESS on success, else failure
 */
int32_t SOC_moduleGetClockFrequency(uint32_t moduleId, uint32_t clkId, uint64_t *clkRate);

/**
 * \brief Convert a core ID to a user readable name
 *
 * \param coreId    [in] see \ref CSL_CoreID
 *
 * \return name as a string
 */
const char *SOC_getCoreName(uint16_t coreId);

/**
 * \brief Get the clock frequency in Hz of the CPU on which the driver is running
 *
 * \return Clock frequency in Hz
 */
uint64_t SOC_getSelfCpuClk(void);

/**
 * \brief Lock control module partition to prevent writes into control MMRs
 *
 * \param domainId    [in] See SOC_DomainId_t
 * \param partition   [in] Partition number to unlock
 */
void SOC_controlModuleLockMMR(uint32_t domainId, uint32_t partition);

/**
 * \brief Unlock control module partition to allow writes into control MMRs
 *
 * \param domainId    [in] See SOC_DomainId_t
 * \param partition   [in] Partition number to unlock
 */
void SOC_controlModuleUnlockMMR(uint32_t domainId, uint32_t partition);

/**
 * \brief Enable or disable ePWM time base clock from Control MMR
 *
 * \param epwmInstance [in] ePWM instance number [0 - (CSL_EPWM_PER_CNT-1)]
 * \param enable       [in] TRUE to enable and FALSE to disable
 */
void SOC_setEpwmTbClk(uint32_t epwmInstance, uint32_t enable);

/**
 *  \brief SOC Virtual (CPU) to Physical address translation function.
 *
 *  \param virtAddr [IN] Virtual/CPU address
 *
 *  \return Corresponding SOC physical address
 */
uint64_t SOC_virtToPhy(void *virtAddr);

/**
 *  \brief Physical to Virtual (CPU) address translation function.
 *
 *  \param phyAddr  [IN] Physical address
 *
 *  \return Corresponding virtual/CPU address
 */
void *SOC_phyToVirt(uint64_t phyAddr);

/**
 * \brief Change boot mode by setting devstat register
 *
 * \param bootMode [IN] Boot mode switch value
 */
void SOC_setDevStat(uint32_t bootMode);

/**
 * \brief Return R5SS supporting single or dual core mode.
 *
 * \param cpuInfo [IN] Pointer to the \ref CSL_ArmR5CPUInfo struct.
 *
 *  \return TRUE if it is Dual Core mode else FALSE.
 */
uint32_t SOC_isR5FDualCoreMode(CSL_ArmR5CPUInfo *cpuInfo);

/**
 * \brief Generate SW Warm Reset Main Domain
 */
void SOC_generateSwWarmResetMainDomain(void);

/**
 * \brief Generate SW POR Reset Main Domain
 */
void SOC_generateSwPORResetMainDomain(void);

/**
 * \brief Get the reset reason source for Main Domain
 *
 * \return Reset Reason Source Main Domain
 */
uint32_t SOC_getWarmResetCauseMainDomain(void);

/**
 * \brief Generate SW WARM Reset Mcu Domain
 */
void SOC_generateSwWarmResetMcuDomain(void);

/**
 * \brief Generate SW WARM Reset Main Domain from Mcu Domain
 */
void SOC_generateSwWarmResetMainDomainFromMcuDomain(void);

/**
 * \brief Generate SW POR Reset Main Domain from Mcu Domain
 */
void SOC_generateSwPORResetMainDomainFromMcuDomain(void);

/**
 * \brief Get the reset reason source for Mcu Domain
 *
 * \return Reset Reason Source Mcu Domain
 */
uint32_t SOC_getWarmResetCauseMcuDomain(void);

/**
 * \brief Clears reason for Warm and Main/Mcu Domain Power On Resets.
 *        CTRLMMR_RST_SRC is just a mirror of CTRLMMR_MCU_RST_SRC register. It
 *        is read only. So we need to write 1 to CTRLMMR_MCU_RST_SRC to clear
 *        the reset reason.
 *
 * \param resetCause [IN] Reset reason value to clear.
 */
void SOC_clearResetCauseMainMcuDomain(uint32_t resetCause);

/** @} */

#ifdef __cplusplus
}
#endif

#endif
